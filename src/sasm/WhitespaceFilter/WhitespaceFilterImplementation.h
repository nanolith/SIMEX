/**
 * \file sasm/WhitespaceFilter/WhitespaceFilterImplementation.h
 *
 * Private header for WhitespaceFilter.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_SASM_WHITESPACE_FILTER_IMPLEMENTATION_HEADER_GUARD
# define SIMEX_SASM_WHITESPACE_FILTER_IMPLEMENTATION_HEADER_GUARD

#include <istream>
#include <simex/sasm/PreprocessorLexer.h>
#include <type_traits>

//this header is C++ specific
#ifdef __cplusplus

namespace simex { namespace sasm {

/**
 * Private implementation details for WhitespaceFilter.
 */
struct WhitespaceFilterImplementation
{
    WhitespaceFilterImplementation(std::istream& in, int lineNumber)
        : in_(in), lineNumber_(lineNumber), haveCachedChar(false),
          cachedChar(-1), inputState(0)
    {
    }

    std::istream& in_;
    int lineNumber_;
    bool haveCachedChar;
    int cachedChar;
    int inputState;
};

/**
 * Function type for filter actions in the filter state machine.
 */
typedef bool (*filter_method_t)(WhitespaceFilterImplementation*, int*);

/**
 * Whitespace filter states.
 */
enum class WhitespaceFilterState
{
    Init            = 0,
    InSpace,
    MaybeComment,
    LineComment,
    BlockComment,
    MaybeEndComment,
    EndOfFile
};

/**
 * Whitespace filter state to index.
 */
inline int whitespaceFilterState2index(WhitespaceFilterState st)
{
    return static_cast<std::underlying_type<WhitespaceFilterState>::type>(st);
}

/* namespace sasm */ } /* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_SASM_WHITESPACE_FILTER_IMPLEMENTATION_HEADER_GUARD
