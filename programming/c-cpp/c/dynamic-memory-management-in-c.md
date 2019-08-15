
# Dynamic memory management in c

A C program executes within a *runtime systm*. This is typically the environment provided by an operating system. The runtime system supports the stack and heap along with other program behavior.

The basci steps used for dynamic memory allocation in C are:
1. Use a *malloc* type function to allocate memory
2. Use this memory to support the application
3. Deallocate the memory using the *free* function

For example:
```c
int *pi = (int*) malloc(sizeof(int)); //returns a pointer to memory allocated from the heap
*pi = 2;
printf("pi: %d\n", *pi);
free(pi);
```

```
        +---------------+
	|   500   2     | Heap
	|         ^     |
	|         |     |
	| +----------+  |
   main | |pi    500 |  | Stack
	| +----------+  |
	+---------------+
```

### Memory Leaks

A memory leak occurs when allocated memory is never used again is not freed. This can happen when:
- The memory's address is lost
- The free function is never invoked though it should be.

An example of losing the address of memory is illustrated in the following code sequence where pi is reassigned a new address. The address of the first allocation of memory is lost when pi is allocated memory a second time.
```c
int *pi = (int*) malloc(sizeof(int));
*pi = 2;
...
pi = (int*) malloc(sizeof(int));
```

Another example allocates memory for a string, initializes it, and then displays the string char by char.
```c
 char *name = (char*)malloc(strlen("Susan")+1);
 strcpy(name,"Susan");
 while(*name != 0) {
   printf("%c",*name);
   name++;
 }
```

```
     +---------------+
     | 600 Susan0    | Heap
     |          ^    |
     |          |    |
     | +----------+  |
 main| |pi    600 |  | Stack
     | +----------+  |
     +---------------+
```

It increments name by one with each loop iteration. At the end, name is left pointing to the string's NUL termination character, the allocated memory's starting address has been lost.

Memory leaks can also occur when the program should release memory but does not. This is frequently the result of programmer oversight.

Memory leaks can also occur when freeing structures created using the *struct* keyword. If the structure contains pointers to dynamically allocated memory, then these pointers may need to be freed before the structure is freed.

### Dynamic memory allocation functions

- malloc   allocates memory from the heap
- realloc  reallocates memory to a larger or smaller amount based on a previously allocated block of memory
- calloc   allocates and zeros out memory from the heap
- free     returns a block of memory to the heap

#### malloc function

The following shows a typical use of the malloc function:
```c
int *pi = (int*) malloc(sizeof(int));

//since the malloc functions may return a NULL value if it is unable to allocate memory
if(pi != NULL) {
  // pointer should be good
} else {
  // bad pointer
}

//you cannot use a fcuntion call when initializing a static or global variable
static int *pi = malloc(sizeof(int)); //generate a compile-time error message
```

The following steps are performed when the malloc function is executed:
1. Memory is allocated from the heap
2. The memory is not modified or otherwise cleared
3. The first byte's address is returned

Because the malloc function's prototype follows:
```c
void* malloc(size_t);
```

Before the pointer to void was introduced to C, explicit casts were required with malloc to stop the generation of warnings when assignments were made between incompatible pointer types. Since a pointer to void can be assigned to any other pointer type, explicit casting is no longer required.

#### calloc function

The calloc function will allocate and clear memory at the same time. Its prototype follows:
```c
//the memory allocated by the product of the numElements and elementSize.
void *calloc(size_t numElements, size_t elementSize);
```

To clear memory means its contents are set to all binary zeros. If either numElements or elementSize is zero, then a null pointer may be returned.
```c
//pi is allocatd a total of 20 bytes, all containing zeros
int *pi = calloc(5,sizeof(int));

//malloc function along with memset functiosn can be used to achieve the same results
int *pi = malloc(5 * sizeof(int));
memset(pi, 0, 5*sizeof(int));

//however, the execution of calloc may take longer than using malloc
```

#### realloc function

The realloc function will reallocate memory. Its prototype follows:
```c
void *realloc(void *ptr, size_t size);
```

It returns a pointer to a block of memory. This function takes two arguments. The first is a pointer to the original block, and the second is the requested size. The requested size may be smaller or larger than the currently allocated amount.

If the first parameter is null, second parameter is NA, same as malloc.

