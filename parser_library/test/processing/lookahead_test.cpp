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

#include "../common_testing.h"
#include "../mock_parse_lib_provider.h"
#include "hlasmparser.h"

// tests for lookahead feature:
// forward/backward jums
// copy/macro jumps
// sequence symbol/attribute lookahead

TEST(lookahead, forward_jump_success)
{
    std::string input(
        R"( 
   AGO .A  
&new seta 1 
.A ANOP
)");

    analyzer a(input);
    a.analyze();

    auto id = a.hlasm_ctx().ids().add("new");
    auto var = a.hlasm_ctx().get_var_sym(id);
    EXPECT_FALSE(var);
}

TEST(lookahead, forward_jump_to_continued)
{
    std::string input(
        R"( 
      AGO      .HERE
&bad seta 1
.HERE LR                                                               x
               1,1
&good seta 1
      LR 1,1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();
    EXPECT_EQ(a.diags().size(), (size_t)0);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);

    EXPECT_FALSE(a.hlasm_ctx().get_var_sym(a.hlasm_ctx().ids().add("bad")));
    EXPECT_TRUE(a.hlasm_ctx().get_var_sym(a.hlasm_ctx().ids().add("good")));
}

TEST(lookahead, forward_jump_from_continued)
{
    std::string input(
        R"( 
      AGO                                                              x
               .HERE
&bad seta 1
.HERE LR                                                               x
               1,1
&good seta 1
      LR 1,1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();
    EXPECT_EQ(a.diags().size(), (size_t)0);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);

    EXPECT_FALSE(a.hlasm_ctx().get_var_sym(a.hlasm_ctx().ids().add("bad")));
    EXPECT_TRUE(a.hlasm_ctx().get_var_sym(a.hlasm_ctx().ids().add("good")));
}

TEST(lookahead, forward_jump_success_valid_input)
{
    std::string input(
        R"( 
   AGO .A  
&new seta 1 
das cvx
tr9023-22
=f2 **
.A ANOP)");

    analyzer a(input);
    a.analyze();

    auto id = a.hlasm_ctx().ids().add("new");
    auto var = a.hlasm_ctx().get_var_sym(id);
    EXPECT_FALSE(var);
    EXPECT_EQ(a.debug_syntax_errors(), (size_t)0);
}

TEST(lookahead, forward_jump_fail)
{
    std::string input(
        R"( 
   AGO .A  
&new seta 1 
.B ANOP
)");

    analyzer a(input);
    a.analyze();

    auto id = a.hlasm_ctx().ids().add("new");
    auto var = a.hlasm_ctx().get_var_sym(id);
    EXPECT_TRUE(var);
}

