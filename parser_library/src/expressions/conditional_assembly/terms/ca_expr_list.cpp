/*
 * Copyright (c) 2019 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Broadcom, Inc. - initial API and implementation
 */

#include "ca_expr_list.h"

#include <cassert>
#include <stack>

#include "../ca_operator_binary.h"
#include "../ca_operator_unary.h"
#include "ca_function.h"
#include "ca_symbol.h"
#include "expressions/conditional_assembly/ca_expr_visitor.h"

namespace hlasm_plugin::parser_library::expressions {

ca_expr_list::ca_expr_list(std::vector<ca_expr_ptr> expr_list, range expr_range)
    : ca_expression(context::SET_t_enum::UNDEF_TYPE, std::move(expr_range))
    , expr_list(std::move(expr_list))
{}

undef_sym_set ca_expr_list::get_undefined_attributed_symbols(const evaluation_context& eval_ctx) const
{
    undef_sym_set tmp;
    for (auto&& expr : expr_list)
        tmp.merge(expr->get_undefined_attributed_symbols(eval_ctx));
    return tmp;
}

bool is_symbol(const ca_expr_ptr& expr) { return dynamic_cast<const ca_symbol*>(expr.get()) != nullptr; }

const std::string& get_symbol(const ca_expr_ptr& expr) { return *dynamic_cast<const ca_symbol&>(*expr).symbol; }

void ca_expr_list::resolve_expression_tree(context::SET_t_enum kind, diagnostic_op_consumer& diags)
{
    expr_kind = kind;

    if (kind == context::SET_t_enum::B_TYPE)
        unknown_functions_to_operators();

    if (kind == context::SET_t_enum::A_TYPE)
        resolve<context::A_t>(diags);
    else if (kind == context::SET_t_enum::B_TYPE)
        resolve<context::B_t>(diags);
    else if (kind == context::SET_t_enum::C_TYPE)
        resolve<context::C_t>(diags);
    else
        assert(false);
}

bool ca_expr_list::is_character_expression(character_expression_purpose purpose) const
{
    return purpose == character_expression_purpose::assignment && expr_list.size() == 1
        && expr_list.front()->is_character_expression(purpose);
}

void ca_expr_list::apply(ca_expr_visitor& visitor) const { visitor.visit(*this); }

context::SET_t ca_expr_list::evaluate(const evaluation_context& eval_ctx) const
{
    assert(expr_list.size() <= 1);

    if (expr_list.empty())
        return context::SET_t(expr_kind);
    return expr_list.front()->evaluate(eval_ctx);
}

void ca_expr_list::unknown_functions_to_operators()
{
    for (int idx = (int)expr_list.size() - 1; idx >= 0; --idx)
    {
        if (auto expr_func = dynamic_cast<ca_function*>(expr_list[idx].get());
            expr_func && expr_func->function == ca_expr_funcs::UNKNOWN && expr_func->parameters.size() == 1)
        {
            auto holder = std::move(expr_list[idx]);
            auto& true_func = dynamic_cast<ca_function&>(*holder);
            if (true_func.duplication_factor)
            {
                auto expr_r = true_func.duplication_factor->expr_range;
                expr_list[idx] = std::make_unique<ca_par_operator>(std::move(true_func.duplication_factor), expr_r);

                expr_r = true_func.parameters.front()->expr_range;
                expr_list.insert(expr_list.begin() + idx + 1,
                    std::make_unique<ca_par_operator>(std::move(true_func.parameters.front()), expr_r));

                expr_r = true_func.expr_range;
                expr_list.insert(
                    expr_list.begin() + idx + 1, std::make_unique<ca_symbol>(true_func.function_name, expr_r));
            }
            else
            {
                auto expr_r = true_func.expr_range;
                expr_list[idx] = std::make_unique<ca_symbol>(true_func.function_name, expr_r);

                expr_r = true_func.parameters.front()->expr_range;
                expr_list.insert(expr_list.begin() + idx + 1,
                    std::make_unique<ca_par_operator>(std::move(true_func.parameters.front()), expr_r));
            }
        }
    }
}

namespace {

struct term_entry
{
    ca_expr_ptr term;
    size_t i;
    bool simple_term;
};
struct op_entry
{
    size_t i;
    ca_expr_ops op_type;
    int priority;
    bool binary;
    bool right_assoc;
    bool requires_terms;
    range r;
};

template<typename T>
struct resolve_stacks
{
    using expr_policy = typename ca_expr_traits<T>::policy_t;

    std::stack<term_entry> terms;
    std::stack<op_entry> op_stack;

    void push_term(term_entry t) { terms.push(std::move(t)); }
    void push_op(op_entry op) { op_stack.push(std::move(op)); }

