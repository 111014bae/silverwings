#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include "led.h"


   static unsigned int ledValue = 0; 
   static int fd = 0;

int ledLibInit(void) 
{ 
   fd=open("/dev/periled", O_WRONLY);
   ledValue = 0;
}

int ledOnOff(int ledNum, int OnOff)
{ 

   int i=1; 
   
   if (OnOff ==0) 
   {
      i=i<<ledNum;
       ledValue = ledValue& (~i); 
      ledValue |= i; 
   }
   else
   {
   ledValue=ledNum;
	}
   write (fd,&ledValue, 4); 
} 

int ledOnAll(void)
{
	ledValue=0xFFFFFFFF;
	write(fd,&ledValue,4);
}

int ledOffAll(void)
{
	ledValue=0x0;
	write(fd,&ledValue,4);
}
int ledLibExit(void)
{
   ledValue = 0;
   ledOnOff (0, 0); 
   close(fd);
}
