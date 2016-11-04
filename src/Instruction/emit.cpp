/**
 * \file Instruction/emit.cpp
 *
 * Instruction::emit() implementation.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <ostream>
#include <simex/Instruction.h>

using namespace simex;
using namespace std;

/**
 * The emit method writes an Instruction to the given binary ostream.
 *
 * \param out       The ostream to which this instruction is emitted.
 */
void Instruction::emit(std::ostream& out) const
{
    out.put(static_cast<char>(opcode()));
    out.put(static_cast<char>(x()));
    out.put(static_cast<char>(y()));
    out.put(static_cast<char>(z()));
}
