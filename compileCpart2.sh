bcc -ansi -c -o testpr1.o testpr1.c
as86 -o userlib.o userlib.asm
ld86 -d -o testpr1 testpr1.o userlib.o
./loadFile testpr1
