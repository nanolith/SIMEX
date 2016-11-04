/**
 * \file TestSReg.cpp
 *
 * Test that SRegs match the SIMEX specification.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <gtest/gtest.h>
#include <simex/SReg.h>

using namespace simex;

/**
 * Test that all special register values match the SIMEX specification.
 */
TEST(SReg, sreg_spec)
{
    EXPECT_EQ(0x1F,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RCC));
    EXPECT_EQ(0x01,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RD));
    EXPECT_EQ(0x02,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RE));
    EXPECT_EQ(0x00,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x00));
    EXPECT_EQ(0x09,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x09));
    EXPECT_EQ(0x0A,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x0A));
    EXPECT_EQ(0x0D,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x0D));
    EXPECT_EQ(0x0E,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x0E));
    EXPECT_EQ(0x0F,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x0F));
    EXPECT_EQ(0x12,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x12));
    EXPECT_EQ(0x13,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x13));
    EXPECT_EQ(0x14,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x14));
    EXPECT_EQ(0x15,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x15));
    EXPECT_EQ(0x16,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x16));
    EXPECT_EQ(0x17,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x17));
    EXPECT_EQ(0x18,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RESERVED_x18));
    EXPECT_EQ(0x1A,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RF));
    EXPECT_EQ(0x19,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RFF));
    EXPECT_EQ(0x10,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RG));
    EXPECT_EQ(0x03,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RH));
    EXPECT_EQ(0x04,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RJ));
    EXPECT_EQ(0x05,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RJJ));
    EXPECT_EQ(0x11,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RL));
    EXPECT_EQ(0x06,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RM));
    EXPECT_EQ(0x0B,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RO));
    EXPECT_EQ(0x1B,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_ROP));
    EXPECT_EQ(0x08,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RP));
    EXPECT_EQ(0x07,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RR));
    EXPECT_EQ(0x0C,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RS));
    EXPECT_EQ(0x1C,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RXX));
    EXPECT_EQ(0x1D,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RYY));
    EXPECT_EQ(0x1E,
        static_cast<std::underlying_type<SReg>::type>(SReg::SR_RZZ));
}
