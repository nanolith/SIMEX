SIMEX Virtual Machine
=====================

The SIMEX virtual machine is a simplified virtual machine inspired by
[MMIX][mmix-link].  While this virtual machine is very similar to MMIX, sharing
nearly all of the opcodes and much of the semantics, it is distinct and
incompatible.  MMIX is meant for teaching Computer Science.  It is excellent in
that regard, and working with MMIX has been a pure joy.  However, my
requirements are divergent from the spirit of [MMIXware][mmixware] and I don't
want to clutter their focus by the sort of changes I wanted to make.  This
project is not meant to compete with MMIX.  It's just a personal remix of MMIX
that I intend to use to build personal software.  I have open sourced it in the
hopes that others might find it useful or inspiring.

[mmix-link]: http://www-cs-faculty.stanford.edu/~uno/mmix.html
[mmixware]: http://mmix.cs.hm.edu

Technical Description of SIMEX
==============================

The SIMEX processor is an MMIX/MIPS inspired general purpose computer.  However,
unlike a real computer, its specification is simplified to make user mode
programming easy, and to provide a simplified interface with the host operating
system through the `SYSCALL` instruction.  Just like a real computer, it has
operating memory, registers, and instructions.

Local data in a function is often managed through a combination of stack,
registers, or single-static-assignment.  The form is largely dependent upon the
virtual or physical machine.  SIMEX borrows heavily from MMIX by implementing an
unlimited number of registers, 256 of which are visible to user code at any
given time.  This greatly simplifies one of the more interesting challenges of
dealing with a physical machine model, which is [the optimal use of limited
registers][register-allocation].  This approach also blends
[stack][stack-machine] and [SSA][ssa] semantics used in many virtual machine
implementations without precluding a simple machine translation to the preferred
form for the purposes of [Just-In-Time compilation][jit] to a host platform.
The goal of SIMEX is to make writing code easier, even at the cost of some
additional compile-time or run-time analysis.

For more details, please see the [SIMEX
Specification](docs/SIMEX_Specification.md).

[register-allocation]: https://en.wikipedia.org/wiki/Register_allocation 
[stack-machine]: https://en.wikipedia.org/wiki/Stack_machine
[ssa]: https://en.wikipedia.org/wiki/Static_single_assignment_form 
[jit]: https://en.wikipedia.org/wiki/Just-in-time_compilation

Register-Stack
--------------

The register-stack consists of a physical and local view.  The physical view is
the physical spill location in RAM at which registers are written / read.  The
local view orients the current application's stack offset such that it sees the
registers starting at local `$0`, `$1`, `$2`, etc.  Additionally, there is a set
of global registers which start at `$255`, `$254`, `$253`, etc.  Two special
registers, `rL` and `rG`, control the thresholds at which the register space is
mapped from the local or the global space.  The special register, `rS` points to
the beginning of the physical register-stack, and the special register, `rO`,
points to the current local offset into that physical register-stack.  Function
calls cause `rO` to increase in size, and function returns cause `rO` to
decrease in size.  As part of the function return, a given number of registers
are copied back to the offset location, and the remaining local registers up to
`rL` are cleared.  This process works similarly to how the register-stack is
maintained by MMIX, but it is somewhat simplified.

Multi-threading
---------------

SIMEX supports two forms of threads: system threads and user threads.  The main
difference between the two is that a system thread appears as if it is running
on a separate processor, and in simulation, it may.  A user thread, in contrast,
is more like a [fiber][fiber-wiki] or [(deprecated) POSIX context][ibm-context].
The main difference between these two mechanisms is that a system thread is
scheduled by the underlying operating system, while a user thread is scheduled
by user code.  User threads are useful for abstracting I/O or for managing
coroutines.  User threads tend to be highly interactive with each other.  An
operating system thread is useful for running computations on separate
processors and cores, and incurs significant overhead for intercommunication,
often requiring a context switch to the OS and back.

[fiber-wiki]: https://en.wikipedia.org/wiki/Fiber_(computer_science)
[ibm-context]: http://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/rmctxt.htm

