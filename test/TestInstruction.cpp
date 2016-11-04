/**
 * \file TestInstruction.cpp
 *
 * Test the Instruction class.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <gtest/gtest.h>
#include <simex/Instruction.h>
#include <sstream>

using namespace simex;
using namespace std;

/**
 * Test that we can decode an instruction.
 */
TEST(Instruction, decode_simple)
{
    const Opcode OP = Opcode::OP_SYSCALL;
    const uint8_t X = 2;
    const uint8_t Y = 21;
    const uint8_t Z = 17;

    //test that we can decode this instruction
    auto ins = Instruction::decode(OP, X, Y, Z);
    //the instruction should be valid
    ASSERT_TRUE(!!ins);

    //test accessors
    EXPECT_EQ(OP, ins->opcode());
    EXPECT_EQ(X, ins->x());
    EXPECT_EQ(Y, ins->y());
    EXPECT_EQ(Z, ins->z());
}

/**
 * Test that we can emit an instruction.
 */
TEST(Instruction, emit_simple)
{
    const Opcode OP = Opcode::OP_SYSCALL;
    const uint8_t X = 2;
    const uint8_t Y = 21;
    const uint8_t Z = 17;

    stringstream ss;

    //decode our instruction
    auto ins = Instruction::decode(OP, X, Y, Z);
    //the instruction should be valid
    ASSERT_TRUE(!!ins);

    //emit this instruction to our string stream
    ins->emit(ss);

    //read the values from the stream
    uint8_t op, x, y, z, e;
    ASSERT_TRUE(!!ss.get((char&)op));
    ASSERT_TRUE(!!ss.get((char&)x));
    ASSERT_TRUE(!!ss.get((char&)y));
    ASSERT_TRUE(!!ss.get((char&)z));
    ASSERT_FALSE(!!ss.get((char&)e));

    //test that the read values match the decoded values
    EXPECT_EQ(static_cast<std::underlying_type<Opcode>::type>(OP), op);
    EXPECT_EQ(X, x);
    EXPECT_EQ(Y, y);
    EXPECT_EQ(Z, z);
}
