/**
 * \file TestPCat2Index.cpp
 *
 * Test that the WhitespaceFilter class filters out extra white space characters
 * and comments.
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
 * Test that the WhitespaceFilter is correctly instantiated.
 */
TEST(WhitespaceFilter, init)
{
    stringstream in("");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //the first line is 1.
    EXPECT_EQ(1, filt->lineNumber());
}

/**
 * Test that reading from an empty input returns a newline followed by EOF.
 */
TEST(WhitespaceFilter, emptyEOF)
{
    stringstream in("");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read an EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that multiple spaces between two values are condensed to a single space.
 */
TEST(WhitespaceFilter, spaceCondense)
{
    stringstream in("A       Z");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the A value
    EXPECT_EQ('A', filt->get());
    //read a single space
    EXPECT_EQ(' ', filt->get());
    //read the Z value
    EXPECT_EQ('Z', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read an EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a newline in the middle of spaces results in NL space.
 */
TEST(WhitespaceFilter, spaceNewlineCondensed)
{
    stringstream in("    \n    X");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the next space
    EXPECT_EQ(' ', filt->get());
    //read the X
    EXPECT_EQ('X', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read an EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a block comment is turned into a space.
 */
TEST(WhitespaceFilter, blockCommentSpace)
{
    stringstream in("/*foo*/X");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the first space
    EXPECT_EQ(' ', filt->get());
    //read the X
    EXPECT_EQ('X', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a slash followed by a value returns the slash and that value.
 */
TEST(WhitespaceFilter, nakedSlash)
{
    stringstream in("x/y");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the x
    EXPECT_EQ('x', filt->get());
    //read the slash
    EXPECT_EQ('/', filt->get());
    //read the y
    EXPECT_EQ('y', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a slash followed by an EOF emits the slash.
 */
TEST(WhitespaceFilter, slashEOF)
{
    stringstream in("/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the slash
    EXPECT_EQ('/', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a newline followed by a slash and EOF emits the slash and a
 * newline.
 */
TEST(WhitespaceFilter, NLslashEOF)
{
    stringstream in("\n/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the slash
    EXPECT_EQ('/', filt->get());
    //We should be on line 2
    ASSERT_EQ(2, filt->lineNumber());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a block comment can have multiple stars.
 */
TEST(WhitespaceFilter, blockCommentStars)
{
    stringstream in("/****foo****/X");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the first space
    EXPECT_EQ(' ', filt->get());
    //read the X
    EXPECT_EQ('X', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that the last line of a file has a newline inserted.
 */
TEST(WhitespaceFilter, lastLineHasNL)
{
    stringstream in("A\nB");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the A.
    EXPECT_EQ('A', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //we are now on line 2
    ASSERT_EQ(2, filt->lineNumber());
    //read the B
    EXPECT_EQ('B', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a trailing space before EOF is elided.
 */
TEST(WhitespaceFilter, noSpaceEOF)
{
    stringstream in(" ");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF.
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a trailing space after the last line in the file is elided.
 */
TEST(WhitespaceFilter, EOFAndNLSpaceElided)
{
    stringstream in("X\nY ");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the X.
    EXPECT_EQ('X', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the Y
    EXPECT_EQ('Y', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a trailing block comment is elided.
 */
TEST(WhitespaceFilter, trailingCommentElided)
{
    stringstream in("/*foo*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that block comment spanning multiple lines elides those lines.
 */
TEST(WhitespaceFilter, blockCommentLinesElided)
{
    stringstream in("/*\n\n\n\n*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
    //the line count should be updated, however
    EXPECT_EQ(5, filt->lineNumber());
}

/**
 * Test that a trailing block comment is elided with a newline.
 */
TEST(WhitespaceFilter, NLtrailingCommentElided)
{
    stringstream in("X\n/*foo*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the X
    EXPECT_EQ('X', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the second newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line comment is turned into a newline.
 */
TEST(WhitespaceFilter, lineCommentSimple)
{
    stringstream in("//X\nY");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the Y
    EXPECT_EQ('Y', filt->get());
    //read the second newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line comment followed by EOF is turned into EOF
 */
TEST(WhitespaceFilter, lineCommentEOF)
{
    stringstream in("//foobarbaz");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an expression followed by a line comment ends with an newline.
 */
TEST(WhitespaceFilter, exprLineComment)
{
    stringstream in("XYZ//foobarbaz");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the X
    EXPECT_EQ('X', filt->get());
    //read the Y
    EXPECT_EQ('Y', filt->get());
    //read the Z
    EXPECT_EQ('Z', filt->get());
    //read the newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that multiple line comments are treated like newlines.
 */
TEST(WhitespaceFilter, lineCommentsToNewlines)
{
    stringstream in("////\n//abc\n//cdef\nA\n//123\n//345");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the A
    EXPECT_EQ('A', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that spaces before a line comment are elided.
 */
TEST(WhitespaceFilter, lineCommentSpacesElided)
{
    stringstream in("    //foo");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end can be read from Init.
 */
TEST(WhitespaceFilter, LineEndInit)
{
    stringstream in("\n");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is emitted from Init.
 */
TEST(WhitespaceFilter, BackslashInit)
{
    stringstream in("\\");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a backslash
    EXPECT_EQ('\\', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a star is emitted from Init.
 */
TEST(WhitespaceFilter, StarInit)
{
    stringstream in("*");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a star
    EXPECT_EQ('*', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is emitted from Init.
 */
TEST(WhitespaceFilter, BangInit)
{
    stringstream in("!");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a bang
    EXPECT_EQ('!', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is emitted from Init.
 */
TEST(WhitespaceFilter, DoubleQuoteInit)
{
    stringstream in("\"");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a doublequote
    EXPECT_EQ('"', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is emitted from Init.
 */
TEST(WhitespaceFilter, ExpInit)
{
    stringstream in("e");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an exp
    EXPECT_EQ('e', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is emitted from Init.
 */
TEST(WhitespaceFilter, AlphaInit)
{
    stringstream in("g");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an alpha
    EXPECT_EQ('g', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is emitted from Init.
 */
TEST(WhitespaceFilter, UnderscoreInit)
{
    stringstream in("_");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an underscore
    EXPECT_EQ('_', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is emitted from Init.
 */
TEST(WhitespaceFilter, HexXInit)
{
    stringstream in("x");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a HexX
    EXPECT_EQ('x', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is emitted from Init.
 */
TEST(WhitespaceFilter, BinBInit)
{
    stringstream in("b");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a BinB
    EXPECT_EQ('b', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is emitted from Init.
 */
TEST(WhitespaceFilter, BNumInit)
{
    stringstream in("1");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a BNum
    EXPECT_EQ('1', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is emitted from Init.
 */
TEST(WhitespaceFilter, ONumInit)
{
    stringstream in("7");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an ONum
    EXPECT_EQ('7', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is emitted from Init.
 */
TEST(WhitespaceFilter, DNumInit)
{
    stringstream in("9");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a DNum
    EXPECT_EQ('9', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is emitted from Init.
 */
TEST(WhitespaceFilter, HNumInit)
{
    stringstream in("C");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an HNum
    EXPECT_EQ('C', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is emitted from Init.
 */
TEST(WhitespaceFilter, DotInit)
{
    stringstream in(".");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Dot
    EXPECT_EQ('.', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is emitted from Init.
 */
TEST(WhitespaceFilter, PlusInit)
{
    stringstream in("+");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Plus
    EXPECT_EQ('+', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is emitted from Init.
 */
TEST(WhitespaceFilter, MinusInit)
{
    stringstream in("-");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Minus
    EXPECT_EQ('-', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is emitted from Init.
 */
TEST(WhitespaceFilter, OParenInit)
{
    stringstream in("(");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an OParen
    EXPECT_EQ('(', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is emitted from Init.
 */
TEST(WhitespaceFilter, CParenInit)
{
    stringstream in(")");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a CParen
    EXPECT_EQ(')', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is emitted from Init.
 */
TEST(WhitespaceFilter, LtInit)
{
    stringstream in("<");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Lt
    EXPECT_EQ('<', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Eq is emitted from Init.
 */
TEST(WhitespaceFilter, EqInit)
{
    stringstream in("=");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an Eq
    EXPECT_EQ('=', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is emitted from Init.
 */
TEST(WhitespaceFilter, GtInit)
{
    stringstream in(">");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Gt
    EXPECT_EQ('>', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is emitted from Init.
 */
TEST(WhitespaceFilter, CommaInit)
{
    stringstream in(",");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Comma
    EXPECT_EQ(',', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is emitted from Init.
 */
TEST(WhitespaceFilter, HashInit)
{
    stringstream in("#");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a Hash
    EXPECT_EQ('#', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is emitted from Init.
 */
TEST(WhitespaceFilter, HighBitInit)
{
    stringstream in("\x82");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a HighBit
    EXPECT_EQ(0x82, filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from Init.
 */
TEST(WhitespaceFilter, EOFInit)
{
    stringstream in("");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is emitted from Init.
 */
TEST(WhitespaceFilter, UnknownInit)
{
    stringstream in("]");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read an Unknown
    EXPECT_EQ(']', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end can be read from InSpace.
 */
TEST(WhitespaceFilter, LineEndInSpace)
{
    stringstream in(" \n");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //the space will be elided
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline (end of second line)
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is emitted from InSpace.
 */
TEST(WhitespaceFilter, BackslashInSpace)
{
    stringstream in(" \\");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a backslash
    EXPECT_EQ('\\', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a star is emitted from InSpace.
 */
TEST(WhitespaceFilter, StarInSpace)
{
    stringstream in(" *");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a star
    EXPECT_EQ('*', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is emitted from InSpace.
 */
TEST(WhitespaceFilter, BangInSpace)
{
    stringstream in(" !");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a bang
    EXPECT_EQ('!', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is emitted from InSpace.
 */
TEST(WhitespaceFilter, DoubleQuoteInSpace)
{
    stringstream in(" \"");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a doublequote
    EXPECT_EQ('"', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is emitted from InSpace.
 */
TEST(WhitespaceFilter, ExpInSpace)
{
    stringstream in(" e");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an exp
    EXPECT_EQ('e', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is emitted from InSpace.
 */
TEST(WhitespaceFilter, AlphaInSpace)
{
    stringstream in(" g");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an alpha
    EXPECT_EQ('g', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is emitted from InSpace.
 */
TEST(WhitespaceFilter, UnderscoreInSpace)
{
    stringstream in(" _");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an underscore
    EXPECT_EQ('_', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is emitted from InSpace.
 */
TEST(WhitespaceFilter, HexXInSpace)
{
    stringstream in(" x");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a HexX
    EXPECT_EQ('x', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is emitted from InSpace.
 */
TEST(WhitespaceFilter, BinBInSpace)
{
    stringstream in(" b");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a BinB
    EXPECT_EQ('b', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is emitted from InSpace.
 */
TEST(WhitespaceFilter, BNumInSpace)
{
    stringstream in(" 1");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a BNum
    EXPECT_EQ('1', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is emitted from InSpace.
 */
TEST(WhitespaceFilter, ONumInSpace)
{
    stringstream in(" 7");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an ONum
    EXPECT_EQ('7', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is emitted from InSpace.
 */
TEST(WhitespaceFilter, DNumInSpace)
{
    stringstream in(" 9");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a DNum
    EXPECT_EQ('9', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is emitted from InSpace.
 */
TEST(WhitespaceFilter, HNumInSpace)
{
    stringstream in(" C");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an HNum
    EXPECT_EQ('C', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is emitted from InSpace.
 */
TEST(WhitespaceFilter, DotInSpace)
{
    stringstream in(" .");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Dot
    EXPECT_EQ('.', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is emitted from InSpace.
 */
TEST(WhitespaceFilter, PlusInSpace)
{
    stringstream in(" +");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Plus
    EXPECT_EQ('+', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is emitted from InSpace.
 */
TEST(WhitespaceFilter, MinusInSpace)
{
    stringstream in(" -");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Minus
    EXPECT_EQ('-', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is emitted from InSpace.
 */
TEST(WhitespaceFilter, OParenInSpace)
{
    stringstream in(" (");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an OParen
    EXPECT_EQ('(', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is emitted from InSpace.
 */
TEST(WhitespaceFilter, CParenInSpace)
{
    stringstream in(" )");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a CParen
    EXPECT_EQ(')', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is emitted from InSpace.
 */
TEST(WhitespaceFilter, LtInSpace)
{
    stringstream in(" <");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Lt
    EXPECT_EQ('<', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Eq is emitted from InSpace.
 */
TEST(WhitespaceFilter, EqInSpace)
{
    stringstream in(" =");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an Eq
    EXPECT_EQ('=', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is emitted from InSpace.
 */
TEST(WhitespaceFilter, GtInSpace)
{
    stringstream in(" >");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Gt
    EXPECT_EQ('>', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is emitted from InSpace.
 */
TEST(WhitespaceFilter, CommaInSpace)
{
    stringstream in(" ,");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Comma
    EXPECT_EQ(',', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is emitted from InSpace.
 */
TEST(WhitespaceFilter, HashInSpace)
{
    stringstream in(" #");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a Hash
    EXPECT_EQ('#', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is emitted from InSpace.
 */
TEST(WhitespaceFilter, HighBitInSpace)
{
    stringstream in(" \x82");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read a HighBit
    EXPECT_EQ(0x82, filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from InSpace.
 */
TEST(WhitespaceFilter, EOFInSpace)
{
    stringstream in(" ");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is emitted from InSpace.
 */
TEST(WhitespaceFilter, UnknownInSpace)
{
    stringstream in(" ]");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ(' ', filt->get());
    //read an Unknown
    EXPECT_EQ(']', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that whitespace causes a slash to be emitted in MaybeComment.
 */
TEST(WhitespaceFilter, WhitespaceMaybeComment)
{
    stringstream in("/ ");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read the slash
    EXPECT_EQ('/', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end causes a slash to be emitted in MaybeComment.
 */
TEST(WhitespaceFilter, LineEndMaybeComment)
{
    stringstream in("/\n");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read a newline (end of second line)
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, BackslashMaybeComment)
{
    stringstream in("/\\");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a forward slash
    EXPECT_EQ('/', filt->get());
    //read a backslash
    EXPECT_EQ('\\', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, BangMaybeComment)
{
    stringstream in("/!");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a bang
    EXPECT_EQ('!', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, DoubleQuoteMaybeComment)
{
    stringstream in("/\"");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a doublequote
    EXPECT_EQ('"', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, ExpMaybeComment)
{
    stringstream in("/e");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an exp
    EXPECT_EQ('e', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, AlphaMaybeComment)
{
    stringstream in("/g");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an alpha
    EXPECT_EQ('g', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, UnderscoreMaybeComment)
{
    stringstream in("/_");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an underscore
    EXPECT_EQ('_', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, HexXMaybeComment)
{
    stringstream in("/x");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a HexX
    EXPECT_EQ('x', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, BinBMaybeComment)
{
    stringstream in("/b");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a BinB
    EXPECT_EQ('b', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, BNumMaybeComment)
{
    stringstream in("/1");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a BNum
    EXPECT_EQ('1', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, ONumMaybeComment)
{
    stringstream in("/7");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an ONum
    EXPECT_EQ('7', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, DNumMaybeComment)
{
    stringstream in("/9");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a DNum
    EXPECT_EQ('9', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, HNumMaybeComment)
{
    stringstream in("/C");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space
    EXPECT_EQ('/', filt->get());
    //read an HNum
    EXPECT_EQ('C', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, DotMaybeComment)
{
    stringstream in("/.");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Dot
    EXPECT_EQ('.', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, PlusMaybeComment)
{
    stringstream in("/+");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Plus
    EXPECT_EQ('+', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, MinusMaybeComment)
{
    stringstream in("/-");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Minus
    EXPECT_EQ('-', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, OParenMaybeComment)
{
    stringstream in("/(");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an OParen
    EXPECT_EQ('(', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, CParenMaybeComment)
{
    stringstream in("/)");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a CParen
    EXPECT_EQ(')', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, LtMaybeComment)
{
    stringstream in("/<");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Lt
    EXPECT_EQ('<', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Eq is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, EqMaybeComment)
{
    stringstream in("/=");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an Eq
    EXPECT_EQ('=', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, GtMaybeComment)
{
    stringstream in("/>");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Gt
    EXPECT_EQ('>', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, CommaMaybeComment)
{
    stringstream in("/,");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Comma
    EXPECT_EQ(',', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, HashMaybeComment)
{
    stringstream in("/#");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a Hash
    EXPECT_EQ('#', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, HighBitMaybeComment)
{
    stringstream in("/\x82");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a HighBit
    EXPECT_EQ(0x82, filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, EOFMaybeComment)
{
    stringstream in("/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is emitted from MaybeComment.
 */
TEST(WhitespaceFilter, UnknownMaybeComment)
{
    stringstream in("/]");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a slash
    EXPECT_EQ('/', filt->get());
    //read an Unknown
    EXPECT_EQ(']', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that whitespace is omitted in a LineComment.
 */
TEST(WhitespaceFilter, WhitespaceLineComment)
{
    stringstream in("// ");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end ends a line comment.
 */
TEST(WhitespaceFilter, LineEndLineComment)
{
    stringstream in("//\nx");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the x
    EXPECT_EQ('x', filt->get());
    //read a newline (end of second line)
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a forward slash is omitted in a LineComment.
 */
TEST(WhitespaceFilter, FSlashLineComment)
{
    stringstream in("///");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is omitted in a LineComment.
 */
TEST(WhitespaceFilter, BackslashLineComment)
{
    stringstream in("//\\");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a star is omitted from a LineComment.
 */
TEST(WhitespaceFilter, StarLineComment)
{
    stringstream in("//*");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is omitted from a LineComment.
 */
TEST(WhitespaceFilter, BangLineComment)
{
    stringstream in("//!");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is omitted in a LineComment.
 */
TEST(WhitespaceFilter, DoubleQuoteLineComment)
{
    stringstream in("//\"");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is omitted from a LineComment.
 */
TEST(WhitespaceFilter, ExpLineComment)
{
    stringstream in("//e");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is omitted from a LineComment.
 */
TEST(WhitespaceFilter, AlphaLineComment)
{
    stringstream in("//g");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is omitted from a LineComment.
 */
TEST(WhitespaceFilter, UnderscoreLineComment)
{
    stringstream in("//_");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is omitted from a LineComment.
 */
TEST(WhitespaceFilter, HexXLineComment)
{
    stringstream in("//x");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is omitted from a LineComment.
 */
TEST(WhitespaceFilter, BinBLineComment)
{
    stringstream in("//b");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is omitted from a LineComment.
 */
TEST(WhitespaceFilter, BNumLineComment)
{
    stringstream in("//1");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is omitted from a LineComment.
 */
TEST(WhitespaceFilter, ONumLineComment)
{
    stringstream in("//7");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is omitted from a LineComment.
 */
TEST(WhitespaceFilter, DNumLineComment)
{
    stringstream in("//9");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is omitted from a LineComment.
 */
TEST(WhitespaceFilter, HNumLineComment)
{
    stringstream in("//C");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is omitted from a LineComment.
 */
TEST(WhitespaceFilter, DotLineComment)
{
    stringstream in("//.");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is omitted from a LineComment.
 */
TEST(WhitespaceFilter, PlusLineComment)
{
    stringstream in("//+");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is omitted from a LineComment.
 */
TEST(WhitespaceFilter, MinusLineComment)
{
    stringstream in("//-");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is omitted from a LineComment.
 */
TEST(WhitespaceFilter, OParenLineComment)
{
    stringstream in("//(");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is omitted from a LineComment.
 */
TEST(WhitespaceFilter, CParenLineComment)
{
    stringstream in("//)");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is omitted from a LineComment.
 */
TEST(WhitespaceFilter, LtLineComment)
{
    stringstream in("//<");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Eq is omitted from a LineComment.
 */
TEST(WhitespaceFilter, EqLineComment)
{
    stringstream in("//=");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is omitted from a LineComment.
 */
TEST(WhitespaceFilter, GtLineComment)
{
    stringstream in("//>");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is omitted from a LineComment.
 */
TEST(WhitespaceFilter, CommaLineComment)
{
    stringstream in("//,");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is omitted from a LineComment.
 */
TEST(WhitespaceFilter, HashLineComment)
{
    stringstream in("//#");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is omitted from a LineComment.
 */
TEST(WhitespaceFilter, HighBitLineComment)
{
    stringstream in("//\x82");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from a LineComment.
 */
TEST(WhitespaceFilter, EOFLineComment)
{
    stringstream in("//");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is omitted from a LineComment.
 */
TEST(WhitespaceFilter, UnknownLineComment)
{
    stringstream in("//]");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that whitespace is omitted in a BlockComment.
 */
TEST(WhitespaceFilter, WhitespaceBlockComment)
{
    stringstream in("/* */");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end is omitted in a BlockComment.
 */
TEST(WhitespaceFilter, LineEndBlockComment)
{
    stringstream in("/*\n*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline (end of second line)
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a forward slash is omitted in a BlockComment.
 */
TEST(WhitespaceFilter, FSlashBlockComment)
{
    stringstream in("/*/*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is omitted in a BlockComment.
 */
TEST(WhitespaceFilter, BackslashBlockComment)
{
    stringstream in("/*\\*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a star is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, StarBlockComment)
{
    stringstream in("/***/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, BangBlockComment)
{
    stringstream in("/*!*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is omitted in a BlockComment.
 */
TEST(WhitespaceFilter, DoubleQuoteBlockComment)
{
    stringstream in("/*\"*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, ExpBlockComment)
{
    stringstream in("/*e*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, AlphaBlockComment)
{
    stringstream in("/*g*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, UnderscoreBlockComment)
{
    stringstream in("/*_*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, HexXBlockComment)
{
    stringstream in("/*x*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, BinBBlockComment)
{
    stringstream in("/*b*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, BNumBlockComment)
{
    stringstream in("/*1*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, ONumBlockComment)
{
    stringstream in("/*7*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, DNumBlockComment)
{
    stringstream in("/*9*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, HNumBlockComment)
{
    stringstream in("/*C*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, DotBlockComment)
{
    stringstream in("/*.*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, PlusBlockComment)
{
    stringstream in("/*+*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, MinusBlockComment)
{
    stringstream in("/*-*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, OParenBlockComment)
{
    stringstream in("/*(*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, CParenBlockComment)
{
    stringstream in("/*)*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, LtBlockComment)
{
    stringstream in("/*<*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Eq is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, EqBlockComment)
{
    stringstream in("/*=*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, GtBlockComment)
{
    stringstream in("/*>*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, CommaBlockComment)
{
    stringstream in("/*,*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, HashBlockComment)
{
    stringstream in("/*#*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, HighBitBlockComment)
{
    stringstream in("/*\x82*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from a BlockComment.
 */
TEST(WhitespaceFilter, EOFBlockComment)
{
    stringstream in("/*");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is omitted from a BlockComment.
 */
TEST(WhitespaceFilter, UnknownBlockComment)
{
    stringstream in("/*]*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that whitespace is omitted in MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, WhitespaceMaybeEndBlockComment)
{
    stringstream in("/** */");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a line end is omitted in a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, LineEndMaybeEndBlockComment)
{
    stringstream in("/**\n*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline (end of second line)
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a forward slash ends a block comment in the middle of a block
 * comment, causing the star and slash that would have ended the block comment
 * to be emitted.
 */
TEST(WhitespaceFilter, FSlashMaybeEndBlockComment)
{
    stringstream in("/**/*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a space which replaces the block comment
    EXPECT_EQ(' ', filt->get());
    //read a star
    EXPECT_EQ('*', filt->get());
    //read a slash
    EXPECT_EQ('/', filt->get());
    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a backslash is omitted in a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, BackslashMaybeEndBlockComment)
{
    stringstream in("/**\\*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a star is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, StarMaybeEndBlockComment)
{
    stringstream in("/****/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a bang is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, BangMaybeEndBlockComment)
{
    stringstream in("/**!*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a double quote is omitted in a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, DoubleQuoteMaybeEndBlockComment)
{
    stringstream in("/**\"*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an exp is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, ExpMaybeEndBlockComment)
{
    stringstream in("/**e*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an alpha is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, AlphaMaybeEndBlockComment)
{
    stringstream in("/**g*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an underscore is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, UnderscoreMaybeEndBlockComment)
{
    stringstream in("/**_*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HexX is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, HexXMaybeEndBlockComment)
{
    stringstream in("/**x*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BinB is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, BinBMaybeEndBlockComment)
{
    stringstream in("/**b*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a BNum is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, BNumMaybeEndBlockComment)
{
    stringstream in("/**1*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an ONum is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, ONumMaybeEndBlockComment)
{
    stringstream in("/**7*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a DNum is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, DNumMaybeEndBlockComment)
{
    stringstream in("/**9*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an HNum is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, HNumMaybeEndBlockComment)
{
    stringstream in("/**C*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Dot is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, DotMaybeEndBlockComment)
{
    stringstream in("/**.*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Plus is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, PlusMaybeEndBlockComment)
{
    stringstream in("/**+*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Minus is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, MinusMaybeEndBlockComment)
{
    stringstream in("/**-*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an OParen is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, OParenMaybeEndBlockComment)
{
    stringstream in("/**(*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a CParen is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, CParenMaybeEndBlockComment)
{
    stringstream in("/**)*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Lt is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, LtMaybeEndBlockComment)
{
    stringstream in("/**<*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Eq is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, EqMaybeEndBlockComment)
{
    stringstream in("/**=*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Gt is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, GtMaybeEndBlockComment)
{
    stringstream in("/**>*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Comma is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, CommaMaybeEndBlockComment)
{
    stringstream in("/**,*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a Hash is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, HashMaybeEndBlockComment)
{
    stringstream in("/**#*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that a HighBit is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, HighBitMaybeEndBlockComment)
{
    stringstream in("/**\x82*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an EOF is emitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, EOFMaybeEndBlockComment)
{
    stringstream in("/**");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}

/**
 * Test that an Unknown is omitted from a MaybeEndBlockComment.
 */
TEST(WhitespaceFilter, UnknownMaybeEndBlockComment)
{
    stringstream in("/**]*/");

    //create the whitespace filter
    auto filt = make_shared<WhitespaceFilter>(in);

    //read a newline
    EXPECT_EQ('\n', filt->get());
    //read the EOF
    EXPECT_EQ(EOF, filt->get());
}
