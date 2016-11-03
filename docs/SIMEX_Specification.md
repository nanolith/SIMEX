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
in MMIX, but most of the MMIX special registers are not used in SIMEX.  Reserved
registers will return 0 when read and will ignore writes.  Future features may
be tested by reading from and writing to these registers as per instructions in
future revisions of this specification.  However, current code should ignore
these registers to remain forward compatible with future versions of this
specification, as writes to reserved registers may result in exceptions in
future versions.

| Register | Shorthand | Description                                   | Note |
|---------:|:----------|:----------------------------------------------|-----:|
|     0x00 |           | RESERVED                                      |      |
|     0x01 | `rD`      | dividend register                             |  1   |
|     0x02 | `rE`      | epsilon register                              |  2   |
|     0x03 | `rH`      | himult register                               |  3   |
|     0x04 | `rJ`      | return-jump register                          |  4   |
|     0x05 | `rJJ`     | SYSCALL return-jump register                  |  5   |
|     0x06 | `rM`      | multiplex mask register                       |  6   |
|     0x07 | `rR`      | remainder register                            |  7   |
|     0x08 | `rP`      | prediction register                           |  8   |
|     0x09 |           | RESERVED                                      |      |
|     0x0A |           | RESERVED                                      |      |
|     0x0B | `rO`      | register stack offset                         |  9   |
|     0x0C | `rS`      | register stack pointer                        |  9   |
|     0x0D |           | RESERVED                                      |      |
|     0x0E |           | RESERVED                                      |      |
|     0x0F |           | RESERVED                                      |      |
|     0x10 | `rG`      | global threshold register                     |  9   |
|     0x11 | `rL`      | local threshold register                      |  9   |
|     0x12 |           | RESERVED                                      |      |
|     0x13 |           | RESERVED                                      |      |
|     0x14 |           | RESERVED                                      |      |
|     0x15 |           | RESERVED                                      |      |
|     0x16 |           | RESERVED                                      |      |
|     0x17 |           | RESERVED                                      |      |
|     0x18 |           | RESERVED                                      |      |
|     0x19 | `rFF`     | fault handler location                        | 10   |
|     0x1A | `rF`      | fault location register                       | 10   |
|     0x1B | `rOP`     | fault Z register value                        | 10   |
|     0x1C | `rXX`     | fault X register value                        | 10   |
|     0x1D | `rYY`     | fault Y register value                        | 10   |
|     0x1E | `rZZ`     | fault Z register value                        | 10   |
|     0x1F | `rCC`     | fault code                                    | 10   |

1. The dividend register, `rD`, is used with the unsigned division instruction
   `DIVU`.  More details are provided with the description of that instruction.

2. The epsilon register, `rE`, is used in conjunction with the floating point
   comparison instructions.  More details are provided with the description of
   those instructions.

3. The himult register, `rH`, is used to store the high half of the 128-bit
   product obtained via the `MULU` instruction.  More details are provided with
   the description of that instruction.

4. The return-jump register, `rJ`, is used for function linkage.  More details
   are provided in the section on functions.

5. The `SYSCALL` return-jump register, `rJJ`, is used for syscall linkage.  More
   details are provided in the section on system calls.

6. The multiplex-mask register, `rM`, is used in conjunction with the `MUX`
   instruction.  More details are provided with the description of that
   instruction.

7. The remainder register, `rR`, is used in conjunction with the `DIV` and
   `DIVU` instructions.  More details are provided with the description of those
   instructions.

8. The prediction register, `rP`, is used in conjunction with the `CSWAP`
   instruction.  More details are provided with the description of that
   instruction.

9. The register stack offset, register stack pointer, global threshold, and
   local threshold registers -- `rO`, `rS`, `rG`, and `rL` respectively -- are
   used in conjunction with the register-stack.  More information can be found
   in the register-stack section.

10. When an exception is detected, the fault handler at address `rFF` is called.
   During an exception, the `rF` register contains the location of the
   instruction that caused the fault.  The `rOP`, `rXX`, `rYY`, and `rZZ`
   registers contain the instruction, the value of the `X` register, the value
   of the `Y` register, and the value of the `Z` register, respectively.  The
   `rOP` register's high Tetra is reserved, and the low Tetra contains the
   instruction that caused the fault.  This information can be used to recover
   from the exception and resume execution.  More information about exception
   handling can be found in the section on exceptions.

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

Instruction Basics
------------------

A SIMEX instruction consists of a Tetra with an opcode, an X value, a Y value,
and a Z value.  The high byte is the opcode.  Stored in Big-Endian format, the
resulting instruction is laid out as in the table below.

| B3     | B2  | B1  | B0  |
|:------:|:---:|:---:|:---:|
| opcode |  X  |  Y  |  Z  |

Since the opcode is a byte, there is a maximum of 256 supported instructions.
Most instructions have two flavors.  One flavor treats the Z value as a
register, and a second immediate flavor uses the Z value directly as a constant.
While the opcodes for these flavors are distinct, the mnemonic used by the
assembler is the same.  The assembler is smart enough to know if the immediate
form is used based upon context.

