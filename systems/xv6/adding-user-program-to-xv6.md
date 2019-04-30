## Adding a User Program to xv6

### 1.For example, my_program.c

```c
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
 printf(1, "my first user program on xv6\n");
 exit();
}
```

### 2. Edit the Makefile

```c
UPROGS=\
    _cat\
    _echo\
    _forktest\
    _grep\
    _init\
    _kill\
    _ln\
    _ls\
    _mkdir\
    _rm\
    _sh\
    _stressfs\
    _usertests\
    _wc\
    _zombie\
    _my_program\
```

```c
EXTRA=\
    mkfs.c ulib.c user.h cat.c echo.c forktest.c grep.c kill.c\
    ln.c ls.c mkdir.c rm.c stressfs.c usertests.c wc.c zombie.c\
    my_program.c\
    printf.c umalloc.c\
    README dot-bochsrc *.pl toc.* runoff runoff1 runoff.list\
    .gdbinit.tmpl gdbutil\
```

### 3. Compile the whole system

```
$ make clean
$ make
```
