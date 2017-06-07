
# 4. printf() with several arguments

- [x86](#x86)
  - [MSVC](#msvc)
  - [GCC](#gcc)
- [x64](#x64)
  - [MSVC](#msvc)
  - [GCC](#gcc)
- [ARM](#arm)
  - [32-bit ARM](#32-bit-arm)
  - [ARM64](#arm64)
- [MIPS](#mips)
- [Conclusion](#conclusion)


## x86

**3 arguments**

Now let's extend the *Hello, world!* example, replacing `printf()` in the `main()` function body with this:
```c
#include <stdio.h>

int main()
{
	printf("a=%d; b=%d; c=%d", 1, 2, 3);
	return 0;
};
```

### MSVC

When we compile it with MSVC 2010 Express we get:
```nasm
$SG3830 DB 'a=%d; b=%d; c=%d', 00H
...
		push 	3
		push 	2
		push 	1
		push 	OFFSET $SG3830
		call 	_printf
		add 	esp, 16 			; 00000010H
```

We can see the `printf()` arguments are pushed onto the stack in reverse order. The first argument is pushed last. By the way, variables of *int* type in 32-bit environment have 32-bit width, that is 4 bytes. So, we have 4 arguments here. 4*4 = 16 - they occupy exactly 16 bytes in the stack: a 32-bit pointer to a string and 3 numbers of type int.

When the stack pointer (ESP register) has changed back by the `add esp, X` instruction after a function call, often, the number of function arguments could be deduced by simply dividing X by 4.

In certain cases where several functions return right after one another, the compiler could merge multiple "ADD ESP, X" instruction into one, after the last call. Here is a real-world example:
```nasm
.text:100113E7 	push 	3
.text:100113E9 	call 	sub_100018B0 ; takes one argument (3)
.text:100113EE 	call 	sub_100019D0 ; takes no arguments at all
.text:100113F3 	call 	sub_10006A90 ; takes no arguments at all
.text:100113F8 	push 	1
.text:100113FA 	call 	sub_100018B0 ; takes one argument (1)
.text:100113FF 	add 	esp, 8 ; drops two arguments from stack at once
```

### GCC 

Now let's compile the same program in Linux using GCC 4.4.1 and take a look at what we have got in IDA:
```nasm
main 		proc near

var_10 		= dword ptr -10h
var_C 		= dword ptr -0Ch
var_8 		= dword ptr -8
var_4 		= dword ptr -4

			push 	ebp
			mov 	ebp, esp
			and 	esp, 0FFFFFFF0h
			sub 	esp, 10h
			mov 	eax, offset aADBDCD ; "a=%d; b=%d; c=%d"
			mov 	[esp+10h+var_4], 3
			mov 	[esp+10h+var_8], 2
			mov 	[esp+10h+var_C], 1
			mov 	[esp+10h+var_10], eax
			call 	_printf
			mov 	eax, 0
			leave
			retn
main 		endp
```

Its noticeable that the difference between the MSVC code and the GCC code is only in the way the arguments are stored on the stack. Here the GCC is working directly with the stack without the use of `PUSH`/`POP`.

Let's try this example also in gdb in linux, `-g` option: include debug information.
```sh
$ gcc 1.c -g -o 1
$ gdb 1
(gdb) b printf
(gdb) run
```

Print 10 stack elements. The most left column contains address on the stack
```nasm
(gdb) x/10w $esp
0xbffff11c: 	0x0804844a 	0x080484f0 	0x00000001 	0x00000002
0xbffff12c: 	0x00000003 	0x08048460 	0x00000000 	0x00000000
0xbffff13c: 	0xb7e29905 	0x00000001
```

The very first element is the `RA` (0x0804844a). We can verify this by disassembling the memory at this address:
```nasm
(gdb) x/5i 0x0804844a
	0x804844a <main+45>: 	mov 	$0x0,%eax
	0x804844f <main+50>: 	leave
	0x8048450 <main+51>: 	ret
	0x8048451: 	xchg 	%ax,%ax
	0x8048453: 	xchg 	%ax,%ax
```

The two `XCHG` instructions are idle instructions, analogous to `NOP`s.

The second element (0x080484f0) is the format string address:
```nasm
(gdb) x/s 0x080484f0
0x80484f0: 		"a=%d; b=%d; c=%d"
```

Next 3 elements (1,2,3) are the `printf()` arguments. The rest of the elements could be just "garbage" on the stack, but could also be values from other functions, their local vairables, etc. We can ignore them for now.

Run "finish" to execute all instructions until the end of the function `printf()`. gdb shows what `printf()` returned in `EAX` (13). This is the number of characters printed out.

gdb generates debugging information, also saves a table of relation between source code line numbers and instruction addresses. gdb is a source-level debugger, after all.

Let's examine the registers. 13 is in `EAX`:
```nasm
(gdb) info registers
eax 	0xd 	13
ecx 	0x0 	0
edx 	0x0 	0
ebx 	0xb7fc0000 		-1208221696
esp 	0xbffff120 		0xbffff120
ebp 	0xbffff138 		0xbffff138
esi 	0x0 	0
edi 	0x0 	0
eip 	0x804844a 		0x804844a <main+45>
...
```

Let's disassemble the current instructions. The arrow points to the isntruction to be executed next. gdb uses AT&T syntax by default. But it is possible to switch to Intel syntax:
```nasm
(gdb) set disassembly-flavor intel
(gdb) disas
Dump of assembler code for function main:
	0x0804841d <+0>: 	push 	ebp
	0x0804841e <+1>: 	mov 	ebp,esp
	0x08048420 <+3>: 	and 	esp,0xfffffff0
	0x08048423 <+6>: 	sub 	esp,0x10
	0x08048426 <+9>: 	mov 	DWORD PTR [esp+0xc],0x3
	0x0804842e <+17>: 	mov 	DWORD PTR [esp+0x8],0x2
	0x08048436 <+25>: 	mov 	DWORD PTR [esp+0x4],0x1
	0x0804843e <+33>: 	mov 	DWORD PTR [esp],0x80484f0
	0x08048445 <+40>: 	call 	0x80482f0 <printf@plt>
=> 	0x0804844a <+45>: 	mov 	eax,0x0
	0x0804844f <+50>: 	leave
	0x08048450 <+51>: 	ret
End of assembler dump.
```

Execute next instruction. gdb shows ending bracket, meaning, it ends the block.
```nasm
(gdb) step
7		};
```

Let's examine the registers after the `MOV EAX, 0` instruction execution. Indeed `EAX` is zero at that point.
```nasm
(gdb) info registers
eax 	0x0 	0
ecx 	0x0 	0
edx 	0x0 	0
ebx 	0xb7fc0000 		-1208221696
esp 	0xbffff120 		0xbffff120
ebp 	0xbffff138 		0xbffff138
esi 	0x0 	0
edi 	0x0 	0
eip 	0x804844f 		0x804844f <main+50>
...
```

## x64

**8 arguments**

To see how other arguments are passed via the stack, let's change our example again by increasing the number of arguments to 9 (`printf()` format string + 8 int variables):
```c
#include <stdio.h>

int main()
{
	printf("a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d\n", 1, 2, 3, 4, 5, 6, 7, 8);
	return 0;
};
```

### MSVC

As it was mentioned eariler, the first 4 arguments has to be passed through the `RCX`, `RDX`, `R8`, `R9` registers in Win64, while all the rest - via the stack. However, the `MOV` instruction, instead of `PUSH`, is used for preparing the stack, so the values are stored to the stack in a straightforward manner.
```nasm
$SG2923 DB 'a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d', 0aH, 00H
main 	PROC
		sub 	rsp, 88
		mov 	DWORD PTR [rsp+64], 8
		mov 	DWORD PTR [rsp+56], 7
		mov 	DWORD PTR [rsp+48], 6
		mov 	DWORD PTR [rsp+40], 5
		mov 	DWORD PTR [rsp+32], 4
		mov 	r9d, 3
		mov 	r8d, 2
		mov 	edx, 1
		lea 	rcx, OFFSET FLAT:$SG2923
		call 	printf
		; return 0
		xor 	eax, eax
		add 	rsp, 88
		ret 	0
main 	ENDP
_TEXT 	ENDS
END
```

Why are 8 bytes allocated for int values, when 4 is enough? The reason: 8 bytes are allocated for any data type shorter than 64 bits. This is established for the vonvenience's sake: it makes it easy to calculate the address of arbitrary argument. Besides, they are all located at aligned memory addresses. It is the same in the 32-bit environments: 4 bytes are reserved for all data types.

### GCC

The picture is similar for x86-64 *NIX OS-es, except that the first 6 arguments are passed through the `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9` registers. All the rest - via the stack. GCC generates the code storing the string pointer into `EDI` instead of `RDI`.
```nasm
.LC0:
		.string "a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d\n"
main:
		sub 	rsp, 40
		mov 	r9d, 5
		mov 	r8d, 4
		mov 	ecx, 3
		mov 	edx, 2
		mov 	esi, 1
		mov 	edi, OFFSET FLAT:.LC0
		xor 	eax, eax ; number of vector registers passed
		mov 	DWORD PTR [rsp+16], 8
		mov 	DWORD PTR [rsp+8], 7
		mov 	DWORD PTR [rsp], 6
		call 	printf
		; return 0
		xor 	eax, eax
		add 	rsp, 40
		ret
```

**GCC + GDB**

Let's try this example in gdb.
```sh
$ gcc -g 2.c -o 2
$ gdb 2
```

Let's set the breakpoint to `printf()` and run. Registers `RSI`/`RDX`/`RCX`/`R8`/`R9` have the expected values. `RIP` has the address of the very first instruction of the `printf()` function.
```nasm
(gdb) b printf
(gdb) run
(gdb) info registers
rax 	0x0			0
rbx 	0x0 		0
rcx 	0x3 		3
rdx 	0x2 		2
rsi 	0x1 		1
rdi 	0x400628 	4195880
rbp 	0x7fffffffdf60 		0x7fffffffdf60
rsp 	0x7fffffffdf38 		0x7fffffffdf38
r8 		0x4 		4
r9 		0x5 		5
r10 	0x7fffffffdce0 		140737488346336
r11 	0x7ffff7a65f60 		140737348263776
r12 	0x400440 	4195392
r13 	0x7fffffffe040 		140737488347200
r14 	0x0 		0
r15 	0x0 		0
rip 	0x7ffff7a65f60 		0x7ffff7a65f60 <__printf>

(gdb) x/s $rdi
0x400628: 	"a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d\n"
```

Let's dump the stack with the x/g command this time -g stands for *giant words*, i.e., 64-bit words.
```nasm
(gdb) x/10g $rsp
0x7fffffffdf38: 0x0000000000400576 		0x0000000000000006
0x7fffffffdf48: 0x0000000000000007 		0x00007fff00000008
0x7fffffffdf58: 0x0000000000000000 		0x0000000000000000
0x7fffffffdf68: 0x00007ffff7a33de5 		0x0000000000000000
0x7fffffffdf78: 0x00007fffffffe048 		0x0000000100000000
```

The very first stack element, just like in the previous case, is the `RA`. 3 values are also passed through the stack: 6, 7, 8. We also see that 8 is passed with the high 32-bits not cleared: `0x00007fff00000008`. That's OK, because the values are of int type, which is 32-bit. So, the high register or stak element part may contain "random garbage".

If you take a look at where the control will return after the `printf()` execution, gdb will show the entire `main()` function:
```nasm
(gdb) set disassembly-flavor intel
(gdb) disas 0x0000000000400576
Dump of assembler code for function main:
	0x000000000040052d <+0>: 	push 	rbp
	0x000000000040052e <+1>: 	mov 	rbp,rsp
	0x0000000000400531 <+4>: 	sub 	rsp,0x20
	0x0000000000400535 <+8>: 	mov 	DWORD PTR [rsp+0x10],0x8
	0x000000000040053d <+16>: 	mov 	DWORD PTR [rsp+0x8],0x7
	0x0000000000400545 <+24>: 	mov 	DWORD PTR [rsp],0x6
	0x000000000040054c <+31>: 	mov 	r9d,0x5
	0x0000000000400552 <+37>: 	mov 	r8d,0x4
	0x0000000000400558 <+43>: 	mov 	ecx,0x3
	0x000000000040055d <+48>: 	mov 	edx,0x2
	0x0000000000400562 <+53>: 	mov 	esi,0x1
	0x0000000000400567 <+58>: 	mov 	edi,0x400628
	0x000000000040056c <+63>: 	mov 	eax,0x0
	0x0000000000400571 <+68>: 	call 	0x400410 <printf@plt>
	0x0000000000400576 <+73>: 	mov 	eax,0x0
	0x000000000040057b <+78>: 	leave
	0x000000000040057c <+79>: 	ret
End of assembler dump.
```

Let's finish executing `printf()`, execute the instruction zeroing `EAX`, and note that the `EAX`, and note that the `EAX` register has a value of exactly zero. `ZIP` now points to the `LEAVE` instruction, i.e., the penultimate one in the `main()` function.
```nasm
(gdb) finish
(gdb) next
(gdb) info registers
rax 	0x0 	0
rbx 	0x0 	0
rcx 	0x26 	38
rdx 	0x7ffff7dd59f0 	140737351866864
rsi 	0x7fffffd9 		2147483609
rdi 	0x0 	0
rbp 	0x7fffffffdf60 	0x7fffffffdf60
rsp 	0x7fffffffdf40 	0x7fffffffdf40
r8 		0x7ffff7dd26a0 	140737351853728
r9 		0x7ffff7a60134 	140737348239668
r10 	0x7fffffffd5b0 	140737488344496
r11 	0x7ffff7a95900 	140737348458752
r12 	0x400440 	4195392
r13 	0x7fffffffe040 	140737488347200
r14 	0x0 	0
r15 	0x0 	0
rip 	0x40057b 0x40057b <main+78>
...
```

## ARM

### 32-bit ARM

**ARM: 3 arguments**

ARM's traditional scheme for passing arguments (calling convention) behaves as follows: the first 4 arguments are passed through the `R0` - `R3` registers; the remaining arguments via the stack.

- Non-optimizing Keil 6/2013 (ARM mode)
```
.text:00000000 main
.text:00000000 10 40 2D E9 	STMFD 	SP!, {R4,LR}
.text:00000004 03 30 A0 E3 	MOV 	R3, #3
.text:00000008 02 20 A0 E3 	MOV 	R2, #2
.text:0000000C 01 10 A0 E3 	MOV 	R1, #1
.text:00000010 08 00 8F E2 	ADR 	R0, aADBDCD 	; "a=%d; b=%d; c=%d"
.text:00000014 06 00 00 EB 	BL 		__2printf
.text:00000018 00 00 A0 E3 	MOV 	R0, #0 			; return 0
.text:0000001C 10 80 BD E8 	LDMFD 	SP!, {R4,PC}
```

So, the first 4 arguments are passed via the `R0`-`R3` registers in this order: a pointer to the `printf()` format string in `R0`, then 1 in `R1`, 2 in `R2` and 3 in `R3`. The instruction at `0x18` writes 0 to `R0` - this is *return 0* C-statement.

- Optimizing Keli 6/2013 (Thumb mode)
```
.text:00000000 main
.text:00000000 10 B5 		PUSH 	{R4,LR}
.text:00000002 03 23 		MOVS 	R3, #3
.text:00000004 02 22 		MOVS 	R2, #2
.text:00000006 01 21 		MOVS 	R1, #1
.text:00000008 02 A0 		ADR 	R0, aADBDCD 	; "a=%d; b=%d; c=%d"
.text:0000000A 00 F0 0D F8 	BL 		__2printf
.text:0000000E 00 20 		MOVS 	R0, #0
.text:00000010 10 BD 		POP 	{R4,PC}
```

There is no significant difference from the non-optimized code for ARM mode.

**ARM: 8 arguments**

- Optimizing Keil 6/2013: ARM mode
```
.text:00000028 				main
.text:00000028
.text:00000028 				var_18 = -0x18
.text:00000028 				var_14 = -0x14
.text:00000028 				var_4 = -4
.text:00000028
.text:00000028 04 E0 2D E5 	STR 	LR, [SP,#var_4]!
.text:0000002C 14 D0 4D E2 	SUB 	SP, SP, #0x14
.text:00000030 08 30 A0 E3 	MOV 	R3, #8
.text:00000034 07 20 A0 E3 	MOV 	R2, #7
.text:00000038 06 10 A0 E3 	MOV 	R1, #6
.text:0000003C 05 00 A0 E3 	MOV 	R0, #5
.text:00000040 04 C0 8D E2 	ADD 	R12, SP, #0x18+var_14
.text:00000044 0F 00 8C E8 	STMIA 	R12, {R0-R3}
.text:00000048 04 00 A0 E3 	MOV 	R0, #4
.text:0000004C 00 00 8D E5 	STR 	R0, [SP,#0x18+var_18]
.text:00000050 03 30 A0 E3 	MOV 	R3, #3
.text:00000054 02 20 A0 E3 	MOV 	R2, #2
.text:00000058 01 10 A0 E3 	MOV 	R1, #1
.text:0000005C 6E 0F 8F E2 	ADR 	R0, aADBDCDDDEDFDGD ; "a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g =%"...
.text:00000060 BC 18 00 EB 	BL 		__2printf
.text:00000064 14 D0 8D E2 	ADD 	SP, SP, #0x14
.text:00000068 04 F0 9D E4 	LDR 	PC, [SP+4+var_4],#4
```

This code can be divided into several parts:
  - Function prologue:

  The very first `STR LR, [SP,#var_4]!` instruction saves `LR` on the stack, because we are going to use this register for the `printf()` call. Exclamation mark at the end indicates *pre-index*.

  This implies that `SP` is to be decreased by 4 first, and then `LR` will be saved at the address in `SP`. This is similar to `PUSH` in x86.

  The second `SUB SP, SP, #0x14` instruction decreases `SP` (the stack pointer) in order to allocate `0x14`(20) bytes on the stack. Indeed, we have to pass 5 32-bit values via the stack to the `printf()` function, and each one occupies 4 bytes, which is exactly 5*4 = 20. The other 4 32-bit values are to be passed through registers.

  - Passing 5, 6, 7 and 8 via the stack: they are stored in the `R0`, `R1`, `R2` and `R3` registers respectively.

  Then, the `ADD R12, SP, #0x18+var_14` instruction writes the stack address where these 4 variables are to be stored, into the `R12` register. *var_14* is an assembly macro, equal to -0x14 by IDA to conveniently display the code accessing the stack. The *var_?* macros generated by IDA reflect variables in the stack.

  So, `SP+4` is to be stored into the `R12` register. The next `STMIA R12, {R0-R3}` instruction writes registers `R0`-`R3` contents to the memory pointed by `R12`. `STMIA` abbreviates *Store Multiple Increment After*, "Increment After" implies that `R12` is to be increased by 4 after each register value is written.

  - Passing 4 via the stack: 4 is stored in `R0` and then this value, with the help of the `STR R0, [SP,#0x18+var_18]` instrcution, is saved on the stack. *var_18* is -0x18, so the offset is to be 0, thus the value from the `R0` register (4) is to be written to the address written in `SP`.

  - Passing 1, 2 and 3 via registers: The values of the first 3 numbers (a,b,c) (1,2,3 respectively) are passed through the `R1`, `R2` and `R3` registers right before the `printf()` call, and the other 5 values are passed via the stack.

  - `printf()` call.

  - Function epilogue:

  The `ADD SP, SP, #0x14` instruction restores the `SP` pointer back to its former value, thus annulling everything what has been stored on the stack. Of course, what has been stored on the stack will stay there, but it will all be rewritten during the execution of subsequent functions.

  The `LDR PC, [SP+4+var_4],#4` instruction loads the saved `LR` value from the stack into the `PC` register, thus causing the function to exit. There is no exclamation mark - indeed, PC is loaded first from the address stored in SP (4+var_4=0, so this instruction is analogous to `LDR PC, [SP],#4`), and then SP is increased by 4. This is referred as *post-index*. Why does IDA display the instruction like that? Because it wants to illustrate the stack layout and the fact that `var_4` is allocated for saving the LR value in the local stack. This instruction is somewhat similar to `POP PC` in x86.

- Optimizing Keil 6/2013: Thumb mode
```
.text:0000001C 		printf_main2
.text:0000001C
.text:0000001C 		var_18 = -0x18
.text:0000001C 		var_14 = -0x14
.text:0000001C 		var_8 = -8
.text:0000001C
.text:0000001C 00 B5 	PUSH 	{LR}
.text:0000001E 08 23 	MOVS 	R3, #8
.text:00000020 85 B0 	SUB 	SP, SP, #0x14
.text:00000022 04 93 	STR 	R3, [SP,#0x18+var_8]
.text:00000024 07 22 	MOVS 	R2, #7
.text:00000026 06 21 	MOVS 	R1, #6
.text:00000028 05 20 	MOVS 	R0, #5
.text:0000002A 01 AB 	ADD 	R3, SP, #0x18+var_14
.text:0000002C 07 C3 	STMIA 	R3!, {R0-R2}
.text:0000002E 04 20 	MOVS 	R0, #4
.text:00000030 00 90 	STR 	R0, [SP,#0x18+var_18]
.text:00000032 03 23 	MOVS 	R3, #3
.text:00000034 02 22 	MOVS 	R2, #2
.text:00000036 01 21 	MOVS 	R1, #1
.text:00000038 A0 A0 	ADR 	R0, aADBDCDDDEDFDGD ; "a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%"...
.text:0000003A 06 F0 D9 F8 	BL 	__2printf
.text:0000003E
.text:0000003E 			loc_3E ; CODE XREF: example13_f+16
.text:0000003E 05 B0 	ADD 	SP, SP, #0x14
.text:00000040 00 BD 	POP 	{PC}
```

The output is almost like in the previous example. However, this is Thumb code the values are packed into stack differently: 8 goes first, then 5, 6, 7, and 4 goes third.

### ARM64

**ARM: 3 arguments**

- Non-optimizing GCC (Linaro) 4.9
```
.LC1:
		.string "a=%d; b=%d; c=%d"
f2:
; save FP and LR in stack frame:
		stp 	x29, x30, [sp, -16]!
; set stack frame (FP=SP):
		add 	x29, sp, 0
		adrp 	x0, .LC1
		add 	x0, x0, :lo12:.LC1
		mov 	w1, 1
		mov 	w2, 2
		mov 	w3, 3
		bl 		printf
		mov 	w0, 0
; restore FP and LR
		ldp 	x29, x30, [sp], 16
		ret
```

The first instruction `STP` (Store Pair) saves `FP` (X29) and `LR` (X30) in the stack. The second `ADD X29, SP, 0` instruction forms the stack frame. It is just writing the value of `SP` into X29.

Next, we see the familiar `ADRP`/`ADD` instruction pair, which forms a pointer to the string. *lo12* meaning low 12 bits, i.e., liker will write low 12 bits of LC1 address into the opcode of `ADD` instruction. `%d` in `printf()` string format is a 32-bit int, so the 1, 2, and 3 are loaded into 32-bit register parts.

**ARM: 8 arguments**

- Non-optimizing GCC(Linaro) 4.9
```
.LC2:
		.string "a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d\n"
f3:
; grab more space in stack:
		sub 	sp, sp, #32
; save FP and LR in stack frame:
		stp 	x29, x30, [sp,16]
; set stack frame (FP=SP):
		add 	x29, sp, 16
		adrp 	x0, .LC2 ; "a=%d; b=%d; c=%d; d=%d; e=%d; f=%d; g=%d; h=%d\n"
		add 	x0, x0, :lo12:.LC2
		mov 	w1, 8 ; 9th argument
		str 	w1, [sp] ; store 9th argument in the stack
		mov 	w1, 1
		mov 	w2, 2
		mov 	w3, 3
		mov 	w4, 4
		mov 	w5, 5
		mov 	w6, 6
		mov 	w7, 7
		bl 		printf
		sub 	sp, x29, #16
; restore FP and LR
		ldp 	x29, x30, [sp,16]
		add 	sp, sp, 32
		ret
```

The first 8 arguments are passed in X- or W- registers. A string pointer requires a 64-bit register, so it's passed in `X0`. All other value have a int 32-bit type, so they are stored in the 32-bit part of the registers (W-). The 9th argument (8) is passed via the stack. Indeed: it's not possible to pass large number of arguments through registers, because the number of registers is limited.

Optimizing GCC (Linaro) 4.9 generates the same code.

## MIPS

**3 arguments**

- Optimizing GCC 4.4.5 (IDA)

3 arguments are passed through the registers $5 ... $7 (or $A0...$A2). That is why these registers are prefixed with A-, which implies they are used for function arguments passing.
```
.text:00000000 main:
.text:00000000
.text:00000000 var_10 		= -0x10
.text:00000000 var_4 		= -4
.text:00000000
; function prologue:
.text:00000000 				lui 	$gp, (__gnu_local_gp >> 16)
.text:00000004 				addiu 	$sp, -0x20
.text:00000008 				la 		$gp, (__gnu_local_gp & 0xFFFF)
.text:0000000C 				sw 		$ra, 0x20+var_4($sp)
.text:00000010 				sw 		$gp, 0x20+var_10($sp)
; load address of printf():
.text:00000014 				lw 		$t9, (printf & 0xFFFF)($gp)
; load address of the text string and set 1st argument of printf():
.text:00000018 				la 		$a0, $LC0 # "a=%d; b=%d; c=%d"
; set 2nd argument of printf():
.text:00000020 				li 		$a1, 1
; set 3rd argument of printf():
.text:00000024 				li 		$a2, 2
; call printf():
.text:00000028 				jalr 	$t9
; set 4th argument of printf() (branch delay slot):
.text:0000002C 				li 		$a3, 3
; function epilogue:
.text:00000030 				lw 		$ra, 0x20+var_4($sp)
; set return value to 0:
.text:00000034 				move 	$v0, $zero
; return
.text:00000038 				jr 		$ra
.text:0000003C 				addiu 	$sp, 0x20 ; branch delay slot
```

IDA has coalesced pair of `LUI` and `ADDIU` instructions into one `LA` pseudo instruction. That's why there are no instruction at address 0x1C: because `LA` occupies 8 bytes.

**8 arguments**

Only the first 4 arguments are passed in the $A0 …$A3 registers, the rest are passed via the stack.

## Conclusion

Here is a rough skeleton of the function call:

- x86
```nasm
...
PUSH 3rd argument
PUSH 2nd argument
PUSH 1st argument
CALL function
; modify stack pointer (if needed)
```

- x64 (MSVC)
```nasm
MOV RCX, 1st argument
MOV RDX, 2nd argument
MOV R8, 3rd argument
MOV R9, 4th argument
...
PUSH 5th, 6th argument, etc. (if needed)
CALL function
; modify stack pointer (if needed)
```

- x64 (GCC)
```nasm
MOV RDI, 1st argument
MOV RSI, 2nd argument
MOV RDX, 3rd argument
MOV RCX, 4th argument
MOV R8, 5th argument
MOV R9, 6th argument
...
PUSH 7th, 8th argument, etc. (if needed)
CALL function
; modify stack pointer (if needed)
```

- ARM
```
MOV R0, 1st argument
MOV R1, 2nd argument
MOV R2, 3rd argument
MOV R3, 4th argument
; pass 5th, 6th argument, etc., in stack (if needed)
BL function
; modify stack pointer (if needed)
```

- ARM64
```
MOV X0, 1st argument
MOV X1, 2nd argument
MOV X2, 3rd argument
MOV X3, 4th argument
MOV X4, 5th argument
MOV X5, 6th argument
MOV X6, 7th argument
MOV X7, 8th argument
; pass 9th, 10th argument, etc., in stack (if needed)
BL function
; modify stack pointer (if needed)
```

- MIPS (O32 calling convention)
```
LI $4, 1st argument ; AKA $A0
LI $5, 2nd argument ; AKA $A1
LI $6, 3rd argument ; AKA $A2
LI $7, 4th argument ; AKA $A3
; pass 5th, 6th argument, etc., in stack (if needed)
LW temp_reg, address of function
JALR temp_reg
```

By the way, this difference between the arguments passing in x86, x64, fastcall, ARM and MIPS is a good illustration of the fact that the CPU is oblivious to how the arguments are passed to functions. It is also possible to create a hypothetical compiler able to pass arguments via a special structure without using stack at all.

MIPS $A0 …$A3 registers are labeled this way only for convenience (that is in the O32 calling convention). Programmers may use any other register (well, maybe except $ZERO) to pass data or use any other calling convention.

The CPU is not aware of calling conventions whatsoever.

## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)
