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

#include "gtest/gtest.h"

#include "analyzer_fixture.h"
#include "lsp_context_test_helper.h"


using namespace hlasm_plugin::parser_library;
using namespace hlasm_plugin::parser_library::lsp;

struct lsp_context_macro_in_opencode : public analyzer_fixture
{
    const static inline std::string input =
        R"(
       MACRO
&LABEL MAC &POS_PAR,&KEY_PAR=1
&LABEL LR &POS_PAR,&KEY_PAR
&
       MEND

LAB    MAC 1,KEY_PAR=1

 LCLA &KEY_PAR
 LR &KEY_PAR,1
&
       UNKNOWN
)";

    lsp_context_macro_in_opencode()
        : analyzer_fixture(input)
    {}
};

TEST_F(lsp_context_macro_in_opencode, definition_macro)
{
    location res = a.context().lsp_ctx->definition(opencode_loc, { 7, 8 });
    check_location_with_position(res, opencode_loc, 2, 7);
}

TEST_F(lsp_context_macro_in_opencode, references_macro)
{
    auto res = a.context().lsp_ctx->references(opencode_loc, { 7, 8 });
    ASSERT_EQ(res.size(), 2U);

    check_location_with_position(res[0], opencode_loc, 2, 7);
    check_location_with_position(res[1], opencode_loc, 7, 7);
}

TEST_F(lsp_context_macro_in_opencode, hover_macro)
{
    auto res = a.context().lsp_ctx->hover(opencode_loc, { 7, 8 });

    EXPECT_EQ(res, R"(```
&LABEL MAC &POS_PAR,&KEY_PAR=1

```
)");
}


TEST_F(lsp_context_macro_in_opencode, definition_macro_param_positional)
{
    location res = a.context().lsp_ctx->definition(opencode_loc, { 3, 11 });
    check_location_with_position(res, opencode_loc, 2, 11);
}

TEST_F(lsp_context_macro_in_opencode, references_macro_param_positional)
{
    auto res = a.context().lsp_ctx->references(opencode_loc, { 3, 11 });
    ASSERT_EQ(res.size(), 2U);

    check_location_with_position(res[0], opencode_loc, 2, 11);
    check_location_with_position(res[1], opencode_loc, 3, 10);
}

TEST_F(lsp_context_macro_in_opencode, hover_macro_param)
{
    auto res = a.context().lsp_ctx->hover(opencode_loc, { 3, 11 });

    EXPECT_EQ(res, "MACRO parameter");
}


TEST_F(lsp_context_macro_in_opencode, definition_macro_param_keyword)
{
    location res = a.context().lsp_ctx->definition(opencode_loc, { 3, 20 });
    check_location_with_position(res, opencode_loc, 2, 20);
}

TEST_F(lsp_context_macro_in_opencode, references_macro_param_keyword)
{
    auto res = a.context().lsp_ctx->references(opencode_loc, { 3, 20 });
    ASSERT_EQ(res.size(), 2U);

    check_location_with_position(res[0], opencode_loc, 2, 20);
    check_location_with_position(res[1], opencode_loc, 3, 19);
}

TEST_F(lsp_context_macro_in_opencode, definition_macro_param_label)
{
    location res = a.context().lsp_ctx->definition(opencode_loc, { 3, 1 });
    check_location_with_position(res, opencode_loc, 2, 0);
}

TEST_F(lsp_context_macro_in_opencode, references_macro_param_label)
{
    auto res = a.context().lsp_ctx->references(opencode_loc, { 3, 1 });
    ASSERT_EQ(res.size(), 2U);

    check_location_with_position(res[0], opencode_loc, 2, 0);
    check_location_with_position(res[1], opencode_loc, 3, 0);
}

TEST_F(lsp_context_macro_in_opencode, definition_local_var_same_name)
{
    location res = a.context().lsp_ctx->definition(opencode_loc, { 10, 5 });
    check_location_with_position(res, opencode_loc, 9, 6);
}

TEST_F(lsp_context_macro_in_opencode, references_local_var_same_name)
{
    auto res = a.context().lsp_ctx->references(opencode_loc, { 10, 5 });
    ASSERT_EQ(res.size(), 2U);

    check_location_with_position(res[0], opencode_loc, 9, 6);
    check_location_with_position(res[1], opencode_loc, 10, 4);
}


auto tie_completion_item(const lsp::completion_item_s& lhs)
{
    return std::tie(lhs.label, lhs.detail, lhs.insert_text, lhs.documentation, lhs.kind);
}

void sort_occurence_vector(std::vector<lsp::completion_item_s>& v)
{
    std::sort(v.begin(), v.end(), [](const lsp::completion_item_s& lhs, const lsp::completion_item_s rhs) {
        return tie_completion_item(lhs) < tie_completion_item(rhs);
    });
}


TEST_F(lsp_context_macro_in_opencode, completion_var_in_macro)
{
    auto res = a.context().lsp_ctx->completion(opencode_loc, { 4, 1 }, '\0', completion_trigger_kind::invoked);

    std::vector<completion_item_s> expected {
        { "&LABEL", "MACRO parameter", "&LABEL", "", completion_item_kind::var_sym },
        { "&POS_PAR", "MACRO parameter", "&POS_PAR", "", completion_item_kind::var_sym },
        { "&KEY_PAR", "MACRO parameter", "&KEY_PAR", "", completion_item_kind::var_sym }
    };

    sort_occurence_vector(res);
    sort_occurence_vector(expected);

    EXPECT_EQ(res, expected);
}

TEST_F(lsp_context_macro_in_opencode, completion_var_outside_macro)
{
    auto res = a.context().lsp_ctx->completion(opencode_loc, { 11, 1 }, '\0', completion_trigger_kind::invoked);

    std::vector<completion_item_s> expected {
        { "&KEY_PAR", "SETA variable", "&KEY_PAR", "", completion_item_kind::var_sym }
    };

    sort_occurence_vector(res);
    sort_occurence_vector(expected);

    EXPECT_EQ(res, expected);
}

TEST_F(lsp_context_macro_in_opencode, hover_unknown_macro)
{
    auto res = a.context().lsp_ctx->hover(opencode_loc, { 12, 10 });

    EXPECT_EQ(res, "");
}
