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

#include "ca_symbol_attribute.h"

#include "ca_var_sym.h"
#include "context/literal_pool.h"
#include "context/ordinary_assembly/dependable.h"
#include "context/ordinary_assembly/ordinary_assembly_dependency_solver.h"
#include "ebcdic_encoding.h"
#include "expressions/conditional_assembly/ca_expr_visitor.h"
#include "expressions/evaluation_context.h"
#include "hlasmparser.h"
#include "lexing/lexer.h"
#include "lexing/token_stream.h"
#include "parsing/parser_impl.h"
#include "processing/op_code.h"
#include "semantics/range_provider.h"
#include "semantics/statement_fields.h"

namespace hlasm_plugin::parser_library::expressions {

context::SET_t_enum get_attribute_type(context::data_attr_kind attr)
{
    switch (attr)
    {
        case context::data_attr_kind::T:
        case context::data_attr_kind::O:
            return context::SET_t_enum::C_TYPE;
        case context::data_attr_kind::L:
        case context::data_attr_kind::S:
        case context::data_attr_kind::I:
        case context::data_attr_kind::K:
        case context::data_attr_kind::N:
        case context::data_attr_kind::D:
            return context::SET_t_enum::A_TYPE;
        default:
            return context::SET_t_enum::UNDEF_TYPE;
    }
}

ca_symbol_attribute::ca_symbol_attribute(
    context::id_index symbol, context::data_attr_kind attribute, range expr_range, range symbol_rng)
    : ca_expression(get_attribute_type(attribute), std::move(expr_range))
    , attribute(attribute)
    , symbol(symbol)
    , symbol_range(symbol_rng)
{}

ca_symbol_attribute::ca_symbol_attribute(
    semantics::vs_ptr symbol, context::data_attr_kind attribute, range expr_range, range symbol_rng)
    : ca_expression(get_attribute_type(attribute), std::move(expr_range))
    , attribute(attribute)
    , symbol(std::move(symbol))
    , symbol_range(symbol_rng)
{}

ca_symbol_attribute::ca_symbol_attribute(
    semantics::literal_si lit, context::data_attr_kind attribute, range expr_range, range symbol_rng)
    : ca_expression(get_attribute_type(attribute), std::move(expr_range))
    , attribute(attribute)
    , symbol(std::move(lit))
    , symbol_range(symbol_rng)
{}

undef_sym_set ca_symbol_attribute::get_undefined_attributed_symbols(const evaluation_context& eval_ctx) const
{
    if (std::holds_alternative<context::id_index>(symbol))
    {
        if (context::symbol_attributes::is_ordinary_attribute(attribute)
            && !eval_ctx.hlasm_ctx.ord_ctx.get_symbol(std::get<context::id_index>(symbol))
            && !eval_ctx.hlasm_ctx.ord_ctx.get_symbol_reference(std::get<context::id_index>(symbol)))
            return { std::get<context::id_index>(symbol) };
        return undef_sym_set();
    }
    else if (std::holds_alternative<semantics::vs_ptr>(symbol))
    {
        const auto& vs = std::get<semantics::vs_ptr>(symbol);

        auto undef_syms = ca_var_sym::get_undefined_attributed_symbols_vs(vs, eval_ctx);

        if (undef_syms.empty() && context::symbol_attributes::is_ordinary_attribute(attribute))
        {
            context::SET_t substituted_name = vs->evaluate(eval_ctx);

            if (substituted_name.type != context::SET_t_enum::C_TYPE)
                return {};

            auto [valid, ord_name] =
                eval_ctx.hlasm_ctx.try_get_symbol_name(try_extract_leading_symbol(substituted_name.access_c()));

            if (!valid)
                return {};

            if (context::symbol_attributes::is_ordinary_attribute(attribute)
                && !eval_ctx.hlasm_ctx.ord_ctx.get_symbol(ord_name)
                && !eval_ctx.hlasm_ctx.ord_ctx.get_symbol_reference(ord_name))
                return { ord_name };
        }
        return undef_syms;
    }
    else if (std::holds_alternative<semantics::literal_si>(symbol))
    {
        // everything needs to be defined
        return undef_sym_set();
    }
    else
    {
        assert(false);
        return undef_sym_set();
    }
}

void ca_symbol_attribute::resolve_expression_tree(context::SET_t_enum kind, diagnostic_op_consumer& diags)
{
    if (kind == context::SET_t_enum::C_TYPE && kind != expr_kind)
        diags.add_diagnostic(diagnostic_op::error_CE004(expr_range));
    else if (std::holds_alternative<semantics::vs_ptr>(symbol))
        std::get<semantics::vs_ptr>(symbol)->resolve(diags);
}

bool ca_symbol_attribute::is_character_expression(character_expression_purpose) const
{
    return get_attribute_type(attribute) == context::SET_t_enum::C_TYPE;
}

void ca_symbol_attribute::apply(ca_expr_visitor& visitor) const { visitor.visit(*this); }

context::SET_t ca_symbol_attribute::evaluate(const evaluation_context& eval_ctx) const
{
    if (std::holds_alternative<context::id_index>(symbol))
    {
        return evaluate_ordsym(std::get<context::id_index>(symbol), eval_ctx);
    }

    if (std::holds_alternative<semantics::vs_ptr>(symbol))
    {
        return evaluate_varsym(std::get<semantics::vs_ptr>(symbol), eval_ctx);
    }

    if (std::holds_alternative<semantics::literal_si>(symbol))
    {
        return evaluate_literal(std::get<semantics::literal_si>(symbol), eval_ctx);
    }

    return context::SET_t(expr_kind);
}
std::string ca_symbol_attribute::try_extract_leading_symbol(std::string_view expr)
{
    // remove parentheses
    while (!expr.empty() && expr.front() == '(' && expr.back() == ')')
    {
        expr.remove_prefix(1);
        expr.remove_suffix(1);
    }

    // remove leading using prefixes
    for (auto p = expr.find_first_of('.'); p != std::string_view::npos && !std::isdigit((unsigned char)expr.front())
         && std::all_of(expr.begin(), expr.begin() + p, lexing::lexer::ord_char);
         p = expr.find_first_of('.'))
        expr.remove_prefix(p + 1);

    // try to isolate one ordinary symbol
    if (!expr.empty() && !std::isdigit((unsigned char)expr.front()) && lexing::lexer::ord_char(expr.front()))
    {
        if (auto d = expr.find_first_of("+-*/()"); d != std::string_view::npos)
            expr = expr.substr(0, d);
    }
    return std::string(expr);
}

context::SET_t ca_symbol_attribute::get_ordsym_attr_value(
    context::id_index name, const evaluation_context& eval_ctx) const
{
    const context::symbol* ord_symbol = eval_ctx.hlasm_ctx.ord_ctx.get_symbol(name);

    if (!ord_symbol)
        ord_symbol = eval_ctx.hlasm_ctx.ord_ctx.get_symbol_reference(name);

    return retrieve_value(ord_symbol, eval_ctx);
}

context::SET_t ca_symbol_attribute::retrieve_value(
    const context::symbol* ord_symbol, const evaluation_context& eval_ctx) const
{
    if (attribute == context::data_attr_kind::T)
        return eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, ord_symbol);

