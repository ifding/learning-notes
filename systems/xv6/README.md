
## Installation

```
# Download the code
git clone https://github.com/mit-pdos/xv6-public.git xv6
cd xv6

# Compile
make

# Install QEMU
sudo apt-get install qemu

# Run under QEMU
make qemu
```

## Remote Debug xv6 Under QEMU using GDB

```
$ make qemu-gdb
...
sed "s/localhost:1234/localhost:26000/" < .gdbinit.tmpl > .gdbinit
*** Now run 'gdb'.
qemu-system-i386 -serial mon:stdio -hdb fs.img xv6.img -smp 2 -m 512  -S -gdb tcp::26000
```

Open GDB from another shell:

```
$ gdb kernel
...
Reading symbols from kernel...done.
+ target remote localhost:26000
warning: A handler for the OS ABI "GNU/Linux" is not built into this configuration
of GDB.  Attempting to continue with the default i8086 settings.

The target architecture is assumed to be i8086
[f000:fff0]    0xffff0: ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file kernel
(gdb)
```

## xv6 System Call

- xv6 source : <https://github.com/mit-pdos/xv6-public>
- How to add system call : <http://stackoverflow.com/questions/8021774/how-do-i-add-a-system-call-utility-in-xv6>
- Installing qemu : <http://wiki.qemu.org/Hosts/Linux>
- Installing qemu : <http://askubuntu.com/questions/138140/how-do-i-install-qemu>
- Running and Debugging xv6 on qemu : <http://zoo.cs.yale.edu/classes/cs422/2010/lec/l2-hw>
- Running xv6 on qemu : <http://ubuntuforums.org/showthread.php?t=2127846>
