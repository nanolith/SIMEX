/**
 * \file TestByte2PCat.cpp
 *
 * Test that the byte2PCat method properly converts all inputs.
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
 * All whitespace characters should be converted to PCat::Whitespace.
 */
TEST(byte2PCat, Whitespace)
{
    //bell is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x07));
    EXPECT_EQ(PCat::Whitespace, byte2PCat('\a'));

    //Horizontal Tab is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x09));
    EXPECT_EQ(PCat::Whitespace, byte2PCat('\t'));

    //Vertical Tab is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x0B));
    EXPECT_EQ(PCat::Whitespace, byte2PCat('\v'));

    //Form feed is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x0C));
    EXPECT_EQ(PCat::Whitespace, byte2PCat('\f'));

    //Carriage return is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x0D));
    EXPECT_EQ(PCat::Whitespace, byte2PCat('\r'));

    //Space is a whitespace character
    EXPECT_EQ(PCat::Whitespace, byte2PCat(0x20));
    EXPECT_EQ(PCat::Whitespace, byte2PCat(' '));
}

/**
 * \n is converted to LineEnd
 */
TEST(byte2PCat, LineEnd)
{
    //LF is LineEnd
    EXPECT_EQ(PCat::LineEnd, byte2PCat(0x0a));
    EXPECT_EQ(PCat::LineEnd, byte2PCat('\n'));
}

/**
 * '/' is converted to FSlash
 */
TEST(byte2PCat, FSlash)
{
    //'/' is FSlash
    EXPECT_EQ(PCat::FSlash, byte2PCat(0x2f));
    EXPECT_EQ(PCat::FSlash, byte2PCat('/'));
}

/**
 * '\' is converted to BSlash
 */
TEST(byte2PCat, BSlash)
{
    //'\\' is FSlash
    EXPECT_EQ(PCat::BSlash, byte2PCat(0x5c));
    EXPECT_EQ(PCat::BSlash, byte2PCat('\\'));
}

/**
 * '*' is converted to Star
 */
TEST(byte2PCat, Star)
{
    //'*' is Star
    EXPECT_EQ(PCat::Star, byte2PCat(0x2a));
    EXPECT_EQ(PCat::Star, byte2PCat('*'));
}

/**
 * '!' is converted to Bang
 */
TEST(byte2PCat, Bang)
{
    //'!' is Bang
    EXPECT_EQ(PCat::Bang, byte2PCat(0x21));
    EXPECT_EQ(PCat::Bang, byte2PCat('!'));
}

/**
 * '"' is converted to DoubleQuote
 */
TEST(byte2PCat, DoubleQuote)
{
    //'"' is DoubleQuote
    EXPECT_EQ(PCat::DoubleQuote, byte2PCat(0x22));
    EXPECT_EQ(PCat::DoubleQuote, byte2PCat('"'));
}

/**
 * 'E' and 'e' are converted to Exp
 */
TEST(byte2PCat, Exp)
{
    //'E' is Exp
    EXPECT_EQ(PCat::Exp, byte2PCat(0x45));
    EXPECT_EQ(PCat::Exp, byte2PCat('E'));

    //'e' is Exp
    EXPECT_EQ(PCat::Exp, byte2PCat(0x65));
    EXPECT_EQ(PCat::Exp, byte2PCat('e'));
}

/**
 * G-Z and g-w,y-z are converted to Alpha
 */
