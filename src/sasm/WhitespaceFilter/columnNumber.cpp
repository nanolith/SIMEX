/**
 * \file sasm/WhitespaceFilter/columnNumber.cpp
 *
 * Implementation of WhitespaceFilter::columnNumber()
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
 * Get the current column number.
 */
int WhitespaceFilter::columnNumber()
{
    if (impl_->isEof)
        return 0;

    return impl_->in_.columnNumber();
}