Most instructions treat the X, Y, and Z values independently.  However, some
instructions combine the YZ values into a 16-bit number, and some, such as the
`JMP` instructions, combine the XYZ values into a 32-bit number.

Instruction Table
-----------------

The following table describes all instructions supported by SIMEX.  Most of
these instructions are compatible with MMIX.  Most of the opcodes are the exact
same opcodes as found in MMIX.

|      | `0x00` | `0x01` | `0x02` | `0x03` | `0x04` | `0x05` | `0x06` | `0x07` |
|-----:|:------:|:------:|:------:|:------:|:------:|:------:|:------:|:------:|
|`0x00`| SYSCALL|  FCMP  |  FUN   |  FEQL  |  FADD  |  FIX   |  FSUB  |  FIXU  |
|`0x08`|  FLOT  |  FLOTI | FLOTU  | FLOTUI |  SFLOT | SFLOTI | SFLOTU | SFLOTUI|
|`0x10`|  FMUL  |  FCMPE |  FUNE  |  FEQLE |  FDIV  |  FSQRT |  FREM  |  FINT  |
|`0x18`|  MUL   |  MULI  |  MULU  |  MULUI |  DIV   |  DIVI  |  DIVU  |  DIVUI |
|`0x20`|  ADD   |  ADDI  |  ADDU  |  ADDUI |  SUB   |  SUBI  |  SUBU  |  SUBUI |
|`0x28`|  2ADDU | 2ADDUI |  4ADDU | 4ADDUI |  8ADDU | 8ADDUI | 16ADDU | 16ADDUI|
|`0x30`|  CMP   |  CMPI  |  CMPU  |  CMPUI |  NEG   |  NEGI  |  NEGU  |  NEGUI |
|`0x38`|   SL   |  SLI   |  SLU   |  SLUI  |   SR   |  SRI   |  SRU   |  SRUI  |
|`0x40`|   BN   |  BNB   |   BZ   |  BZB   |   BP   |  BPB   |  BOD   |  BODB  |
|`0x48`|  BNN   |  BNNB  |  BNZ   |  BNZB  |  BNP   |  BNPB  |  BEV   |  BEVB  |
|`0x50`|  PBN   |  PBNB  |  PBZ   |  PBZB  |  PBP   |  PBPB  |  PBOD  |  PBODB |
|`0x58`|  PBNN  |  PBNNB |  PBNZ  |  PBNZB |  PBNP  |  PBNPB |  PBEV  |  PBEVB |
|`0x60`|  CSN   |  CSNI  |  CSZ   |  CSZI  |  CSP   |  CSPI  |  CSOD  |  CSODI |
|`0x68`|  CSNN  |  CSNNI |  CSNZ  |  CSNZI |  CSNP  |  CSNPI |  CSEV  |  CSEVI |
|`0x70`|  ZSN   |  ZSNI  |  ZSZ   |  ZSZI  |  ZSP   |  ZSPI  |  ZSOD  |  ZSODI |
|`0x78`|  ZSNN  |  ZSNNI |  ZSNZ  |  ZSNZI |  ZSNP  |  ZSNPI |  ZSEV  |  ZSEVI |
|`0x80`|  LDB   |  LDBI  |  LDBU  |  LDBUI |  LDW   |  LDWI  |  LDWU  |  LDWUI |
|`0x88`|  LDT   |  LDTI  |  LDTU  |  LDTUI |  LDO   |  LDOI  |  LDOU  |  LDOUI |
|`0x90`|  LDSF  |  LDSFI |  LDHT  |  LDHTI |  CSWAP | CSWAPI |  LDUNC | LDUNCI |
|`0x98`|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|   GO   |  GOI   |
|`0xA0`|  STB   |  STBI  |  STBU  |  STBUI |  STW   |  STWI  |  STWU  |  STWUI |
|`0xA8`|  STT   |  STTI  |  STTU  |  STTUI |  STO   |  STOI  |  STOU  |  STOUI |
|`0xB0`|  STSF  |  STSFI |  STHT  |  STHTI |  STCO  |  STCOI |  STUNC | STUNCI |
|`0xB8`|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED| PUSHGO | PUSHGOI|
|`0xC0`|   OR   |  ORI   |  ORN   |  ORNI  |  NOR   |  NORI  |  XOR   |  XORI  |
|`0xC8`|  AND   |  ANDI  |  ANDN  |  ANDNI |  NAND  |  NANDI |  NXOR  |  NXORI |
|`0xD0`|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|
|`0xD8`|  MUX   |  MUXI  |RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|RESERVED|
|`0xE0`|  SETH  |  SETMH |  SETML |  SETL  |  INCH  |  INCMH |  INCML |  INCL  |
|`0xE8`|  ORH   |  ORMH  |  ORML  |  ORL   |  ANDNH | ANDNMH | ANDNML |  ANDNL |
|`0xF0`|  JMP   |  JMPB  |  PUSHJ | PUSHJB |  GETA  |  GETAB |  PUT   |  PUTI  |
|`0xF8`|  POP   | RESUME |  SAVE  | UNSAVE |  SYNC  |  SWYM  |  GET   |RESERVED|

