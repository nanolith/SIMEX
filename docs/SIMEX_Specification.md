SIMEX Specification
===================

This document outlines the SIMEX virtual machine.  This specification will be
subject to significant change prior to the 1.0 release.  After 1.0, only bug
fixes and extensions that do not break backward compatibility will be accepted.

The SIMEX virtual machine was heavily influenced by MMIX, which was designed by
Donald Knuth with contributes from John Hennessy and Dick Sites.  While MMIX
focuses on teaching Computer Science and providing a stable platform for the
analysis of data structures and algorithms, SIMEX focuses on software running in
userland.  SIMEX is very similar to MMIX and borrows heavily from its
specification, but makes several breaking changes for both simplification and
ease of implementation and integration.

Scope
-----

The scope of this specification is limited to the mechanical description needed
to describe the SIMEX virtual machine and its instructions.  Descriptions of how
a native system can simulate this machine is limited to concepts which are
necessary to understand in order to build software to this specification.
Otherwise, as long as fidelity to this specification is met, much of the
underlying mechanism of simulation is implementation-defined.  This ensures that
implementations can consider aggressive optimizations that may change the
instruction-by-instruction evaluation of SIMEX, but not the overall effect of
the evaluation of software written to this specification.

Words, Memory, and Endianness
-----------------------------

SIMEX is a 64-bit virtual machine.  Data is stored in Big-Endian format, meaning
that the most significant bytes are stored before bytes of lesser significance.

The nomenclature for word sizes in SIMEX is borrowed directly from MMIX.  The
native 64-bit word is called an Octa.  This can be subdivided into two 32-bit
words, each of which is called a Tetra.  A Tetra can be subdivided into two
16-bit words, each of which is called a Wyde.  A Wyde can be subdivided into two
8-bit words, each of which is called a Byte.  A Byte can be subdivided into two
4-bit words, each of which is called a Nybble.

Each instruction in memory is stored as a Tetra and is aligned on a Tetra
boundary.  Floating point numbers are stored as Octa values.  Multibyte
character streams (i.e. UTF-8) are stored as Bytes.  Wide-character 16-bit
character streams are stored as Wydes.  Wydes are preferred for string
formatting in code as these allow for 65536 potential values, which makes some
Unicode operations easier.

Memory references are resolved by a 64-bit address.  Reads and writes of values
should be aligned by their size.  For instance, Octa values should be aligned on
an Octa boundary, Tetra values should be aligned on a Tetra boundary, etc.
Unaligned memory access will result in undefined behavior.  Implementations may
throw a memory alignment exception or may ignore the least significant bits of
the memory address.  The latter is compatible with MMIX, but the former is the
preferred behavior and must be followed for theorem proving against SIMEX code
to ensure correctness of behavior.

Word values can be signed or unsigned.  How these values are treated is
dependent upon the instruction used when evaluating these word values.  The most
significant bit of the word value is used to determine the sign of the word in
signed instructions.  For unsigned instructions, this bit is the most
significant binary value of the word.  When words smaller than an Octa value are
loaded into a register using a signed instruction, the sign of this word is
extended into the Octa value to preserve the value.  For instance, the value -1
is stored as a Byte using the hexadecimal value `0xFF`.  When the `LDB`
instruction loads this value into a register, it is sign-extended to the
hexadecimal value `0xFFFFFFFFFFFFFFFF`, which corresponds to the Octa
representation of the value -1.  On the other hand, the value 7 is stored as a
Byte using the hexadecimal value `0x07`.  When this is loaded using the `LDB`
instruction, the sign bit is extended again, resulting in the Octa
representation of 7, which is `0x0000000000000007`.

Register-Stack
--------------

The register-stack is a hybrid register and stack structure that represents the
working memory of the SIMEX virtual machine.  At any given time, there are 256
registers visible to a SIMEX program.  These are split between local registers
and global registers.  Local registers are part of the register-stack, and
global registers are part of a memory segment that remains fixed in place in the
register-stack.  Two special registers, `rL` and `rG`, control the visibility of
registers for the program.  The `rL` register provides the number of local
registers available, starting from 0 and moving upward, and `rG` provides the
number of global registers available, starting from 255 and moving downward.
Between these two ranges is a "hole" which is plugged by increasing the `rL`
value as registers beyond this range are accessed.  Register values in the
"hole" are always 0, and once they are accessed for reading or writing, the `rL`
register is automatically updated to encompass this extended range.  During a
function call, `rL` is deducted by the same number as `rO`, thus shrinking the
number of local registers available.  When a function returns, `rO` is restored
and `rL` is adjusted based upon the number of values returned by the function.
This effectively erases any register values not returned by the function as
these would now exist in the "hole" and would be reset to 0 upon subsequent
access.