TEST(byte2PCat, Alpha)
{
    //'G' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x47));
    EXPECT_EQ(PCat::Alpha, byte2PCat('G'));
    //'H' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x48));
    EXPECT_EQ(PCat::Alpha, byte2PCat('H'));
    //'I' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x49));
    EXPECT_EQ(PCat::Alpha, byte2PCat('I'));
    //'J' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4a));
    EXPECT_EQ(PCat::Alpha, byte2PCat('J'));
    //'K' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4b));
    EXPECT_EQ(PCat::Alpha, byte2PCat('K'));
    //'L' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4c));
    EXPECT_EQ(PCat::Alpha, byte2PCat('L'));
    //'M' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4d));
    EXPECT_EQ(PCat::Alpha, byte2PCat('M'));
    //'N' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4e));
    EXPECT_EQ(PCat::Alpha, byte2PCat('N'));
    //'O' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x4f));
    EXPECT_EQ(PCat::Alpha, byte2PCat('O'));
    //'P' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x50));
    EXPECT_EQ(PCat::Alpha, byte2PCat('P'));
    //'Q' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x51));
    EXPECT_EQ(PCat::Alpha, byte2PCat('Q'));
    //'R' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x52));
    EXPECT_EQ(PCat::Alpha, byte2PCat('R'));
    //'S' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x53));
    EXPECT_EQ(PCat::Alpha, byte2PCat('S'));
    //'T' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x54));
    EXPECT_EQ(PCat::Alpha, byte2PCat('T'));
    //'U' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x55));
    EXPECT_EQ(PCat::Alpha, byte2PCat('U'));
    //'V' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x56));
    EXPECT_EQ(PCat::Alpha, byte2PCat('V'));
    //'W' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x57));
    EXPECT_EQ(PCat::Alpha, byte2PCat('W'));
    //'X' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x58));
    EXPECT_EQ(PCat::Alpha, byte2PCat('X'));
    //'Y' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x59));
    EXPECT_EQ(PCat::Alpha, byte2PCat('Y'));
    //'Z' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x5a));
    EXPECT_EQ(PCat::Alpha, byte2PCat('Z'));

    //'g' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x67));
    EXPECT_EQ(PCat::Alpha, byte2PCat('g'));
    //'h' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x68));
    EXPECT_EQ(PCat::Alpha, byte2PCat('h'));
    //'i' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x69));
    EXPECT_EQ(PCat::Alpha, byte2PCat('i'));
    //'j' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6a));
    EXPECT_EQ(PCat::Alpha, byte2PCat('j'));
    //'k' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6b));
    EXPECT_EQ(PCat::Alpha, byte2PCat('k'));
    //'l' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6c));
    EXPECT_EQ(PCat::Alpha, byte2PCat('l'));
    //'m' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6d));
    EXPECT_EQ(PCat::Alpha, byte2PCat('m'));
    //'n' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6e));
    EXPECT_EQ(PCat::Alpha, byte2PCat('n'));
    //'o' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x6f));
    EXPECT_EQ(PCat::Alpha, byte2PCat('o'));
    //'p' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x70));
    EXPECT_EQ(PCat::Alpha, byte2PCat('p'));
    //'q' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x71));
    EXPECT_EQ(PCat::Alpha, byte2PCat('q'));
    //'r' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x72));
    EXPECT_EQ(PCat::Alpha, byte2PCat('r'));
    //'s' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x73));
    EXPECT_EQ(PCat::Alpha, byte2PCat('s'));
    //'t' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x74));
    EXPECT_EQ(PCat::Alpha, byte2PCat('t'));
    //'u' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x75));
    EXPECT_EQ(PCat::Alpha, byte2PCat('u'));
    //'v' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x76));
    EXPECT_EQ(PCat::Alpha, byte2PCat('v'));
    //'w' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x77));
    EXPECT_EQ(PCat::Alpha, byte2PCat('w'));
    //'y' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x79));
    EXPECT_EQ(PCat::Alpha, byte2PCat('y'));
    //'z' is Alpha
    EXPECT_EQ(PCat::Alpha, byte2PCat(0x7a));
    EXPECT_EQ(PCat::Alpha, byte2PCat('z'));
}

/**
 * '_' is converted to Underscore
 */
TEST(byte2PCat, Underscore)
{
    //'_' is Underscore
    EXPECT_EQ(PCat::Underscore, byte2PCat(0x5f));
    EXPECT_EQ(PCat::Underscore, byte2PCat('_'));
}

/**
 * 'b' is converted to BinB
 */
TEST(byte2PCat, BinB)
{
    //'b' is BinB
    EXPECT_EQ(PCat::BinB, byte2PCat(0x62));
    EXPECT_EQ(PCat::BinB, byte2PCat('b'));
}

/**
 * 'x' is converted to HexX
 */
TEST(byte2PCat, HexX)
{
    //'x' is HexX
    EXPECT_EQ(PCat::HexX, byte2PCat(0x78));
    EXPECT_EQ(PCat::HexX, byte2PCat('x'));
}

/**
 * 0 and 1 are converted to BNum.
 */
TEST(byte2PCat, BNum)
{
    //'0' is BNum
    EXPECT_EQ(PCat::BNum, byte2PCat(0x30));
    EXPECT_EQ(PCat::BNum, byte2PCat('0'));
    //'1' is BNum
    EXPECT_EQ(PCat::BNum, byte2PCat(0x31));
    EXPECT_EQ(PCat::BNum, byte2PCat('1'));
}

/**
 * 2-7 are converted to ONum.
 */