In both cases, threads receive their own register-stack copies.  SIMEX provides compare-and-exchange instructions that can be used to implement synchronization primitives or to create lockless data structures.

For more information, please see the [SIMEX Multithreading
Guide](docs/SIMEX_Multithreading_Guide.md).

System Calls
------------

One significant departure from MMIX in SIMEX is the `SYSCALL` facility, which
replaces the `TRAP` instruction.  The details of how the `TRAP` instruction
operates with regard to the operating system to provide system level calls is
vague in the MMIX specification, and the mechanism provided by MMIXware is
somewhat less flexible than desired.  Specifically, file I/O traps have a
hard-coded file handle encoded into the instruction, which makes managing
arbitrary file handles difficult.  Instead, `SYSCALL` works exactly like
`PUSHJ`, except that the `YZ` argument represents the 16-bit index of the system
call table, and arguments are copied from the local stack into a system stack,
and returned values are copied from the system stack back onto the local stack.
This mechanism is very similar to how system calls work on modern processors
that support similar opcodes, but designed to be compatible with how SIMEX
function calls already work.

The SIMEX specification reserves the first 16384 system calls, indices `0x0000`
through `0x3FFF`.  It also provides a list of required and optional system calls
within this range and a mechanism that user code can use to determine whether a
given optional system call is implemented or not.  The remaining 49152 system
calls, indices `0x2000` through `0xFFFF` are implementation-defined.  This
provides users with the ability of writing plugins to interface with
operating-system-specific features or other functionality not provided by the
specification.  Other mechanisms are also provided for a foreign-function
interface, but the `SYSCALL` mechanism has a particular interface that users may
find useful for their purposes.

For more information, please see the [SIMEX System Call
Guide](docs/SIMEX_System_Call_Guide.md).

Foreign-Function Interface
--------------------------

A [foreign-function interface (FFI)][foreign-function] is provided by the
`GET_FFI_METHOD` system call.  This system call accepts a 128-bit UUID
representing the registered method.  User code can store methods in a string
table that resolves the correct UUID, or can use the label mechanism in
assembler to resolve the method name to the specific UUID for that method.  The
reason why a UUID is used instead of the raw string of the function name is to
avoid namespace clashes and to provide a uniform interface for resolving
registered methods that avoids demangling or other tricks used by various
compilers and languages.  The return value from this system call is a 64-bit
physical address that can be called using `PUSHGO`.  When this address is
called, the foreign function will be called.

The FFI interface provides a C wrapper which allows the foreign function to read
and write the current local registers as if they were SIMEX functions.  The same
register-stack semantics are in place, which ensures that FFI calls work just
like they were written as SIMEX code.

For more information, please see the [SIMEX Foreign Function
Interface](docs/SIMEX_Foreign_Function_Interface.md).

[foreign-function]: https://en.wikipedia.org/wiki/Foreign_function_interface

Assembler
---------

The SIMEX assembler is a simple table assembler with a macro interface.  This
assembler produces [ELF][elf-file-format] object files which can then be linked
using the Linker Utility.

The description of SIMEX Assembly Language can be found in the [SIMEX Assembly
Language Guide](docs/SIMEX_Assembly_Language_Guide.md).  The description of the
assembler utility can be found in the [SASM Guide](docs/SASM_GUIDE.md).

Linker
------

The SIMEX Linker is used to create executables by linking together one or more
object files or library files.  The SIMEX Linker can also be used to generate
dynamic libraries which can be loaded at runtime by the SIMEX simulator.

More information about the SIMEX Linker can be found in the [SIMEX Linker
Guide](docs/SIMEX_Linker_Guide.md).

Simulator
---------

The SIMEX Simulator is a simple interpreter which executes SIMEX instructions
and simulates the virtual machine.  The current implementation of the simulator
is an interpreter, but there are plans to implement a JIT system that can
execute SIMEX executables at near-native speed.

More information about the SIMEX Simulator can be found in the [SIMEX Simulator
Guide](docs/SIMEX_Simulator_Guide.md).
