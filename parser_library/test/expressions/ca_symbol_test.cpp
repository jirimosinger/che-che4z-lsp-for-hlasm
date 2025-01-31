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

#include "diagnostic_adder.h"
#include "expressions/conditional_assembly/terms/ca_symbol.h"
#include "expressions/evaluation_context.h"

using namespace hlasm_plugin::parser_library::expressions;
using namespace hlasm_plugin::parser_library::semantics;
using namespace hlasm_plugin::parser_library;

TEST(ca_symbol, undefined_attributes)
{
    context::hlasm_context ctx;
    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { ctx, workspaces::empty_parse_lib_provider::instance, diags };
    std::string name = "n";

    ca_symbol sym(&name, range());

    auto res = sym.get_undefined_attributed_symbols(eval_ctx);

    ASSERT_EQ(res.size(), 0U);
}

TEST(ca_symbol, resolve_expr_tree)
{
    diagnostic_adder add_diags;

    ca_symbol sym(nullptr, range());
    diagnostic_op_consumer_container diags;

    sym.resolve_expression_tree(context::SET_t_enum::C_TYPE, diags);

    EXPECT_FALSE(diags.diags.empty());
}

TEST(ca_symbol, is_char)
{
    EXPECT_FALSE(ca_symbol(nullptr, range()).is_character_expression(character_expression_purpose::assignment));
    EXPECT_FALSE(
        ca_symbol(nullptr, range()).is_character_expression(character_expression_purpose::left_side_of_comparison));
}
