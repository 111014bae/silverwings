#ifndef _TOUCH_H_ 
#define _TOUCH_H_ 

int touchLibInit(void); 
int touchLibExit(void);
int beatLibInit(void); 
int beatLibExit(void);

typedef struct 
{
   unsigned long num;
   int screenx; 
   int screeny;    
   int who;    
}TOUCH_MSG_T;


 #endif 
