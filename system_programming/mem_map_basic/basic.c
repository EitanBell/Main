

#include <stdio.h>
#include <stdlib.h>

static int StaticFunction(void);
extern int ExternFunction(void);
size_t RecorsiveFibbo(size_t n);

int non_const_glob = 5;
const int const_global = 99;
static int static_global = 10;
char *string_literal = "My name is CHIKKI CHIKKI CHIKKI";

int main(int argc, char **argv, char **envp)
{
	
	static int local_static = 20;
	char *string_literal = "Hello! Is it me you looking for?";
	
	(void)envp;
	(void)argc;
	(void)argv;
	(void)local_static;
	(void)static_global;
	(void)string_literal;	
	
	StaticFunction();
	RecorsiveFibbo(10);
	
	return 0;
}

static int StaticFunction(void)
{
	char *my_heap = (char *)malloc(sizeof(char) * 20);
	
	free(my_heap);
	my_heap = NULL;
	
	return 0;
	
}

extern int ExternFunction(void)
{
	return 0;
}

size_t RecorsiveFibbo(size_t n)
{
	int elephent = 8;
	char *my_local_heap = (char *)malloc(sizeof(char) * 50);
	++static_global;
	++elephent;
	
	if (1 == n || 0 == n)
	{
		free(my_local_heap);
		return 1;
	}
	return (RecorsiveFibbo(n-1) + RecorsiveFibbo(n-2));
}

