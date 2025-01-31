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

// tests for EQU instruction

TEST(EQU, simple)
{
    std::string input(R"(
A EQU 1
 LR A,A
)");
    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "A"), 1);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, complex)
{
    std::string input(R"(
A EQU 1
B EQU A+A-10
 LR A,B
)");
    analyzer a(input);
    a.analyze();

    EXPECT_TRUE(a.hlasm_ctx().ord_ctx.symbol_defined(a.hlasm_ctx().ids().add("A")));

    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "B"), -8);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)1);
}

TEST(EQU, length_explicit)
{
    std::string input = R"(
Y EQU X,12
X EQU 5,2
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "X"), 5);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "X")->attributes().length(), (symbol_attributes::len_attr)2);

    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "Y"), 5);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "Y")->attributes().length(), (symbol_attributes::len_attr)12);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, length_implicit)
{
    std::string input = R"(
X EQU 5,2
Y EQU X
Z EQU 1+X
ZZ EQU *+X
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "Y")->attributes().length(), (symbol_attributes::len_attr)2);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "Z")->attributes().length(), (symbol_attributes::len_attr)1);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "ZZ")->attributes().length(), (symbol_attributes::len_attr)1);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, length_dep)
{
    std::string input = R"(
LEN EQU 11
X EQU UNKNOWN,LEN
UNKNOWN EQU L'X
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "X"), 11);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, length_bounds)
{
    std::string input = R"(
A EQU 1,12
LEN EQU 1+A,-100
LEM EQU A+1,100000
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEN")->attributes().length(), (symbol_attributes::len_attr)1);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEM")->attributes().length(), (symbol_attributes::len_attr)12);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)2);
}

TEST(EQU, type_explicit)
{
    std::string input = R"(
LEN EQU 11,3,4
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEN")->attributes().type(), 4);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, type_implicit)
{
    std::string input = R"(
LEN EQU 11,3
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEN")->attributes().type(), symbol_attributes::undef_type);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, type_bounds)
{
    std::string input = R"(
LEN EQU 11,1,-1
LEM EQU 11,1,300
)";

    analyzer a(input);
    a.analyze();

    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEN")->attributes().type(), symbol_attributes::undef_type);
    EXPECT_EQ(get_symbol(a.hlasm_ctx(), "LEM")->attributes().type(), symbol_attributes::undef_type);

    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)2);
}

TEST(EQU, loctr_use)
{
    std::string input = R"(
A  DS    A
B  EQU   256
C  EQU   B-*+A
   DS    XL(C)
)";

    analyzer a(input);
    a.analyze();
    a.collect_diags();
    ASSERT_EQ(a.diags().size(), (size_t)0);
}

TEST(EQU, deps_with_multiplication)
{
    std::string input = R"(
TEST CSECT
     DS    XL(D)
A    DS    A
L    EQU   (A-TEST)*(A-TEST)
     DS    XL(L)
D    EQU   4
RES  EQU   *-TEST
)";

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(a.diags().empty());
    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "RES"), 24);
}

TEST(EQU, reloc_deps_with_multiplication)
{
    std::string input = R"(
TEST CSECT
     DS    XL(D)
A    DS    A
L    EQU   TEST+(A-TEST)*(A-TEST)
     DS    XL(L-TEST)
D    EQU   4
RES  EQU   *-TEST
)";

    analyzer a(input);
    a.analyze();
    a.collect_diags();

    EXPECT_TRUE(a.diags().empty());
    EXPECT_EQ(get_symbol_abs(a.hlasm_ctx(), "RES"), 24);
}
