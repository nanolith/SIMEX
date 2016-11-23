/**
 * \file sasm/PreprocessorLexer/PreprocessorLexerImplementation.h
 *
 * Private header for PreprocessorLexer.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_SASM_PREPROCESSOR_LEXER_IMPLEMENTATION_HEADER_GUARD
# define SIMEX_SASM_PREPROCESSOR_LEXER_IMPLEMENTATION_HEADER_GUARD

#include <istream>
#include <simex/sasm/PreprocessorLexer.h>
#include <type_traits>

//this header is C++ specific
#ifdef __cplusplus

namespace simex { namespace sasm {

/**
 * Preprocessor Lexer states.
 */
enum class PreprocessorLexerState
{
    Init            = 0,
    Identifier,
};

/**
 * Preprocessor Lexer state to index.
 */
inline int preprocessorLexerState2index(PreprocessorLexerState st)
{
    return static_cast<std::underlying_type<PreprocessorLexerState>::type>(st);
}

/**
 * Private implementation details for PreprocessorLexer.
 */
struct PreprocessorLexerImplementation
{
    PreprocessorLexerImplementation(std::istream& in)
        : filt_(in), haveCachedChar(false), cachedChar(0), inputState(0),
          line(1), column(0), isEof(false)
    {
    }

    WhitespaceFilter filt_;
    bool haveCachedChar;
    int cachedChar;
    int inputState;
    int line;
    int column;
    bool isEof;
};

/**
 * Function type for preprocessor actions in the preprocessor state machine.
 */
typedef bool (*preprocessor_method_t)(PreprocessorLexerImplementation*, int,
                                      PTok*);

/* namespace sasm */ } /* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_SASM_PREPROCESSOR_LEXER_IMPLEMENTATION_HEADER_GUARD
