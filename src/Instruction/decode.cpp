/**
 * \file Instruction/decode.cpp
 *
 * Instruction::decode() implementation.
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

namespace {

    /**
     * Internal concrete implementation of Instruction that can be used for
     * invalid instructions.
     */
    class InvalidInstruction : public Instruction
    {
    public:
        InvalidInstruction(Opcode op, uint8_t x, uint8_t y, uint8_t z)
            : Instruction(op, x, y, z)
        {
        }
    };
}

/**
 * The decode method is a factory method which creates an Instruction.
 *
 * \param op    The opcode for this instruction.
 * \param x     The X value for this instruction.
 * \param y     The Y value for this instruction.
 * \param z     The Z value for this instruction.
 *
 * \returns the decoded instruction, which can be emitted or evaluated.
 */
shared_ptr<Instruction>
Instruction::decode(Opcode op, std::uint8_t x, std::uint8_t y, std::uint8_t z)
{
    return make_shared<InvalidInstruction>(op, x, y, z);
}
