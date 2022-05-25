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

#include "../common_testing.h"
#include "../mock_parse_lib_provider.h"

using namespace hlasm_plugin::utils::path;

// test for COPY instruction
// various cases of instruction occurence in the source
namespace {
mock_parse_lib_provider create_copy_mock()
{
    static const std::string content_COPYR =
        R"(   
 LR 1,1
 MACRO
 M1
 LR 1,1
 
 MACRO
 M2
 LR 2,2
 MEND
 AGO .A
.A ANOP
 MEND

&VARX SETA &VARX+1
.A ANOP
.B ANOP
&VAR SETA &VAR+1
)";
    static const std::string content_COPYF =
        R"(  
 LR 1,1
&VARX SETA &VARX+1
 COPY COPYR
&VAR SETA &VAR+1
.C ANOP
)";

    static const std::string content_COPYD =
        R"(  

 LR 1,
)";

    static const std::string content_COPYREC =
        R"(  
 ANOP
 COPY COPYREC
 ANOP
)";

    static const std::string content_COPYU =
        R"(  
 ANOP
 MACRO
 M
 MEND
 MEND
 ANOP
)";

    static const std::string content_COPYL =
        R"(  
 LR 1,1
.A ANOP
&VARX SETA &VARX+1
 AGO .X
&VAR SETA &VAR+1
.A ANOP
.C ANOP
)";

    static const std::string content_COPYN =
        R"( 
 MAC
)";

    static const std::string content_MAC =
        R"( MACRO
 MAC
 LR 1,1
 COPY COPYM
 MEND
)";

    static const std::string content_COPYM =
        R"(
.A ANOP
 GBLA &X
&X SETA 4
)";

    static const std::string content_COPYJ =
        R"(
 AGO .X
 ;%
.X ANOP
)";
    static const std::string content_COPYJF =
        R"(
 AGO .X
 LR
)";

    static const std::string content_COPYND1 =
        R"(
 COPY COPYND2
)";

    static const std::string content_COPYND2 =
        R"(



 LR 1,)";

    static const std::string content_COPYBM =
        R"( 
 MACRO
 M
 LR 1
 MEND
)";
    static const std::string content_EMPTY = "";
    static const std::string content_COPYEMPTY = " COPY EMPTY";

    return mock_parse_lib_provider { { "COPYR", content_COPYR },
        { "COPYF", content_COPYF },
        { "COPYD", content_COPYD },
        { "COPYREC", content_COPYREC },
        { "COPYU", content_COPYU },
        { "COPYL", content_COPYL },
        { "COPYN", content_COPYN },
        { "MAC", content_MAC },
        { "COPYM", content_COPYM },
        { "COPYJ", content_COPYJ },
        { "COPYJF", content_COPYJF },
        { "COPYND1", content_COPYND1 },
        { "COPYND2", content_COPYND2 },
        { "COPYBM", content_COPYBM },
        { "EMPTY", content_EMPTY },
        { "COPYEMPTY", content_COPYEMPTY } };
}

external_resource start("start");
external_resource copybm("COPYBM");
external_resource copyd("COPYD");
external_resource copyjf("COPYJF");
external_resource copyl("COPYL");
external_resource copynd1("COPYND1");
external_resource copynd2("COPYND2");
external_resource copyr("COPYR");

void check_diag(const hlasm_plugin::parser_library::diagnostic_s& diag, size_t expected_line, const external_resource& expected_file)
{
    EXPECT_EQ(diag.diag_range.start.line, expected_line);
    EXPECT_EQ(diag.file_name, expected_file.get_url());
}

void check_related_diag(
    const hlasm_plugin::parser_library::diagnostic_related_info_s& diag, size_t expected_line, const external_resource& expected_file)
{
    EXPECT_EQ(diag.location.rang.start.line, expected_line);
    EXPECT_EQ(diag.location.uri, expected_file.get_url());
}

} // namespace
TEST(copy, copy_enter_fail)
{
    std::string input =
        R"(
 COPY A+1
 COPY UNKNOWN
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)0);
    EXPECT_EQ(a.hlasm_ctx().whole_copy_stack().size(), (size_t)0);

    EXPECT_EQ(a.diags().size(), (size_t)2);
}

