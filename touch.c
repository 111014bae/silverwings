#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/mman.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <linux/input.h>
#include  "touch.h"

#define EVENT_DEVICE    "/dev/input/event4"
#define EVENT_TYPE   EV_ABS
#define EVENT_CODE_X ABS_X
#define EVENT_CODE_Y ABS_Y

#define TOUCH_ID1 1123
//#define BEAT_ID 1121

  
    static int fd = 0; 
    static pthread_t touchTh_id; 
   static void *touchThFunc(void*a); 
   static int touchID = 1; 
   //static int beatID = 0;  

int touchLibInit(void) 
{
   fd = open(EVENT_DEVICE, O_RDONLY);
   //beatID = msgget(BEAT_ID, IPC_CREAT|0666);
   touchID = msgget(TOUCH_ID1, IPC_CREAT|0666);
   pthread_create(&touchTh_id, NULL, &touchThFunc, NULL);
}

int touchLibExit(void) 
{
	pthread_cancel(touchTh_id);
	close(fd);
}

static void *touchThFunc(void*a) 
{
   //BEAT_MSG_T beatTx;
   TOUCH_MSG_T touchTx;
    
    touchTx.num = 1;
    touchTx.who=0;
    
   struct input_event ev;
   int savey;
   
      while(1)
{
        const size_t ev_size = sizeof(struct input_event);
        ssize_t size;

        size = read(fd, &ev, ev_size);
        savey=touchTx.screeny;
        if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X|| ev.code == EVENT_CODE_Y)) {
            if(ev.code==EVENT_CODE_X)
            {
            touchTx.screenx=ev.value;
         }
         else
         {
            savey=touchTx.screeny;
            touchTx.screeny=ev.value;   
            if((savey-touchTx.screeny)<15&&(savey-touchTx.screeny)>-15){
               msgsnd(touchID, &touchTx, sizeof(int)*3, 0);
         }
         }
   
     }
 }
}
