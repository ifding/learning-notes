
## Adding a system call to xv6

Now we will add our system call to xv6.

### First add the following line at the end of the file syscall.h.

```c
#define SYS_getmysize 23
```

Note that the 23 here might change depending on the number given before the line you are going to add in the file. That is if the system call number is 22 in the line before the line that you are going to add, your line should have the number 23. Simple as that.

### Now add the following lines to the syscall.c file.

```c
extern int sys_getmysize(void);
```

and in the array of syscalls, append the following line in.

```c
[SYS_getmysize] sys_getmysize,
```

### In the file sysproc.c 

that is where the implementation of your system call goes if it is a system call related to process handling or memory management, put the implementation of our system call as follows.

```c
int 
sys_getmysize(void)
{
   return proc->sz;
}
```

### Now in the usys.S file, add the line,

```c
SYSCALL(getmysize)
```

### Then in the user.h file, add the following. 

This is how your user program sees your system call.

```c
int getmysize(void);
```

Ok. Now you are ready to run your user program and see how your system call runs.

## Writing a user program

```c

#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
  printf(1,"The size of my address space is %d bytes\n", getmysize());
  exit();
}
```

## Reference

- [how do i add a system call / utility in xv6](https://stackoverflow.com/questions/8021774/how-do-i-add-a-system-call-utility-in-xv6)