    if (!ord_symbol)
    {
        eval_ctx.diags.add_diagnostic(diagnostic_op::warning_W013(expr_range));
        return context::symbol_attributes::default_value(attribute);
    }

    if ((attribute == context::data_attr_kind::S || attribute == context::data_attr_kind::I)
        && !ord_symbol->attributes().can_have_SI_attr())
    {
        eval_ctx.diags.add_diagnostic(diagnostic_op::error_E066(expr_range));
        return context::symbol_attributes::default_value(attribute);
    }

    if (!ord_symbol->attributes().is_defined(attribute))
    {
        eval_ctx.diags.add_diagnostic(diagnostic_op::warning_W013(expr_range));
        return context::symbol_attributes::default_value(attribute);
    }

    return eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, ord_symbol);
}

context::SET_t ca_symbol_attribute::evaluate_ordsym(context::id_index name, const evaluation_context& eval_ctx) const
{
    if (context::symbol_attributes::is_ordinary_attribute(attribute))
    {
        return get_ordsym_attr_value(name, eval_ctx);
    }
    else if (attribute == context::data_attr_kind::D)
    {
        return eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, name);
    }
    else if (attribute == context::data_attr_kind::O)
    {
        auto tmp = eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, name);
        if (tmp.access_c() == "U" && eval_ctx.lib_provider.has_library(*name, eval_ctx.hlasm_ctx.opencode_file_name()))
            return std::string("S");
        return tmp;
    }
    else
    {
        eval_ctx.diags.add_diagnostic(diagnostic_op::error_E066(expr_range));
        return context::symbol_attributes::default_ca_value(attribute);
    }
}

context::SET_t ca_symbol_attribute::evaluate_literal(
    const semantics::literal_si& lit, const evaluation_context& eval_ctx) const
{
    auto& literals = eval_ctx.hlasm_ctx.ord_ctx.literals();

    if (attribute == context::data_attr_kind::D)
        return literals.defined_for_ca_expr(std::shared_ptr<const expressions::data_definition>(lit, &lit->get_dd()));

    literals.mentioned_in_ca_expr(std::shared_ptr<const expressions::data_definition>(lit, &lit->get_dd()));

    if (attribute == context::data_attr_kind::O)
        return "U";
    else if (attribute == context::data_attr_kind::T)
        return std::string { lit->get_dd().get_type_attribute() };
    else
    {
        context::ordinary_assembly_dependency_solver solver(eval_ctx.hlasm_ctx.ord_ctx, context::address());
        context::symbol_attributes attrs = lit->get_dd().get_symbol_attributes(solver, eval_ctx.diags);
        if ((attribute == context::data_attr_kind::S || attribute == context::data_attr_kind::I)
            && !attrs.can_have_SI_attr())
        {
            eval_ctx.diags.add_diagnostic(diagnostic_op::warning_W011(symbol_range));
            return 0;
        }
        return attrs.get_attribute_value(attribute);
    }
}