TEST(byte2PCat, ONum)
{
    //'2' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x32));
    EXPECT_EQ(PCat::ONum, byte2PCat('2'));
    //'3' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x33));
    EXPECT_EQ(PCat::ONum, byte2PCat('3'));
    //'4' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x34));
    EXPECT_EQ(PCat::ONum, byte2PCat('4'));
    //'5' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x35));
    EXPECT_EQ(PCat::ONum, byte2PCat('5'));
    //'6' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x36));
    EXPECT_EQ(PCat::ONum, byte2PCat('6'));
    //'7' is ONum
    EXPECT_EQ(PCat::ONum, byte2PCat(0x37));
    EXPECT_EQ(PCat::ONum, byte2PCat('7'));
}

/**
 * 8 and 9 are converted to DNum.
 */
TEST(byte2PCat, DNum)
{
    //'8' is DNum
    EXPECT_EQ(PCat::DNum, byte2PCat(0x38));
    EXPECT_EQ(PCat::DNum, byte2PCat('8'));
    //'9' is DNum
    EXPECT_EQ(PCat::DNum, byte2PCat(0x39));
    EXPECT_EQ(PCat::DNum, byte2PCat('9'));
}

/**
 * A,B,C,D,F and a,c,d,f are converted to HNum.
 */
TEST(byte2PCat, HNum)
{
    //'A' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x41));
    EXPECT_EQ(PCat::HNum, byte2PCat('A'));
    //'B' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x42));
    EXPECT_EQ(PCat::HNum, byte2PCat('B'));
    //'C' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x43));
    EXPECT_EQ(PCat::HNum, byte2PCat('C'));
    //'D' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x44));
    EXPECT_EQ(PCat::HNum, byte2PCat('D'));
    //'F' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x46));
    EXPECT_EQ(PCat::HNum, byte2PCat('F'));

    //'a' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x61));
    EXPECT_EQ(PCat::HNum, byte2PCat('a'));
    //'c' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x63));
    EXPECT_EQ(PCat::HNum, byte2PCat('c'));
    //'d' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x64));
    EXPECT_EQ(PCat::HNum, byte2PCat('d'));
    //'f' is HNum
    EXPECT_EQ(PCat::HNum, byte2PCat(0x66));
    EXPECT_EQ(PCat::HNum, byte2PCat('f'));
}

/**
 * '.' is converted to Dot
 */
TEST(byte2PCat, Dot)
{
    //'.' is Dot
    EXPECT_EQ(PCat::Dot, byte2PCat(0x2e));
    EXPECT_EQ(PCat::Dot, byte2PCat('.'));
}

/**
 * '+' is converted to Plus
 */
TEST(byte2PCat, Plus)
{
    //'+' is Plus
    EXPECT_EQ(PCat::Plus, byte2PCat(0x2b));
    EXPECT_EQ(PCat::Plus, byte2PCat('+'));
}

/**
 * '-' is converted to Minus
 */
TEST(byte2PCat, Minus)
{
    //'-' is Plus
    EXPECT_EQ(PCat::Minus, byte2PCat(0x2d));
    EXPECT_EQ(PCat::Minus, byte2PCat('-'));
}

/**
 * '(' is converted to OParen
 */
TEST(byte2PCat, OParen)
{
    //'(' is OParen
    EXPECT_EQ(PCat::OParen, byte2PCat(0x28));
    EXPECT_EQ(PCat::OParen, byte2PCat('('));
}

/**
 * ')' is converted to CParen
 */
TEST(byte2PCat, CParen)
{
    //')' is CParen
    EXPECT_EQ(PCat::CParen, byte2PCat(0x29));
    EXPECT_EQ(PCat::CParen, byte2PCat(')'));
}

/**
 * ',' is converted to Comma
 */
TEST(byte2PCat, Comma)
{
    //',' is Comma
    EXPECT_EQ(PCat::Comma, byte2PCat(0x2C));
    EXPECT_EQ(PCat::Comma, byte2PCat(','));
}

/**
 * '#' is converted to Hash
 */
TEST(byte2PCat, Hash)
{
    //'#' is Hash
    EXPECT_EQ(PCat::Hash, byte2PCat(0x23));
    EXPECT_EQ(PCat::Hash, byte2PCat('#'));
}

/**
 * Any byte with the high-bit set is converted to HighBit.
 */
TEST(byte2PCat, HighBit)
{
    for (int i = 128; i < 256; ++i)
    {
        EXPECT_EQ(PCat::HighBit, byte2PCat(i));
    }
}
