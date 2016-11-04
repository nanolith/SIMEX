/**
 * \file TestOpcode.cpp
 *
 * Test that Opcodes match the SIMEX specification.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <gtest/gtest.h>
#include <simex/Opcode.h>

using namespace simex;

/**
 * Test that all instruction opcodes match the SIMEX specification.
 */
TEST(Opcode, opcode_spec)
{
    EXPECT_EQ((uint8_t)0x2E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_16ADDU));
    EXPECT_EQ((uint8_t)0x2F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_16ADDUI));
    EXPECT_EQ((uint8_t)0x28,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_2ADDU));
    EXPECT_EQ((uint8_t)0x29,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_2ADDUI));
    EXPECT_EQ((uint8_t)0x2A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_4ADDU));
    EXPECT_EQ((uint8_t)0x2B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_4ADDUI));
    EXPECT_EQ((uint8_t)0x2C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_8ADDU));
    EXPECT_EQ((uint8_t)0x2D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_8ADDUI));
    EXPECT_EQ((uint8_t)0x20,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ADD));
    EXPECT_EQ((uint8_t)0x21,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ADDI));
    EXPECT_EQ((uint8_t)0x22,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ADDU));
    EXPECT_EQ((uint8_t)0x23,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ADDUI));
    EXPECT_EQ((uint8_t)0xC8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_AND));
    EXPECT_EQ((uint8_t)0xC9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDI));
    EXPECT_EQ((uint8_t)0xCA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDN));
    EXPECT_EQ((uint8_t)0xEC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDNH));
    EXPECT_EQ((uint8_t)0xCB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDNI));
    EXPECT_EQ((uint8_t)0xEF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDNL));
    EXPECT_EQ((uint8_t)0xED,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDNMH));
    EXPECT_EQ((uint8_t)0xEE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ANDNML));
    EXPECT_EQ((uint8_t)0x4E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BEV));
    EXPECT_EQ((uint8_t)0x4F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BEVB));
    EXPECT_EQ((uint8_t)0x40,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BN));
    EXPECT_EQ((uint8_t)0x41,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNB));
    EXPECT_EQ((uint8_t)0x48,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNN));
    EXPECT_EQ((uint8_t)0x49,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNNB));
    EXPECT_EQ((uint8_t)0x4C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNP));
    EXPECT_EQ((uint8_t)0x4D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNPB));
    EXPECT_EQ((uint8_t)0x4A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNZ));
    EXPECT_EQ((uint8_t)0x4B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BNZB));
    EXPECT_EQ((uint8_t)0x46,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BOD));
    EXPECT_EQ((uint8_t)0x47,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BODB));
    EXPECT_EQ((uint8_t)0x44,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BP));
    EXPECT_EQ((uint8_t)0x45,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BPB));
    EXPECT_EQ((uint8_t)0x42,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BZ));
    EXPECT_EQ((uint8_t)0x43,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_BZB));
    EXPECT_EQ((uint8_t)0x30,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CMP));
    EXPECT_EQ((uint8_t)0x31,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CMPI));
    EXPECT_EQ((uint8_t)0x32,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CMPU));
    EXPECT_EQ((uint8_t)0x33,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CMPUI));
    EXPECT_EQ((uint8_t)0x6E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSEV));
    EXPECT_EQ((uint8_t)0x6F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSEVI));
    EXPECT_EQ((uint8_t)0x60,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSN));
    EXPECT_EQ((uint8_t)0x61,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNI));
    EXPECT_EQ((uint8_t)0x68,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNN));
    EXPECT_EQ((uint8_t)0x69,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNNI));
    EXPECT_EQ((uint8_t)0x6C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNP));
    EXPECT_EQ((uint8_t)0x6D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNPI));
    EXPECT_EQ((uint8_t)0x6A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNZ));
    EXPECT_EQ((uint8_t)0x6B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSNZI));
    EXPECT_EQ((uint8_t)0x66,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSOD));
    EXPECT_EQ((uint8_t)0x67,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSODI));
    EXPECT_EQ((uint8_t)0x64,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSP));
    EXPECT_EQ((uint8_t)0x65,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSPI));
    EXPECT_EQ((uint8_t)0x94,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSWAP));
    EXPECT_EQ((uint8_t)0x95,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSWAPI));
    EXPECT_EQ((uint8_t)0x62,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSZ));
    EXPECT_EQ((uint8_t)0x63,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_CSZI));
    EXPECT_EQ((uint8_t)0x1C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_DIV));
    EXPECT_EQ((uint8_t)0x1D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_DIVI));
    EXPECT_EQ((uint8_t)0x1E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_DIVU));
    EXPECT_EQ((uint8_t)0x1F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_DIVUI));
    EXPECT_EQ((uint8_t)0x04,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FADD));
    EXPECT_EQ((uint8_t)0x01,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FCMP));
    EXPECT_EQ((uint8_t)0x11,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FCMPE));
    EXPECT_EQ((uint8_t)0x14,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FDIV));
    EXPECT_EQ((uint8_t)0x03,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FEQL));
    EXPECT_EQ((uint8_t)0x13,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FEQLE));
    EXPECT_EQ((uint8_t)0x17,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FINT));
    EXPECT_EQ((uint8_t)0x05,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FIX));
    EXPECT_EQ((uint8_t)0x07,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FIXU));
    EXPECT_EQ((uint8_t)0x08,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FLOT));
    EXPECT_EQ((uint8_t)0x09,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FLOTI));
    EXPECT_EQ((uint8_t)0x0A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FLOTU));
    EXPECT_EQ((uint8_t)0x0B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FLOTUI));
    EXPECT_EQ((uint8_t)0x10,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FMUL));
    EXPECT_EQ((uint8_t)0x16,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FREM));
    EXPECT_EQ((uint8_t)0x15,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FSQRT));
    EXPECT_EQ((uint8_t)0x06,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FSUB));
    EXPECT_EQ((uint8_t)0x02,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FUN));
    EXPECT_EQ((uint8_t)0x12,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_FUNE));
    EXPECT_EQ((uint8_t)0xFE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_GET));
    EXPECT_EQ((uint8_t)0xF4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_GETA));
    EXPECT_EQ((uint8_t)0xF5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_GETAB));
    EXPECT_EQ((uint8_t)0x9E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_GO));
    EXPECT_EQ((uint8_t)0x9F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_GOI));
    EXPECT_EQ((uint8_t)0xE4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_INCH));
    EXPECT_EQ((uint8_t)0xE7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_INCL));
    EXPECT_EQ((uint8_t)0xE5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_INCMH));
    EXPECT_EQ((uint8_t)0xE6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_INCML));
    EXPECT_EQ((uint8_t)0xF0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_JMP));
    EXPECT_EQ((uint8_t)0xF1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_JMPB));
    EXPECT_EQ((uint8_t)0x80,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDB));
    EXPECT_EQ((uint8_t)0x81,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDBI));
    EXPECT_EQ((uint8_t)0x82,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDBU));
    EXPECT_EQ((uint8_t)0x83,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDBUI));
    EXPECT_EQ((uint8_t)0x92,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDHT));
    EXPECT_EQ((uint8_t)0x93,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDHTI));
    EXPECT_EQ((uint8_t)0x8C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDO));
    EXPECT_EQ((uint8_t)0x8D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDOI));
    EXPECT_EQ((uint8_t)0x8E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDOU));
    EXPECT_EQ((uint8_t)0x8F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDOUI));
    EXPECT_EQ((uint8_t)0x90,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDSF));
    EXPECT_EQ((uint8_t)0x91,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDSFI));
    EXPECT_EQ((uint8_t)0x88,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDT));
    EXPECT_EQ((uint8_t)0x89,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDTI));
    EXPECT_EQ((uint8_t)0x8A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDTU));
    EXPECT_EQ((uint8_t)0x8B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDTUI));
    EXPECT_EQ((uint8_t)0x96,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDUNC));
    EXPECT_EQ((uint8_t)0x97,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDUNCI));
    EXPECT_EQ((uint8_t)0x84,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDW));
    EXPECT_EQ((uint8_t)0x85,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDWI));
    EXPECT_EQ((uint8_t)0x86,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDWU));
    EXPECT_EQ((uint8_t)0x87,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_LDWUI));
    EXPECT_EQ((uint8_t)0x18,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MUL));
    EXPECT_EQ((uint8_t)0x19,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MULI));
    EXPECT_EQ((uint8_t)0x1A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MULU));
    EXPECT_EQ((uint8_t)0x1B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MULUI));
    EXPECT_EQ((uint8_t)0xD8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MUX));
    EXPECT_EQ((uint8_t)0xD9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_MUXI));
    EXPECT_EQ((uint8_t)0xCC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NAND));
    EXPECT_EQ((uint8_t)0xCD,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NANDI));
    EXPECT_EQ((uint8_t)0x34,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NEG));
    EXPECT_EQ((uint8_t)0x35,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NEGI));
    EXPECT_EQ((uint8_t)0x36,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NEGU));
    EXPECT_EQ((uint8_t)0x37,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NEGUI));
    EXPECT_EQ((uint8_t)0xC4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NOR));
    EXPECT_EQ((uint8_t)0xC5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NORI));
    EXPECT_EQ((uint8_t)0xCE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NXOR));
    EXPECT_EQ((uint8_t)0xCF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_NXORI));
    EXPECT_EQ((uint8_t)0xC0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_OR));
    EXPECT_EQ((uint8_t)0xE8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORH));
    EXPECT_EQ((uint8_t)0xC1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORI));
    EXPECT_EQ((uint8_t)0xEB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORL));
    EXPECT_EQ((uint8_t)0xE9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORMH));
    EXPECT_EQ((uint8_t)0xEA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORML));
    EXPECT_EQ((uint8_t)0xC2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORN));
    EXPECT_EQ((uint8_t)0xC3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ORNI));
    EXPECT_EQ((uint8_t)0x5E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBEV));
    EXPECT_EQ((uint8_t)0x5F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBEVB));
    EXPECT_EQ((uint8_t)0x50,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBN));
    EXPECT_EQ((uint8_t)0x51,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNB));
    EXPECT_EQ((uint8_t)0x58,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNN));
    EXPECT_EQ((uint8_t)0x59,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNNB));
    EXPECT_EQ((uint8_t)0x5C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNP));
    EXPECT_EQ((uint8_t)0x5D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNPB));
    EXPECT_EQ((uint8_t)0x5A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNZ));
    EXPECT_EQ((uint8_t)0x5B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBNZB));
    EXPECT_EQ((uint8_t)0x56,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBOD));
    EXPECT_EQ((uint8_t)0x57,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBODB));
    EXPECT_EQ((uint8_t)0x54,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBP));
    EXPECT_EQ((uint8_t)0x55,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBPB));
    EXPECT_EQ((uint8_t)0x52,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBZ));
    EXPECT_EQ((uint8_t)0x53,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PBZB));
    EXPECT_EQ((uint8_t)0xF8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_POP));
    EXPECT_EQ((uint8_t)0xBE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUSHGO));
    EXPECT_EQ((uint8_t)0xBF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUSHGOI));
    EXPECT_EQ((uint8_t)0xF2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUSHJ));
    EXPECT_EQ((uint8_t)0xF3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUSHJB));
    EXPECT_EQ((uint8_t)0xF6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUT));
    EXPECT_EQ((uint8_t)0xF7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_PUTI));
    EXPECT_EQ((uint8_t)0x98,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x98));
    EXPECT_EQ((uint8_t)0x99,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x99));
    EXPECT_EQ((uint8_t)0x9A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x9A));
    EXPECT_EQ((uint8_t)0x9B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x9B));
    EXPECT_EQ((uint8_t)0x9C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x9C));
    EXPECT_EQ((uint8_t)0x9D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_x9D));
    EXPECT_EQ((uint8_t)0xB8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xB8));
    EXPECT_EQ((uint8_t)0xB9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xB9));
    EXPECT_EQ((uint8_t)0xBA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xBA));
    EXPECT_EQ((uint8_t)0xBB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xBB));
    EXPECT_EQ((uint8_t)0xBC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xBC));
    EXPECT_EQ((uint8_t)0xBD,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xBD));
    EXPECT_EQ((uint8_t)0xD0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD0));
    EXPECT_EQ((uint8_t)0xD1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD1));
    EXPECT_EQ((uint8_t)0xD2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD2));
    EXPECT_EQ((uint8_t)0xD3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD3));
    EXPECT_EQ((uint8_t)0xD4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD4));
    EXPECT_EQ((uint8_t)0xD5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD5));
    EXPECT_EQ((uint8_t)0xD6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD6));
    EXPECT_EQ((uint8_t)0xD7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xD7));
    EXPECT_EQ((uint8_t)0xDA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDA));
    EXPECT_EQ((uint8_t)0xDB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDB));
    EXPECT_EQ((uint8_t)0xDC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDC));
    EXPECT_EQ((uint8_t)0xDD,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDD));
    EXPECT_EQ((uint8_t)0xDE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDE));
    EXPECT_EQ((uint8_t)0xDF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xDF));
    EXPECT_EQ((uint8_t)0xFF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESERVED_xFF));
    EXPECT_EQ((uint8_t)0xF9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_RESUME));
    EXPECT_EQ((uint8_t)0xFA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SAVE));
    EXPECT_EQ((uint8_t)0xE0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SETH));
    EXPECT_EQ((uint8_t)0xE3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SETL));
    EXPECT_EQ((uint8_t)0xE1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SETMH));
    EXPECT_EQ((uint8_t)0xE2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SETML));
    EXPECT_EQ((uint8_t)0x0C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SFLOT));
    EXPECT_EQ((uint8_t)0x0D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SFLOTI));
    EXPECT_EQ((uint8_t)0x0E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SFLOTU));
    EXPECT_EQ((uint8_t)0x0F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SFLOTUI));
    EXPECT_EQ((uint8_t)0x38,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SL));
    EXPECT_EQ((uint8_t)0x39,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SLI));
    EXPECT_EQ((uint8_t)0x3A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SLU));
    EXPECT_EQ((uint8_t)0x3B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SLUI));
    EXPECT_EQ((uint8_t)0x3C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SR));
    EXPECT_EQ((uint8_t)0x3D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SRI));
    EXPECT_EQ((uint8_t)0x3E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SRU));
    EXPECT_EQ((uint8_t)0x3F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SRUI));
    EXPECT_EQ((uint8_t)0xA0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STB));
    EXPECT_EQ((uint8_t)0xA1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STBI));
    EXPECT_EQ((uint8_t)0xA2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STBU));
    EXPECT_EQ((uint8_t)0xA3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STBUI));
    EXPECT_EQ((uint8_t)0xB4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STCO));
    EXPECT_EQ((uint8_t)0xB5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STCOI));
    EXPECT_EQ((uint8_t)0xB2,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STHT));
    EXPECT_EQ((uint8_t)0xB3,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STHTI));
    EXPECT_EQ((uint8_t)0xAC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STO));
    EXPECT_EQ((uint8_t)0xAD,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STOI));
    EXPECT_EQ((uint8_t)0xAE,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STOU));
    EXPECT_EQ((uint8_t)0xAF,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STOUI));
    EXPECT_EQ((uint8_t)0xB0,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STSF));
    EXPECT_EQ((uint8_t)0xB1,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STSFI));
    EXPECT_EQ((uint8_t)0xA8,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STT));
    EXPECT_EQ((uint8_t)0xA9,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STTI));
    EXPECT_EQ((uint8_t)0xAA,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STTU));
    EXPECT_EQ((uint8_t)0xAB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STTUI));
    EXPECT_EQ((uint8_t)0xB6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STUNC));
    EXPECT_EQ((uint8_t)0xB7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STUNCI));
    EXPECT_EQ((uint8_t)0xA4,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STW));
    EXPECT_EQ((uint8_t)0xA5,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STWI));
    EXPECT_EQ((uint8_t)0xA6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STWU));
    EXPECT_EQ((uint8_t)0xA7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_STWUI));
    EXPECT_EQ((uint8_t)0x24,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SUB));
    EXPECT_EQ((uint8_t)0x25,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SUBI));
    EXPECT_EQ((uint8_t)0x26,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SUBU));
    EXPECT_EQ((uint8_t)0x27,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SUBUI));
    EXPECT_EQ((uint8_t)0xFD,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SWYM));
    EXPECT_EQ((uint8_t)0xFC,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SYNC));
    EXPECT_EQ((uint8_t)0x00,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_SYSCALL));
    EXPECT_EQ((uint8_t)0xFB,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_UNSAVE));
    EXPECT_EQ((uint8_t)0xC6,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_XOR));
    EXPECT_EQ((uint8_t)0xC7,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_XORI));
    EXPECT_EQ((uint8_t)0x7E,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSEV));
    EXPECT_EQ((uint8_t)0x7F,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSEVI));
    EXPECT_EQ((uint8_t)0x70,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSN));
    EXPECT_EQ((uint8_t)0x71,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNI));
    EXPECT_EQ((uint8_t)0x78,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNN));
    EXPECT_EQ((uint8_t)0x79,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNNI));
    EXPECT_EQ((uint8_t)0x7C,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNP));
    EXPECT_EQ((uint8_t)0x7D,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNPI));
    EXPECT_EQ((uint8_t)0x7A,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNZ));
    EXPECT_EQ((uint8_t)0x7B,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSNZI));
    EXPECT_EQ((uint8_t)0x76,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSOD));
    EXPECT_EQ((uint8_t)0x77,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSODI));
    EXPECT_EQ((uint8_t)0x74,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSP));
    EXPECT_EQ((uint8_t)0x75,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSPI));
    EXPECT_EQ((uint8_t)0x72,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSZ));
    EXPECT_EQ((uint8_t)0x73,
        static_cast<std::underlying_type<Opcode>::type>(Opcode::OP_ZSZI));
}