Under the covers, the register-stack actually references a memory region that is
pointed to by the `rS` register.  The `rO` register is an offset into the memory
region pointed to by the `rS` register which represents the current offset for
register `$0`.  During function calls, the `rO` offset is adjusted upward to
"hide" local registers for the caller, and adjusted downward to "restore" local
registers after a function return.  In this way, local working memory is
preserved between function calls.  When the `rS` memory region is first
initialized, `rS` is incremented by `rG` Octa values.  Global memory starts at
the Octa index `rS-1` (255) and grows downward to `rS-rG`.  In this way, global
values can always be referenced even as `rO` changes.

Segments
--------

Memory regions are arranged into arbitrarily-sized segments.  These segments are
implementation-defined.  However, some characteristics of these segments are
part of the specification.  For instance, segments have policies which pertain
to their usage.  At a minimum, segments have a read bit, a write bit, and an
execute bit.  When the read bit is set, segments can be read.  When the read bit
is unset, segments cannot be read.  When the write bit is set, segments can be
written to.  When the write bit is unset, segments cannot be written to.  When
the execute bit is set, segments contain executable code and can be called or
jumped into.  When the execute bit is unset, segments cannot be executed.

Segments also have a minimum size.  When a segment is allocated through a system
call, it is guaranteed to be at LEAST the minimum size if the allocation is
successful.  However, only the size which was requested should be accessed.
There is no guarantee that the segment will be larger.

The policy bits associated with a segment can be modified on platforms that
support it.  For instance, it may be possible to generate new code on the fly.
The mechanism is to create a write-only segment, write SIMEX instructions to
this segment, then use the "Change Segment Policy" system call to set this
segment as execute-only.  The implementation may use this process to schedule a
segment for JIT compilation, after which point, calling into this segment will
result in a call to the given generated function's entry point.  Setting a
segment as execute-only MAY require a call to set all of the entry points to
this segment ahead of time, and an implementation MUST provide a failure code
indicating that entry points are required IF the implementation will JIT this
code.  This ensures that the appropriate address translation between the
interpreted SIMEX code and the compiled native code can be performed when
calling into this segment.

Performing an action which is not permitted by the segment's policy causes a
memory protection exception.

The segment associated with the register-stack MUST have the execute bit
cleared and the read/write bits set.

Special Registers
-----------------

SIMEX supports up to 256 special registers, but only the first 32 are defined as
below.  This list is incomplete.  Many of the registers are the same as is found
in MMIX, but most of the MMIX special registers are not used in SIMEX.

* 0x00 - RESERVED
* 0x01 - `rD`, dividend register
* 0x02 - `rE`, epsilon register
* 0x03 - `rH`, himult register
* 0x04 - `rJ`, return-jump register
* 0x05 - `rJJ`, SYSCALL return-jump register
* 0x06 - `rM`, multiplex mask register
* 0x07 - `rR`, remainder register
* 0x08 - `rP`, prediction register
* 0x09 - RESERVED
* 0x0A - RESERVED
* 0x0B - `rO`, register stack offset
* 0x0C - `rS`, register stack pointer
* 0x0D - RESERVED
* 0x0E - RESERVED
* 0x0F - RESERVED
* 0x10 - `rG`, global threshold register
* 0x11 - `rL`, local threshold register
* 0x12 - RESERVED
* 0x13 - RESERVED
* 0x14 - RESERVED
* 0x15 - RESERVED
* 0x16 - RESERVED
* 0x17 - RESERVED
* 0x18 - RESERVED
* 0x19 - `rFF`, fault handler location
* 0x1A - `rF`, fault location register
* 0x1B - `rOP`, fault Z register value
* 0x1C - `rXX`, fault X register value
* 0x1D - `rYY`, fault Y register value
* 0x1E - `rZZ`, fault Z register value
* 0x1F - `rCC`, fault code

The dividend register, `rD`, is used with the unsigned division instruction
`DIVU`.  More details are provided with the description of that instruction.

The epsilon register, `rE`, is used in conjunction with the floating point
comparison instructions.  More details are provided with the description of
those instructions.

The himult register, `rH`, is used to store the high half of the 128-bit
product obtained via the `MULU` instruction.  More details are provided with the
description of that instruction.

The return-jump register, `rJ`, is used for function linkage.  More details are
provided in the section on functions.

The `SYSCALL` return-jump register, `rJJ`, is used for syscall linkage.  More
details are provided in the section on system calls.

The multiplex-mask register, `rM`, is used in conjunction with the `MUX`
instruction.  More details are provided with the description of that
instruction.