TEST(lookahead, rewinding_from_last_line)
{
    std::string input(
        R"( 
 ACTR 2 
.A ANOP
 AGO .A)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(lookahead, rewinding_from_one_from_last_line)
{
    std::string input(
        R"( 
 ACTR 2 
.A ANOP
 AGO .A
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(lookahead, forward_jump_before_comment)
{
    std::string input(
        R"( 
 AGO .A
 BAD_INSTR
*COMMENT
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(lookahead, forward_jump_before_continued_comment)
{
    std::string input(
        R"( 
 AGO .A
 BAD_INSTR
*COMMENT                                                               X IGNORED
                 COMMENT
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_triggered)
{
    std::string input("L'X");
    analyzer a(input);
    auto& expr = a.parser().expr()->ca_expr;

    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { a.hlasm_ctx(), workspaces::empty_parse_lib_provider::instance, diags };

    EXPECT_EQ(expr->get_undefined_attributed_symbols(eval_ctx).size(), (size_t)1);

    EXPECT_EQ(diags.diags.size(), (size_t)0);
}

TEST(attribute_lookahead, nested_lookup_triggered)
{
    std::string input("L'&V1(L'&V2)");
    analyzer a(input);
    auto& expr = a.parser().expr()->ca_expr;

    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { a.hlasm_ctx(), workspaces::empty_parse_lib_provider::instance, diags };

    auto v1 = a.hlasm_ctx().create_local_variable<context::C_t>(a.hlasm_ctx().ids().add("V1"), false);
    v1->access_set_symbol<context::C_t>()->set_value("A", 0);
    auto v2 = a.hlasm_ctx().create_local_variable<context::C_t>(a.hlasm_ctx().ids().add("V2"), true);
    v2->access_set_symbol<context::C_t>()->set_value("B");

    auto res = expr->get_undefined_attributed_symbols(eval_ctx);
    ASSERT_EQ(res.size(), (size_t)1);
    EXPECT_TRUE(res.find(a.hlasm_ctx().ids().add("B")) != res.end());

    a.hlasm_ctx().ord_ctx.add_symbol_reference(context::symbol(a.hlasm_ctx().ids().add("B"),
        context::symbol_value(),
        context::symbol_attributes(context::symbol_origin::EQU, 'U'_ebcdic, 1),
        location(),
        {}));

    res = expr->get_undefined_attributed_symbols(eval_ctx);
    ASSERT_EQ(res.size(), (size_t)1);
    EXPECT_TRUE(res.find(a.hlasm_ctx().ids().add("A")) != res.end());

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_not_triggered)
{
    std::string input("L'X");
    analyzer a(input);
    auto& expr = a.parser().expr()->ca_expr;

    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { a.hlasm_ctx(), workspaces::empty_parse_lib_provider::instance, diags };

    // define symbol with undefined length
    auto tmp = a.hlasm_ctx().ord_ctx.create_symbol(
        a.hlasm_ctx().ids().add("X"), symbol_value(), symbol_attributes(symbol_origin::DAT, 200), {});
    ASSERT_TRUE(tmp);

    // although length is undefined the actual symbol is defined so no lookup should happen
    EXPECT_EQ(expr->get_undefined_attributed_symbols(eval_ctx).size(), (size_t)0);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_of_two_refs)
{
    std::string input("L'X+L'Y");
    analyzer a(input);
    auto& expr = a.parser().expr()->ca_expr;

    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { a.hlasm_ctx(), workspaces::empty_parse_lib_provider::instance, diags };

    EXPECT_EQ(expr->get_undefined_attributed_symbols(eval_ctx).size(), (size_t)2);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_of_two_refs_but_one_symbol)
{
    std::string input("S'X+L'X");
    analyzer a(input);
    auto& expr = a.parser().expr()->ca_expr;

    diagnostic_op_consumer_container diags;
    evaluation_context eval_ctx { a.hlasm_ctx(), workspaces::empty_parse_lib_provider::instance, diags };

    EXPECT_EQ(expr->get_undefined_attributed_symbols(eval_ctx).size(), (size_t)1);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU_attribute_lookahead, correct_attribute_refereces)
{
    std::string input(
        R"( 
&A SETC T'X
&B SETA L'X
X EQU 1,10,C'T'
&C SETA L'Y
Y EQU X+1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "A"), "T");
    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "B"), 10);
    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "C"), 10);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU_attribute_lookahead, incorrect_attribute_reference)
{
    std::string input(
        R"( 
&A SETA S'X
X EQU 1,10,C'T'
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 0);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(EQU_attribute_lookahead, unresolvable_attribute_reference)
{
    std::string input(
        R"( 
&A SETA L'X
X EQU 1,Y+11,C'T'
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 1);

    EXPECT_EQ(a.diags().size(), (size_t)1);
    EXPECT_EQ(a.diags().front().diag_range.start.line, (size_t)2);
}

TEST(EQU_attribute_lookahead, errorous_but_resolable_statement_incorrect_operand)
{
    std::string input(
        R"( 
&A SETA L'X
 AGO .A
X EQU 1,2,**&
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 2);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU_attribute_lookahead, errorous_but_resolable_statement_last_operand_model)
{
    std::string input(
        R"( 
&A SETA L'X
 AGO .A
X EQU 1,2,&a
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 2);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU_attribute_lookahead, errorous_but_unresolable_statement_first_operand_model)
{
    std::string input(
        R"( 
&A SETA L'X
 AGO .A
X EQU &a,2
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 1);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(EQU_attribute_lookahead, errorous_but_unresolable_statement_first_operand_invalid)
{
    std::string input(
        R"( 
&A SETA L'X
 AGO .A
X EQU =**)-,2
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 1);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(attribute_lookahead, lookup_to_copy)
{
    std::string input(
        R"( 
&A SETA L'X
&WAS_BEFORE SETB 1
 COPY LIB
&WAS_AFTER SETB 1
)");
    std::string LIB =
        R"( 
X EQU 1,2,C'X'
&WAS_IN SETB 1
)";

    mock_parse_lib_provider mock { { "LIB", LIB } };
    analyzer a(input, analyzer_options { &mock });
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 2);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_BEFORE"), true);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_IN"), true);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_AFTER"), true);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_from_copy)
{
    std::string input(
        R"( 
&WAS_BEFORE SETB 1
 COPY LIB2
X EQU 1,2
&WAS_AFTER SETB 1
)");

    std::string LIB2 =
        R"( 
&A SETA L'X
&WAS_IN SETB 1
)";

    mock_parse_lib_provider mock { { "LIB2", LIB2 } };
    analyzer a(input, analyzer_options { &mock });
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 2);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_BEFORE"), true);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_IN"), true);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "WAS_AFTER"), true);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_from_macro)
{
    std::string input(
        R"(
 MACRO
 MAC
 GBLA A
&A SETA L'X
X EQU 2,3
 MEND
*
 COPY LIB3
X EQU 1,2
)");
    std::string LIB3 =
        R"( 
 MAC 
&AFTER_MAC SETB 1
)";

    mock_parse_lib_provider mock { { "LIB3", LIB3 } };
    analyzer a(input, analyzer_options { &mock });
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.hlasm_ctx()
                  .globals()
                  .find(a.hlasm_ctx().ids().add("A"))
                  ->second->access_set_symbol_base()
                  ->access_set_symbol<A_t>()
                  ->get_value(),
        2);
    EXPECT_EQ(get_var_value<B_t>(a.hlasm_ctx(), "AFTER_MAC"), true);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(attribute_lookahead, lookup_from_macro_last_line)
{
    std::string input(
        R"( macro
 GETMAIN &b=,&l=
 AIF   (T'&l NE 'O' AND T'&b NE 'O').ERR14      @L1A 
 mend
         GETMAIN   b=svc)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_from_macro_one_to_last_line)
{
    std::string input(
        R"( macro
 GETMAIN &b=,&l=
 AIF   (T'&l NE 'O' AND T'&b NE 'O').ERR14      @L1A 
 mend
         GETMAIN   b=svc
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookup_of_two_refs_evaluation)
{
    std::string input(
        R"( 
&A SETA L'X+L'Y
X EQU 1,10
Y EQU 2,11
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 21);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, machine_label_lookahead)
{
    std::string input(
        R"( 
&A SETA L'X
&B SETC T'X
X LR 1,1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 2);
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "B"), "I");

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, section_label_lookahead)
{
    std::string input(
        R"( 
&A SETA L'X
&B SETC T'X
X CSECT
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 1);
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "B"), "J");

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(data_def_attribute_lookahead, correct_attribute_reference)
{
    std::string input(
        R"( 
&A SETA L'X
&B SETC T'X
&C SETA S'X
X DC FS24'6'       remark
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 4);
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "B"), "F");
    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "C"), 24);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(data_def_attribute_lookahead, incorrect_attribute_reference)
{
    std::string input(
        R"( 
&A SETA S'X
X DC C'A'
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 0);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(attribute_lookahead, lookup_of_two_refs_but_one_symbol_evaluation)
{
    std::string input(
        R"( 
&A SETA L'X+L'Y
X EQU 1,10
Y EQU 2,11
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 21);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, failed_lookup)
{
    std::string input(
        R"( 
&A SETA L'X+L'Y
X EQU 1,10
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 11);

    EXPECT_EQ(a.diags().size(), (size_t)1);
}

TEST(attribute_lookahead, nested_lookup)
{
    std::string input(
        R"( 
&V(1) SETC 'A','B','C'
&A SETA L'&V(L'X)
X EQU 1,2
B EQU 2,22
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(get_var_value<A_t>(a.hlasm_ctx(), "A"), 22);

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookahead_from_macro_bad_following_statement)
{
    std::string input(
        R"( 
 MACRO
 M
&A SETA L'A
 MEND

 M
 AGO .A
F 
A EQU 1,1,1
.A ANOP
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookahead_from_instruction_field)
{
    std::string input(
        R"( 
&A(1) SETC 'LR','SAM64','LR'
 &A(L'A) 
A EQU 1,2,1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookahead_from_instruction_field_macro)
{
    std::string input(
        R"( 
 MACRO
 M
&A(1) SETC 'LR','SAM64','LR'
 &A(L'A) 
 MEND
 M
A EQU 1,2,1
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, lookahead_from_var_sym_label_index)
{
    std::string input(
        R"(&VAR(L'C) SETA 47

C DC C'STH'
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
    auto var = a.context().hlasm_ctx->get_var_sym(a.context().hlasm_ctx->ids().add("VAR"));
    ASSERT_NE(var, nullptr);
    ASSERT_EQ(var->var_kind, variable_kind::SET_VAR_KIND);
    auto value = var->access_set_symbol_base()->access_set_symbol<int>()->get_value(2);

    EXPECT_EQ(value, 47);
}

TEST(EQU_attribute_lookahead, location_counter_use)
{
    std::string input(
        R"( 
  AIF (L'X EQ 2).NO
Y LR 1,1
X EQU 1,*-Y

  AIF (L'A EQ 4).NO
B LR 1,1
A DC AL(*-B+2)(*)
)");

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    ASSERT_EQ(a.diags().size(), (size_t)1);
    EXPECT_EQ(a.diags().front().severity, diagnostic_severity::warning);
    EXPECT_EQ(a.diags().front().diag_range.start.line, (size_t)5);
}

TEST(attribute_lookahead, ignore_invalid_code)
{
    std::string input = R"(
      AIF (L'C GT 0).SKIP
      'invalid
.SKIP ANOP
C     DC C'STH'
)";

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_EQ(a.diags().size(), (size_t)0);
}

TEST(attribute_lookahead, dc_in_copybook)
{
    std::string input =
        R"(
      GBLC &RESULT
&S    SETC 'STR'
      COPY LIB
*
&L    SETA L'LABEL
&T    SETC T'LABEL
&RESULT2 SETC '&L &T'
      END
)";

    std::string LIB =
        R"(
      MAC   LABEL
LABEL DC    C'&S'
)";

    std::string MAC = R"(   MACRO
      MAC &LBL
      GBLC &RESULT
&L    SETA L'&LBL
&T    SETC T'&LBL
&RESULT SETC '&L &T'
      MEND
)";

    mock_parse_lib_provider mock { { "LIB", LIB }, { "MAC", MAC } };
    analyzer a(input, analyzer_options { &mock });
    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(matches_message_codes(a.diags(), { "W013" }));
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "RESULT"), "1 U");
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "RESULT2"), "3 C");
}

TEST(attribute_lookahead, mach_in_copybook)
{
    std::string input =
        R"(
      GBLC &RESULT
&S    SETC 'STR'
      COPY LIB
      END
)";

    std::string LIB =
        R"(
      MAC   LABEL
LABEL LLILF 0,C'&S'
)";

    std::string MAC = R"(   MACRO
      MAC &LBL
      GBLC &RESULT
&L    SETA L'&LBL
&T    SETC T'&LBL
&RESULT SETC '&L &T'
      MEND
)";

    mock_parse_lib_provider mock { { "LIB", LIB }, { "MAC", MAC } };
    analyzer a(input, analyzer_options { &mock });
    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(a.diags().empty());
    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "RESULT"), "6 I");
}

TEST(attribute_lookahead, expression)
{
    std::string input = R"(
&X SETC 'A(3)'
&T SETC T'&X
A  DS   C
)";

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(a.diags().empty());

    EXPECT_EQ(get_var_value<C_t>(a.hlasm_ctx(), "T"), "C");
}
