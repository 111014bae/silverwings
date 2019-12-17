#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>  
#include <linux/input.h>
#include "bitmap.h"
#include "gyro.h"

#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"

  /* char Pic[1000] = "ball.bmp";*/
   

int GyroDo(void)
{
   while(1)
   {
      int fd = 0;
      FILE *fp = NULL;
   
      fd = open (GYROPATH "enable",O_WRONLY);
      dprintf (fd,"1");
      close(fd);
      fp = fopen (GYROPATH "data" , "rt");
      int gyro[3];
      fscanf(fp,"%d, %d, %d",&gyro[0],&gyro[1],&gyro[2]);
      printf ("I read Gyroscope %d, %d, %d\r\n",gyro[0],gyro[1],gyro[2]);
      fclose(fp);
      
   
   if( ( ( gyro[0])^2 ) >= 600 || ( ( gyro[1])^2 ) >= 600 || ( ( gyro[2])^2 ) >= 600 || ( ( ( ( gyro[0])^2 )+( ( gyro[1])^2 )+( ( gyro[2])^2 ) )^(1/2) ) >= 1000 ){
         break;
         }
         
 }
}

int  Gyroend(void)
{
	int fd = 0;
      FILE *fp = NULL;
      close(fd);
      fclose(fp);
  }
	
