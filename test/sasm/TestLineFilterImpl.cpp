/**
 * \file TestLineFilterImpl.cpp
 *
 * Test that the LineFilterImpl class properly tracks line and column
 * information.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <gtest/gtest.h>
#include <simex/sasm/PreprocessorLexer.h>
#include <sstream>

using namespace simex::sasm;
using namespace std;

/**
 * Test that LineFilterImpl is correctly instantiated.
 */
TEST(LineFilterImpl, init)
{
    stringstream in("");

    //create the whitespace filter
    auto filt = make_shared<LineFilterImpl>(in);

    //the first line is 1.
    EXPECT_EQ(1, filt->lineNumber());
    //the first column is 0.
    EXPECT_EQ(0, filt->columnNumber());
}

/**
 * Test that LineFilter increments columnNumber() on get().
 */
TEST(LineFilterImpl, columnNumberIncrement)
{
    stringstream in("abc");

    //create the whitespace filter
    auto filt = make_shared<LineFilterImpl>(in);

    //the first line is 1.
    EXPECT_EQ(1, filt->lineNumber());
    //the first column is 0.
    EXPECT_EQ(0, filt->columnNumber());

    //get a (Line 1:1)
    EXPECT_EQ('a', filt->get());
    EXPECT_EQ(1, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
    //get b (Line 1:2)
    EXPECT_EQ('b', filt->get());
    EXPECT_EQ(1, filt->lineNumber());
    EXPECT_EQ(2, filt->columnNumber());
    //get c (Line 1:3)
    EXPECT_EQ('c', filt->get());
    EXPECT_EQ(1, filt->lineNumber());
    EXPECT_EQ(3, filt->columnNumber());
}

/**
 * Test that LineFilter increments lineNumber() and resets columnNumber()
 * on get() == '\n'.
 */
TEST(LineFilterImpl, lineNumberGet)
{
    stringstream in("a\nb");

    //create the whitespace filter
    auto filt = make_shared<LineFilterImpl>(in);

    //the first line is 1.
    EXPECT_EQ(1, filt->lineNumber());
    //the first column is 0.
    EXPECT_EQ(0, filt->columnNumber());

    //get a (Line 1:1)
    EXPECT_EQ('a', filt->get());
    EXPECT_EQ(1, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
    //get newline (Line 2:0)
    EXPECT_EQ('\n', filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(0, filt->columnNumber());
    //get b (Line 2:1)
    EXPECT_EQ('b', filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
}

/**
 * Test that LineFilter stops incrementing on EOF.
 */
TEST(LineFilterImpl, filterEOF)
{
    stringstream in("a\nb");

    //create the whitespace filter
    auto filt = make_shared<LineFilterImpl>(in);

    //the first line is 1.
    EXPECT_EQ(1, filt->lineNumber());
    //the first column is 0.
    EXPECT_EQ(0, filt->columnNumber());

    //get a (Line 1:1)
    EXPECT_EQ('a', filt->get());
    EXPECT_EQ(1, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
    //get newline (Line 2:0)
    EXPECT_EQ('\n', filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(0, filt->columnNumber());
    //get b (Line 2:1)
    EXPECT_EQ('b', filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
    //get EOF
    EXPECT_EQ(EOF, filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
    //get EOF
    EXPECT_EQ(EOF, filt->get());
    EXPECT_EQ(2, filt->lineNumber());
    EXPECT_EQ(1, filt->columnNumber());
}
