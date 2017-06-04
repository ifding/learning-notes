
# 3. Stack

The stack is one of the most fundamental data structures. Technically, it is just a block of memory in process memory along with the `esp` or `rsp` register in x86 or x64, or the `sp` register in ARM, as a pointer within that block.

The most frequently used stack access instructions are `PUSH` and `POP` (in both x86 and ARM Thumb-mode). `PUSH` subtracts from `esp`/`rsp`/`sp` 4 in 32-bit mode (or 8 in 64-bit mode) and then writes the contents of its sole operand to the memory address pointed by `esp`/`rsp`/`sp`.

`POP` is the reverse operation: retrieve the data from the memeory location that `SP` points to, load it into the instruction operand (often a register) and then add 4 (or 8) to the `stack pointer`.

After stack allocation, the `stack pointer` points at the bottom of the stack. `PUSH` decreases the `stack pointer` and `POP` increases it. The bottom of the stack is actually at the beginning of the memory allocated for the stack block. It seems strange, but that's the way it is.

ARM supports both descending the ascending stacks. For example the `STMFD`/`LDMFD`, `STMED`/`LDMED` instructions are intended to deal with a descending stack (grows downwards, starting with a high address and progressing to a lower one). The `STMFA`/`LDMFA`, `STMEA`/`LDMEA` instructions are intended to deal with an ascending stack (grows upwards, starting from
a low address and progressing to a higher one).

## Why does the stack grow backwards?

The reason that the stack grows backward is probably historical. When the computers were big and occupied a whole room, it was easy to divide memory into two parts, one for the *heap* and one for the stack. Of course, it was unknown how big the *heap* and the *stack* would be during program execution, so this solution was the simplest possible.

## What is the stack used for?

### Save the function's return address

- x86

When calling another function with a `CALL` instruction, the address of the point exactly after the `CALL` instruction is saved to the stack and then an uncoditional jump to the address in the `CALL` operand is executed.

The `CALL` instruction is equivalent to a `PUSH address_after_call` / `JMP operand` instruction pair.

`RET` fetches a value from the stack and jumps to it - that is equivalent to a `POP tmp`/ `JMP tmp` instruction pair.

- ARM

ARM programs also use the stack for saving return addresses, but differently. As mentioned in "Hello, world!", the `RA` is saved to the `LR` (Link Register). If one needs, however, to call another function and use the `LR` register one more time, its value has to be saved. Usually it is saved in the function prologue.

Often, we see instructions like `PUSH R4-R7, LR` along with this instruction in epilogue `POP R4-R7, PC` - thus register values to be used in the function are saved in the stack, including `LR`.

