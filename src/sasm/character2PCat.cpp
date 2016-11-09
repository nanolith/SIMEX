/**
 * \file sasm/character2PCat.cpp
 *
 * character2PCat implementation.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>

using namespace simex::sasm;
using namespace std;

static PCat pcatTranslationArray[256] = {
    //0x00             0x01               0x02               0x03
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x04             0x05               0x06               0x07 BEL
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Whitespace,
    //0x08             0x09 TAB           0x0a LF            0x0b VT
    PCat::Unknown,     PCat::Whitespace,  PCat::LineEnd,     PCat::Whitespace,
    //0x0c FF          0x0d CR            0x0e               0x0f
    PCat::Whitespace,  PCat::Whitespace,  PCat::Unknown,     PCat::Unknown,
    //0x10             0x11               0x12               0x13
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x14             0x15               0x16               0x17
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x18             0x19               0x1a               0x1b
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x1c             0x1d               0x1e               0x1f
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x20 ' '         0x21 '!'           0x22 '"'           0x23 '#'
    PCat::Whitespace,  PCat::Bang,        PCat::DoubleQuote, PCat::Hash,
    //0x24             0x25               0x26               0x27
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x28 '('         0x29 ')'           0x2a '*'           0x2b '+'
    PCat::OParen,      PCat::CParen,      PCat::Star,        PCat::Plus,
    //0x2c ','         0x2d '-'           0x2e '.'           0x2f '/'
    PCat::Comma,       PCat::Minus,       PCat::Dot,         PCat::FSlash,
    //0x30 '0'         0x31 '1'           0x32 '2'           0x33 '3'
    PCat::BNum,        PCat::BNum,        PCat::ONum,        PCat::ONum,
    //0x34 '4'         0x35 '5'           0x36 '6'           0x37 '7'
    PCat::ONum,        PCat::ONum,        PCat::ONum,        PCat::ONum,
    //0x38 '8'         0x39 '9'           0x3a               0x3b
    PCat::DNum,        PCat::DNum,        PCat::Unknown,     PCat::Unknown,
    //0x3c '<'         0x3d '='           0x3e '>'           0x3f
    PCat::Lt,          PCat::Eq,          PCat::Gt,          PCat::Unknown,
    //0x40             0x41 'A'           0x42 'B'           0x43 'C'
    PCat::Unknown,     PCat::HNum,        PCat::HNum,        PCat::HNum,
    //0x44 'D'         0x45 'E'           0x46 'F'           0x47 'G'
    PCat::HNum,        PCat::Exp,         PCat::HNum,        PCat::Alpha,
    //0x48 'H'         0x49 'I'           0x4a 'J'           0x4b 'K'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x4c 'L'         0x4d 'M'           0x4e 'N'           0x4f 'O'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x50 'P'         0x51 'Q'           0x52 'R'           0x53 'S'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x54 'T'         0x55 'U'           0x56 'V'           0x57 'W'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x58 'X'         0x59 'Y'           0x5a 'Z'           0x5b
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Unknown,
    //0x5c '\\'        0x5d               0x5e               0x5f '_'
    PCat::BSlash,      PCat::Unknown,     PCat::Unknown,     PCat::Underscore,
    //0x60             0x61 'a'           0x62 'b'           0x63 'c'
    PCat::Unknown,     PCat::HNum,        PCat::BinB,        PCat::HNum,
    //0x64 'd'         0x65 'e'           0x66 'f'           0x67 'g'
    PCat::HNum,        PCat::Exp,         PCat::HNum,        PCat::Alpha,
    //0x68 'h'         0x69 'i'           0x6a 'j'           0x6b 'k'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x6c 'l'         0x6d 'm'           0x6e 'n'           0x6f 'o'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x70 'p'         0x71 'q'           0x72 'r'           0x73 's'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x74 't'         0x75 'u'           0x76 'v'           0x77 'w'
    PCat::Alpha,       PCat::Alpha,       PCat::Alpha,       PCat::Alpha,
    //0x78 'x'         0x79 'y'           0x7a 'z'           0x7b
    PCat::HexX,        PCat::Alpha,       PCat::Alpha,       PCat::Unknown,
    //0x7c             0x7d               0x7e               0x7f
    PCat::Unknown,     PCat::Unknown,     PCat::Unknown,     PCat::Unknown,
    //0x80             0x81               0x82               0x83
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x84             0x85               0x86               0x87
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x88             0x89               0x8a               0x8b
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x8c             0x8d               0x8e               0x8f
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x90             0x91               0x92               0x93
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x94             0x95               0x96               0x97
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x98             0x99               0x9a               0x9b
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0x9c             0x9d               0x9e               0x9f
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xa0             0xa1               0xa2               0xa3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xa4             0xa5               0xa6               0xa7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xa8             0xa9               0xaa               0xab
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xac             0xad               0xae               0xaf
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xb0             0xb1               0xb2               0xb3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xb4             0xb5               0xb6               0xb7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xb8             0xb9               0xba               0xbb
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xbc             0xbd               0xbe               0xbf
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xc0             0xc1               0xc2               0xc3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xc4             0xc5               0xc6               0xc7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xc8             0xc9               0xca               0xcb
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xcc             0xcd               0xce               0xcf
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xd0             0xd1               0xd2               0xd3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xd4             0xd5               0xd6               0xd7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xd8             0xd9               0xda               0xdb
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xdc             0xdd               0xde               0xdf
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xe0             0xe1               0xe2               0xe3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xe4             0xe5               0xe6               0xe7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xe8             0xe9               0xea               0xeb
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xec             0xed               0xee               0xef
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xf0             0xf1               0xf2               0xf3
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xf4             0xf5               0xf6               0xf7
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xf8             0xf9               0xfa               0xfb
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit,
    //0xfc             0xfd               0xfe               0xff
    PCat::HighBit,     PCat::HighBit,     PCat::HighBit,     PCat::HighBit };

/**
 * Convert a character to a preprocesser input category.
 * This is a lossy conversion.
 *
 * \param ch        The character to convert.
 *
 * \returns the preprocessor input category for this character.
 */
PCat simex::sasm::character2PCat(int ch)
{
    if (ch < 0 || ch > 255)
        return PCat::EndOfFile;

    return pcatTranslationArray[(uint8_t)ch];
}
