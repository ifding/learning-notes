
# 1. Some basics

- [A short introduction to the CPU](#a-short-introduction-to-the-cpu)
- [Numeral systems](#numeral-systems)
- [Empty function](#empty-function)
- [Returning value](#returning-value)
- [Hello World](#hello-world)
  - [x86](#x86)
  - [x86-64](#x86-64)
  - [ARM](#arm)
  - [MIPS](#mips)
  - [Conclusion](#conclusion)

## A short introduction to the CPU

**A short glossary**:

**Instruction** : A primitive CPU command. The simplest examples include: moving data between registers,working with memory, primitive arithmetic operations. As a rule, each CPU has its own instruction set architecture (ISA).

**Machine code** : Code that the CPU directly processes. Each instruction is usually encoded by several bytes.

**Assembly language** : Mnemonic code and some extensions like macros that are intended to make a programmer’s life easier. CPU register : Each CPU has a fixed set of general purpose registers (GPR2). ≈ 8 in x86, ≈ 16 in x86-64, ≈ 16 in ARM. The easiest way to understand a register is to think of it as an untyped temporary variable. Imagine if you were working with a high-level PL (Programming Language) and could only use eight 32-bit (or 64-bit) variables. Yet a lot can be done using just these!

The **x86** ISA has always been one with variable-length instructions, so when the 64-bit era came, the x64 extensions did not impact the ISA very significantly. In fact, the x86 ISA still contains a lot of instructions that first appeared in 16-bit 8086 CPU, yet are still found in the CPUs of today.

**ARM** is a RISC (Reduced instruction set computing) CPU designed with constant-length instructions in mind, which had some advantages in the past. In the very beginning, all ARM instructions were encoded in 4 bytes. This is now referred to as “ARM mode”. Then
they thought it wasn’t as frugal as they first imagined. In fact, most used CPU instructions
in real world applications can be encoded using less information. They therefore added another ISA, called **Thumb**, where each instruction was encoded in just 2 bytes. This is now referred as “Thumb mode”. 

However, not all ARM instructions can be encoded in just 2 bytes, so the Thumb instruction set is somewhat limited. It is worth noting that code compiled for ARM mode and Thumb mode may of course coexist within one single program. The ARM creators thought Thumb could be extended, giving rise to **Thumb-2**, which appeared in ARMv7. Thumb-2 still uses 2-byte instructions, but has some new instructions which have the size of 4 bytes. There is a common misconception that Thumb-2 is a mix of ARM and Thumb. This is incorrect. Rather, Thumb-2 was extended to fully support all processor features so it could compete with ARM mode—a goal that was clearly achieved, as the majority of applications for iPod/iPhone/iPad are compiled for the Thumb-2 instruction set (admittedly, largely due to the fact that Xcode does this by default). 

Later the 64-bit ARM came out. This ISA has 4-byte instructions, and lacked the need of any additional Thumb mode. However, the 64-bit requirements affected the ISA, resulting in us now having three ARM instruction sets: ARM mode, Thumb mode (including Thumb-2) and ARM64. These ISAs intersect partially, but it can be said that they are different ISAs, rather than variations of the same one. 

There are, by the way, many other RISC ISAs with fixed length 32-bit instructions, such as MIPS, PowerPC and Alpha AXP.

## Numeral systems

- Decimal numbers, i.e., 1234. or with "d" suffix: 1234d
- Binary numbers, with "0b" prefix:  0b100110111, or with "b" suffix:  100110111b
- Hexadecimal numbers, with "0x" prefix:  0x1234ABCD, or with "h" suffix: 1234ABCDh
- Octal numbers, 8 digits (0..7) and each is mapped to 3 bits.

As many *NIX users know, *chmod* argument can be a number of 3 digits. The first digit is rights for owner of file, second is rights for group (to which file belongs), third is for everyone else. (owner/group/others)
```
decimal binary meaning
	7 	111 	rwx
	6 	110 	rw-
	5 	101 	r-x
	4 	100 	r--
	3 	011 	-wx
	2 	010 	-w-
	1 	001 	--x
	0 	000 	---
```

- Divisibility

Hexadecimal number 0x1230 is divisible by 0x10 (or 16), 0x123000 is divisible by 0x1000 (or
4096), etc. This property can be used often to realize quickly if a size of some block in memory is padded to some boudary.  For example, sections in PE (Portable Executable) files are almost always started at addresses ending with 3 hexadecimal zeros: 0x41000, 0x10001000, etc. The reason behind this is in the fact that almost all PE sections are padded to boundary of 0x1000 (4096) bytes.

- Multi-precision arithmetic

Multi-precision arithmetic can use huge numbers, and each one may be stored in several bytes. For example, RSA keys, both public and private, are spanning up to 4096 bits and maybe even more.

## Empty function
```c
// c/c++ code
void f()
{
	return;
}
```

- x86
There is just one instruction: *RET*, which returns execution to the *caller*.
```nasm
f:
		ret
```

- ARM
The return address is not saved on the local stack in the ARM ISA, but rather in the link register, so the `BX LR` instruction causes execution to jump to that address - effectively returning execution to the caller.
```
f 		PROC
		BX 		lr
		ENDP
```

- MIPS
There are two naming conventions used in the world of MIPS when naming registers: by number (from $0 to $31) or by pseudo name ($V0, $A0, etc.).
```
	j 		$31
	nop
```

The first instruction is the jump instruction (J or JR) which returns the execution flow to the caller, jumping to the address in the $31 (or $RA) register. This is the register analogous to LR (Link Register) in ARM. The second instruction is NOP (No Operation), which does nothing. We can ignore it so far.

Register and instruction names in the world of MIPS are traditionally written in lowercase.

Empty functions are quite frequent in low-level code. First of all, debugging functions are quite popular.
```c
void dbg_print (const char *fmt, ...)
{
#ifdef _DEBUG
	// open log file
	// write to log file
	// close log file
#endif
};

void save_file ()
{
#ifndef DEMO
	// actual saving code
#endif
};
```

In non-debugging build (e.g., "release"), `_DEBUG` is not defined, so `dbg_print()` function, despite still being called during execution, will be empty.

Another popular way of software protection is make several builds: one for legal customers, and a demo build.

## Returning value
```c
int f()
{
	return 123;
};
```

- x86
```nasm
f:
		mov 	eax, 123
		ret
```

There are just two instructions: the first places the value 123 into the `EAX` register, which is used by convention for storing the return value and the second one is `RET`, which returns execution to the caller. 

The caller will take the result from the `EAX` register.

- ARM
```
f 		PROC
		MOV 	r0,#0x7b ; 123
		BX lr
		ENDP
```

ARM uses the register `R0` for returning the results of functions, so 123 is copied into `R0`. It is worth nothing that `MOV` is a misleading name for the instruction in both x86 and ARM ISAs. The data is not in fact moved, but copied.

- MIPS
```
	j 		$31
	li 		$2,123 		# 0x7b
```

The $2 (or $V0) register is used to store the function's return value. `LI` stands for "Load Immediate" and is the MIPS equivalent to `MOV`.

The other instruction in the jump instruction (J or JR) which returns the execution flow to the caller.

Why positions of the load instruction (LI) and the jump instruction (J or JR) are
swapped. This is due to a RISC feature called “branch delay slot”. In MIPS, the instruction following a jump or branch instruction is executed before the jump/branch instruction itself. As a consequence, branch instructions always swap places with the instruction which must be executed beforehand.

- In practice

Functions which merely returns 1 (true) or 0 (false) are very frequent in practice. 

Smallest ever standard UNIX utilities, /bin/true and /bin/false returns 0 and 1 respectively, as an exit code (zero as an exit code usually means success, non-zero means error).

## Hello World
```c
#include <stdio.h>
int main()
{
	printf("hello, world\n");
	return 0;
}
```

### x86

- MSVC
Let's compile it in MSVC 2010:
```nasm
CONST SEGMENT
$SG3830 DB 		'hello, world', 0AH, 00H
CONST ENDS
PUBLIC _main
EXTRN _printf:PROC
; Function compile flags: /Odtp
_TEXT SEGMENT
_main 		PROC
			push 	ebp
			mov 	ebp, esp
			push 	OFFSET $SG3830
			call 	_printf
			add 	esp, 4
			xor 	eax, eax
			pop 	ebp
			ret 	0
_main ENDP
_TEXT ENDS
```

In our case, the file contains two segments: `CONST` (for data constants) and `_TEXT` (for code). The string `hello, world` in C/C++ has type `const char[]`, but it does not have its own name. The compiler needs to deal with the string somehow so it defines the internal name `$SG3830` for it.

That is why the example may be rewritten as follows:
```c
#include <stdio.h>
const char $SG3830[]="hello, world\n";
int main()
{
	printf($SG3830);
	return 0;
}
```

As we can see, the string is terminated by a zero byte, which is standard for C/C++ strings. In the code segment, `_TEXT`, there is only one function so far: `main()`. The function `main()` starts with prologue code and ends with epilogue code.

After the function prologue we see the call to the `printf()` function: `CALL _printf`. Before the call, a string address (or a pointer to it) containing our greeting is placed on the stack with the help of the `PUSH` instruction.

When the `printf()` function returns the control to the `main()` function, the string address (or a pointer to it) is still on the stack. Since we do not need it anymore, the stack pointer (the ESP register) needs to be corrected.

`ADD ESP, 4` means add 4 to the `ESP` register value. Why 4? Since this a 32-bit program, we need exactly 4 bytes for address passing through the stack. If it was x64 code we would need 8 bytes. `ADD EXP, 4` is effectively equivalent to `POP register` but without using any register. The Intel C++ compiler supposedly uses `POP ECX` since this instruction's opcode is shorter than `ADD ESP, x` (1 byte for `POP` against 3 for `ADD`).

After calling `printf()`, the original C/C++ code contains the statement `return 0` - return 0 as the result of the `main()` function.

In the generated code this is implemented by the instruction `XOR EAX, EAX`. `XOR` is in fact just "eXclusive OR" but the compilers often use it instead of `MOV EAX, 0` - again because it is slightly shorter opcode (2 bytes for `XOR` against 5 for `MOV`).

The last instruction `RET` returns the control to the caller. Usually, this is C/C++ CRT (C runtime library) code which in turn returns control to the OS.

### GCC
Now Let's try to compile the same C/C++ code in the GCC 4.4.1 compiler in Linux. Next, with the assistance of the [IDA disassembler](https://hex-rays.com/).
```
main proc near
var_10 		= dword ptr -10h
			push 	ebp
			mov 	ebp, esp
			and 	esp, 0FFFFFFF0h
			sub 	esp, 10h
			mov 	eax, offset aHelloWorld ; "hello, world\n"
			mov 	[esp+10h+var_10], eax
			call 	_printf
			mov 	eax, 0
			leave
			retn
main 		endp
```

The result is almost the same. The address of the `hello, world` string (stored in the data segment) is loaded in the `eax` register first and then it is saved onto the stack. In addition, the function prologue has `and esp, 0FFFFFFF0h` - this instruction aligns the `esp` register value on a 16-byte boundary. This results in all values in the stack being aligned the same way (The CPU performs better if the values it is dealing with are located in memory at addresses aligned on a 4-byte or 16-byte boundary).

`sub esp, 10h` allocates 16 bytes on the stack. Although, as we can see hereafter, only 4 are necessary here. This is because the size of the allocated stack is also aligned on a 16-byte boundary.

The string addres (or a pointer to the string) is then stored directly onto the stack without using the `PUSH` instruction. *var_10* - is a local variable and is also an argument for `printf()`. Then the `printf()` function is called.

Unlike MSVC, when GCC is compiling without oprimization turned on, it emits `mov eax, 0` instead of a shorter opcode.

The last instruction, `leave` - is the equivalent of the `mov ebp, esp` and `pop ebp` instruction pair - in other words, this instruction sets the stack pointer (esp) back and restores the `ebp` register to its initial state. This is necessary since we modified these register values (`esp` and `ebp`) at the beginning of the function.

- GCC: AT&T syntax
This syntax is much more popular in the UNIX-world.
```gas
.LC0:
		.string "hello, world\n"
main:
		pushl 	%ebp
		movl 	%esp, %ebp
		andl 	$-16, %esp
		subl 	$16, %esp
		movl 	$.LC0, (%esp)
		call 	printf
		movl 	$0, %eax
		leave
		ret
```

Some of the major differences between Intell and AT&T syntax are:
  - Source and destination operands are written in opposite order. In Intel-syntax: <instruction> <destination operand> <source operand>. In AT&T syntax: <instruction> <source operand> <destination operand>.
  - When you deal with Intel-syntax, you can imagine that there is an equality sign (=) between operands and when you deal with AT&T-syntax imagine there is a right arrow (->).
  - AT&T: Before register names, a parcent sign must be written (%) and before numbers a dollar sign ($). Parentheses are used instead of brackets.
  - AT&T: a suffix is added to instructions to define the operand size: q - quad (64 bits), l - long (32 bits), w - word (16 bits), b - byte (8 bits).

With one subtle difference: `0FFFFFFF0h` is presented as `$-16`. It is the same thing: `16` in the decimal system is `0x10` in hexadecimal. `-0x10` is equal to `0xFFFFFFF0` (for a 32-bit data type).

The return value is to be set to 0 by using the usual `movl`, not `XOR`. `movl` just loads a value to a register. Its name is a misnomer (data is not moved but rather copied). In other architectures, this instruction is named "LOAD" or "STORE" or something similar.

### x86-64

Let's also try 64-bit MSVC:
```nasm
$SG2989 DB 'hello, world', 0AH, 00H
main 	PROC
		sub 	rsp, 40
		lea 	rcx, OFFSET FLAT:$SG2989
		call 	printf
		xor 	eax, eax
		add 	rsp, 40
		ret 	0
main 	ENDP
```

In x84-64, all registers were extended to 64-bit and now their names have an `R-` prefix. In order to use the stack less often (in other words, to access external memory/cache less often), there exists a popular way to pass function arguments via registers. i.e., a part of the function arguments is passed in registers, the rest - via the stack. In Win64, 4 function arguments are passed in the `RCX`, `RDX`, `R8`, `R9` registers.  That is what we see here: a pointer to the string for `printf()` is now passed not in the stack, but in the `RCX` register. The pointers are 64-bit now, so they are passed in the 64-bit registers (which have the `R-` prefix). However, for backward compatibility, it is still possible to access the 32-bit parts, using the `E-` prefix.

The `main()` function returns an *int-*typed value, which is, in C/C++, for beter backward compatibility and portability, still 32-bit, so that is why the `eax` register is cleared at the function end (i.e., the 32-bit part of the register) instead of `rax`. There are also 40 bytes allocated in the local stack. This is called the "shadow space".

- GCC: x86-64
Let's also try GCC in 64-bit Linux:
```nasm
.string "hello, world\n"
main:
		sub 	rsp, 8
		mov 	edi, OFFSET FLAT:.LC0 ; "hello, world\n"
		xor 	eax, eax ; number of vector registers passed
		call 	printf
		xor 	eax, eax
		add 	rsp, 8
		ret
```

A method to pass function arguments in registers is also used in Linux and Mac OS x. The first 6 arguments are passed in the `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9` registers, and the rest - via the stack.

It is important to keep in mind that all `MOV` instructions in 64-bit mode that write something into the lower 32-bit register part also clear the higher 32-bits. i.e., the `MOV EAX, 011223344h` writes a value into `RAX` correctly, since the higher bits will be cleared.

If we open the compiled object file (.o), we can also see all the instructions' opcodes:
```nasm
.text:00000000004004D0 					main proc near
.text:00000000004004D0 48 83 EC 08 		sub 	rsp, 8
.text:00000000004004D4 BF E8 05 40 00 	mov 	edi, offset format ; "hello, world\n"
.text:00000000004004D9 31 C0 			xor 	eax, eax
.text:00000000004004DB E8 D8 FE FF FF 	call 	_printf
.text:00000000004004E0 31 C0 			xor 	eax, eax
.text:00000000004004E2 48 83 C4 08 		add 	rsp, 8
.text:00000000004004E6 C3 				retn
.text:00000000004004E6 					main endp
```

As we can see, the instruction that writes into `edi` at `0x4004D4` occupies 5 bytes. The same instruction writing a 64-bit value into `rdi` occupies 7 bytes. Apparently, GCC is trying to save some space. Besides, it can be sure that the data segment containing the string will not be allocated at the addresses higher than 4 GiB.

We also see that the `eax` register has been cleared before the `printf()` function call. This is done because according to **ABI!** standard mentioned above, the number of used vector registers is passed in `eax` in *NIX systems on x86-64.

### ARM

For my experiments with ARM processors, several compilers were used:
  - Popular in the embedded area: Keil Release 6/2013.
  - Apple Xcode 4.6.3 IDE with the LLVM-GCC 4.2 compiler.
  - GCC 4.9 (Linaro) (for ARM64), 32-bit ARM code (including Thumb and Thumb-2 modes).

- Non-optimizing Keil 6/2013 (ARM mode)

The armcc compiler produces assembly listings in Intel-syntax, but it has high-level ARM-processor related macros.
```
.text:00000000 				main
.text:00000000 10 40 2D E9 		STMFD 	SP!, {R4,LR}
.text:00000004 1E 0E 8F E2 		ADR 	R0, aHelloWorld ; "hello, world"
.text:00000008 15 19 00 EB 		BL 		__2printf
.text:0000000C 00 00 A0 E3 		MOV 	R0, #0
.text:00000010 10 80 BD E8 		LDMFD 	SP!, {R4,PC}
.text:000001EC 68 65 6C 6C+aHelloWorld DCB "hello, world",0 ; DATA XREF: main+4
```

In the example, we can easily see each instruction has a size of 4 bytes. Indeed, we compiled our code for ARM mode, not for Thumb.

The very first instruction, `STMFD SP!, {R4,LR}`, works as an x86 `PUSH` instruction, writing the values of two registers (`R4` and `LR`) into the stack. This instruction first decrements the `SP` so it points to the place in the stack that is free for new entries, then it saves the values of the `R4` and `LR` registers at the address stored in the modified. The `STMFD` instruction is a generalization of the `PUSH` instruction (extending its features), since it can work with any register, not just with `SP`. In other words, `STMFD` may be used for storing a set of registers at the specified memory address.

The `ADR R0, aHelloWorld` instruction adds or subtracts the value in the PC (Program Counter) register to the offset where the `hello world` string is located (PC in ARM, IP/EIP/RIP in x86/64). How is the `PC` register used here? This is called "position-independent code". Such code can be executed at a non-fixed address in memory. In other words, this is PC-relative addressing.

The `ADR` instruction takes into account the difference between the address of this instruction and the address where the string is located. This difference (offset) is always to be the same, no matter at what address our code is loaded by the OS. That's why all we need is to add the address of the current instruction (from PC) in order to get the absolute memory address of our C-string.

`BL __2printf` instruction calls the `printf()` function. BL (Branch with Link). Here's how this instruction works:
  - store the address following the `BL` instruction (`0xC`) into the `LR`;
  - then pass the control to `printf()` by writing its address into the PC register.

When `printf()` finishes its execution it must have information about where it needs to return the control to. That's why each function passes control to the address stored in the `LR` register.

That is a difference between "pure" RISC-processors like ARM and CISC-processors (Complex instruction set computing) like x86, where the return address is usually stored on the stack.

All ARM-mode instructions have a size of 4 bytes (32 bits). Hence, they can only be located on 4-byte boundary addresses. This implies that the last 2 bits of the instruction address (which are always zero bits) may be omitted. In summary, we have 26 bits for offset encoding. 

Next, the `MOV R0, #0` instruction just writes 0 into the `R0` register. That's because our C-function returns 0 and the return value is to be placed in the `R0` register.

The last instruction `LDMFD SP!, R4, PC`. It loads values from the stack (or any other memory place) in order to save them into `R4` and `PC`, and increments the stack pointer `SP`. It works like liek `POP` here. The very first instruction `STMFD` saved the `R4` and `LR` register pair on the stack, but `R4` and `PC` are restored during the `LDMFD` execution.

The address of the place where each function must return control to is usually saved in the `LR` register. The very first instruction saves its value in the stack because the same register will be used by our `main()` function when calling `printf()`. In the function's end, this value can be written directly to the `PC` register, thus passing control to where our function has been called.

Since `main()` is usually the primary function in C/C++, the control will be returned to the OS loader or to a point in a CRT, or something like that. All that allows omitting the `BX LR` instruction at the end of the function.

`DCB` is an assembly language directive defining an array of bytes or ASCII strings, akin to the DB directive in the x86-assembly language.

- Non-optimizing Keil 6/2013 (Thumb mode)

We are getting (in IDA):
```
.text:00000000 				main
.text:00000000 10 B5 			PUSH 	{R4,LR}
.text:00000002 C0 A0 			ADR 	R0, aHelloWorld ; "hello, world"
.text:00000004 06 F0 2E F9 		BL 		__2printf
.text:00000008 00 20 			MOVS 	R0, #0
.text:0000000A 10 BD 			POP 	{R4,PC}
.text:00000304 68 65 6C 6C+aHelloWorld DCB "hello, world",0 ; DATA XREF: main+2
```

The `BL` instruction consists of two 16-bit instructions. This is because it is impossible to load an offset for the `printf()` function while using the small space in one 16-bit opcode. Therefore, the first 16-bit instruction loads the higher 10 bits of the offset and the second instruction loads the lower 11 bits of the offset.

As was noted, all instructions in Thumb mode have a size of 2 bytes (or 16 bits). This implies it is impossible for a Thumb-instruction to be at an odd address whatsoever. Given the above, the last address bit may be omitted while encoding instructions.

As for the other instructions in the function: `PUSH` and `POP` work here just like the described `STMFD` / `LDMFD` only the `SP` register is not mentioned explicitly here. `ADR` works just like in the previous example. `MOVS` writes 0 into the R0 register in order to return zero.

- ARM64

Let's compile the example using GCC 4.8.1 in ARM64:
```
0000000000400590 <main>:
 400590: 	a9bf7bfd 	stp 	x29, x30, [sp,#-16]!
 400594: 	910003fd 	mov 	x29, sp
 400598: 	90000000 	adrp 	x0, 400000 <_init-0x3b8>
 40059c: 	91192000 	add 	x0, x0, #0x648
 4005a0: 	97ffffa0 	bl 		400420 <puts@plt>
 4005a4: 	52800000 	mov 	w0, #0x0 					// #0
 4005a8: 	a8c17bfd 	ldp 	x29, x30, [sp],#16
 4005ac: 	d65f03c0 	ret
 ...

Contents of section .rodata:
 400640 01000200 00000000 48656c6c 6f210a00 ........Hello!..
```

There are no Thumb and Thumb-2 modes in ARM64, only ARM, so there are 32-bit instructions only. Register count is doubled. 64-bit registers have `X-` prefixes, while its 32-bit parts - `W-`.

The `STP` instruction (Store Pair) saves two registers in the stack simultaneously: `X29` and `X30`. This instruction is able to save this pair at an arbitrary place in memory, but the `SP` register is specified here, so the pair is saved in the stack.

ARM64 registers are 64-bit ones, each has a size of 8 bytes, so one needs 16 bytes for saving two registers.

The exclamation mark ("!") after the operand means that 16 is to be subtracted from `SP` first, and only then are values from register pair to be written into the stack. This is also called *pre-index*.

Hence, in terms of the more familiar x86, the first instruction is just an analogue to a pair of `PUSH X29` and `PUSH X30`. `X29` is used as `FP` (Frame Pointer) in ARM64, and `X30` as `LR`, so that's why they are saved in the function prologue and restored in the function epilogue.

The second instruction copies `SP` in `X29` (or FP). This is made so to set up the function stack frame. `ADRP` and `ADD` instructions are used to fill the address of the string "Hello!" into the `X0` register, because the first function argument is passed in this register. There are no instructions, whatsoever, in ARM that can store a large number into a register (because the instruction length is limited to 4 bytes). So several instructions must he utilized. The first instruction (`ADRP`) writes the address of the 4KiB page, where the string is located, into `x0`, and the second one (ADD) just adds the remainder to the address.

`0x400000 + 0x648 = 0x400648`, and we see our "Hello!" C-string in the `.rodata` data segment at this address.

`MOV` writes 0 into `W0`. `W0` is the lower 32 bits of the 64-bit `X0` register. The function result is returned via `X0` and `main()` returns 0. But why use the 32-bit part? Because the `int` data type in ARM64, just like in x86-64, is still 32-bit, for better compatibility. So if a function returns a 32-bit int, only the lower 32 bits of `X0` register have to be filled.

`LDP`(Load Pair) then restores the `x29` and `X30` registers. There  is no exclamation mark after the instruction: this implies that the values are first loaded from the stack, and only then is SP increased by 16. This is called *post-index*.

A new instruction appeared in ARM64: `RET`. It works just as `BX LR`, only a special hint bit is added, informing the CPU that this is a return from a function, not just another jump instruction, so it can execute it more optimally.

### MIPS

One important MIPS concepts is the "global pointer". As we may already know, each MIPS instruction has a size of 32 bits, so it's impossible to embed a 32-bit address into one instruction. It's possible, however, to load data from the address in the range of *register* -32768 ... *register* +32767 using one single instruction (because 16 bits of signed offset could be encoded in a single instruction).

So we can allocate some register for this purpose and also allocate a 64KiB area of most used data. This allocated register is called a "global pointer" and it points to the middle of the 64KiB area. This area usually contains global variables and addresses of imported functions like `printf()`, because the GCC developers decided that getting the address of some function must be as fast as a single instruction execution instead of two. In an ELF file this 64KiB area is located partly in sections .sbss ("small BSS") for uninitialized data and .sdata ("small data") for initialized data. This implies that the programmer may choose what data he/she wants to be accessed fast and place it into .sdata/.sbss.

- Optimizing GCC

Let consider the following example, which illustrates the "global pointer" concept.
```
$LC0:
; \000 is zero byte in octal base:
		 .ascii "Hello, world!\012\000"
main:
; function prologue.
; set the GP:
		 lui 		$28,%hi(__gnu_local_gp)
		 addiu 		$sp,$sp,-32
		 addiu 		$28,$28,%lo(__gnu_local_gp)
; save the RA to the local stack:
		 sw 		$31,28($sp)
; load the address of the puts() function from the GP to $25:
		 lw 		$25,%call16(puts)($28)
; load the address of the text string to $4 ($a0):
		 lui 		$4,%hi($LC0)
; jump to puts(), saving the return address in the link register:
		 jalr 		$25
		 addiu 		$4,$4,%lo($LC0) ; branch delay slot
; restore the RA:
		 lw 		$31,28($sp)
; copy 0 from $zero to $v0:
		 move 		$2,$0
; return by jumping to the RA:
		 j 			$31
; function epilogue:
		 addiu 		$sp,$sp,32 ; branch delay slot + free local stack
```

As we see, the $GP register is set in the function prologue to point to the middle of this area. The RA register is also saved in the local stack.  `puts()` is also used here instead of `printf()`. The address of the `puts()` function is loaded into $25 using `LW` the instruction ("Load Word"). Then the address of the text string is loaded to $4 using `LUI` ("Load Upper Immediate") and `ADDIU` ("Add Immediate Unsigned Word") instruction pair. `LUI` sets the high 16 bits of the register (hence "upper" word in instruction name) and `ADDIU` adds the lower 16 bits of the address.

`ADDIU` follows `JALR`. The register $4 is also called $A0, which is used for passing the first function argument.

`JALR` ("Jump and Link Register") jumps to the address stored in the $25 register (address of `puts()`) while saving the address of the next instruction (LW) in RA. This is very similar to ARM. One important thing is that the address saved in RA is not the address of the next instruction (because it's in a delay slot and is executed before the jump instruction), but the address of the instruction after the next one (after the delay slot). Hence, PC+8 is written to RA during the execution of `JALR`, in our case, this is the address of the `LW` instruction next to `ADDIU`.

`LW` (“Load Word”) restores RA from the local stack (this instruction is actually part of the function epilogue).

`MOVE` copies the value from the $0 ($ZERO) register to $2 ($V0).

MIPS has a constant register, which always holds zero. Apparently, the MIPS developers came up with the idea that zero is in fact the busiest constant in the computer programming, so let’s just use the $0 register every time zero is needed.

Another interesting fact is that MIPS lacks an instruction that transfers data between registers. In fact, `MOVE DST, SRC` is `ADD DST, SRC`, $ZERO (DST = SRC + 0), which does the same. Apparently, the MIPS developers wanted to have a compact opcode table. This does not mean an actual addition happens at each MOVE instruction. Most likely, the CPU optimizes these pseudo instructions and the ALU is never used.

`J` jumps to the address in `RA`, which is effectively performing a return from the function. `ADDIU` after J is in fact executed before `J` (remember branch delay slots?) and is part of the function epilogue.

### Conclusion

The main difference between x86/ARM and x64/ARM64 code is that the pointer to the string is now 64-bits in length. Indeed, modern CPUs are now 64-bit due to both the reduced cost of memory and the greater demand for it by modern applications. We can add much more memory to our computers than 32-bit pointers are able to address. As such, all pointers are now 64-bit.

## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)
