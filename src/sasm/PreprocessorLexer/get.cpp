/**
 * \file sasm/PreprocessorLexer/get.cpp
 *
 * Implementation of PreprocessorLexer::get() method.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>
#include "PreprocessorLexerImplementation.h"

using namespace simex::sasm;
using namespace std;

/**
 * The get method returns a token from the input stream.
 *
 * \returns a token from the input stream, or PTok::EndOfFile if the end of
 * the input stream has been reached.
 */
PTok PreprocessorLexer::get()
{
    int ch = impl_->filt_.get();

    if (ch == '\n')
        return PTok::Newline;
    else if (ch == EOF)
        return PTok::EndOfFile;
    else
        return PTok::Unknown;
}
