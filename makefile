

final.elf : 1.elf 2.elf 3.elf 4.elf

1.elf : 1.c
  gcc -o 1.elf 1.c

2.elf : 2.c
  gcc -o 2.elf 2.c

3.elf : 3.c
  gcc -o 3.elf 3.c

4.elf : 4.c
  gcc -o 4.elf 4.c
 
