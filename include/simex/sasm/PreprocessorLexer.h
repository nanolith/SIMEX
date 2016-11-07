/**
 * \file PreprocessorLexer.h
 *
 * Lexer for the Preprocessor.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD
# define SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD

#include <cstdint>
#include <type_traits>

//this header is C++ specific
#ifdef __cplusplus

namespace simex { namespace sasm {

/**
 * Preprocessor Input Category.
 *
 * The preprocessor lexer categorizes input bytes as belonging to one of the
 * following categories.  These categories are then used by the lexer automaton
 * to create tokens for the preprocessor's parser.
 */
enum class PCat
{
    //all whitespace characters
    Whitespace                      = 0,
    //line end character
    LineEnd,
    //forward slash
    FSlash,
    //backslash
    BSlash,
    //*
    Star,
    //!
    Bang,
    //"
    DoubleQuote,
    //e, E
    Exp,
    //alphabetical characters, other than e, E, A-F, and a-f
    Alpha,
    //_
    Underscore,
    //0, 1
    BNum,
    //2, 3, 4, 5, 6, 7
    ONum,
    //8, 9
    DNum,
    //A-F, a-f
    HNum,
    //.
    Dot,
    //+
    Plus,
    //-
    Minus,
    //(
    OParen,
    //)
    CParen,
    //,
    Comma,
    //#
    Hash,
    //a byte with the high bit set
    HighBit,
    //all other bytes
    Unknown
};

/**
 * Convert a byte to a preprocesser input category.  This is a lossy conversion.
 *
 * \param by        The byte to convert.
 *
 * \returns the preprocessor input category for this byte.
 */
PCat byte2PCat(std::uint8_t by);

/**
 * Convert a preprocessor input category to an index.
 *
 * \param cat       The input category to convert.
 *
 * \returns a 0-based index representing the input category.
 */
inline int pcat2index(PCat cat)
{
    //explicit conversion to int
    return static_cast<std::underlying_type<PCat>::type>(cat);
}

/* namespace sasm */ } /* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD
