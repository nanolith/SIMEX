/**
 * \file sasm/LineFilter/get.cpp
 *
 * Implementation of LineFilterImpl::get().
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <istream>
#include <simex/sasm/PreprocessorLexer.h>

using namespace simex::sasm;

/**
 * The get method works like istream::get(), except that the input is
 * filtered.
 *
 * \returns a filtered input byte, or EOF on EOF.
 */
int LineFilterImpl::get()
{
    int ch = in_.get();

    if (ch == '\n')
    {
        ++lineNumber_;
        columnNumber_ = 0;
    }
    else if (ch != EOF)
    {
        ++columnNumber_;
    }

    return ch;
}
