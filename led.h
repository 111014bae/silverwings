#ifndef _LED_H_ 
#define _LED_H_ 

int ledLibInit(void);
int ledOnOff(int ledNum, int OnOff);
int ledOnAll(void);
int ledOffAll(void);
int ledLibExit(void);


#endif 
