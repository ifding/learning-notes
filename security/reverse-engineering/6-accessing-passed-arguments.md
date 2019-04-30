
# 6. Accessing passed arguments

- [Simple example](#simple-example)
  - [x86-64](#x86-64)
- [More about results returning](#more-about-results-returning)
  - [returning void](#returning-void)
  - [What if we do not use the function result](#what-if-we-do-not-use-the-function-result)
  - [Returning a structure](#returning-a-structure)


## Simple example

Now we figured out that the caller function is passing arguments to the callee via the stack. But how does the callee access them?

```c
#include <stdio.h>

int f (int a, int b, int c)
{
	return a*b+c;
};

int main()
{
	printf ("%d\n", f(1, 2, 3));
	return 0;
};
```

### x86-64

Here is what we get from Radare2:
```sh
/ (fcn) entry0 64
|   entry0 ();
|           ; var int local_8h @ rbp-0x8
|           ; var int local_4h @ rbp-0x4
|           0x100000f30      55             push rbp
|           0x100000f31      4889e5         mov rbp, rsp
|           0x100000f34      4883ec10       sub rsp, 0x10
|           0x100000f38      bf01000000     mov edi, 1
|           0x100000f3d      be02000000     mov esi, 2
|           0x100000f42      ba03000000     mov edx, 3
|           0x100000f47      c745fc000000.  mov dword [local_4h], 0
|           0x100000f4e      e8bdffffff     call sym._f
|           0x100000f53      488d3d380000.  lea rdi, 0x100000f92       ; section.3.__TEXT.__cstring ; "%d\n"
|           0x100000f5a      89c6           mov esi, eax
|           0x100000f5c      b000           mov al, 0
|           0x100000f5e      e80d000000     call sym.imp.printf        ; int printf(const char *format)
|           0x100000f63      31d2           xor edx, edx
|           0x100000f65      8945f8         mov dword [local_8h], eax
|           0x100000f68      89d0           mov eax, edx
|           0x100000f6a      4883c410       add rsp, 0x10
|           0x100000f6e      5d             pop rbp
\           0x100000f6f      c3             ret

/ (fcn) sym._f 27
|   sym._f ();
|           ; var int local_ch @ rbp-0xc
|           ; var int local_8h @ rbp-0x8
|           ; var int local_4h @ rbp-0x4
|              ; CALL XREF from 0x100000f4e (entry0)
|           0x100000f10      55             push rbp                   ; section 0 va=0x100000f10 pa=0x00000f10 sz=96 vsz=96 rwx=m-r-x 0.__TEXT.__text
|           0x100000f11      4889e5         mov rbp, rsp
|           0x100000f14      897dfc         mov dword [local_4h], edi
|           0x100000f17      8975f8         mov dword [local_8h], esi
|           0x100000f1a      8955f4         mov dword [local_ch], edx
|           0x100000f1d      8b55fc         mov edx, dword [local_4h]
|           0x100000f20      0faf55f8       imul edx, dword [local_8h]
|           0x100000f24      0355f4         add edx, dword [local_ch]
|           0x100000f27      89d0           mov eax, edx
|           0x100000f29      5d             pop rbp
\           0x100000f2a      c3             ret
```

What we see is that the `entry0()` function pushes 3 numbers in the regisers: edi, esi, and edx, then calls f(int, int, int).

Argument access inside `sym._f ()`, it pushes 3 arguments onto the stack, with negative offsets. So, we are addressing the outside of the stack frame. The value of a (edi) is stored into local_4h,  b (esi) is in local_8h, c (edx) is in local_ch. Then the value of a is stored into `edx`.  After `imul` instruction execution, the value in `edx` is a product of the value of `edx` and the value of local_8h (b). After that, `add` adds the value in local_ch to `edx`. Then the value in `edx` need to be moved to `eax`, it is where it must be. On returning to caller, it takes the `eax` value and use it as an argument to `printf()`.

## More about results returning

In x86, the result of function execution is usually returned in the `eax` register. If it is byte or a character (char), then the lowest part of register `eax` (`al`) is used. If a function returns a float number, the FPU register `ST(0)` is used instead. In ARM, the result is usually returned in the `R0` register.

### returning void

So, what if the `main()` function return value was declared of type *void* and not *int*? The so-called startup-code is calling `main()` roughly as follows:
```nasm
push envp
push argv
push argc
call main
push eax
call exit
```

In other words:
```c
exit(main(argc,argv,envp));
```

If you declare `main()` as void, nothing is to be returned explicitly (using the `return` statement), then something random, that has been stored in the `EAX` register at the end of `main()` becomes the sole argument of the exit() function. Most likely, there will be a random value, left from your function execution, so the exit code of program is pseudorandom.

### What if we do not use the function result

`printf()` returns the count of characters successfully output, but the result of this function is rarely used in practice.

It is also possible to call a function whose essence is in returning a value, and not use it:
```c
int f()
{
	// skip first 3 random values:
	rand();
	rand();
	rand();
	// and use 4th:
	return rand();
};
```

The result of the rand() function is left in `eax`, in all four cases. But in the first 3 cases, the value in `eax` is just not used.

### Returning a structure

Let's go back to the fact that the return value is left in the `eax` register.

That's why old C compilers cannot create functions capable of returning something that does not fit in one register (usually int), but if one needs it, one have to return information via pointers passed as function's arguments.

So, ususally, if a function needs to return several values, it returns only one, and all the rest - via pointers.

Now it has become possible to return, say, an entire structure, but that is still not very popular. If a function has to return a large structure, the caller must allocate it and pass a pointer to it via the first argument, transparently for the programmer. That is almost the same as to pass a pointer in the first argument manually, but the compiler hides it.


## Reference

* [Reverse Engineering for Beginners By Dennis Yurichev](https://beginners.re/RE4B-EN.pdf)