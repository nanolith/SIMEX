/**
 * \file TestCharacter2PCat.cpp
 *
 * Test that the character2PCat method properly converts all inputs.
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
TEST(character2PCat, Whitespace)
{
    //bell is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x07));
    EXPECT_EQ(PCat::Whitespace, character2PCat('\a'));

    //Horizontal Tab is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x09));
    EXPECT_EQ(PCat::Whitespace, character2PCat('\t'));

    //Vertical Tab is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x0B));
    EXPECT_EQ(PCat::Whitespace, character2PCat('\v'));

    //Form feed is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x0C));
    EXPECT_EQ(PCat::Whitespace, character2PCat('\f'));

    //Carriage return is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x0D));
    EXPECT_EQ(PCat::Whitespace, character2PCat('\r'));

    //Space is a whitespace character
    EXPECT_EQ(PCat::Whitespace, character2PCat(0x20));
    EXPECT_EQ(PCat::Whitespace, character2PCat(' '));
}

/**
 * \n is converted to LineEnd
 */
TEST(character2PCat, LineEnd)
{
    //LF is LineEnd
    EXPECT_EQ(PCat::LineEnd, character2PCat(0x0a));
    EXPECT_EQ(PCat::LineEnd, character2PCat('\n'));
}

/**
 * '/' is converted to FSlash
 */
TEST(character2PCat, FSlash)
{
    //'/' is FSlash
    EXPECT_EQ(PCat::FSlash, character2PCat(0x2f));
    EXPECT_EQ(PCat::FSlash, character2PCat('/'));
}

/**
 * '\' is converted to BSlash
 */
TEST(character2PCat, BSlash)
{
    //'\\' is FSlash
    EXPECT_EQ(PCat::BSlash, character2PCat(0x5c));
    EXPECT_EQ(PCat::BSlash, character2PCat('\\'));
}

/**
 * '*' is converted to Star
 */
TEST(character2PCat, Star)
{
    //'*' is Star
    EXPECT_EQ(PCat::Star, character2PCat(0x2a));
    EXPECT_EQ(PCat::Star, character2PCat('*'));
}

/**
 * '!' is converted to Bang
 */
TEST(character2PCat, Bang)
{
    //'!' is Bang
    EXPECT_EQ(PCat::Bang, character2PCat(0x21));
    EXPECT_EQ(PCat::Bang, character2PCat('!'));
}

/**
 * '"' is converted to DoubleQuote
 */
TEST(character2PCat, DoubleQuote)
{
    //'"' is DoubleQuote
    EXPECT_EQ(PCat::DoubleQuote, character2PCat(0x22));
    EXPECT_EQ(PCat::DoubleQuote, character2PCat('"'));
}

/**
 * 'E' and 'e' are converted to Exp
 */
TEST(character2PCat, Exp)
{
    //'E' is Exp
    EXPECT_EQ(PCat::Exp, character2PCat(0x45));
    EXPECT_EQ(PCat::Exp, character2PCat('E'));

    //'e' is Exp
    EXPECT_EQ(PCat::Exp, character2PCat(0x65));
    EXPECT_EQ(PCat::Exp, character2PCat('e'));
}

/**
 * G-Z and g-w,y-z are converted to Alpha
 */