/*
------------------------  before first recursive frame -------------------------

1: &elephent = (int *) 0x7fffffffded4
(gdb) display &static_global                     
2: &static_global = (int *) 0x555555558014 <static_global>
(gdb) display my_local_heap 
3: my_local_heap = 0x5555555551c7 <StaticFunction+38> "H\307", <incomplete sequence \370>
(gdb) bt
#0  RecorsiveFibbo (n=10) at basic.c:57
#1  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame
Stack level 0, frame at 0x7fffffffdf00:
 rip = 0x5555555551f6 in RecorsiveFibbo (basic.c:57); saved rip = 0x55555555519a
 called by frame at 0x7fffffffdf40
 source language c.
 Arglist at 0x7fffffffdef0, args: n=10
 Locals at 0x7fffffffdef0, Previous frame's sp is 0x7fffffffdf00
 Saved registers:
  rbx at 0x7fffffffdee8, rbp at 0x7fffffffdef0, rip at 0x7fffffffdef8
(gdb) info locals
elephent = 32767
my_local_heap = 0x5555555551c7 <StaticFunction+38> "H\307", <incomplete sequence \370>


------------------------ first recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffded4
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x5555555592c0 ""
(gdb) 
62              if (1 == n || 0 == n)
1: &elephent = (int *) 0x7fffffffded4
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x5555555592c0 ""
(gdb) bt
#0  RecorsiveFibbo (n=10) at basic.c:62
#1  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame
Stack level 0, frame at 0x7fffffffdf00:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555519a
 called by frame at 0x7fffffffdf40
 source language c.
 Arglist at 0x7fffffffdef0, args: n=10
 Locals at 0x7fffffffdef0, Previous frame's sp is 0x7fffffffdf00
 Saved registers:
  rbx at 0x7fffffffdee8, rbp at 0x7fffffffdef0, rip at 0x7fffffffdef8
(gdb) info locals
elephent = 9
my_local_heap = 0x5555555592c0 ""



------------------------ seconde recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffde94
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559300 ""
(gdb) 
62              if (1 == n || 0 == n)
1: &elephent = (int *) 0x7fffffffde94
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559300 ""
(gdb) by
Undefined command: "by".  Try "help".
(gdb) info frame
Stack level 0, frame at 0x7fffffffdec0:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555524f
 called by frame at 0x7fffffffdf00
 source language c.
 Arglist at 0x7fffffffdeb0, args: n=9
 Locals at 0x7fffffffdeb0, Previous frame's sp is 0x7fffffffdec0
 Saved registers:
  rbx at 0x7fffffffdea8, rbp at 0x7fffffffdeb0, rip at 0x7fffffffdeb8
(gdb) info locals
elephent = 9
my_local_heap = 0x555555559300 ""

------------------------ third recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffde54
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559340 ""
(gdb) bt
#0  RecorsiveFibbo (n=8) at basic.c:62
#1  0x000055555555524f in RecorsiveFibbo (n=9) at basic.c:67
#2  0x000055555555524f in RecorsiveFibbo (n=10) at basic.c:67
#3  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame
Stack level 0, frame at 0x7fffffffde80:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555524f
 called by frame at 0x7fffffffdec0
 source language c.
 Arglist at 0x7fffffffde70, args: n=8
 Locals at 0x7fffffffde70, Previous frame's sp is 0x7fffffffde80
 Saved registers:
  rbx at 0x7fffffffde68, rbp at 0x7fffffffde70, rip at 0x7fffffffde78
(gdb) info locals
elephent = 9
my_local_heap = 0x555555559340 ""

------------------------ fourth recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffde14
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559380 ""
(gdb) 
62              if (1 == n || 0 == n)
1: &elephent = (int *) 0x7fffffffde14
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559380 ""
(gdb) bt
#0  RecorsiveFibbo (n=7) at basic.c:62
#1  0x000055555555524f in RecorsiveFibbo (n=8) at basic.c:67
#2  0x000055555555524f in RecorsiveFibbo (n=9) at basic.c:67
#3  0x000055555555524f in RecorsiveFibbo (n=10) at basic.c:67
#4  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame 
Stack level 0, frame at 0x7fffffffde40:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555524f
 called by frame at 0x7fffffffde80
 source language c.
 Arglist at 0x7fffffffde30, args: n=7
 Locals at 0x7fffffffde30, Previous frame's sp is 0x7fffffffde40
 Saved registers:
  rbx at 0x7fffffffde28, rbp at 0x7fffffffde30, rip at 0x7fffffffde38
(gdb) info locals
elephent = 9
my_local_heap = 0x555555559380 ""


------------------------ fifth recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffddd4
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x5555555593c0 ""
(gdb) bt
#0  RecorsiveFibbo (n=6) at basic.c:62
#1  0x000055555555524f in RecorsiveFibbo (n=7) at basic.c:67
#2  0x000055555555524f in RecorsiveFibbo (n=8) at basic.c:67
#3  0x000055555555524f in RecorsiveFibbo (n=9) at basic.c:67
#4  0x000055555555524f in RecorsiveFibbo (n=10) at basic.c:67
#5  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame
Stack level 0, frame at 0x7fffffffde00:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555524f
 called by frame at 0x7fffffffde40
 source language c.
 Arglist at 0x7fffffffddf0, args: n=6
 Locals at 0x7fffffffddf0, Previous frame's sp is 0x7fffffffde00
 Saved registers:
  rbx at 0x7fffffffdde8, rbp at 0x7fffffffddf0, rip at 0x7fffffffddf8
(gdb) info locals
elephent = 9
my_local_heap = 0x5555555593c0 ""


------------------------ sixth recursive frame ---------------------------------

1: &elephent = (int *) 0x7fffffffdd94
2: &static_global = (int *) 0x555555558014 <static_global>
3: my_local_heap = 0x555555559400 ""
(gdb) bt
#0  RecorsiveFibbo (n=5) at basic.c:62
#1  0x000055555555524f in RecorsiveFibbo (n=6) at basic.c:67
#2  0x000055555555524f in RecorsiveFibbo (n=7) at basic.c:67
#3  0x000055555555524f in RecorsiveFibbo (n=8) at basic.c:67
#4  0x000055555555524f in RecorsiveFibbo (n=9) at basic.c:67
#5  0x000055555555524f in RecorsiveFibbo (n=10) at basic.c:67
#6  0x000055555555519a in main (argc=1, argv=0x7fffffffe048, envp=0x7fffffffe058) at basic.c:29
(gdb) info frame
Stack level 0, frame at 0x7fffffffddc0:
 rip = 0x55555555521e in RecorsiveFibbo (basic.c:62); saved rip = 0x55555555524f
 called by frame at 0x7fffffffde00
 source language c.
 Arglist at 0x7fffffffddb0, args: n=5
 Locals at 0x7fffffffddb0, Previous frame's sp is 0x7fffffffddc0
 Saved registers:
  rbx at 0x7fffffffdda8, rbp at 0x7fffffffddb0, rip at 0x7fffffffddb8
(gdb) info locals
elephent = 9
my_local_heap = 0x555555559400 ""

------------------------ (gdb) info proc all -----------------------------------

process 73629
cmdline = '/home/eitan/git/system_programming/mem_map_basic/a.out'
cwd = '/home/eitan/git/system_programming/mem_map_basic'
exe = '/home/eitan/git/system_programming/mem_map_basic/a.out'
Mapped address spaces:

          Start Addr           End Addr       Size     Offset  Perms  objfile
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]
      0x7ffff7d7e000     0x7ffff7d81000     0x3000        0x0  rw-p   
      0x7ffff7d81000     0x7ffff7da9000    0x28000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7da9000     0x7ffff7f3e000   0x195000    0x28000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f3e000     0x7ffff7f96000    0x58000   0x1bd000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f96000     0x7ffff7f9a000     0x4000   0x214000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f9a000     0x7ffff7f9c000     0x2000   0x218000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f9c000     0x7ffff7fa9000     0xd000        0x0  rw-p   
      0x7ffff7fbb000     0x7ffff7fbd000     0x2000        0x0  rw-p   
      0x7ffff7fbd000     0x7ffff7fc1000     0x4000        0x0  r--p   [vvar]
      0x7ffff7fc1000     0x7ffff7fc3000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc5000     0x7ffff7fef000    0x2a000     0x2000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fef000     0x7ffff7ffa000     0xb000    0x2c000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x37000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x39000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0  --xp   [vsyscall]
Name:	a.out
Umask:	0002
State:	t (tracing stop)
Tgid:	73629
Ngid:	0
Pid:	73629
PPid:	73540
TracerPid:	73540
Uid:	1000	1000	1000	1000
--Type <RET> for more, q to quit, c to continue without paging--Quit
(gdb) 


------------------------ (gdb) info proc all with c ----------------------------
(gdb) info proc all
process 73629
cmdline = '/home/eitan/git/system_programming/mem_map_basic/a.out'
cwd = '/home/eitan/git/system_programming/mem_map_basic'
exe = '/home/eitan/git/system_programming/mem_map_basic/a.out'
Mapped address spaces:

          Start Addr           End Addr       Size     Offset  Perms  objfile
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /home/eitan/git/system_programming/mem_map_basic/a.out
      0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]
      0x7ffff7d7e000     0x7ffff7d81000     0x3000        0x0  rw-p   
      0x7ffff7d81000     0x7ffff7da9000    0x28000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7da9000     0x7ffff7f3e000   0x195000    0x28000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f3e000     0x7ffff7f96000    0x58000   0x1bd000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f96000     0x7ffff7f9a000     0x4000   0x214000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f9a000     0x7ffff7f9c000     0x2000   0x218000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f9c000     0x7ffff7fa9000     0xd000        0x0  rw-p   
      0x7ffff7fbb000     0x7ffff7fbd000     0x2000        0x0  rw-p   
      0x7ffff7fbd000     0x7ffff7fc1000     0x4000        0x0  r--p   [vvar]
      0x7ffff7fc1000     0x7ffff7fc3000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc5000     0x7ffff7fef000    0x2a000     0x2000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fef000     0x7ffff7ffa000     0xb000    0x2c000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x37000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x39000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0  --xp   [vsyscall]
Name:	a.out
Umask:	0002
State:	t (tracing stop)
Tgid:	73629
Ngid:	0
Pid:	73629
PPid:	73540
TracerPid:	73540
Uid:	1000	1000	1000	1000
--Type <RET> for more, q to quit, c to continue without paging--c
Gid:	1000	1000	1000	1000
FDSize:	64
Groups:	4 24 27 30 46 122 134 135 1000 1003 
NStgid:	73629
NSpid:	73629
NSpgid:	73629
NSsid:	66999
VmPeak:	    2772 kB
VmSize:	    2772 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    1204 kB
VmRSS:	    1204 kB
RssAnon:	      96 kB
RssFile:	    1108 kB
RssShmem:	       0 kB
VmData:	     224 kB
VmStk:	     132 kB
VmExe:	       4 kB
VmLib:	    1796 kB
VmPTE:	      32 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB
CoreDumping:	0
THP_enabled:	1
Threads:	1
SigQ:	1/62870
SigPnd:	0000000000000000
ShdPnd:	0000000000000000
SigBlk:	0000000000000000
SigIgn:	0000000000000000
SigCgt:	0000000000000000
CapInh:	0000000000000000
CapPrm:	0000000000000000
CapEff:	0000000000000000
CapBnd:	000001ffffffffff
CapAmb:	0000000000000000
NoNewPrivs:	0
Seccomp:	0
Seccomp_filters:	0
Speculation_Store_Bypass:	thread vulnerable
SpeculationIndirectBranch:	conditional enabled
Cpus_allowed:	ff
Cpus_allowed_list:	0-7
Mems_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:	0
voluntary_ctxt_switches:	7
nonvoluntary_ctxt_switches:	1
Process: 73629
Exec file: a.out
State: t
Parent process: 73540
Process group: 73629
Session id: 66999
TTY: 34816
TTY owner process group: 73540
Flags: 0x40000000
Minor faults (no memory page): 178
Minor faults, children: 0
Major faults (memory page faults): 0
Major faults, children: 0
utime: 0
stime: 0
utime, children: 0
stime, children: 0
jiffies remaining in current time slice: 20
'nice' value: 0
jiffies until next timeout: 1
jiffies until next SIGALRM: 0
start time (jiffies since system boot): 4801142
Virtual memory size: 2838528
Resident set size: 301
rlim: 18446744073709551615
Start of text: 0x555555555000
End of text: 0x555555555279
Start of stack: 0x7fffffffe050
Kernel: Between 0xffffffffffff and 0x7fffffffe050



*/






