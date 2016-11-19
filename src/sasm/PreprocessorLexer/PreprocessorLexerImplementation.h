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
 * Private implementation details for PreprocessorLexer.
 */
struct PreprocessorLexerImplementation
{
    PreprocessorLexerImplementation(std::istream& in)
        : filt_(in), inputState(0)
    {
    }

    WhitespaceFilter filt_;
    int inputState;
};

/* namespace sasm */ } /* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_SASM_PREPROCESSOR_LEXER_IMPLEMENTATION_HEADER_GUARD
