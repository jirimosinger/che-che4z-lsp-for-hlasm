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

#include "gmock/gmock.h"

#include "expressions/conditional_assembly/terms/ca_constant.h"
#include "expressions/conditional_assembly/terms/ca_expr_list.h"
#include "expressions/conditional_assembly/terms/ca_function.h"
#include "expressions/conditional_assembly/terms/ca_string.h"
#include "expressions/conditional_assembly/terms/ca_symbol.h"
#include "expressions/conditional_assembly/terms/ca_symbol_attribute.h"
#include "expressions/evaluation_context.h"
#include "semantics/concatenation_term.h"

using namespace hlasm_plugin::parser_library::expressions;
using namespace hlasm_plugin::parser_library::semantics;
using namespace hlasm_plugin::parser_library;

TEST(ca_expr_list, unknown_function_to_operator)
{
    context::hlasm_context ctx;
    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { ctx, workspaces::empty_parse_lib_provider::instance, diags };

    std::string name = "AND";
    auto c = std::make_unique<ca_constant>(1, range());
    auto dupl = std::make_unique<ca_constant>(1, range());
    std::vector<ca_expr_ptr> params;
    params.emplace_back(std::move(c));
    auto unknown_func =
        std::make_unique<ca_function>(&name, ca_expr_funcs::UNKNOWN, std::move(params), std::move(dupl), range());

    std::vector<ca_expr_ptr> list;
    list.emplace_back(std::move(unknown_func));
    ca_expr_list expr_list(std::move(list), range());

    //   function  =>    operator
    // ((1)AND(1)) => ((1) AND (1))
    expr_list.resolve_expression_tree(context::SET_t_enum::B_TYPE, diags);
    auto res = expr_list.evaluate(eval_ctx);

    EXPECT_TRUE(diags.diags.empty());
    EXPECT_EQ(res.access_b(), true);
}

TEST(ca_expr_list, resolve_C_type)
{
    context::hlasm_context ctx;
    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { ctx, workspaces::empty_parse_lib_provider::instance, diags };

    std::string name = "UPPER";
    auto sym = std::make_unique<ca_symbol>(&name, range());

    concat_chain value;
    value.push_back(std::make_unique<char_str_conc>("low", range()));
    auto str = std::make_unique<ca_string>(std::move(value), nullptr, ca_string::substring_t(), range());

    std::vector<ca_expr_ptr> list;
    list.emplace_back(std::move(sym));
    list.emplace_back(std::move(str));
    // (UPPER 'low')
    ca_expr_list expr_list(std::move(list), range());
    expr_list.resolve_expression_tree(context::SET_t_enum::C_TYPE, diags);
    auto res = expr_list.evaluate(eval_ctx);

    EXPECT_TRUE(diags.diags.empty());
    EXPECT_EQ(res.access_c(), "LOW");
}

TEST(ca_expr_list, get_undefined_attributed_symbols)
{
    std::string name = "X";
    auto sym = std::make_unique<ca_symbol_attribute>(&name, context::data_attr_kind::L, range(), range());

    concat_chain value;
    value.push_back(std::make_unique<char_str_conc>("low", range()));
    auto str = std::make_unique<ca_string>(std::move(value), nullptr, ca_string::substring_t(), range());

    std::vector<ca_expr_ptr> list;
    list.emplace_back(std::move(sym));
    list.emplace_back(std::move(str));
    // (L'X 'low')
    ca_expr_list expr_list(std::move(list), range());

    context::hlasm_context ctx;
    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { ctx, workspaces::empty_parse_lib_provider::instance, diags };
    auto res = expr_list.get_undefined_attributed_symbols(eval_ctx);

    ASSERT_TRUE(res.size());
}

TEST(ca_expr_list, is_character_expression)
{
    concat_chain value;
    value.push_back(std::make_unique<char_str_conc>("low", range()));
    auto str = std::make_unique<ca_string>(std::move(value), nullptr, ca_string::substring_t(), range());

    std::vector<ca_expr_ptr> list;
    list.emplace_back(std::move(str));
    // ('low')
    ca_expr_list expr_list(std::move(list), range());

    EXPECT_TRUE(expr_list.is_character_expression(character_expression_purpose::assignment));
    EXPECT_FALSE(expr_list.is_character_expression(character_expression_purpose::left_side_of_comparison));
}

TEST(ca_expr_list, unfinished_expressions)
{
    // ()
    {
        diagnostic_op_consumer_container diags;
        ca_expr_list expr_list({}, range());
        expr_list.resolve_expression_tree(context::SET_t_enum::B_TYPE, diags);

        EXPECT_FALSE(diags.diags.empty());
    }
    // (NOT)
    {
        diagnostic_op_consumer_container diags;
        std::string name = "NOT";
        auto sym = std::make_unique<ca_symbol>(&name, range());

        std::vector<ca_expr_ptr> list;
        list.emplace_back(std::move(sym));

        ca_expr_list expr_list(std::move(list), range());
        expr_list.resolve_expression_tree(context::SET_t_enum::B_TYPE, diags);

        EXPECT_FALSE(diags.diags.empty());
    }
    // (1 AND)
    {
        diagnostic_op_consumer_container diags;
        auto c = std::make_unique<ca_constant>(1, range());

        std::string name = "AND";
        auto sym = std::make_unique<ca_symbol>(&name, range());

        std::vector<ca_expr_ptr> list;
        list.emplace_back(std::move(c));
        list.emplace_back(std::move(sym));

        ca_expr_list expr_list(std::move(list), range());
        expr_list.resolve_expression_tree(context::SET_t_enum::B_TYPE, diags);

        EXPECT_FALSE(diags.diags.empty());
    }
    // (1 AND 1 EQ)
    {
        diagnostic_op_consumer_container diags;
        auto c = std::make_unique<ca_constant>(1, range());
        auto c2 = std::make_unique<ca_constant>(1, range());

        std::string and_name = "AND";
        std::string eq_name = "EQ";
        auto and_sym = std::make_unique<ca_symbol>(&and_name, range());
        auto eq_sym = std::make_unique<ca_symbol>(&eq_name, range());

        std::vector<ca_expr_ptr> list;
        list.emplace_back(std::move(c));
        list.emplace_back(std::move(and_sym));
        list.emplace_back(std::move(c2));
        list.emplace_back(std::move(eq_sym));

        ca_expr_list expr_list(std::move(list), range());
        expr_list.resolve_expression_tree(context::SET_t_enum::B_TYPE, diags);

        EXPECT_FALSE(diags.diags.empty());
    }
}
