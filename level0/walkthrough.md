# Search

```
$ ls -l
-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0

$ ./level0 
Segmentation fault (core dumped)

$ ./level0 sdf
No !

$ ./level0 sdfas dffasd
No !

$ ./level0 423
$ whoami
level1

$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```

There is an executable, the executable segfault if no argument provided and print 'No !' if some arguments are given, and start a new shell in which we are level1

This is because the executable has a setuid bit '-rws...' the s mean that if we execute the code we do this as the owner which is level1

# Lets analyze with gdb

```
$ gdb level0

(gdb) disas main
...
	0x08048ed1 <+17>:    mov    %eax,(%esp)
	0x08048ed4 <+20>:    call   0x8049710 <atoi>
	0x08048ed9 <+25>:    cmp    $0x1a7,%eax
	0x08048ede <+30>:    jne    0x8048f58 <main+152>
	compare 423 with the return of atoi which is argv[1]
	IF jne = jump if not equal to main+152
...
	0x08048ee0 <+32>:    movl   $0x80c5348,(%esp)
	0x08048ee7 <+39>:    call   0x8050bf0 <strdup>
	0x08048eec <+44>:    mov    %eax,0x10(%esp)
	0x08048ef0 <+48>:    movl   $0x0,0x14(%esp)
	0x08048ef8 <+56>:    call   0x8054680 <getegid>
	0x08048efd <+61>:    mov    %eax,0x1c(%esp)
	0x08048f01 <+65>:    call   0x8054670 <geteuid>
	0x08048f06 <+70>:    mov    %eax,0x18(%esp)
	0x08048f0a <+74>:    mov    0x1c(%esp),%eax
	0x08048f0e <+78>:    mov    %eax,0x8(%esp)
	0x08048f12 <+82>:    mov    0x1c(%esp),%eax
	0x08048f16 <+86>:    mov    %eax,0x4(%esp)
	0x08048f1a <+90>:    mov    0x1c(%esp),%eax
	0x08048f1e <+94>:    mov    %eax,(%esp)
	0x08048f21 <+97>:    call   0x8054700 <setresgid>
	0x08048f26 <+102>:   mov    0x18(%esp),%eax
	0x08048f2a <+106>:   mov    %eax,0x8(%esp)
	0x08048f2e <+110>:   mov    0x18(%esp),%eax
	0x08048f32 <+114>:   mov    %eax,0x4(%esp)
	0x08048f36 <+118>:   mov    0x18(%esp),%eax
	0x08048f3a <+122>:   mov    %eax,(%esp)
	0x08048f3d <+125>:   call   0x8054690 <setresuid>
	0x08048f42 <+130>:   lea    0x10(%esp),%eax
	0x08048f46 <+134>:   mov    %eax,0x4(%esp)
	0x08048f4a <+138>:   movl   $0x80c5348,(%esp)
	0x08048f51 <+145>:   call   0x8054640 <execv>
	0x08048f56 <+150>:   jmp    0x8048f80 <main+192>
	0x08048f58 <+152>:   mov    0x80ee170,%eax 
	0x08048f5d <+157>:   mov    %eax,%edx
	0x08048f5f <+159>:   mov    $0x80c5350,%eax
	0x08048f64 <+164>:   mov    %edx,0xc(%esp) 
	0x08048f68 <+168>:   movl   $0x5,0x8(%esp)
	0x08048f70 <+176>:   movl   $0x1,0x4(%esp)
	0x08048f78 <+184>:   mov    %eax,(%esp)
	0x08048f7b <+187>:   call   0x804a230 <fwrite>
	0x08048f80 <+192>:   mov    $0x0,%eax
	0x08048f85 <+197>:   leave
	0x08048f86 <+198>:   ret

(gdb) x/s 0x80c5348
0x80c5348:       "/bin/sh"
(gdb) x/s 0x80c5350
0x80c5350:       "No !\n"
```

ok so there is an if which compares '423' with argv[1] after atoi\
if the condition is validated there is a call to strdup with argument "/bin/sh"\
after there is a getegid and geteuid and setresgid and setresuid before the execve which have in argmument "/bin/sh"\
if the condition wasnt validated there is a "No !\n" as argument of fwrite