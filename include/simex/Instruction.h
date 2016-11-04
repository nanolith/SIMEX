/**
 * \file Instruction.h
 *
 * Instruction class for decoding, evaluating, and emitting instructions.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_INSTRUCTION_HEADER_GUARD
# define SIMEX_INSTRUCTION_HEADER_GUARD

#include <cstdint>
#include <iosfwd>
#include <memory>

#include <simex/Opcode.h>

//this header is C++ specific
#ifdef __cplusplus

namespace simex {

/**
 * Forward declaration for MachineState.
 */
class MachineState;

/**
 * An instruction is a four byte sequence containing a opcode, an X value, a Y
 * value, and a Z value.  Instructions can be decoded, evaluated, and emitted.
 */
class Instruction 
{
public:

    /**
     * Virtual destructor.
     */
    virtual ~Instruction();

    /**
     * By default, the evaluate method places the machine into an invalid opcode
     * exception state.  This behavior can be overridden by derived types.
     *
     * \param state     The current machine state.
     *
     * \sideeffect The machine state is updated based upon the evaluation of
     * this instruction.
     */
    virtual void evaluate(MachineState* state) const;

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
    static std::shared_ptr<Instruction>
    decode(Opcode op, std::uint8_t x, std::uint8_t y, std::uint8_t z);

    /**
     * The emit method writes an Instruction to the given binary ostream.
     *
     * \param out       The ostream to which this instruction is emitted.
     */
    void emit(std::ostream& out) const;

    /**
     * Get the opcode for this instruction.
     *
     * \returns this instruction's opcode.
     */
    inline Opcode opcode() const { return op_; }

    /**
     * Get the X value for this instruction.
     *
     * \returns this instruction's X value.
     */
    inline std::uint8_t x() const { return x_; }

    /**
     * Get the Y value for this instruction.
     *
     * \returns this instruction's Y value.
     */
    inline std::uint8_t y() const { return y_; }

    /**
     * Get the Z value for this instruction.
     *
     * \returns this instruction's Z value.
     */
    inline std::uint8_t z() const { return z_; }

protected:
    /**
     * The constructor takes an opcode, and an XYZ value.
     */
    Instruction(Opcode op, std::uint8_t x, std::uint8_t y, std::uint8_t z)
        : op_{op}, x_{x}, y_{y}, z_{z}
    {
    }

private:
    Opcode op_;
    std::uint8_t x_;
    std::uint8_t y_;
    std::uint8_t z_;
};

/* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_INSTRUCTION_HEADER_GUARD