    bool reduce_binary(const op_entry& op, diagnostic_op_consumer& diags)
    {
        auto right = std::move(terms.top());
        terms.pop();
        auto left = std::move(terms.top());
        terms.pop();

        if (op.requires_terms && !left.simple_term || left.i > op.i)
        {
            diags.add_diagnostic(diagnostic_op::error_CE003(range(op.r.start)));
            return false;
        }
        if (op.requires_terms && !right.simple_term || right.i < op.i)
        {
            diags.add_diagnostic(diagnostic_op::error_CE003(range(op.r.end)));
            return false;
        }

        terms.push({
            std::make_unique<ca_function_binary_operator>(
                std::move(left.term), std::move(right.term), op.op_type, context::object_traits<T>::type_enum, op.r),
            left.i,
            false,
        });
        return true;
    }

    bool reduce_unary(const op_entry& op, diagnostic_op_consumer& diags)
    {
        auto right = std::move(terms.top());
        terms.pop();

        if (op.requires_terms && !right.simple_term || right.i < op.i)
        {
            diags.add_diagnostic(diagnostic_op::error_CE003(range(op.r.end)));
            return false;
        }

        terms.push({
            std::make_unique<ca_function_unary_operator>(
                std::move(right.term), op.op_type, expr_policy::set_type, op.r),
            op.i,
            false,
        });
        return true;
    }

    bool reduce_stack_entry(diagnostic_op_consumer& diags)
    {
        auto op = std::move(op_stack.top());
        op_stack.pop();
        if (terms.size() < 1 + op.binary)
        {
            diags.add_diagnostic(diagnostic_op::error_CE003(range(terms.size() < op.binary ? op.r.start : op.r.end)));
            return false;
        }
        return op.binary ? reduce_binary(op, diags) : reduce_unary(op, diags);
    }

    bool reduce_stack(diagnostic_op_consumer& diags, int prio_limit, bool right_assoc)
    {
        while (!op_stack.empty())
        {
            if (op_stack.top().priority + right_assoc > prio_limit)
                break;
            if (!reduce_stack_entry(diags))
                return false;
        }
        return true;
    }

    bool reduce_stack_all(diagnostic_op_consumer& diags)
    {
        while (!op_stack.empty())
        {
            if (!reduce_stack_entry(diags))
                return false;
        }
        return true;
    }
};

} // namespace

template<typename T>
void ca_expr_list::resolve(diagnostic_op_consumer& diags)
{
    using expr_policy = typename ca_expr_traits<T>::policy_t;

    resolve_stacks<T> stacks;

    auto i = (size_t)-1;
    bool prefer_next_term = true;
    bool last_was_terminal = false;
    for (auto& curr_expr : expr_list)
    {
        ++i;
        if (is_symbol(curr_expr))
        {
            const auto& symbol = get_symbol(curr_expr);
            auto op_type_var = expr_policy::get_operator_properties(symbol);
            if (std::holds_alternative<std::monostate>(op_type_var))
            {
                // fallback to term
            }
            else if (std::holds_alternative<invalid_by_policy>(op_type_var))
            {
                if (!prefer_next_term)
                {
                    diags.add_diagnostic(diagnostic_op::error_CE002(symbol, curr_expr->expr_range));
                    expr_list.clear();
                    return;
                }
                // fallback to term
            }
            else if (const auto& op_type = std::get<ca_expr_op>(op_type_var);
                     !(prefer_next_term && op_type.binary)) // ... AND AND is interpreted as AND term,
                                                            // ... AND NOT ... is apparently not
            {
                if (op_type.requires_terms && !last_was_terminal)
                {
                    diags.add_diagnostic(diagnostic_op::error_CE003(range(curr_expr->expr_range.start)));
                    expr_list.clear();
                    return;
                }
                if (!stacks.reduce_stack(diags, op_type.priority, op_type.right_assoc))
                {
                    expr_list.clear();
                    return;
                }
                stacks.push_op({
                    i,
                    op_type.op,
                    op_type.priority,
                    op_type.binary,
                    op_type.right_assoc,
                    op_type.requires_terms,
                    curr_expr->expr_range,
                });
                prefer_next_term = op_type.binary || op_type.requires_terms;
                last_was_terminal = false;
                continue;
            }
        }
        stacks.push_term({ std::move(curr_expr), i, true });
        prefer_next_term = false;
        last_was_terminal = true;
    }

    if (!stacks.reduce_stack_all(diags))
    {
        expr_list.clear();
        return;
    }
    if (stacks.terms.empty())
    {
        diags.add_diagnostic(diagnostic_op::error_CE003(expr_range));
        expr_list.clear();
        return;
    }
    if (stacks.terms.size() > 1)
    {
        diags.add_diagnostic(diagnostic_op::error_CE001(range(stacks.terms.top().term->expr_range)));
        expr_list.clear();
        return;
    }

    ca_expr_ptr final_expr = std::move(stacks.terms.top().term);

    // resolve created tree
    final_expr->resolve_expression_tree(context::object_traits<T>::type_enum, diags);

    // move resolved tree to the front of the array
    expr_list.clear();
    expr_list.emplace_back(std::move(final_expr));
}

} // namespace hlasm_plugin::parser_library::expressions
