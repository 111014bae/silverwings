final.elf : 1.o 2.o 3.o 4.o main.o
	gcc -o final.elf 1.o 2.o 3.o 4.o main.o

main.o : main.c myProject.h
	gcc -c main.c

1.o : 1.c myProject.h
	gcc -c 1.c

2.o : 2.c myProject.h
	gcc -c 2.c

3.o : 3.c myProject.h
	gcc -c 3.c

4.o : 4.c myProject.h
	gcc -c 4.c
