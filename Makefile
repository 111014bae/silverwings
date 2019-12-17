all : final_test1.elf libMyPeri.a

final_test1.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc main.c -L. -lMyPeri -o final_test1.elf -fno-stack-protector -lpthread
   
libMyPeri.a : textlcd.c textlcd.h led.c led.h button.c button.h buzzer.c buzzer.h fnd.c fnd.h bitmap.c bitmap.h touch.c touch.h gyro.c gyro.h
	arm-linux-gnueabi-gcc -c textlcd.c -o textlcd.o
	arm-linux-gnueabi-gcc -c led.c -o led.o
	arm-linux-gnueabi-gcc -c button.c -o button.o
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
	arm-linux-gnueabi-gcc -c bitmap.c -o bitmap.o
	arm-linux-gnueabi-gcc -c touch.c -o touch.o
	arm-linux-gnueabi-gcc -c gyro.c -o gyro.o
	arm-linux-gnueabi-ar rc libMyPeri.a textlcd.o led.o button.o buzzer.o fnd.o bitmap.o touch.o gyro.o
	
   
clean :
	rm -rf *.o *.elf *.a
