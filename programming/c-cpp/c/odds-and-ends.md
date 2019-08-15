
# Odds and Ends

Pointers are vital to almost all aspects of C. This post examines several topics related to pointers:
- [Casting pointers](#casting-pointers)
  - [Accessing a special purpose address](#accessing-a-special-purpose-address)
  - [Accessing a port](#accessing-a-port)
  - [Accessing memory using DMA](#accessing-memory-using-dma)
  - [Determining the endianness](#determining-the-endianness)
- [Aliasing and strict aliasing](#aliasing-and-strict-aliasing)
  - [Using a union](#using-a-union)
  - [Strict aliasing](#strict-aliasing)
  - [Using the restrict keyword](#using-the-restrict-keyword)
- [Threads and pointers](#threads-and-pointers)
  - [Sharing pointers between threads](#sharing-pointers-between-threads)
  - [Using function pointers to support callbacks](#using-function-pointers-to-support-callbacks)
  

With regards to threads, there are two areas of interest. The first deals with the basic problem of sharing data between threads using pointers. The second discusses how pointers are used to support callbacks. An operation may invoke a function to perform a task. When the actual function called changes, this is referred to as a callback function. A callback is also used to communicate between threads.

## Casting pointers

Casting is a basic operator that can be quite useful when used with pointers. Casting pointers are useful for a number of reasons, including:
- Accessing a special purpose address
- Assigning an address to represent a port
- Determining a machine's endianness

### Accessing a special purpose address

For some situations, such as when we need to address memory location zero, we may
need to cast a pointer to an integer and then cast it back to a pointer. This is more
common on older systems where a pointer’s size is the same size as an integer. However,
this does not always work. The approach is illustrated below, where the output is
implementation-dependent:
```c
pi = &num;
printf("Before: %p\n",pi);
int tmp = (int)pi;
pi = (int*)tmp;
printf("After: %p\n",pi);
```

Casting a pointer to an integer and then back to a pointer has never been considered
good practice. 

Remember that casting to and from an integer is different from casting to and from
void.

The term *handle* is sometimes confused with a pointer. A handle is a reference to a system resource. Access to the resource is provided through the handle. However, the handle generally does not provide direct access to the resource. In constrast, a pointer contains the resource's address.

The need to access a special purpose address often occurs on embedded systems where there is minimal operating system mediation. For example, in some low-level OS kernels the address of video RAM for a PC is 0xB8000. This address holds the character to be displayed in the first row and first column when in text mode. We can assign this address to a pointer and then assign a character to the location.
```c
#define VIDEO_BASE 0xB8000
int *video = (int *) VIDEO_BASE;
*video = 'A';
```

```
         +-------+
     100 |  ...  | 
         +-------+
vido 104 |0xB8000| 
         +-------+
     108 |  ...  | 
         +-------+
     112 |  ...  |  
         +-------+
         ...  
         +-------+
 0xB8000 |  A    |   <---- Video Memory
         +-------+
 0xB8004 |  ...  | 
         +-------+
 0xB8008 |  ...  | 
         +-------+
 0xB800C |  ...  |  
         +-------+
```

If appropriate, the address can also be read. This is not typically done for video memory.

When you need to address memory at location zero, sometimes the compiler will treat it as a NULL pointer value. Access to location zero is often needed in low-level kernel programs. Here are a few techniques to address this situcation:
- Set the pointer to zero (this does not always work)
- Assign a zero to an integer and then cast the integer to the pointer
- Use the *memset* function to assign a zero to the pointer

The memory referenced by ptr is set to all zeros:
```c
memset((void*)&ptr, 0, sizeof(ptr));
```

### Accessing a port

A port is both a hardware and a software concept. Servers use software ports to indicate they should receive certain messages sent to the machine. A hardware port is typically a physical input/output system component connected to an external device. By either reading or writing to a hardware port, information and commands can be processed by the program.

The following illustrates the use of pointers to access a port:
```c
#define PORT 0xB0000000
unsigned int volatile * const port = (unsigned int *) PORT;
```

The machine uses the hexadecimal value address to designate a port. The data is treated as an unsigned integer. The *volatile* keyword qualifier indicates that the variable can be changed outside of the program. For example, an external device may write data to a port. This write operation is performed independent of the computer's processor. Compilers will sometimes termporarily use a cache, or register, to hold the value in a memory location for optimizaton purposes. If the external write modifies the memory location, then this change will not be reflected in the cached or register value.

Using the volatile keyword will prevent the runtime system from using a register to temporarily store the port value. Each port access requires the system to read or write to the port instead of reading a possibly stale value stored in a register. We don't want to declare all variables as volatile, as this will prevent the compiler from performing certain types of optimizations.

The application can then read or write to the port by dereferencing the port pointer as follows. The External Device can read/write to the memory at 0xB0000000:
```c
*port = 0x0BF4; // write to the port
value = *port; // read from the port
```

```
	         +-----------+
	     100 |    ...    | 
	         +-----------+
	port 104 |0xB80000000| 
	         +-----------+
	     108 |    ...    | 
	         +-----------+
	     112 |    ...    |  
	         +-----------+
	         ...  
	         +-----------+
      0xB0000000 |  0x0BF4   |   <----> External Device
	         +-----------+
      0xB0000004 |    ...    | 
	         +-----------+
```

It is not a good idea to access volatile memory with a nonvolatile variable. Using such a variable can result in undefined behavior.

### Accessing memory using DMA

*Direct Memory Access* (DMA) is a low-level operation that assists in transferring data between main memory and some device. It is not part of the ANSI C specification but operating systems typically provide support for this operation. DMA operations are normally conducted in parallel with the CPU. This frees up the CPU for other processing and can result in better performance.

The programmer will invoke a DMA function and then wait for the operation's completion. Often, a callback function is provided by the programmer. When the operation completes, the callback function is invoked by the operation system.

### Determining the endianness

The cast operator can also be used to determine the endianness of architecture. Endian refers to the order of bytes in a unit of memory. For example, for a four-byte representation of an integer using little endian ordering, the integer's least significant byte is stored in the lowest address of the four bytes.
```c
  int num = 0x12345678;
  char *pc = (char*) &num;
  for (int i = 0; i < 4; ++i)
  {
      printf("%p: %02x \n", pc, (unsigned char) *pc++);
  }
``` 

The output of this code snippet as executed on an Intel PC reflects a little-endian architecture.
```
100: 78
101: 56
102: 34
103: 12
```

Little endian means the low-order bytes are stored in the lowest address, while big endian means the high-order bytes are stored at the lowest address. 

## Aliasing and strict aliasing

One pointer is said to alias another pointer if they both reference the same memory location.
```c
int num = 5;
int* p1 = &num;
int* p2 = &num;
```

When the compiler generates code for pointers, it has to assume that aliasing many occur unless told otherwise. The use of aliasing imposes restrictions on compiler-generated code. If two pointers reference the same location, either can potentially modify that location. 

When the compiler generates code to read or write to that location, it is not always able to optimize the code by storing the value in a register. It is forced to perform machine-level load and store operations with each reference. The repeated load/store sequence can be inefficient. In some situations, the compiler must also be concerned about the order in which the operations are performed.

*Strict aliasing* is another form of aliasing. Strict aliasing does not allow a pointer of onew data type to alias a pointer of a different data type. Strict aliasing does not apply to pointers differing only by sign or qualifier. The following are all valid strict aliase:
```c
int num;
const int *ptr1 = &num;
int *ptr2 = &num;
int volatile ptr3 = &num;
``` 

To avoid aliasing problems, several techniques are available:
- Disable strict aliasing
- Use a pointer to char
- Use a union

The GCC compiler has the following compiler options:
- `-fno-strict-aliasing` to turn it off
- `-fstrict-aliasing` to turn it on
- `-Wstrict-aliasing` to warn of strict aliasing-related problems

Code requiring strict aliasing to be turned off probably reflects poor memory access
practices. When possible, take time to resolve these issues instead of turning off strict
aliasing.

A pointer to char is always assumed to potentially alias any object. Thus, it can be used safely in most situations. However, casting a pointer to one data type to a pointer to char and then casting the pointer to char to a second pointer to char to a second pointer data type will result in undefined behavior and should be avoided.

### Using a union

C is a typed language. When a variable is declared, a type is assigned to it. Multiple variables can exist with different types. At times, it may be desirable to convert one type to another type. This is normally achieved with casting but can also be performed using a union. The term *type punning* describes the technique used to subvert the type system.

When the conversion involves pointers, serious problems can result. To illustrate this technique, we will use three different functions. These will determine whether a floating point number is positive.

The first function shown below uses a union of a float and an unsigned integer. The function first assigns the floating point value to the union and then extracts the integer to perform the test:
```c
typedef union _conversion {
  float fNum;
  unsigned int uiNum;
} Conversion;

int isPositive1(float number) {
  Conversion conversion = { .fNum =number};
 return (conversion.uiNum & 0x80000000) == 0;
}
```

This will work correctly and does not involve aliasing because no pointers are involved. The next version uses a union that contains pointers to the two data types. The floating pointer number's address is assigned to the first pointer. The integer's pointer is then dereferenced to perform the test. This violates the strict aliasing rule:
```c
typedef union _conversion2 {
  float *fNum;
  unsigned int *uiNum;
} Conversion2;

int isPositive2(float number) {
  Conversion2 conversion;
  conversion.fNum =&number;
  return (*conversion.uiNum & 0x80000000) == 0;
}
```

The following function does not use a union and violates the strict aliasing rule since the ptrValue pointer shares the same address as number:
```c
int isPositive3(float number) {
  unsigned int *ptrValue = (unsigned int *)&number;
  return (*ptrValue & 0x80000000) == 0;
}
```

### Strict aliasing

A compiler does not enforce strict aliasing. It will only generate warnings. The compiler assumes that two or more pointers of different types will never reference the same object. This includes pointers to structures with different names but that are otherwise identical. With strict aliasing, the compiler is able to perform certain optimizations.

Even if two structures have the same field but different names, two pointers to these structures should never reference the same object. In the following example, it is assumed the *person* and *employee* pointers will never reference the same object:
```c
typedef struct _person {
  char* firstName;
  char* lastName;
  unsigned int age;
} Person;

typedef struct _employee {
  char* firstName;
  char* lastName;
  unsigned int age;
} Employee;

Person* person;
Employee* employee;
```

However, the pointers can reference the same object if the structure definitions differ
only by their name, as illustrated below:
```c
typedef struct _person {
  char* firstName;
  char* lastName;
  unsigned int age;
} Person;

typedef Person Employee;
Person* person;
Employee* employee;
```

### Using the restrict keyword

C compilers assume pointers are aliasing by default. Using the *restrict* keyword when declaring a pointer tells the compiler that the pointer is not aliased. This allows the compiler to generate more efficient code.

The following function illustrates the definition and use of the *restirct* keyword. The function adds two vectors together and stores the result in the first vector:
```c
void add(int size, double * restrict arr1, const double * restrict arr2) {
  for (int i = 0; i < size; i++) {
  arr1[i] += arr2[i];
  }
}
```

The *restrict* keyword is used with both array parameters, but they should not both reference the same block of memory.
```c
double vector1[] = {1.1, 2.2, 3.3, 4.4};
double vector2[] = {1.1, 2.2, 3.3, 4.4};

add(4,vector1,vector2);

double *vector3 = vector1;
add(4, vector1, vector3);    //the function is called improperly, it uses an alias
```

Several standard C functions use the *restrict* keyword, including:
```
void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
char *strcpy(char * restrict s1, const char * restrict s2);
char *strncpy(char * restrict s1, const char * restrict s2, size_t n);
int printf(const char * restrict format, ... );
int sprintf(char * restrict s, const char * restrict format, ... );
int snprintf(char * restrict s, size_t n, const char * restrict format, ... );
int scanf(const char * restrict format, ...);
```

The *restrict* keyword has two implications:
- To the compiler it means it can perfrom certain code optimizations
- To the programmer it means these pointers should not be aliased; otherwise, the results of the operation are undefined.

## Threads and pointers

When threads share data, numerous problems can occur. One common problem is the corruption of data. One threads may write to an object but the thread may be suspended momentarily, leaving that object in an inconsistent state. Subsequently, a second thread may read that object before the first thread is able to resume. The second thread is now using an invalid or corrupted object.

Since pointers are a common way of referencing data in another thread, mutexes are frequently used to protect data. We will use Portable Operating System Interface (POSIX) threads since they are readily available. 

### Sharing pointers between threads

We will implement a multi-threaded function that computes the dot product of two vectors. The multiple threads will simultaneously access two vectors and a sum field. When the threads complete, the sum field will hold the dot product value. The dot product of two vectors is computed by summing the product of the corresponding elements of each vector. We will use two data structures in support of the operation.

The first one, *VectorInfo*, contains information about the two vectors being manipulated.
```c
typedef struct {
  double *vectorA;   //two vectors being manupulated
  double *vectorB;
  double sum;       //hold the dot product
  int length;       //specify the vector segment's size used by the dot product function
} VectorInfo;
```

The second data structure, *Product*, contains a pointer to a *VectorInfo* instance and the beginning index the dot Product vector will use. We will create a new instance of this structure for each thread with a different beginning index:
```c
typedef struct {
  VectorInfo *info;
  int beginningIndex;
} Product;
```

While each thread will be acting on both vectors at the same time, they will be accessing different parts of the vector, so there is no conflict there. Each thread will compute a sum for its section of the vectors. However, this sum will need to be added to the *sum* field of the *VectorInfo* structure.

Since multiple threads may be added to the *sum* field at the same time, it is necessary to protect this data using a *mutex* as declared below. A mutex allows only one thread to access a protected variable at a time. The following declares a mutex to protect the *sum* variable. It is declared at a global level to allow multiple threads to access it:
```c
pthread_mutex_t mutexSum;
```

When a thread is created, this function will be called. Since we are using POSIX, it is necessary to declare this function as returning void and being passed a pointer to void. This pointer information to the function. We will pass an instance of the *Product* structure.
```c
void dotProduct(void *prod) {
  //variables are declared to hold the beginning and ending indexes
  Product *product = (Product*)prod;
  VectorInfo *vectorInfo = Product->info;
  int beginningIndex = Product->beginningIndex;
  int endingIndex = beginningIndex + vectorInfo->length;
  double total = 0;

  //for loop performs the actual multiplication and keeps a cummulative total
  for (int i = beginningIndex; i < endingIndex; i++) {
    total += (vectorInfo->vectorA[i] * vectorInfo->vectorB[i]);
  }

  //locks the mutex, adds total to sum, and then unlocks the mutex
  pthread_mutex_lock(&mutexSum);
  vectorInfo->sum += total;
  pthread_mutex_unlock(&mutexSum);
  pthread_exit((void*) 0);
}
```

The code to create the threads is shown below. Two simple vectors are declared along with an instance of VectorInfo.
```c
#define NUM_THREADS 4
void threadExample() {
  VectorInfo vectorInfo;
  double vectorA[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                      9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  double vectorB[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                      9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  double sum;
  vectorInfo.vectorA = vectorA;
  vectorInfo.vectorB = vectorB;
  vectorInfo.length = 4;

  //a four-element array of threads is created
  pthread_t threads[NUM_THREADS];
  
  //initialize the mutex and an attribute field for the thread
  void *status;
  pthread_attr_t attr;

  pthread_mutex_init(&mutexSum, NULL);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  int returnValue;
  int threadNumber;

  for (threadNumber = 0; threadNumber < NUM_THREADS; threadNumber++) {
    //a new instance of the product structure is created
    Product *product = (Product*) malloc(sizeof(Product));
    product->beginningIndex = threadNumber * 4;   //a unique index based on threadNumber
    product->info = &vectorInfo;   //assign the address of vectorInfo
    returnValue = pthread_create(&threads[threadNumber], &attr,
                                 dotProduct, (void *) (void*) (product));
    if (returnValue) {
      printf("ERROR; Unable to create thread: %d\n", returnValue);
      exit(-1);
    }
  }
  //the thread attribute and mutex variables are destroyed
  pthread_attr_destroy(&attr);

  //the for loop ensures the program will wait until all four threads have completed
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], &status);
  }

  pthread_mutex_destroy(&mutexSum);
  printf("Dot Product sum: %lf\n", vectorInfo.sum);  //1496
  pthread_exit(NULL);
}
```

### Using function pointers to support callbacks

A more widely accepted definition of a callback is when an event in one thread results in the invocation, or callback, of a function in another thread. One thread is passed a function pointer to a callback function. An event in the function can trigger a call to the callback function. This approach is useful in GUI applications to handle user thread events.

We will illustrate this approach using a function to compute the factorial of a number. The function will callback a second function when the factorial has been computed. Information regarding the factorial is encapsulated in a *FactorialData* structure and is passed between th functions. 
```c
//the data consists of the factorial number, the results, and a function pointer for the callback
typedef struct _factorialData {
  int number;
  int result;
  void (*callBack)(struct _factorialData*);
} FactorialData;

//the factorial function uses this data to compute the factorial, store the answer in the
//result field, call the callback function, and then terminate the thread
void factorial(void *args) {
  FactorialData *factorialData = (FactorialData*) args;
  void (*callBack)(FactorialData*); // Function prototype
  int number = factorialData->number;
  callBack = factorialData->callBack;
  int num = 1;
    for(int i = 1; i<=number; i++) {
    num *= i;
  }
  factorialData->result = num;
  callBack(factorialData);
  pthread_exit(NULL);
}
```

The thread is created in a *startThread* function as shown below:
```c
void startThread(FactorialData *data) {
  pthread_t thread_id;
  int thread = pthread_create(&thread_id, NULL, factorial, (void *) data);
}

//the callback function simply displays the factorial results
void callBackFunction(FactorialData *factorialData) {
  printf("Factorial is %d\n", factorialData->result);
}

//the factorial data is initialized
FactorialData *data = (FactorialData*) malloc(sizeof(FactorialData));
if(!data) {
   printf("Failed to allocate memory\n");
   return;
}
data->number = 5;
data->callBack = callBackFunction;
startThread(data);   //Factorial is 120
Sleep(2000);  //provides time for all of the threads to terminate properly
```

## Reference

* Reese, Richard. Understanding and Using C pointers. " O'Reilly Media, Inc.", 2013.
