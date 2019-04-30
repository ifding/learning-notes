
## Table of Contents

   * [A small Hello World](#a-small-hello-world)
   * [Cross-compile](#cross-compile)
      * [Install OpenWrt build system](#install-openwrt-build-system)
      * [Cross-compile hello world](#cross-compile-hello-world)

> 2017-05-06


The first section shows how to call the Autotools (autoconf and automake) to create the amhello-1.0 package from scratch, while the second one shows how to cross-compile amhello-1.0.tar.gz for MIPS architecture.  

## A small Hello World

I will use the use case in [GNU.org](https://www.gnu.org/software/automake/manual/automake.html#Hello-World), this amhello-1.0.tar.gz is simple enough so that we will only need to write 5 files.



Crete the following files in an empty directory.

- *src/main.c* is the source file for the *hello* program.

```sh
~/amhello $ cat src/main.c
#include <config.h>
#include <stdio.h>

int
main (void)
{
  puts ("Hello World!");
  puts ("This is " PACKAGE_STRING ".");
  return 0;
}
```

- *README* contains some very limited documentation for our little packages.

```sh
~/amhello $ cat README
This is a demonstration package for GNU Automake.
Type 'info Automake' to read the Automake manual.
```

- `Makefile.am` and `src/Makefile.am` contain Automake instructions for these two directories.

```sh
~/amhello $ cat src/Makefile.am
bin_PROGRAMS = hello
hello_SOURCES = main.c
~/amhello $ cat Makefile.am
SUBDIRS = src
dist_doc_DATA = README
```

- Finally, `configure.ac` contains Autoconf instructions to create the configure script.

```sh
~/amhello $ cat configure.ac
AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
```

Once you have these five files, it is time to run the Autotools to instantiate the build system. Do this using the `autoreconf` command as follows:
```sh
~/amhello$ autoreconf --install
configure.ac:3: installing './compile'
configure.ac:2: installing './install-sh'
configure.ac:2: installing './missing'
src/Makefile.am: installing './depcomp'
```

At this point the build system is complete.

More details about the meaning of the `configure.ac` and `Makefile.am` files read by the Autotools, please see [GNU-build-system](https://github.com/ifding/awesome-notes/blob/master/compiler/GNU-build-system.md) and [The Basics of Autoconf and Automake](https://github.com/ifding/awesome-notes/blob/master/compiler/the-basics-of-autoconf-and-automake.md).


## Cross-compile

**Note: Do everything as non-root user!**

cross-compile tutorial is [here](https://github.com/ifding/awesome-notes/blob/master/compiler/Building_and_cross-compile_tutorial.md)

### Install OpenWrt build system

- Install *build tools* to do the cross-compilation process:

```
$ sudo apt-get update
$ sudo apt-get install git-core build-essential libssl-dev libncurses5-dev unzip gawk zlib1g-dev
$ git clone https://github.com/openwrt/openwrt.git
$ cd openwrt
$ ./scripts/feeds update -a
$ make menuconfig
```

Locate the toolchain binaries in the staging_dir/staging_dir/toolchain-mipsel_24kec+dsp_gcc-5.3.0_musl-1.1.16/bin/ directory.

Add that directory to the PATH environment vairable to the end of (e.g., `~/.bashrc`): 

```
export PATH=$PATH:/home/ubuntu/Downloads/openwrt/staging_dir/toolchain-mipsel_24kec+dsp_gcc-5.3.0_musl-1.1.16/bin/
export STAGING_DIR=/home/ubuntu/Downloads/openwrt/staging_dir/
```

```sh
$ source ~/.bashrc
```

Now it is ready for compilation.

### Cross-compile hello world

Pass the host to the build system of the package to trigger cross-compile and override the CC and LD environment variables.
```sh
~/amhello$ CC=mipsel-openwrt-linux-musl-gcc LD=mipsel-openwrt-linux-musl-ld LDFLAGS=-L/home/ubuntu/Downloads/openwrt/staging_dir/target-mipsel_24kec+dsp_musl-1.1.16/usr/lib CFLAGS=-I/home/ubuntu/Downloads/openwrt/staging_dir/target-mipsel_24kec+dsp_musl-1.1.16/usr/include ./configure --host mipsel-openwrt-linux-musl
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for mipsel-openwrt-linux-musl-strip... mipsel-openwrt-linux-musl-strip
checking for a thread-safe mkdir -p... /bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking for mipsel-openwrt-linux-musl-gcc... mipsel-openwrt-linux-musl-gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... yes
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether mipsel-openwrt-linux-musl-gcc accepts -g... yes
checking for mipsel-openwrt-linux-musl-gcc option to accept ISO C89... none needed
checking whether mipsel-openwrt-linux-musl-gcc understands -c and -o together... yes
checking for style of include used by make... GNU
checking dependency style of mipsel-openwrt-linux-musl-gcc... gcc3
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating src/Makefile
config.status: creating config.h
config.status: executing depfiles commands
```

You can see `Makefile`, `src/Makefile`, and `config.h` being created at the end after *configure* has probed the system. Because Autoconf and Automake have separate manuals, it is important to understand that `autoconf` is in charge of creating `configure` from `configrue.ac`, while `automake` is in charge of creating `Makefile.in` from `Makefile.am` and `configure.ac`.
```sh
~/amhello$ ls
aclocal.m4          compile      config.log     configure.ac  Makefile     missing  stamp-h1
amhello-1.0.tar.gz  config.h     config.status  depcomp       Makefile.am  README
autom4te.cache      config.h.in  configure      install-sh    Makefile.in  src
```

It is now possible to run all the targets we wish:
```sh
~/amhello$ make
make  all-recursive
make[1]: Entering directory `/home/ubuntu/Documents/test/amhello'
Making all in src
make[2]: Entering directory `/home/ubuntu/Documents/test/amhello/src'
mipsel-openwrt-linux-musl-gcc -DHAVE_CONFIG_H -I. -I..     -I/home/ubuntu/Downloads/openwrt/staging_dir/target-mipsel_24kec+dsp_musl-1.1.16/usr/include -MT main.o -MD -MP -MF .deps/main.Tpo -c -o main.o main.c
mv -f .deps/main.Tpo .deps/main.Po
mipsel-openwrt-linux-musl-gcc  -I/home/ubuntu/Downloads/openwrt/staging_dir/target-mipsel_24kec+dsp_musl-1.1.16/usr/include  -L/home/ubuntu/Downloads/openwrt/staging_dir/target-mipsel_24kec+dsp_musl-1.1.16/usr/lib -o hello main.o  
make[2]: Leaving directory `/home/ubuntu/Documents/test/amhello/src'
make[2]: Entering directory `/home/ubuntu/Documents/test/amhello'
make[2]: Leaving directory `/home/ubuntu/Documents/test/amhello'
make[1]: Leaving directory `/home/ubuntu/Documents/test/amhello'
~/amhello$ ls
aclocal.m4          compile      config.log     configure.ac  Makefile     missing  stamp-h1
amhello-1.0.tar.gz  config.h     config.status  depcomp       Makefile.am  README
autom4te.cache      config.h.in  configure      install-sh    Makefile.in  src
~/amhello$ ls src/
hello  main.c  main.o  Makefile  Makefile.am  Makefile.in
~/amhello$ file src/hello 
src/hello: ELF 32-bit LSB  executable, MIPS, MIPS32 rel2 version 1, dynamically linked (uses shared libs), not stripped
```

Run file program to confirm cross-compiling was successful. So let's copy this `hello` binary to the openwrt device and run it:
```sh
~/amhello$ scp src/hello root@192.168.1.1:
```

On OpenWrt:
```sh
$ ./hello
Hello World!
This is amhello 1.0.
```

## Reference

* [A Small Hello World](https://www.gnu.org/software/automake/manual/automake.html#Hello-World)
* [Cross Compile](https://wiki.openwrt.org/doc/devel/crosscompile)