Note that a few instructions found in MMIX have been removed from SIMEX.  These
have been reserved.  Some of these have been removed to simplify the
specification.  Some have been removed due to architectural differences between
the SIMEX machine and the MMIX machine.  The `TRAP` instruction has been
replaced by the `SYSCALL` instruction, and the `TRIP` instruction has been
removed.  The functionality provided by `TRAP` and `TRIP` have been replaced by
system calls, the exception mechanism, and system calls associated with signals.

Some of the more exotic bit fiddling operations have been removed to simplify
JIT implementations.  Many of the virtual memory, synchronization, and cache
manipulation instructions have been removed for portability reasons.  There are
still uncached write instructions, the `CSWAP/I` instructions, and the `SYNC`
instruction.  Cache invalidation instructions may be added in the future if JIT
implementations require these for performance purposes.

The following sections describe these instructions.

Integer Math Instructions
-------------------------

There are 22 integer math instructions in SIMEX.

* `ADD $X, $Y, $Z` / `ADD $X, $Y, Z` "add"

  $X = $Y + $Z or $X = $Y + Z.

  This operation is performed using signed, two's complement arithmetic.  If the
  resulting sum is greater than or equal to 2<sup>63</sup> or less than
  -2<sup>63</sup>, an integer overflow exception results.

* `ADDU $X, $Y, $Z` / `ADDU $X, $Y, Z` "add unsigned"

  $X = ($Y + $Z) mod 2<sup>64</sup> or $X = ($Y + Z) mod 2<sup>64</sup>.

  This operation is similar to `ADD`, except that no integer overflow exception
  will result if the result would exceed the bounds of a 64-bit signed integer.
  Instead, the result, mod 2<sup>64</sup> is stored in $X.

* `2ADDU $X, $Y, $Z` / `2ADDUI $X, $Y, $Z` "times 2 and add unsigned"

  $X = (2$Y + $Z) mod 2<sup>64</sup> or $X = (2$Y + Z) mod 2<sup>64</sup>.

* `4ADDU $X, $Y, $Z` / `4ADDUI $X, $Y, $Z` "times 4 and add unsigned"

  $X = (4$Y + $Z) mod 2<sup>64</sup> or $X = (4$Y + Z) mod 2<sup>64</sup>.

* `8ADDU $X, $Y, $Z` / `8ADDUI $X, $Y, $Z` "times 8 and add unsigned"

  $X = (8$Y + $Z) mod 2<sup>64</sup> or $X = (8$Y + Z) mod 2<sup>64</sup>.

* `16ADDU $X, $Y, $Z` / `16ADDUI $X, $Y, $Z` "times 16 and add unsigned"

  $X = (16$Y + $Z) mod 2<sup>64</sup> or $X = (16$Y + Z) mod 2<sup>64</sup>.

* `SUB $X, $Y, $Z` / `SUB $X, $Y, Z` "subtract"

  $X = $Y - $Z or $X = $Y - Z.

  This operation is performed using signed, two's complement arithmetic.  If the
  resulting difference is greater than or equal to 2<sup>63</sup> or less than
  -2<sup>63</sup>, an integer overflow exception results.

* `SUBU $X, $Y, $Z` / `SUBU $X, $Y, Z` "subtract unsigned"

  $X = ($Y - $Z) mod 2<sup>64</sup> or $X = ($Y - Z) mod 2<sup>64</sup>.

  This operation is similar to `SUB`, except that no integer overflow exception
  will result if the result would exceed the bounds of a 64-bit signed integer.
  Instead, the result, mod 2<sup>64</sup> is stored in $X.

* `NEG $X, $Y, $Z` / `NEG $X, $Y, Z` "negate"

  $X = Y - $Z or $X = Y - Z.

  This operation is performed using signed, two's complement arithmetic.  If the
  resulting difference is greater than or equal to 2<sup>63</sup> or less than
  -2<sup>63</sup>, an integer overflow exception results.  Unlike `SUB`, this
  operation treats the value Y as a constant.  This can be used to negate the Z
  value or to create negative constant values.  When Y is 0, overflow occurs
  only if $Z = -2<sub>63</sub>.

* `NEGU $X, $Y, $Z` / `NEGU $X, $Y, Z` "negate unsigned"

  $X = (Y - $Z) mod 2<sup>64</sup> or $X = (Y - Z) mod 2<sup>64</sup>.

  This operation is similar to `NEG`, except that no integer overflow exception
  will result if the result would exceed the bounds of a 64-bit signed integer.
  Instead, the result, mod 2<sup>64</sup> is stored in $X.
