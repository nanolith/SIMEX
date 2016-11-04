/**
 * \file Instruction/evaluate.cpp
 *
 * Instruction::evaluate() implementation.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/Instruction.h>

using namespace simex;
using namespace std;

/**
 * By default, the evaluate method places the machine into an invalid opcode
 * exception state.  This behavior can be overridden by derived types.
 *
 * \param state     The current machine state.
 *
 * \sideeffect The machine state is updated based upon the evaluation of
 * this instruction.
 */
void Instruction::evaluate(MachineState* state) const
{
    //TODO - implement
}
