/**
 * \file SReg.h
 *
 * Enumeration of special registers.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_SREG_HEADER_GUARD
# define SIMEX_SREG_HEADER_GUARD

#include <cstdint>

//this header is C++ specific
#ifdef __cplusplus

namespace simex {

/**
 * The SReg Enumeration contains all special registers supported by SIMEX.
 */
enum class SReg : std::uint8_t
{
    SR_RESERVED_x00     =   0x00,
    SR_RD               =   0x01,
    SR_RE               =   0x02,
    SR_RH               =   0x03,
    SR_RJ               =   0x04,
    SR_RJJ              =   0x05,
    SR_RM               =   0x06,
    SR_RR               =   0x07,
    SR_RP               =   0x08,
    SR_RESERVED_x09     =   0x09,
    SR_RESERVED_x0A     =   0x0A,
    SR_RO               =   0x0B,
    SR_RS               =   0x0C,
    SR_RESERVED_x0D     =   0x0D,
    SR_RESERVED_x0E     =   0x0E,
    SR_RESERVED_x0F     =   0x0F,
    SR_RG               =   0x10,
    SR_RL               =   0x11,
    SR_RESERVED_x12     =   0x12,
    SR_RESERVED_x13     =   0x13,
    SR_RESERVED_x14     =   0x14,
    SR_RESERVED_x15     =   0x15,
    SR_RESERVED_x16     =   0x16,
    SR_RESERVED_x17     =   0x17,
    SR_RESERVED_x18     =   0x18,
    SR_RFF              =   0x19,
    SR_RF               =   0x1A,
    SR_ROP              =   0x1B,
    SR_RXX              =   0x1C,
    SR_RYY              =   0x1D,
    SR_RZZ              =   0x1E,
    SR_RCC              =   0x1F,
};

/* namespace simex */}

//end of C++ specific section
#endif //__cplusplus

#endif //SIMEX_SREG_HEADER_GUARD
