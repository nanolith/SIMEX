/**
 * \file TestPTokToIndex.cpp
 *
 * Test that we can convert a PTok value to an array index via ptok2index.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <gtest/gtest.h>
#include <simex/sasm/PreprocessorLexer.h>

using namespace simex::sasm;
using namespace std;

/**
 * Test that PTok enumerated values match up with expected indices.
 */
TEST(ptok2index, basicTest)
{
    EXPECT_EQ(0,  ptok2index(PTok::Newline));
    EXPECT_EQ(1,  ptok2index(PTok::String));
    EXPECT_EQ(2,  ptok2index(PTok::SystemString));
    EXPECT_EQ(3,  ptok2index(PTok::Include));
    EXPECT_EQ(4,  ptok2index(PTok::IfDef));
    EXPECT_EQ(5,  ptok2index(PTok::If));
    EXPECT_EQ(6,  ptok2index(PTok::ElIf));
    EXPECT_EQ(7,  ptok2index(PTok::Else));
    EXPECT_EQ(8,  ptok2index(PTok::Pragma));
    EXPECT_EQ(9,  ptok2index(PTok::Warning));
    EXPECT_EQ(10, ptok2index(PTok::Error));
    EXPECT_EQ(11, ptok2index(PTok::And));
    EXPECT_EQ(12, ptok2index(PTok::Or));
    EXPECT_EQ(13, ptok2index(PTok::Not));
    EXPECT_EQ(14, ptok2index(PTok::OParen));
    EXPECT_EQ(15, ptok2index(PTok::CParen));
    EXPECT_EQ(16, ptok2index(PTok::Comma));
    EXPECT_EQ(17, ptok2index(PTok::Identifier));
    EXPECT_EQ(18, ptok2index(PTok::Integer));
    EXPECT_EQ(19, ptok2index(PTok::Number));
    EXPECT_EQ(20, ptok2index(PTok::Concat));
    EXPECT_EQ(21, ptok2index(PTok::Stringify));
    EXPECT_EQ(22, ptok2index(PTok::Unknown));
}
