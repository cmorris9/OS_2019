bcc -ansi -c -o shell.o shell.c
as86 -o userlib.o userlib.asm
ld86 -d -o kernel kernel_c.o userlib.o
./loadFile shell