TEST(copy, copy_enter_success)
{
    std::string input =
        R"(
 COPY COPYR
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.hlasm_ctx().macros().size(), (size_t)1);

    EXPECT_TRUE(a.hlasm_ctx().get_sequence_symbol(a.hlasm_ctx().ids().add("A")));

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(copy, copy_enter_diag_test)
{
    std::string input =
        R"(
 COPY COPYD
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    ASSERT_EQ(a.diags().size(), (size_t)1);

    const auto& diag = a.diags()[0];

    check_diag(diag, 2, copyd);
    EXPECT_EQ(diag.related.size(), (size_t)1);
    check_related_diag(diag.related[0], 1, start);
}

TEST(copy, copy_jump)
{
    std::string input =
        R"(
&VARX SETA 0
&VAR SETA 0
 COPY COPYF
 AIF (&VAR LT 4).A
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)2);

    EXPECT_EQ(a.diags().size(), (size_t)0);

    EXPECT_EQ(get_var_value<context::A_t>(a.hlasm_ctx(), "VAR"), 4);
    EXPECT_EQ(get_var_value<context::A_t>(a.hlasm_ctx(), "VARX"), 2);
}

TEST(copy, copy_unbalanced_macro)
{
    std::string input =
        R"(
 COPY COPYU
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)0);

    EXPECT_EQ(lib_provider.analyzers.count("COPYU"), 1U);
    EXPECT_EQ(lib_provider.analyzers["COPYU"]->diags().size(), 1U);
}

TEST(copy, copy_twice)
{
    std::string input =
        R"(
 COPY COPYR
 COPY COPYR
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)2);
}

TEST(copy, macro_call_from_copy_enter)
{
    std::string input =
        R"(
 COPY COPYR
 M1
 M2
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)0);

    EXPECT_EQ(a.hlasm_ctx().macros().size(), (size_t)2);
}

TEST(copy, copy_enter_from_macro_call)
{
    std::string input =
        R"(
 MACRO
 M
 LR 1,1
.B ANOP
 COPY COPYR
 MEND

 M
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.hlasm_ctx().macros().size(), (size_t)2);

    auto mac = a.hlasm_ctx().macros().find(a.hlasm_ctx().ids().add("M"));
    ASSERT_TRUE(mac != a.hlasm_ctx().macros().end());

    EXPECT_TRUE(mac->second->labels.find(a.hlasm_ctx().ids().add("A")) != mac->second->labels.end());
    EXPECT_TRUE(mac->second->labels.find(a.hlasm_ctx().ids().add("B")) != mac->second->labels.end());

    ASSERT_EQ(mac->second->used_copy_members.size(), 1U);
    EXPECT_EQ(mac->second->used_copy_members.begin()->get()->name, a.hlasm_ctx().ids().add("COPYR"));

    ASSERT_EQ(a.diags().size(), (size_t)1);

    check_diag(a.diags()[0], 16, copyr);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)1);
    check_related_diag(a.diags()[0].related[0], 5, start);
}

TEST(copy, copy_enter_from_lookahead)
{
    std::string input =
        R"(
&V SETA 0
 AGO .C
&V SETA &V+1
 COPY COPYL
&V SETA &V+1
 
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(get_var_value<context::A_t>(a.hlasm_ctx(), "V"), 1);

    ASSERT_EQ(a.diags().size(), (size_t)1);

    check_diag(a.diags()[0], 6, copyl);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)1);
    check_related_diag(a.diags()[0].related[0], 4, start);
}

TEST(copy, nested_macro_copy_call)
{
    std::string input =
        R"(
 COPY COPYN
 
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)2);
    ASSERT_EQ(a.hlasm_ctx().macros().size(), (size_t)1);
    auto mac = a.hlasm_ctx().macros().find(a.hlasm_ctx().ids().add("MAC"));

    EXPECT_TRUE(mac->second->labels.find(a.hlasm_ctx().ids().add("A")) != mac->second->labels.end());

    EXPECT_EQ(a.hlasm_ctx()
                  .globals()
                  .find(a.hlasm_ctx().ids().add("X"))
                  ->second->access_set_symbol_base()
                  ->access_set_symbol<context::A_t>()
                  ->get_value(),
        4);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(copy, macro_from_copy_call)
{
    std::string input =
        R"(
 COPY COPYBM
 M
 
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);
    ASSERT_EQ(a.hlasm_ctx().macros().size(), (size_t)1);
    auto mac = a.hlasm_ctx().macros().find(a.hlasm_ctx().ids().add("M"));
    ASSERT_NE(a.hlasm_ctx().macros().end(), mac);

    ASSERT_EQ(a.diags().size(), (size_t)1);

    check_diag(a.diags()[0], 3, copybm);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)1);
    check_related_diag(a.diags()[0].related[0], 2, start);
}

