/**
 * \file sasm/PreprocessorLexer/PreprocessorLexer.cpp
 *
 * Constructor for PreprocessorLexer.
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
 * A PreprocessorLexer is created from an input stream.
 *
 * \param istream       The input stream used for this filter.
 */
PreprocessorLexer::PreprocessorLexer(std::istream& in)
    : impl_(make_unique<PreprocessorLexerImplementation>(in))
{
}