std::vector<size_t> transform(const std::vector<context::A_t>& v)
{
    std::vector<size_t> ret;
    for (auto val : v)
        ret.push_back((size_t)val);
    return ret;
}

context::SET_t ca_symbol_attribute::evaluate_varsym(
    const semantics::vs_ptr& vs, const evaluation_context& eval_ctx) const
{
    auto [var_name, expr_subscript] = vs->evaluate_symbol(eval_ctx);

    // get symbol
    auto var_symbol = eval_ctx.hlasm_ctx.get_var_sym(var_name);

    if (!var_symbol)
    {
        eval_ctx.diags.add_diagnostic(diagnostic_op::error_E010("variable", vs->symbol_range));
        return context::symbol_attributes::default_ca_value(attribute);
    }

    // must substitute var sym
    if (context::symbol_attributes::requires_ordinary_symbol(attribute))
    {
        return evaluate_substituted(var_name, std::move(expr_subscript), vs->symbol_range, eval_ctx);
    }
    else if (attribute == context::data_attr_kind::T)
    {
        if (!test_symbol_for_read(var_symbol, expr_subscript, vs->symbol_range, eval_ctx.diags))
            return std::string("U");

        context::SET_t value =
            eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, var_symbol, transform(expr_subscript)).access_c();

        if (value.access_c() != "U")
            return value;
        return evaluate_substituted(
            var_name, std::move(expr_subscript), vs->symbol_range, eval_ctx); // is type U, must substitute var sym
    }
    else
    {
        if (attribute == context::data_attr_kind::K
            && !test_symbol_for_read(var_symbol, expr_subscript, vs->symbol_range, eval_ctx.diags))
            return context::symbol_attributes::default_ca_value(attribute);
        return eval_ctx.hlasm_ctx.get_attribute_value_ca(attribute, var_symbol, transform(expr_subscript));
    }
}

context::SET_t ca_symbol_attribute::evaluate_substituted(context::id_index var_name,
    std::vector<context::A_t> expr_subscript,
    range var_range,
    const evaluation_context& eval_ctx) const
{
    context::SET_t substituted_name =
        get_var_sym_value(eval_ctx.hlasm_ctx, var_name, expr_subscript, var_range, eval_ctx.diags);

    if (substituted_name.type != context::SET_t_enum::C_TYPE)
    {
        if (attribute != context::data_attr_kind::O && attribute != context::data_attr_kind::T)
            eval_ctx.diags.add_diagnostic(diagnostic_op::error_E066(expr_range));
        return context::symbol_attributes::default_ca_value(attribute);
    }

    const auto& text = substituted_name.access_c();

    if (!text.empty() && text.starts_with('='))
        return evaluate_substituted_literal(text, var_range, eval_ctx);

    auto [valid, ord_name] = eval_ctx.hlasm_ctx.try_get_symbol_name(try_extract_leading_symbol(text));

    if (!valid)
    {
        if (attribute != context::data_attr_kind::O && attribute != context::data_attr_kind::T)
            eval_ctx.diags.add_diagnostic(diagnostic_op::error_E065(expr_range));
        return context::symbol_attributes::default_ca_value(attribute);
    }
    else
        return evaluate_ordsym(ord_name, eval_ctx);
}

context::SET_t ca_symbol_attribute::evaluate_substituted_literal(
    const std::string& text, range var_range, const evaluation_context& eval_ctx) const
{
    // error production is suppressed when evaluating D', T' and O' attributes
    using attr_kind = context::data_attr_kind;
    const bool suppress = attribute == attr_kind::T || attribute == attr_kind::D || attribute == attr_kind::O;
    bool error = false;
    diagnostic_consumer_transform add_diag_subst([&text, &eval_ctx, suppress, &error](diagnostic_op diag) {
        error = true;
        if (suppress)
            return;
        diag.message = diagnostic_decorate_message(text, diag.message);
        eval_ctx.diags.add_diagnostic(std::move(diag));
    });
    auto h = parsing::parser_holder::create(nullptr, &eval_ctx.hlasm_ctx, &add_diag_subst);

    h->input->reset(text);

    h->lex->reset();
    h->lex->set_file_offset(var_range.start);
    h->lex->set_unlimited_line(true);

    h->stream->reset();

    h->parser->reinitialize(&eval_ctx.hlasm_ctx,
        semantics::range_provider(var_range, semantics::adjusting_state::SUBSTITUTION),
        processing::processing_status(processing::processing_format(processing::processing_kind::ORDINARY,
                                          processing::processing_form::CA,
                                          processing::operand_occurence::ABSENT),
            processing::op_code()),
        &add_diag_subst);

    h->parser->reset();

    h->parser->get_collector().prepare_for_next_statement();

    auto literal_context = h->parser->literal_reparse();

    if (!error && literal_context->value)
        return evaluate_literal(literal_context->value, eval_ctx);
    else
        return context::symbol_attributes::default_ca_value(attribute);
}

} // namespace hlasm_plugin::parser_library::expressions
