/**
 * \file sasm/LineFilter/columnNumber.cpp
 *
 * Implementation of LineFilterImpl::columnNumber().
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>

using namespace simex::sasm;

/**
 * Get the current column number.
 */
int LineFilterImpl::columnNumber()
{
    return columnNumber_;
}