TEST(copy, inner_copy_jump)
{
    std::string input =
        R"(
 COPY COPYJ
 LR
 
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)1);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);
}

TEST(copy, jump_from_copy_fail)
{
    std::string input =
        R"(
 COPY COPYJF
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)2);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);

    check_diag(a.diags()[1], 2, copyjf);
    ASSERT_EQ(a.diags()[1].related.size(), (size_t)1);
    check_related_diag(a.diags()[1].related[0], 1, start);

    check_diag(a.diags()[0], 1, copyjf);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)1);
    check_related_diag(a.diags()[0].related[0], 1, start);
}

TEST(copy, jump_in_macro_from_copy_fail)
{
    std::string input =
        R"(
 MACRO
 m
 copy copyJF
 mend

 m
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)2);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);

    check_diag(a.diags()[0], 1, copyjf);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)2);
    check_related_diag(a.diags()[0].related[0], 3, start);
    check_related_diag(a.diags()[1].related[1], 6, start);
}

TEST(copy, macro_nested_diagnostics)
{
    std::string input =
        R"( MACRO
 MAC

 copy COPYND1

 MEND
 
 MAC  
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)2);

    EXPECT_EQ(a.diags().size(), (size_t)1);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);

    check_diag(a.diags()[0], 4, copynd2);
    ASSERT_EQ(a.diags()[0].related.size(), (size_t)3);
    check_related_diag(a.diags()[0].related[0], 1, copynd1);
    check_related_diag(a.diags()[0].related[1], 3, start);
    check_related_diag(a.diags()[0].related[2], 7, start);
}

TEST(copy, copy_call_with_jump_before_comment)
{
    std::string input =
        R"( 
 COPY COPYJ
***
 ANOP
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)0);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);
}

TEST(copy, copy_empty_file)
{
    std::string input =
        R"(
 MACRO
 M
 COPY COPYEMPTY
 MEND

 MACRO
 M2
 COPY EMPTY
 MEND
)";
    auto lib_provider = create_copy_mock();
    analyzer a(input, analyzer_options { "start", &lib_provider });
    a.analyze();

    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx().copy_members().size(), (size_t)2);

    EXPECT_EQ(a.hlasm_ctx().macros().size(), (size_t)2);

    auto mac = a.hlasm_ctx().get_macro_definition(a.hlasm_ctx().ids().add("M"));
    ASSERT_TRUE(mac != nullptr);

    ASSERT_EQ(mac->used_copy_members.size(), 2U);
    EXPECT_EQ(mac->used_copy_members.count(a.hlasm_ctx().get_copy_member(a.hlasm_ctx().ids().add("EMPTY"))), 1U);
    EXPECT_EQ(mac->used_copy_members.count(a.hlasm_ctx().get_copy_member(a.hlasm_ctx().ids().add("COPYEMPTY"))), 1U);

    auto mac2 = a.hlasm_ctx().get_macro_definition(a.hlasm_ctx().ids().add("M2"));
    ASSERT_TRUE(mac2 != nullptr);

    ASSERT_EQ(mac2->used_copy_members.size(), 1U);
    EXPECT_EQ(mac2->used_copy_members.count(a.hlasm_ctx().get_copy_member(a.hlasm_ctx().ids().add("EMPTY"))), 1U);

    ASSERT_EQ(a.diags().size(), 0U);
}

TEST(copy, skip_invalid)
{
    std::string copybook_content = R"(
        AGO .SKIP
        2 a
a       a                                                              X
aaaaaaaaa
.SKIP   ANOP
)";
    std::string input = R"(
        COPY COPYBOOK
)";
    mock_parse_lib_provider lib_provider { { "COPYBOOK", copybook_content } };
    analyzer a(input, analyzer_options { &lib_provider });

    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(a.diags().empty());
}
