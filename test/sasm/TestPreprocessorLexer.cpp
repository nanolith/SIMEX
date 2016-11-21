/**
 * \file TestPreprocessorLexer.cpp
 *
 * Test that the PreprocessorLexer class properly tokenizes an input stream for
 * feeding into the PreprocessorParser.
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
 * Test that the PreprocessorLexer is correctly instantiated.
 */
TEST(PreprocessorLexer, init)
{
    stringstream in("");

    //create the preprocessor lexer
    auto lexer = make_shared<PreprocessorLexer>(in);

    //the first line is 1.
    EXPECT_EQ(1, lexer->lineNumber());
    //the first column is 0.
    EXPECT_EQ(0, lexer->columnNumber());
}

/**
 * Test that empty input returns an empty line followed by EOF.
 */
TEST(PreprocessorLexer, readEOF)
{
    stringstream in("");

    //create the preprocessor lexer
    auto lexer = make_shared<PreprocessorLexer>(in);

    //read a newline token
    EXPECT_EQ(PTok::Newline, lexer->get());
    //read an EOF
    EXPECT_EQ(PTok::EndOfFile, lexer->get());
    //the line is 2
    EXPECT_EQ(2, lexer->lineNumber());
    //the column is 0.
    EXPECT_EQ(0, lexer->columnNumber());
}
