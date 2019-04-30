# Building and cross-compile tutorial

Written by [Fabrizio Dini](http://www.fabriziodini.eu/posts/cross_compile_tutorial/)


## Building a program

- **host**, the machine (or architecture, or platform) that you are using to compile the code.
- **target**, the machine (or architecture, or platform) that is intended to run the code.

If you use your PC to comiple a code to make it run on the same PC, the *host* and *target* plaform are the same. In cross-compilation, the *host* and *target* plaform differs. For example, you may want to use your PC to compile a program that is meant to run on Raspberry Pi.

### Compiling the code

Let's start with the usual example, helloworld.c:

```c
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Hello World!\n");
    return 0;
}
```

You can compile with:

```
$ gcc -c helloworld.c
```

This will produce `helloworld.o`, an object file. You need to link the object file to the libraries that contains the runtime and possibly other code you may be invoking in your program.

### Linking the code

The `ld` command in linux invokes the linker. However, it is much easier to call `gcc` and have it call the actual linker, since this will hide much of the complexity to you. For example, to turn your `helloworld.o` object file into an executable binary file you should provide `ld` with the exact name and path of the standard C runtime for your platform.

```
$ gcc helloworld.o
```

gcc will understand from the parameter you passed that it need to invoke `ld`, and will pass all the parameter it needs to link the object code to the C runtime. 

```
$ ./a.out
Hello World!
```

### Include directories

You'll likely need to add a few arguments to your gcc command. The first thing you will probably need are a few *include dirs*. Include directories are the paths to all headers files that are needed by your code due to the `#include` directives it contains. Include dirs are passed to gcc with `-I` option:

```
$ gcc -c -I./include helloworld.c
```

This command line will tell gcc to look for .h files in the `include/` folder.  Note that the path is relative, but you can obviously use absolute paths, and more than one path at a time:

```
$ gcc -c -I./include -I/usr/local/include helloworld.c
```

Note that the include paths do not need to actually exists. If they are not found, the compiler won’t complain (usually). Instead, if you miss an include directory that’s actually needed, you will get errors at compile time like this:

```
helloworld.c:2:22: fatal error: myheader.h: No such file or directory
```

Your local header files are likely stored in some other (local to your home folder) directories and thus you will need to add their path as an include dir with the -I option to gcc.

### Making a library

If you need to pack your code into a library, then you probably need the compiler only.

- Static

To build a static library, you need to compile the source code to obtain the object files, and then use the archiver `ar` to pack everything into a single .a file. Here's an example:

```
$ gcc -c -I./include -I/usr/local/include -o my_library_obj_file.o my_library_source_file.c
$ ar rcs my_library.a my_library_obj_file.o
```

- Shared

Building a shared library, you can do everything with gcc, without calling the archiver, but will need to specify a few more parameters:

```
$ gcc -fPIC -shared -I./include -I/usr/local/include -o my_shared_library.so my_library_source_file.c
```

## Cross-compilation

Basically, to cross-compile a program or library you need two things:

1. a tool-chain running on your host, targeting your target architecture;
2. the file system of your target machine ("sysroot" in the following).

The tool-chain can be achieved in many different ways. The easiest is undoubtedly to find a .deb or .rpm package to install the tool-chain on your host system. For example, this is possible when the target architecture is the Raspberry-Pi and the host is your PC (see *https://github.com/raspberrypi/tools* for details). If a binary package is not available, you may need to compile a custom tool-chain from scratch! In this case, tools like crosstool-ng may help (*http://crosstool-ng.org/#introduction*)

The sysroot is a mere copy of the file system of your target platform. Actually, you do not need to copy the entire file system on your host: the folders /usr and /lib would suffice.

It is a good idea to keep all these things gathered in a single place. I suggest you create a folder (e.g. x-compile) and store the tool-chain and the sysroot in there. 

### Satisfy the dependencies

If the code depends on some library that is NOT in the sysroot, there's no way out but to find them somewhere, somehow. Dependencies can be statisfied in two ways: with static libraries or with shared libraries. There are a few different situations that can happen, but basically everything reduces to two cases:

1. In the sysroot. If you are satisfying the dependencies with shared libraries (*.so* files) this is probably the most common solution. Remeber that when everything will be up and running, these libraries must be installed somewhere in the file system of the target platform. For example in `/usr/lib` (the binary shared files) and `/usr/include` (the header files). Or in any other path that allow the loader to find those libraries when the program executes. AND, install them in the file system of the actual target machine, in the same places, in order to make everything work as expected. Please note that static libraries (*.a* files) does not need to be installed in the target file system since their code is embedded in the executable file when you cross-compile a program.

2. In a different folder. This could be an interesting solution to keep the libraries that you cross-compiled on your own separated from the other libraries (for example, the system libraries). You can do that if you want, but if you do, you must remember to provide to compiler and linker programs with the paths where header files and binary files can be found. With static libraries, this information are only needed at compile and linking time, but if you are using shared libraries, this won’t suffice. You also must specify where these libraries can be found at run time.

### How to do that

Now let's see how to actually cross-compile, I will assume that:

1. You have your tool-chain installed, that it is the correct tool-chain and the PATH environment variable is correctly set, so that the cross-compiler and all other cross-tools binaries can be called from any folder.
2. You have the sysroot installed in ~/x-compile/sysroot
3. Your code depends on a library for which you have the source code in ~/x-compile/depsrc/
4. You have the source code to be cross-compiled in ~/x-compile/src

### Cross-compile the dependecies

Many open source libraries use auto-tools to compile, which means that for these libraries the compilation requires the following commands(DON'T DO THIS YET):

```
$ ./configure
$ make
$ sudo make install
```

Since what we are trying to do is cross-compile the library, here's an example:

```
$ ./configure CC=arm-linux-gnueabihf-gcc --prefix=~/x-compile/deps --host=arm-linux-gnueabihf
$ make
$ make install
```

The meaning of these commands is the following:

1. we call the *configure* script passing a few parameters. The first tells configure to use the cross-compiler instead of the usual gcc; the second sets the destination folder for compilation products; the third sets the architecture of the host that will be running the binaries.

2. call *make* to use makefile to build a project. This actually perform the compilation and linking steps.

3. call *make* with *install* target, which means we are asking *make* to install the binaries to the folder we previously set with the *--prefix* option.

### Cross-compile your code

To cross-compile your code you obviously need to invoke the cross-compiler coming with the tool-chain you installed. I will refer to the case where the Raspberry Pi is the target architecture.

The tool-chain compiler is usually a particular version of gcc. Typically, the binary name is prefixed with a string identifying the target architecture. For the Raspberry-Pi architecture, a common tool-chain provides arm-linux-gnueabihf-gcc.

```
$ arm-linux-gnueabihf-gcc -o hello_world hello_world.c
```

But things get more complex when the code is not trivial. The command line would be something like this:

```
$ arm-linux-gnueabihf-gcc --sysroot=~/x-compile/sysroot -I./include -I/usr/local/include -L~/x-compile/deps -o hello_world -lmy_shared_library ~/x-compile/deps/my_static_library.a hello_world.c
```

Let's give a closer look at these parameters and options.

1. *--sysroot=~/x-compile/sysroot* is a very important option, since it tells the cross-compiler to resolve all paths in the *-I* and *-L* options with respect to the given path. So, we are basically saying that the *./include* and the */usr/local/include* folders should be first look for in *~/x-compile/sysroot*.

2. *-L~/x-compile/deps* adds the path *~/x-compile/deps* to the list of paths where static (.a) and shared (.so) libraries are searched at compile and linking time. I am supposing that there exist two libraries: *my_static_library.a* and *libmy_shared_library.so* within the *~/x-compile/deps* folder.

3. *-lmy_shared_library* tells the linker we are linking against *libmy_shared_library.so* (remember what I said above about the *-L* option...)

4. *~/x-compile/deps/my_static_library.a* simply tells the linker to include the code from this library (the complete path could be omitted thanks to the *-L* option)

You can verify that by using the command file on the result:

```
$ file hello_world
```

### Install your code to the target architecture

At this point, you probably have already copied the binary file to the Raspberry (or your target machine) and see that it does not work… :) 

If the program fails by saying it was unable to load (or find) a .so library, it is because we didn't tell the loader where that library can be found. And if everything was done correctly, the error should refer to our dependency, *libmy_shared_library.so*. If so, there are a few ways you can fix things:

1. copy *libmy_shared_library.so* to a place that the system looks into for other libraries, for example */usr/lib* or */usr/locla/lib*

2. copy *libmy_shared_library.so* wherever you like and start the program like this: *LD_LIBRARY_PATH=/path/to/the/folder/containing/the/library ./hello_world*

3. modify the value of *LD_LIBRARY_PATH* envrionment value before calling the program: 
export *LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/the/folder/containing/the/library ./hello_world*.

All of this should work. Symbolic links are also ok, so if you prefer you may just create a symlink in */usr/lib* pointing to *libmy_shared_library.so*, wherever it is placed. But the solution I prefer is a little different: I like to set an rpath into the binary file of my program.

An rpath is a path that will be stored within the binary file itself, and that the loader will use to look for libraries when every other path have been checked. To do this, you have to add a few other option to your gcc command line, like this:

```
$ arm-linux-gnueabihf-gcc --sysroot=~/x-compile/sysroot -I./include -I/usr/local/include -L~/x-compile/deps -o hello_world -Xlinker -rpath=./ -lmy_shared_library ~/x-compile/deps/my_static_library.a hello_world.c
```

The *-Xlinker -rapth=./* tells the linker to add ./ as an rpath when it creates the binary file. In this way, you can simply put your dependencies in the same folder as the executable binary file. I think it is a very practical solution to distribute an application with its own dependencies without having to install the libraries system-wide.

