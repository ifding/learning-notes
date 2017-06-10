# 5. scanf()

- [Simple example](#simple-example)
  - [About pointers](#about-pointers)
  - [x86](#x86)
  - [ARM](#arm)
  - [MIPS](#mips)
- [scanf](#scanf)



## Simple example

```c
#include <stdio.h>

int main()
{
	int x;
	printf ("Enter X:\n");
	scanf ("%d", &x);
	printf ("You entered %d...\n", x);
	return 0;
};
```

It's not clever to use `scanf()` for user interactions nowdays. But we can, however, illustrate passing a pointer to a variable of type int.

### About pointers

Often, passing a large array, structure or object as an argument to another function is too expensive, while passing their address is much cheaper. For example, if you are going to print a text string to console, it's much easier to pass its address into OS kernel.

In addition if the callee function needs to modify something in the large array or structure received as a parameter and return back the entire structure then the situation is close to absurd. So the simplest thing to do is to pass the address of the array or structure to the callee function, and let it change what needs to be changed.

A pointer in C/C++ is simply an address of some memory location.

In x86, the address is represented as a 32-bit number (i.e., it occupies 4 bytes), while in x86-64 it is a 64-bit number (occupying 8 bytes). It means all pointers in the x64 architecture require twice as much space, including cache memory, which is "expensive" memory.

It is possible to work with untyped pointers only, given some effor; e.g. the standard C function `memcpy()`, that copies a block from one memory location to another, taks 2 pointers of type `void*` as arguments, since it is impossible to predict the type of the data you would like to copy. Data types are not important, only the block size matters.

*scanf()* function is such a case, besides the fact that the function needs to indicate how many values were successfully read, it also needs to return all these values.

In C/C++ the pointer type is only needed for compile-time type checking. Internally, in the compiled code there is no information about pointer types at all.

### x86

- MSVC
Here is what we get after compiling with MSVC 2010:
```nasm
CONST SEGMENT
$SG3831 DB 'Enter X:', 0aH, 00H
$SG3832 DB '%d', 00H
$SG3833 DB 'You entered %d...', 0aH, 00H
CONST ENDS
PUBLIC _main
EXTRN _scanf:PROC
EXTRN _printf:PROC
; Function compile flags: /Odtp
_TEXT SEGMENT
_x$ = -4 ; size = 4
_main PROC
	push 	ebp
	mov 	ebp, esp
	push 	ecx
	push 	OFFSET $SG3831 ; 'Enter X:'
	call 	_printf
	add 	esp, 4
	lea 	eax, DWORD PTR _x$[ebp]
	push 	eax
	push 	OFFSET $SG3832 ; '%d'
	call 	_scanf
	add 	esp, 8
	mov 	ecx, DWORD PTR _x$[ebp]
	push 	ecx
	push 	OFFSET $SG3833 ; 'You entered %d...'
	call 	_printf
	add 	esp, 8

	; return 0
	xor 	eax, eax
	mov 	esp, ebp
	pop 	ebp
	ret 	0
_main ENDP
_TEXT ENDS
```

`x` is a local variable. Traditionally, local variables are stored on the stack. There are probably other ways to allocate them, but in x86 that is the way it is.

The goal of the instruction following the function prologue, `push ecx`, is not to save the `ecx` state (notice the absence of corresponding `pop ecx` at the function's end).

In fact it allocates 4 bytes on the stack for storing the `x` variable. `x` is to be accessed with the assistance of the `_x$` macro (it equals to -4) and the `ebp` register pointing to the current frame.

Over the span of the function's exeuction, `ebp` is pointing to the current stack frame making it possible to access local variables and function arguments via `ebp+offset`.

It is also possible to use `esp` for the same purpose, although that is not very convenient since it changes frequently. The value of the `ebp` could be perceived as a frozen state of the value in `esp` at the start of the function's execution.

Here is a typical stack frame layout in 32-bit environment:

|         |                                    |
| ------- |:----------------------------------:|
| ...     | ...                                |
| EBP-8   | argument#2, marked in IDA as arg_8 |
| EBP-4   | argument#1, marked in IDA as arg_4 |
| EBP     | saved value of `EBP`               |
| EBP+4   | return address                     |
| EBP+8   | argument#1, marked in IDA as arg_0 |
| EBP+0xC | argument#2, marked in IDA as arg_4 |
| EBP+0x10| argument#3, marked in IDA as arg_8 |
| ...     | ...                                |

The `scanf()` function in our example has two arguments. The first one is pointer to the string containing `%d` and the second is the address of the `x` variable.

First, the `x` variable's address is loaded into the `eax` register by the `lea eax, DWORD PTR _x$[ebp]` instruction. `lea` stands for *load effective address*, and is often used for forming an address.

We could say that in this case `lea` simply stores the sum of the `ebp` register value and the `_x$` macro in the `eax` register. This is the same as `lea eax, [ebp-4]`.

Next the `eax` register value is pushed into the stack and `scanf()` is being called.

`printf()` is being called after that with its first argument - a pointer to the string: `You entered %d...\n`. The second argument is prepared with : `mov ecx, [ebp-4]`. The instruction stores the `x` variable value and not its address, in the `ecx` register.

Next the value in the `ecx` is stored on the stack and the last `printf()` is being called.

- GCC

Let's try to compile this code in GCC 4.4.1 under Linux:
```nasm
main 		proc near

var_20 		= dword ptr -20h
var_1C 		= dword ptr -1Ch
var_4 		= dword ptr -4

			push 	ebp
			mov 	ebp, esp
			and 	esp, 0FFFFFFF0h
			sub 	esp, 20h
			mov 	[esp+20h+var_20], offset aEnterX ; "Enter X:"
			call 	_puts
			mov 	eax, offset aD ; "%d"
			lea 	edx, [esp+20h+var_4]
			mov 	[esp+20h+var_1C], edx
			mov 	[esp+20h+var_20], eax
			call 	___isoc99_scanf
			mov 	edx, [esp+20h+var_4]
			mov 	eax, offset aYouEnteredD___ ; "You entered %d...\n"
			mov 	[esp+20h+var_1C], edx
			mov 	[esp+20h+var_20], eax
			call 	_printf
			mov 	eax, 0
			leave
			retn
main endp
```

GCC replaced the `printf()` call with call to `_puts()`.  As in the MSVC example - the arguments are placed on the stack using the `MOV` instruction.

### ARM

Optimizing Keil 6/2013 (Thumb mode)
```
.text:00000042 				scanf_main
.text:00000042
.text:00000042 				var_8 = -8
.text:00000042
.text:00000042 08 B5 		PUSH {R3,LR}
.text:00000044 A9 A0 		ADR R0, aEnterX ; "Enter X:\n"
.text:00000046 06 F0 D3 F8 	BL __2printf
.text:0000004A 69 46 		MOV R1, SP
.text:0000004C AA A0 		ADR R0, aD ; "%d"
.text:0000004E 06 F0 CD F8 	BL __0scanf
.text:00000052 00 99 		LDR R1, [SP,#8+var_8]
.text:00000054 A9 A0 		ADR R0, aYouEnteredD___ ; "You entered %d...\n"
.text:00000056 06 F0 CB F8 	BL __2printf
.text:0000005A 00 20 		MOVS R0, #0
.text:0000005C 08 BD 		POP {R3,PC}
```

In order for `scanf()` to be able to read item it needs a parameter - pointer to an int. int is 32-bit, so we need 4 bytes to store it somewhere in memory, and it fits exactly in a 32-bit register. A place for the local variable `x` is allocated in the stack and IDA has named it *var_8*. 

It is not necessary, however, to allocate a such since SP (stack pointer) is already pointing to that space and it can be used directly. So, SP'value is copied to the `R1` register and, with the format-string, passed to `scanf()`.

Later, with the help of the `LDR` instruction, this value is moved from the stack to the `R1` register in order to be passed to `printf()`.

### MIPS

A place in the local stack is allocated for the x variable, and it is to be referred as `$sp + 24`. Its address is passed to `scanf()`, and the user input values is loaded using the `LW` (“Load Word”) instruction and then passed to `printf()`.

Optimizing GCC 4.4.5(IDA)
```
.text:00000000 main:
.text:00000000
.text:00000000 var_18 = -0x18
.text:00000000 var_10 = -0x10
.text:00000000 var_4 = -4
.text:00000000
; function prologue:
.text:00000000 		lui $gp, (__gnu_local_gp >> 16)
.text:00000004 		addiu $sp, -0x28
.text:00000008 		la $gp, (__gnu_local_gp & 0xFFFF)
.text:0000000C 		sw $ra, 0x28+var_4($sp)
.text:00000010 		sw $gp, 0x28+var_18($sp)
; call puts():
.text:00000014 		lw $t9, (puts & 0xFFFF)($gp)
.text:00000018 		lui $a0, ($LC0 >> 16) # "Enter X:"
.text:0000001C 		jalr $t9
.text:00000020 		la $a0, ($LC0 & 0xFFFF) # "Enter X:" ; branch delay slot
; call scanf():
.text:00000024 		lw $gp, 0x28+var_18($sp)
.text:00000028 		lui $a0, ($LC1 >> 16) # "%d"
.text:0000002C 		lw $t9, (__isoc99_scanf & 0xFFFF)($gp)
; set 2nd argument of scanf(), $a1=$sp+24:
.text:00000030 		addiu $a1, $sp, 0x28+var_10
.text:00000034 		jalr $t9 ; branch delay slot
.text:00000038 		la $a0, ($LC1 & 0xFFFF) # "%d"
; call printf():
.text:0000003C 		lw $gp, 0x28+var_18($sp)
; set 2nd argument of printf(),
; load word at address $sp+24:
.text:00000040 		lw $a1, 0x28+var_10($sp)
.text:00000044 		lw $t9, (printf & 0xFFFF)($gp)
.text:00000048 		lui $a0, ($LC2 >> 16) # "You entered %d...\n"
.text:0000004C 		jalr $t9
.text:00000050 		la $a0, ($LC2 & 0xFFFF) # "You entered %d...\n" ; branch delay slot
; function epilogue:
.text:00000054 		lw $ra, 0x28+var_4($sp)
; set return value to 0:
.text:00000058 		move $v0, $zero
; return:
.text:0000005C 		jr $ra
.text:00000060 		addiu $sp, 0x28 ; branch delay slot
```

## scanf

As was noted before, it is slightly old-fashioned to use `scanf()` today. But if we have to, we have to check it `scanf()` finishes correctly without an error.
```c
#include <stdio.h>

int main()
{
	int x;
	printf ("Enter X:\n");
	if (scanf ("%d", &x)==1)
		printf ("You entered %d...\n", x);
	else
		printf ("What you entered? Huh?\n");
	return 0;
};
```

By standard, the `scanf()` function returns the number of fields it has successfully read. In our case, if everything goes fine and the user enters a number `scanf()` returns 1, or in case of error - 0.

Let's add some C code to check the `scanf()` return value and print error message in case of an error.
```sh
$ ./test 
Enter X:
123
You entered 123...
$ ./test 
Enter X:
what?
What you entered? Huh?
```

- Radare2: darwin-x86-64

Here is what we get in the assembly output:
```sh
/ (fcn) entry0 111
|   entry0 ();
|           ; var int local_14h @ rbp-0x14
|           ; var int local_10h @ rbp-0x10
|           ; var int local_ch @ rbp-0xc
|           ; var int local_8h @ rbp-0x8
|           ; var int local_4h @ rbp-0x4
|           0x100000ec0      55             push rbp                   ; section 0 va=0x100000ec0 pa=0x00000ec0 sz=111 vsz=111 rwx=m-r-x 0.__TEXT.__text
|           0x100000ec1      4889e5         mov rbp, rsp
|           0x100000ec4      4883ec20       sub rsp, 0x20
|           0x100000ec8      488d3d910000.  lea rdi, str.Enter_X:_n    ; section.3.__TEXT.__cstring ; 0x100000f60 ; "Enter X:\n"
|           0x100000ecf      c745fc000000.  mov dword [local_4h], 0
|           0x100000ed6      b000           mov al, 0
|           0x100000ed8      e853000000     call sym.imp.printf        ; int printf(const char *format)
|           0x100000edd      488d3d860000.  lea rdi, 0x100000f6a       ; "%d"
|           0x100000ee4      488d75f8       lea rsi, [local_8h]
|           0x100000ee8      8945f4         mov dword [local_ch], eax
|           0x100000eeb      b000           mov al, 0
|           0x100000eed      e844000000     call sym.imp.scanf         ; int scanf(const char *format)
|           0x100000ef2      3d01000000     cmp eax, 1
|       ,=< 0x100000ef7      0f8519000000   jne 0x100000f16
|       |   0x100000efd      488d3d690000.  lea rdi, str.You_entered__d..._n ; 0x100000f6d ; "You entered %d...\n"
|       |   0x100000f04      8b75f8         mov esi, dword [local_8h]
|       |   0x100000f07      b000           mov al, 0
|       |   0x100000f09      e822000000     call sym.imp.printf        ; int printf(const char *format)
|       |   0x100000f0e      8945f0         mov dword [local_10h], eax
|      ,==< 0x100000f11      e911000000     jmp 0x100000f27
|      |`-> 0x100000f16      488d3d630000.  lea rdi, str.What_you_entered__Huh__n ; 0x100000f80 ; "What you entered? Huh?\n"
|      |    0x100000f1d      b000           mov al, 0
|      |    0x100000f1f      e80c000000     call sym.imp.printf        ; int printf(const char *format)
|      |    0x100000f24      8945ec         mov dword [local_14h], eax
|      |       ; JMP XREF from 0x100000f11 (entry0)
|      `--> 0x100000f27      31c0           xor eax, eax
|           0x100000f29      4883c420       add rsp, 0x20
|           0x100000f2d      5d             pop rbp
\           0x100000f2e      c3             ret
```

The caller function (`main()`) needs the callee function (`scanf()`) result, so the callee returns it in the `eax` register (`mov al, 0`).
```
eax = [x x x x]  ax = [- - x x]     al = [- - - x]    ah = [- - x -]  
      31     0            15 0                 7 0            15 7      
```

We check it with the help of the intruction `cmp eax, 1`. In other words, we compare the value in the `eax` register with 1. A `jne` conditional jump follows the `cmp` instruction. `jne` stands for *jump if not equal*.

So, if the value in the `eax` register is not equal to 1, the CPU will pass the execution to the address mentioned in the `jne 0x100000f16` operand. Passing the control to this address results in the CPU executing `printf()` with the argument `What you entered? Huh?\n`. Buf if everything is fine, the conditional jump is not be taken, and another `printf()` call is to be executed, with two arguments: `You entered %d...\n` and the value of `x`,

Since in this case the second `printf()` has not to be executed,there is a `jmp` precding it (unconditional jump). It passes the control to the point after the second `printf()` and just before the `xor eax, eax` instruction, which implements `return 0`.

So, it could be said that comparing a value with another is usually implemented by `cmp`/`jcc` instruction pair, where *cc* is condition code. `jmp` compares two values and sets processor flags. `jcc` checks those flags and decides to either pass the control to the specified address or not.

All arithmetic instructions set processor flags, not just `cmp`. If we compare 1 and 1, 1 - 1 is 0 so the `zf` flag would be set (meaning that the last result is 0). In no other cirumstances `zf` can be set, except when the operands are equal. `jne` checks only the `zf` flag and jumps only if it is not set. `jne` is in fact a synonym for `jnz` (jump if not zero). Assembler translates both `jne` and `jnz` instructions into the same opcode. So, the `cmp` instruction can be replaced with a `sub` instruction and almost everyhing will be fine, with the difference that `sub` alters the value of the first operand. `cmp` is `sub` without saving the result, bu affecting flags.

## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)