TEST(character2PCat, Alpha)
{
    //'G' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x47));
    EXPECT_EQ(PCat::Alpha, character2PCat('G'));
    //'H' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x48));
    EXPECT_EQ(PCat::Alpha, character2PCat('H'));
    //'I' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x49));
    EXPECT_EQ(PCat::Alpha, character2PCat('I'));
    //'J' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4a));
    EXPECT_EQ(PCat::Alpha, character2PCat('J'));
    //'K' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4b));
    EXPECT_EQ(PCat::Alpha, character2PCat('K'));
    //'L' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4c));
    EXPECT_EQ(PCat::Alpha, character2PCat('L'));
    //'M' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4d));
    EXPECT_EQ(PCat::Alpha, character2PCat('M'));
    //'N' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4e));
    EXPECT_EQ(PCat::Alpha, character2PCat('N'));
    //'O' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x4f));
    EXPECT_EQ(PCat::Alpha, character2PCat('O'));
    //'P' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x50));
    EXPECT_EQ(PCat::Alpha, character2PCat('P'));
    //'Q' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x51));
    EXPECT_EQ(PCat::Alpha, character2PCat('Q'));
    //'R' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x52));
    EXPECT_EQ(PCat::Alpha, character2PCat('R'));
    //'S' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x53));
    EXPECT_EQ(PCat::Alpha, character2PCat('S'));
    //'T' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x54));
    EXPECT_EQ(PCat::Alpha, character2PCat('T'));
    //'U' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x55));
    EXPECT_EQ(PCat::Alpha, character2PCat('U'));
    //'V' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x56));
    EXPECT_EQ(PCat::Alpha, character2PCat('V'));
    //'W' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x57));
    EXPECT_EQ(PCat::Alpha, character2PCat('W'));
    //'X' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x58));
    EXPECT_EQ(PCat::Alpha, character2PCat('X'));
    //'Y' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x59));
    EXPECT_EQ(PCat::Alpha, character2PCat('Y'));
    //'Z' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x5a));
    EXPECT_EQ(PCat::Alpha, character2PCat('Z'));

    //'g' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x67));
    EXPECT_EQ(PCat::Alpha, character2PCat('g'));
    //'h' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x68));
    EXPECT_EQ(PCat::Alpha, character2PCat('h'));
    //'i' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x69));
    EXPECT_EQ(PCat::Alpha, character2PCat('i'));
    //'j' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6a));
    EXPECT_EQ(PCat::Alpha, character2PCat('j'));
    //'k' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6b));
    EXPECT_EQ(PCat::Alpha, character2PCat('k'));
    //'l' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6c));
    EXPECT_EQ(PCat::Alpha, character2PCat('l'));
    //'m' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6d));
    EXPECT_EQ(PCat::Alpha, character2PCat('m'));
    //'n' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6e));
    EXPECT_EQ(PCat::Alpha, character2PCat('n'));
    //'o' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x6f));
    EXPECT_EQ(PCat::Alpha, character2PCat('o'));
    //'p' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x70));
    EXPECT_EQ(PCat::Alpha, character2PCat('p'));
    //'q' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x71));
    EXPECT_EQ(PCat::Alpha, character2PCat('q'));
    //'r' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x72));
    EXPECT_EQ(PCat::Alpha, character2PCat('r'));
    //'s' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x73));
    EXPECT_EQ(PCat::Alpha, character2PCat('s'));
    //'t' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x74));
    EXPECT_EQ(PCat::Alpha, character2PCat('t'));
    //'u' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x75));
    EXPECT_EQ(PCat::Alpha, character2PCat('u'));
    //'v' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x76));
    EXPECT_EQ(PCat::Alpha, character2PCat('v'));
    //'w' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x77));
    EXPECT_EQ(PCat::Alpha, character2PCat('w'));
    //'y' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x79));
    EXPECT_EQ(PCat::Alpha, character2PCat('y'));
    //'z' is Alpha
    EXPECT_EQ(PCat::Alpha, character2PCat(0x7a));
    EXPECT_EQ(PCat::Alpha, character2PCat('z'));
}

/**
 * '_' is converted to Underscore
 */
TEST(character2PCat, Underscore)
{
    //'_' is Underscore
    EXPECT_EQ(PCat::Underscore, character2PCat(0x5f));
    EXPECT_EQ(PCat::Underscore, character2PCat('_'));
}

/**
 * 'b' is converted to BinB
 */
TEST(character2PCat, BinB)
{
    //'b' is BinB
    EXPECT_EQ(PCat::BinB, character2PCat(0x62));
    EXPECT_EQ(PCat::BinB, character2PCat('b'));
}

/**
 * 'x' is converted to HexX
 */
TEST(character2PCat, HexX)
{
    //'x' is HexX
    EXPECT_EQ(PCat::HexX, character2PCat(0x78));
    EXPECT_EQ(PCat::HexX, character2PCat('x'));
}

/**
 * 0 and 1 are converted to BNum.
 */
TEST(character2PCat, BNum)
{
    //'0' is BNum
    EXPECT_EQ(PCat::BNum, character2PCat(0x30));
    EXPECT_EQ(PCat::BNum, character2PCat('0'));
    //'1' is BNum
    EXPECT_EQ(PCat::BNum, character2PCat(0x31));
    EXPECT_EQ(PCat::BNum, character2PCat('1'));
}

/**
 * 2-7 are converted to ONum.
 */
