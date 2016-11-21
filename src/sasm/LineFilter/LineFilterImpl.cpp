/**
 * \file sasm/LineFilter/LineFilterImpl.cpp
 *
 * Constructor for LineFilterImpl.
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
 * Constructor.  Create a LineFilterImpl from an input stream.
 */
LineFilterImpl::LineFilterImpl(std::istream& in)
    : in_(in), lineNumber_(1), columnNumber_(0)
{
}