The remainder register, `rR`, is used in conjunction with the `DIV` and `DIVU`
instructions.  More details are provided with the description of those
instructions.

The prediction register, `rP`, is used in conjunction with the `CSWAP`
instruction.  More details are provided with the description of that
instruction.

The register stack offset, register stack pointer, global threshold, and local
threshold registers -- `rO`, `rS`, `rG`, and `rL` respectively -- are used in
conjunction with the register-stack.  More information can be found in the
register-stack section.

When an exception is detected, the fault handler at address `rFF` is called.
During an exception, the `rF` register contains the location of the instruction
that caused the fault.  The `rOP`, `rXX`, `rYY`, and `rZZ` registers contain the
instruction, the value of the `X` register, the value of the `Y` register,
and the value of the `Z` register, respectively.  The `rOP` register's high
Tetra is reserved, and the low Tetra contains the instruction that caused
the fault.  This information can be used to recover from the exception and
resume execution.  More information about exception handling can be found in the
section on exceptions.

Functions
---------

Function call semantics in SIMEX are very similar to how functions are managed
in MMIX.  Two instructions, `PUSHJ` and `PUSHGO`, provide the ability to call
functions using relative addressing and absolute addressing, respectively.  The
`X` position of the instruction holds the register offset of the first return
value when the call completes.  The subsequent register offsets become registers
`$0` through `$N` of the resulting function.  The `X` register is modified to
hold the number of register values that are "pushed" under the stack by the
function call.  These registers are invisible to the called function and
preserve the state of the caller.  The `rJ` special register is updated to hold
the return address (current address + 4).  The `rO` special register is
incremented such that the first register after the `X` register becomes register
`$0`. The `rL` register is decremented so that the scope of local registers is
preserved with this new offset. Control is transferred to the function.

In many ABIs, functions have a prologue and an epilogue.  Because of the
semantics of function calls in SIMEX, most of the heavy lifting is handled by
the simulator.  A function's prologue should preserve `rJ` if it will be calling
functions, but this preservation can be lowered to the call site.  A function's
epilogue generally consists of just the `POP` instruction.

The `POP` instruction rewinds the function call.  The `X` value of this
instruction determines how many of the local registers, starting at `$0`, will
be copied back to the caller.  The register value at `rO - 1` Octa values holds
the number of registers preserved by the `PUSHJ` or `PUSHGO` call.  The `rO`
register is reduced by this number.  This register location will also hold the
first of the returned register values.  The specified number of local registers
are copied starting at this register.  The `rL` register is increased so that
the originally preserved registers and the copied registers are all visible in
the local scope.  Any registers not copied will exist in the "hole" after rL and
will automatically be initialized to 0 when accessed.

For instance, if function A calls function B using `PUSHJ $7, <reladdr>`, then
the following occurs during the call.  The return address is placed into `rJ`.
The number 7 is placed into the `$7` register, and `rO` is incremented by `7+1`,
such that register `$8` becomes register `$0` for the caller.  Finally, control
is transferred to the relative address of function B.  If function B returns
control to function A using `POP 1,0`, then the following occurs during the
return.  The number at location `rO-1` is read, which is 7.  The `rO` register
is decremented by `7+1`, restoring the original local register offsets.
Then, 1 register value is copied from `rO+7+1` to `rO+7` to satisfy the `POP`
count.  The `rL` register is modified so that only `rO+7+1` local registers are
visible.  Finally, control is returned to the location specified by `rJ`.

Note that the `POP` instruction differs from the `POP` instruction in MMIX.  The
`YZ` value is reserved and must remain 0.  In MMIX, this value is used to
compute an offset relative to `rJ` at which control is returned.  SIMEX may
support this functionality or similar functionality in the future, but for now,
a non-zero `YZ` value will result in an "Unsupported Instruction" exception.
Users who wish to simulate the MMIX semantics can hook the fault handler and
manually simulate the MMIX flavor of this instruction.

System Calls
------------

System calls work in a similar way to function calls.  The difference is that
the `SYSCALL` instruction invokes a system call at index `YZ`, which transfers
control to the underlying system.  This facility is used to support the
simulation of operating system level calls.  System calls work just like
function calls in that arguments are passed in the registers, and the `X` value
of the `SYSCALL` instruction is used like the `X` value in the `PUSHJ`
instruction.  Likewise, when the `SYSCALL` returns, the `X` value's register
will contain the first returned value from the system call, and `rL` will be
adjusted so that all returned values are visible.

Since this specification is specific to SIMEX itself, this section does not
cover the list of supported system calls.  Please see the [SIMEX System Call
Guide](SIMEX_System_Call_Guide.md) for more information.