TEST(character2PCat, ONum)
{
    //'2' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x32));
    EXPECT_EQ(PCat::ONum, character2PCat('2'));
    //'3' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x33));
    EXPECT_EQ(PCat::ONum, character2PCat('3'));
    //'4' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x34));
    EXPECT_EQ(PCat::ONum, character2PCat('4'));
    //'5' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x35));
    EXPECT_EQ(PCat::ONum, character2PCat('5'));
    //'6' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x36));
    EXPECT_EQ(PCat::ONum, character2PCat('6'));
    //'7' is ONum
    EXPECT_EQ(PCat::ONum, character2PCat(0x37));
    EXPECT_EQ(PCat::ONum, character2PCat('7'));
}

/**
 * 8 and 9 are converted to DNum.
 */
TEST(character2PCat, DNum)
{
    //'8' is DNum
    EXPECT_EQ(PCat::DNum, character2PCat(0x38));
    EXPECT_EQ(PCat::DNum, character2PCat('8'));
    //'9' is DNum
    EXPECT_EQ(PCat::DNum, character2PCat(0x39));
    EXPECT_EQ(PCat::DNum, character2PCat('9'));
}

/**
 * A,B,C,D,F and a,c,d,f are converted to HNum.
 */
TEST(character2PCat, HNum)
{
    //'A' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x41));
    EXPECT_EQ(PCat::HNum, character2PCat('A'));
    //'B' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x42));
    EXPECT_EQ(PCat::HNum, character2PCat('B'));
    //'C' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x43));
    EXPECT_EQ(PCat::HNum, character2PCat('C'));
    //'D' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x44));
    EXPECT_EQ(PCat::HNum, character2PCat('D'));
    //'F' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x46));
    EXPECT_EQ(PCat::HNum, character2PCat('F'));

    //'a' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x61));
    EXPECT_EQ(PCat::HNum, character2PCat('a'));
    //'c' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x63));
    EXPECT_EQ(PCat::HNum, character2PCat('c'));
    //'d' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x64));
    EXPECT_EQ(PCat::HNum, character2PCat('d'));
    //'f' is HNum
    EXPECT_EQ(PCat::HNum, character2PCat(0x66));
    EXPECT_EQ(PCat::HNum, character2PCat('f'));
}

/**
 * '.' is converted to Dot
 */
TEST(character2PCat, Dot)
{
    //'.' is Dot
    EXPECT_EQ(PCat::Dot, character2PCat(0x2e));
    EXPECT_EQ(PCat::Dot, character2PCat('.'));
}

/**
 * '+' is converted to Plus
 */
TEST(character2PCat, Plus)
{
    //'+' is Plus
    EXPECT_EQ(PCat::Plus, character2PCat(0x2b));
    EXPECT_EQ(PCat::Plus, character2PCat('+'));
}

/**
 * '-' is converted to Minus
 */
TEST(character2PCat, Minus)
{
    //'-' is Plus
    EXPECT_EQ(PCat::Minus, character2PCat(0x2d));
    EXPECT_EQ(PCat::Minus, character2PCat('-'));
}

/**
 * '(' is converted to OParen
 */
TEST(character2PCat, OParen)
{
    //'(' is OParen
    EXPECT_EQ(PCat::OParen, character2PCat(0x28));
    EXPECT_EQ(PCat::OParen, character2PCat('('));
}

/**
 * ')' is converted to CParen
 */
TEST(character2PCat, CParen)
{
    //')' is CParen
    EXPECT_EQ(PCat::CParen, character2PCat(0x29));
    EXPECT_EQ(PCat::CParen, character2PCat(')'));
}

/**
 * ',' is converted to Comma
 */
TEST(character2PCat, Comma)
{
    //',' is Comma
    EXPECT_EQ(PCat::Comma, character2PCat(0x2C));
    EXPECT_EQ(PCat::Comma, character2PCat(','));
}

/**
 * '#' is converted to Hash
 */
TEST(character2PCat, Hash)
{
    //'#' is Hash
    EXPECT_EQ(PCat::Hash, character2PCat(0x23));
    EXPECT_EQ(PCat::Hash, character2PCat('#'));
}

/**
 * Any character with the high-bit set is converted to HighBit.
 */
TEST(character2PCat, HighBit)
{
    for (int i = 128; i < 256; ++i)
    {
        EXPECT_EQ(PCat::HighBit, character2PCat(i));
    }
}
