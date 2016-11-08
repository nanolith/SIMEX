/**
 * \file sasm/WhitespaceFilter/lineNumber.cpp
 *
 * Implementation of WhitespaceFilter::lineNumber()
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>
#include "WhitespaceFilterImplementation.h"

using namespace simex::sasm;
using namespace std;

/**
 * Returns the current physical line number for this input stream.
 */
int WhitespaceFilter::lineNumber()
{
    return impl_->lineNumber_;
}
