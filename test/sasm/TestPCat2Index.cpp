/**
 * \file TestPCat2Index.cpp
 *
 * Test that we can convert a PCat value to an array index via pcat2index.
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
 * Test that PCat enumerated values match up with expected indices.
 */
TEST(pcat2index, basicTest)
{
    EXPECT_EQ( 0, pcat2index(PCat::Whitespace));
    EXPECT_EQ( 1, pcat2index(PCat::LineEnd));
    EXPECT_EQ( 2, pcat2index(PCat::FSlash));
    EXPECT_EQ( 3, pcat2index(PCat::BSlash));
    EXPECT_EQ( 4, pcat2index(PCat::Star));
    EXPECT_EQ( 5, pcat2index(PCat::Bang));
    EXPECT_EQ( 6, pcat2index(PCat::DoubleQuote));
    EXPECT_EQ( 7, pcat2index(PCat::Exp));
    EXPECT_EQ( 8, pcat2index(PCat::Alpha));
    EXPECT_EQ( 9, pcat2index(PCat::Underscore));
    EXPECT_EQ(10, pcat2index(PCat::HexX));
    EXPECT_EQ(11, pcat2index(PCat::BinB));
    EXPECT_EQ(12, pcat2index(PCat::BNum));
    EXPECT_EQ(13, pcat2index(PCat::ONum));
    EXPECT_EQ(14, pcat2index(PCat::DNum));
    EXPECT_EQ(15, pcat2index(PCat::HNum));
    EXPECT_EQ(16, pcat2index(PCat::Dot));
    EXPECT_EQ(17, pcat2index(PCat::Plus));
    EXPECT_EQ(18, pcat2index(PCat::Minus));
    EXPECT_EQ(19, pcat2index(PCat::OParen));
    EXPECT_EQ(20, pcat2index(PCat::CParen));
    EXPECT_EQ(21, pcat2index(PCat::Lt));
    EXPECT_EQ(22, pcat2index(PCat::Eq));
    EXPECT_EQ(23, pcat2index(PCat::Gt));
    EXPECT_EQ(24, pcat2index(PCat::Comma));
    EXPECT_EQ(25, pcat2index(PCat::Hash));
    EXPECT_EQ(26, pcat2index(PCat::Ampersand));
    EXPECT_EQ(27, pcat2index(PCat::Pipe));
    EXPECT_EQ(28, pcat2index(PCat::HighBit));
    EXPECT_EQ(29, pcat2index(PCat::EndOfFile));
    EXPECT_EQ(30, pcat2index(PCat::Unknown));
}