If the first parameter is Not null,
- second parameter is 0, original block is freed.
- second parameter: less than the original block's size, then the excess memory is returned to the heap. There is no guarantee that the excess memory will be cleared.
- second parameter: larger than the original block's size, then if possible, the memory will be allocated from the region immediately following the current allocation. Otherwise, memory is allocated from a different region of the heap and the old memory is copied to the new region.

#### alloca function and Variable Length Arrays

The alloca function allocates memory by placing it in the stack frame for the function. When the function returns, the memory is automatically freed. This function can be difficult to implement if the underlying runtime system is not stack-based. As a result, this function is nonstandard and should be avoided if the application needs to be portable.

In C99, Variable Length Arrays (VLAs) were introduced, allowing the declaration and creation of an array within a function whose size is based on a variable. For example,
```c
void init(int size) {
  char* buffer[size];
  ...
}
```

This means the allocation of memory is done at runtime and memory is allocated as part of the stack frame. Also, when the sizeof operator is used with the array, it will be executed at runtime rather than compile time.

A small runtime penalty will be imposed. Also, when the function exits, the memory is effectively deallocated. Since we did not use a malloc type function to create it, we should not use the free function to deallocate it. The function should not return a pointer  to this memory either.

#### free function

This is normally performed using the free function, whose prototype is shown below:
```c
void free(void *ptr);
```

The pointer argument should contain the address of memory allocated by a malloc type function. This memory is returned to the heap. While the pointer may still point to the region, always assume it points to garbage. This region may be reallocated later and populated with different data.

If the pointer passed has been allocated by other than a malloc type function, then the function's behavior is undefined.

Assigning NULL to a freed pointer:
```c
 int *pi = (int*) malloc(sizeof(int));
 ...
 free(pi);
 pi = NULL; //result in a runtime execption, not good.
```

Double free:
```c
 p1 = (int*) malloc(sizeof(int));
 int *p2 = p1; //two pointers reference the same location, it is referred to as aliasing.
 free(p1);
 ...
 free(p2);
```

Unfortunately, heap managers have a difficult time determining whether a block has already been deallocated. Thus, they don't attempt to detect the same memory being freed twice. This normally results in a corrupt heap and program termination.

#### The heap and system memory

The heap typically uses operating system functions to manage its memory. The heap's size may be fixed when the program is created, or it may be allowed to grow. However, the heap manager does not necessarily return memory to the operating system when the free function is called. The deallocated memory is simply made avaiable for subsequent use by the application.

Thus, when a program allocates and then frees up memory, the deallocation of memory is not normally reflected in the application's memory usage as seen from the operating system perspective.

It is always a good habit to free memory after it is no longer needed, even if the application is terminating. If you use a tool to detect memory leaks or similar problems, then deallocating memory will clean up the output of such tools.

### Dangling pointers

If a pointer still references the original memory after it has been freed, it is called a dangling pointer. It can result in a number of different types of problems:
- Unpredictable behavior if the memory is accessed
- Segmentation faults when the memory is no longer accessible
- Potential security risks

```c
 int *p1 = (int*) malloc(sizeof(int));
 *p1 = 3;
 ...
 int *p2;
 p2 = p1;
 ...
 free(p1);
 ...
 *p2 = 8; // Dangling pointer
```

A subtle problem can occur when using block statements, as shown below:
```c
int *pi; //may be a global variable or a local variable
...
{
  int tmp = 5;
  pi = &tmp;
} //when tmp's enclosing block is popped off of the program stack, the address is no longer valid
// pi is now a dangling pointer
foo();
```

Most compilers will treat a block statement as a stack frame. The variable tmp was allocated on the stack frame and subsequently popped off the stack when the block statement was exited. The pointer pi is now left pointing to a region of memory that may eventually be overridden by a different activation record, such as the function foo.

Several approaches exit for dealing with dangling pointers, including:
- Writing special functions to replace the free function.
- Some systems (runtime/debugger) will overwrite data when it is freed (e.g., 0xDEADBEFF).
- Use third-party tools to detect dangling pointers and other problems.
- Displaying pointer values can be helpful in debugging dangling pointers.

## Reference

* Reese, Richard. Understanding and Using C pointers. " O'Reilly Media, Inc.", 2013.
