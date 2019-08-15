
# Improper Use of Pointers

- [Pointer Declaration and Initialization](#pointer-declaration-and-initialization)
  - [Improper pointer declaration](#improper-pointer-declaration)
  - [Failure to initialize a pointer](#failure-to-initialize-a-pointer)
- [Improper pointer usage](#improper-pointer-usage)
  - [Misuse of the dereference operator](#misuse-of-the-dereference-operator)
  - [Dangling Pointers](#dangling-pointers)
  - [Accessing memory outside the bounds of an Array](#accessing-memory-outside-the-bounds-of-an-array)
  - [Incorrect usage of array size](#incorrect-usage-of-array-size)
  - [Always match pointer types](#always-match-pointer-types)
  - [Bounded Pointers](#bounded-pointers)
  - [Pointer arithmetic and structures](#pointer-arithmetic-and-structures)
- [Dealocation problems](#dealocation-problems)
  - [Clearing sensitive data](#clearing-sensitive-data)
  

The _Address Space Layout Randomization_ (ASLR) process arranges an application's data region randomly in memory. These data regions include the code, stack, and heap. Randomizing the placement of these regions makes it more difficult for attackers to predict where memory will be placed and thus more difficult to use them. Certain types of attacks, such as the _return-to-libc_ attack, overwrite portions of the stack and transfer control to this region. This area is frequently the shared C library, libc. If the location of the stack and libc are not known, then such attacks will be less likely to succeed.

The _Data Execution Prevention_ (DEP) technique prevents the execution of code if it is in a nonexecutable region of memory. In some types of attacks, a region of memory is overwritten with a malicious code and then control is transferred to it. If this region of code is nonexecutable, such as the stack of heap, then it is prevented from executing. This technique can be implemented either in hardware or in software.

## Pointer Declaration and Initialization

### Improper pointer declaration

Consider the following declaration:
```c
int* ptr1, ptr2;

//a pointer to an integer is wrapped in a define directive and then used to declare variables
#define PINT int*
PINT ptr1, ptr2;
```

However, only *ptr1* is a pointer. The correct approach is to declare them both as pointers using a single line:
```c
int *ptr1, *ptr2;

//using a type definition, both variables are delcared as pointers to integers
typedef int* PINT;
PINT ptr1, ptr2;
```

### Failure to initialize a pointer

Using a pointer before it is initialized can result in a run-time error. This is sometimes referred to as a _wild pointer_.

Three approaches are used to deal with uninitialized pointers:

- Always initialize a pointer with NULL

Initializing a pointer to NULL will make it easier to check for proper usage. Even then, checking for a null value can be tedious, as shown below:
```c
int *pi = NULL;
...
if(pi == NULL) {
  // pi should not be dereferenced
} else {
  // pi can be used
}
```

- Use the assert function

If the expression is true, then nothing happens. The assert function is found in the _assert.h_ header file.
```c
assert(pi != NULL);
```

- Use third-party tools

## Improper pointer usage

Many security issues revolve around the concept of a buffer overflow.

**Buffer overflow** occurs when memory outside the object's bounds is overwritten. This memory may be part of the program's address space or another process. When the memory is outside of the program address space, most OS will issue a segmentation fault and terminate the program. Termination for this reason constitutes a denial of service attack when donw maliciously. This type of attack does not attempt to gain unauthorized access but tries to take down the application and potentially a server.

If the buffer overflow occurs within the application's address space, then it can result in unauthorized access to data and/or the transfer of control to another segment of code, thereby potentially compromising the system. This is of particular concern if the application is executing with supervisor privileges.

Buffer overflow can happen by:
- Not checking the index values used when accessing an array's elements
- Not being careful when performing pointer arithmetic with array pointers
- Using functions such as _gets_ to read in a string from standard input
- Using functions such as _strcpy_ and _strcat_ improperly

When buffer overflow occurs with a stack frame element, it is possible to overwrite the return address portion of the stack frame with a call to malicious code created at the same time. When the function returns, it will transfer control to the malicious function. This function can then perform any operation, restrained only by the current user's privilege level.

### Misuse of the dereference operator

A common approach for declaring and initializing a pointer is shown below:
```c
int num;
int *pi = &num;
```

Another seemingly equivalent declaration sequences follows:
```c
int num;
int *pi;
*pi = &num;
```

However, this is not correct. Notice the use of the dereference operator on the last line. We are attempting to assign the address of num not to pi but rather to the memory location specified by the contents of pi. The pointer, pi, has not been initialized yet. We have made a simple mistake of misusing the dereference operator. The correct sequence follows:
```c
int num;
int *pi;
pi = &num;
```

In the original declaration, `int *pi = &num`, th asterisk declared the variable to be a pointer. It was not used as the dereference operator.

### Dangling Pointers

A dangling pointer occurs when a pointer is freed but still references that memory. This problem is described in details in [here](https://github.com/ifding/awesome-notes/blob/master/language/c/dynamic-memory-management-in-c.md#dangling-pointers). If an attempt is made to access this memory later, then its contents may well have changed. A write operation against this memory, and a read operation may return invalid data. Either could potentially result in the termination of the program.

### Accessing memory outside the bounds of an Array

Nothing can prevent a program from accessing memory outside of the space allocated for an array. The arrays are assumed to be allocated in consecutive memory locations.
```c
char firstName[8] = "1234567";
char middleName[8] = "1234567";
char lastName[8] = "1234567";

middleName[-2] = 'X';
middleName[0] = 'X';
middleName[10] = 'X';

printf("%p %s\n",firstName,firstName);
printf("%p %s\n",middleName,middleName);
printf("%p %s\n",lastName,lastName);
```

To illustrate how memory is overwritten, three arrays are initialized to a simple sequence of numbers. While the behavior of the program will vary by compiler and machine, this will normally execute and overwrite characters in *firstName* and *lastName*.

![](./images/using-invalid-array-indexes.png)

### Incorrect usage of array size

When passing an array to a function, always pass the size of the array at the same time. This information will help the function avoid exceeding the bounds of the array.
```c
char name[8];
strcpy(name,"Alexander");
```

Functions like *strcpy* that do not pass the buffer's size should be used with caution.

An example of misusing the *sizeof* operator occurs when we attempt to check our pointer bounds but do it incorrectly.
```c
int buffer[20];
int *pbuffer = buffer;
for(int i=0; i<sizeof(buffer); i++) {
  *(pbuffer++) = 0;
}
```

However, the sizeof(buffer) expression returns 80 since the size of the buffer in bytes is 80 (20 multiplied by 4 byte elements). Avoid this by using the expression *sizeof(buffer)/sizeof(int)* in the test condition of the for statement.

### Always match pointer types

It is a good idea to always use the appropriate pointer type for the data. 
```c
 int num = 2147483647;
 int *pi = &num;
 short *ps = (short*)pi;
 printf("pi: %p Value(16): %x Value(10): %d\n", pi, *pi, *pi);
 printf("ps: %p Value(16): %hx Value(10): %hd\n",
         ps, (unsigned short)*ps, (unsigned short)*ps);
```

The output of the snippet follows:
```
pi: 100 Value(16): 7fffffff Value(10): 2147483647
ps: 100 Value(16): ffff Value(10): -1
```

This apparent contradiction is an artifact of executing this sequence on a little endian machine.
```
 +-------------------+
 | ff | ff | ff | 7f |  
 +-------------------+
  100  101  102  103
```

If we treat this as a short number and only use the first two bytes, then we get the short value of –1. If we treat this as an integer and use all four bytes, then we get 2,147,483,647.

### Bounded Pointers

The term *bounded pointers* describes pointers whose use is restricted to only valid regions. C does not provide any direct support for this approach. However, it can be enforced explicitly by the programmer, as shown below:
```c
 #define SIZE 32
 char name[SIZE];
 char *p = name;
 if(name != NULL) {
   if(p >= name && p < name+SIZE) {
   // Valid pointer - continue
   } else {
   // Invalid pointer - error condition
   }
 }
 ```

This approach can get tedious. An interesting variation is to create a pointer validation function.

### Pointer arithmetic and structures

Pointer arithmetic should only be used with arrays. Because arrays are guaranteed to be allocated in a contiguous block of memory, pointer arithmetic will result in a valid offset. However, they should not be used within structures, as the structure's fields may not be allocated in consecutive regions of memory.

This is illustrated with the following structure.
```c
//since the integer will be aligned on a four-byte boundary, there will be a gap between the two fields
typedef struct _employee {
  char name[10];
  int age;
} Employee;

Employee employee;
// Initialize eployee
char *ptr = employee.name;
ptr += sizeof(employee.name);  //try to use a pointer to access the age field
```

The pointer will contain the addres 110, which is the address of the two bytes found between the two fields.
```
         +-----------+
name 100 |  |  |  |  | 
         +-----------+
     104 |  |  |  |  | 
         +-----------+
     108 |  |  | gap | 
         +-----------+
 age 112 |  |  |  |  |  
         +-----------+
```

Improperly aligned pointers can result in an abnormal program termination or retrieval of bad data. In addition, slower pointer access is possible if the compiler is required to generate additional machine code to compensate for the improper alignment.

Even if the memory within a structure is contiguous, it is not a good pratice to use pointer arithmetic with the structure's fields.
```c
typedef struct _item {
  int partNumber;
  int quantity;
  int binNumber;
}Item;
```

The following code sequence declares a part and then uses pointer arithmetic to access each field:
```c
Item part = {12345, 35, 107};
int *pi = &part.partNumber;
printf("Part number: %d\n",*pi);
pi++;
printf("Quantity: %d\n",*pi);
pi++;
printf("Bin number: %d\n",*pi);
```

Normally, the output will be as expected, but it is not guaranteed to work. A better approach is as shown below:
```c
printf("Part number: %d\n",part.partNumber);
printf("Quantity: %d\n",part.quantity);
printf("Bin number: %d\n",part.binNumber);
```

## Dealocation problems

### Clearing sensitive data 

It is a good idea to overwrite senstitive data in memory once it is no longer needed. When your application terminates, most operating systems do not zero out or otherwise manipulate the memory used by your application. Your old space may be allocated to another program, which will have access to its contents.
```c
char name[32];
int userID;
char *securityQuestion;

// assign values
...

// Delete sensitive information
memset(name,0,sizeof(name));
userID = 0;
memset(securityQuestion,0,strlen(securityQuestion));
```

If name has been declared as a pointer, then we should clear its memory before we deallocate it, as shown below:
```c
char *name = (char*)malloc(...);
...
memset(name,0,sizeof(name));
free(name);
```

## Reference

* Reese, Richard. Understanding and Using C pointers. " O'Reilly Media, Inc.", 2013.