Nevertheless, if a function never calls any other function, in `RISC` terminology it is called a `leaf function`. As a consequence, leaf functions do not save the `LR` register (because they don't modify it). If such function is small and uses a small number of registers, it may not use the stack at all. Thus, it is possible to call leaf functions without using the stack, which can be faster than on older x86 machines because external RAM is not used for the stack. This can be also useful for situations when memory for the stack is not yet allocated or not available.

### Passing function arguments

The most popular way to pass parameters in x86 is called "cdecl":
```nasm
push arg3
push arg2
push arg1
call f
add esp, 12 ; 4*3=12
```

Callee functions get their arguments via the stack pointer. This is how the argument values are located in the stack before the execution of the `f()` function's very first instruction:

| ------- |:----------------------------------:|
| ESP     | return address                     |
| ESP+4   | argument#1, marked in IDA as arg_0 |
| ESP+8   | argument#2, marked in IDA as arg_4 |
| ESP+0xC | argument#3, marked in IDA as arg_8 |
| ...     | ...                                |

By the way, the callee function does not have any information about how many arguments were passed. C functions with a variable number of arguments (like `printf()`) determine their number using format string specifiers (which begin with the % symbol).
```c
printf("%d %d %d\n", 1234);
```

`printf()` will print 1234, and then two random numbers, which were lying next to it in the stack.

That's why it is not very important how we declare the `main()` function: as `main()`, `main(int argc, char *argv[])` or `main(int argc, char *argv[], char *envp[])`.

In fact, the CRT-code is calling `main()` roughly as:
```nasm
push envp
push argv
push argc
call main
...
```

If you declare `main()` as `main()` without arguments, they are, nevertheless, still present in the stack, but are not used. If you declare `main()` as `main(int argc, char *argv[])`, you will be able to use first two arguments, and the third will remain "invisible" for your function. Even more, it is possible to declare `main(int argc)`, and it will work.

Alternative ways of passing arguments:
- via global variables,  drawback: function cannot call itself recursively and this is also not thread-safe (each thread would have its own stack with its own arguments/variables).
- MS-DOS had a way of passing all function arguments via resigters. It's very similar to call syscalls in Linux and Windows.

### Local variable storage

A function could allocate space in the stack for its local variables just by decreasing the `stack pointer` towards the stack bottom.

Hence, it's very fast, no matter how many local variables are defined. It is also not a requirement to store local variables in the stack. You could store local variables wherever you like, but traditionally this is how it's done.

- x86: alloca() function

It's worth nothing the `alloca()` function. This function works like `malloc()`, but allocates memory directly on the stack. The allocated memory chunk does not have to be freed via a `free()` function call, since the function epilogue returns `ESP` back to its initial state and the allocated memory is just dropped. It is worth nothing how `alloca()` is implemented. In simple terms, this function just shifts `ESP` downwards toward the stack bottom by the number of bytes you nedd and sets `ESP` as a pointer to the allocated block.

Let's try:
```c
#ifdef __GNUC__
#include <alloca.h> // GCC
#else
#include <malloc.h> // MSVC
#endif
#include <stdio.h>
void f()
{
	char *buf=(char*)alloca (600);
#ifdef __GNUC__
	snprintf (buf, 600, "hi! %d, %d, %d\n", 1, 2, 3); // GCC
#else
	_snprintf (buf, 600, "hi! %d, %d, %d\n", 1, 2, 3); // MSVC
#endif
	puts (buf);
};
```

`_snprintf()` function works just like `printf()`, but instead of dumping the result into *stdout* (e.g., to terminal or console), it writes it to the `buf` buffer. Function `puts()` copies the contents of `buf` to *stdout*. Of course, these two function calls might be replaced by one `printf()` call, but we have to illustrate small buffer usage.

- MSVC

Let's compile (MSVC 2010):
```nasm
mov 	eax, 600 ; 00000258H
call 	__alloca_probe_16
mov 	esi, esp
push 	3
push 	2
push 	1
push 	OFFSET $SG2672
push 	600 ; 00000258H
push 	esi
call 	__snprintf
push 	esi
call 	_puts
add 	esp, 28
```

The sole `alloca()` argument is passed via `EAX` (instead of pushing it into the stack).

- GCC + Intel syntax

GCC 4.4.1 does the same without calling external functions:
```nasm
.LC0:
		.string "hi! %d, %d, %d\n"
f:
		push 	ebp
		mov 	ebp, esp
		push 	ebx
		sub 	esp, 660
		lea 	ebx, [esp+39]
		and 	ebx, -16 ; align pointer by 16-bit border
		mov 	DWORD PTR [esp], ebx ; s
		mov 	DWORD PTR [esp+20], 3
		mov 	DWORD PTR [esp+16], 2
		mov 	DWORD PTR [esp+12], 1
		mov 	DWORD PTR [esp+8], OFFSET FLAT:.LC0 ; "hi! %d, %d, %d\n"
		mov 	DWORD PTR [esp+4], 600 ; maxlen
		call 	_snprintf
		mov 	DWORD PTR [esp], ebx ; s
		call 	puts
		mov 	ebx, DWORD PTR [ebp-4]
		leave
		ret
```

### A typical stack layout

A typical stack layout in a 32-bit environment at the start of a function, before the first instruction execution looks like this:

| ------- |:----------------------------------:|
| ...     | ...                                |
| ESP-0xc | argument#2, marked in IDA as arg_8 |
| ESP-8   | argument#1, marked in IDA as arg_4 |
| ESP-4   | saved value of `EBP`               |
| ESP     | return address                     |
| ESP+4   | argument#1, marked in IDA as arg_0 |
| ESP+8   | argument#2, marked in IDA as arg_4 |
| ESP+0xC | argument#3, marked in IDA as arg_8 |
| ...     | ...                                |

### Noise in stack

Where do "noise" or "garbage" values in the stack or memory come from? There are what has been left there after other functions' executions. Short example:
```c
#include <stdio.h>

void f1()
{
	int m=1, n=2;
};

void f2()
{
	int a, b, c;
	printf ("%d, %d, %d\n", a, b,c);
};

int main()
{
	f1();
	printf("hello, world\n");
	f2();
	return 0;
};
```

x86-64 gcc 6.3 Compiling, it will print: `1, 2, 3`. We did not send any variables in `f2()`. These are "ghosts" values, which are still in the stack.
```nasm
f1():
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], 1
        mov     DWORD PTR [rbp-8], 2
        mov     DWORD PTR [rbp-12], 3
        nop
        pop     rbp
        ret
.LC0:
        .string "%d, %d, %d\n"
f2():
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     ecx, DWORD PTR [rbp-12]
        mov     edx, DWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rbp-4]
        mov     esi, eax
        mov     edi, OFFSET FLAT:.LC0
        mov     eax, 0
        call    printf
        nop
        leave
        ret
main:
        push    rbp
        mov     rbp, rsp
        call    f1()
        call    f2()
        mov     eax, 0
        pop     rbp
        ret
```

When `f1 ()` assigns the variables *a*, *b* and *c*, their values are stored at the address `0x1FF860` and so on. 

And when `f2()` executes, *a*, *b* and *c* of `f2()` are located at the same addresses! No one has overwritten the values yet, so at that point they are still untouched. So, for this weird situation to occur, several functions have to be called one after another and `SP` has to be the same at each function entry (i.e., they have the same number of arguments). Then the local variables will be located at the same positions in the stack.

Summarizing, all values in the stack (and memory cells in general) have values left there from previous function executions. They are not random in the strict sense, but rather have unpredictable values. It would probably be possible to clear portions of the stack before each function execution, but that's too much extra (and unnecessary) work.

## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)
