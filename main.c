#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <ctype.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/input.h>
#include "led.h"
#include "textlcd.h"
#include "button.h"
#include "buzzer.h"
#include "fnddrv.h"
#include "bitmap.h"
#include "touch.h"
#include "gyro.h"

#define MESSAGE_ID 1122
#define TOUCH_ID 1123
#define MODE_STATIC_DIS      0
#define MODE_TIME_DIS      1
#define MODE_COUNT_DIS      2

int num;
int timer;
int count;
int call =0;
pthread_t fnd;


typedef struct 
{
	int x;	//touch x값
	int y;	//touch y값
}value;

void *fndcount(void *pArg)
{
	while(1){
		if(num == 3){
		fndDisp(timer, 0);
		usleep(1000*1000);	//1초마다 fnd표시
		if(num == 0){
			count = timer;
			break;	//게임이 종료되면 fnd를 종료
         }
		timer++;	//시간 증가
		}
		else if(num == 4){
			fndDisp(timer, 0);
			if(timer == 0){
				Pic("loser.bmp");
				exit(0);	//timer가 0이면 강제종료
			}
			usleep(1000*1000);	//1초마다 fnd표시
			if(num == 0){
				count = timer;
				break;	//게임이 종료되면 fnd종료
			}
			timer--;	//시간 감소
		}
    }
    return (void *)(count);	//count의 값을 반환
}

void linenum1(char linenum[])
{
	unsigned int linenum1 = 1;
	char input[20] = {0};
	int len = strlen(input);
   
	textlcdLibInit(); //textlcd On
   
	textlcdLine(linenum1, len, input);	//textlcdline1에 문자열 추가
	strcpy(input, linenum);	//linenum의 문자열을 input으로 복사	
	len = strlen(input);	//input의 문자열의 길이를 계산
	textlcdLine(linenum1, len, input); 	//textlcdline1에 문자열 추가
}
   
void linenum2(char linenum[])
{
	unsigned int linenum2 = 2;
	char input[20] = {0};
	int len = strlen(input);
   
	textlcdLibInit();	//textlcd On
   
	textlcdLine(linenum2, len, input); 	//textlcdline2에 문자열 추가
	strcpy(input, linenum);	//linenum의 문자열을 input으로 복사	
	len = strlen(input);	//input의 문자열의 길이를 계산
	textlcdLine(linenum2, len, input); //textlcdline2에 문자열 추가
}

void Pic(char Pic[])
{
	char Picture[100];
	
	strcpy(Picture, Pic);	//Pic의 문자열을 Picture에 복사
	bitmapSe(Picture);   //스크린에 Picture의 bmp파일을 보여줌
}

int wrong(void)
{
	int sound = 2; 
	
	buzzerInit();	//buzzer On
	ledLibInit();	//led On
	
	ledOnAll();	//led를 전부 킨다.
	buzzerPlaySong(sound);	//buzzer로 sound값의 소리를 낸다.
	usleep(100*1000);	//0.1초 usleep
	ledOffAll();	//led를 전부 끈다.
	usleep(100*1000);
	ledOnAll();
	usleep(100*1000);
	ledOffAll();
	usleep(100*1000);
	buzzerStopSong();	//buzzer의 소리를 종료시킨다.
	buzzerExit();	//buzze Off
}

int Gyro(void)
{
	GyroDo();	//gyro센서를 실행시킨다.
}

value touch(void)
{
	int touchID = msgget(TOUCH_ID, IPC_CREAT|0666);	
	struct input_event  stEvent;
	TOUCH_MSG_T touchRx;
	int savey;
	value a;
	
	touchRx.screenx=0;
	touchRx.screeny=0;
	touchRx.who=1;

	touchLibExit();
	touchLibInit();

	savey=touchRx.screeny;
	msgrcv (touchID, &touchRx, sizeof(int)*3, 0, 0);
	
	while(1){
		if((savey-touchRx.screeny)>5 || (savey-touchRx.screeny)<-5)
		{
			a.x = touchRx.screenx;
			a.y = touchRx.screeny;
			break;
		}
	}
	
	touchLibExit();
	
	return a;
}

      
int button(void)
{
	int messageID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	BUTTON_MSG_T rxMsg;
	
	buttonInit();	//button On
   
	msgrcv(messageID, &rxMsg, sizeof(rxMsg) - sizeof(long int),0 ,0);	//button.c에서 button의 값을 받는다.
   
	switch(rxMsg.keyInput)
	{
		case KEY_HOME: 
			call = 1;
			break;
		case KEY_BACK: 
			call = 2;
			break;
		case KEY_SEARCH: 
			call =3;
			break;
		case KEY_MENU: 
			call = 4;
			break;
		case KEY_VOLUMEUP: 
			call = 5;
			break;
		case KEY_VOLUMEDOWN:
			call = 6;
			break;
		}
		
	buttonExit();	//button Off
		
	return call;	//call의 값을 반환한다.
}
   
int main(void)
{
	int conFD = open ("/dev/tty0", O_RDWR);
	ioctl(conFD, KDSETMODE, KD_GRAPHICS);
	
	int gyro = 0;	// gyro센서의 값을 저장	
	int correct=0;	// led를 표시하기 위한 값
	int n = 10;	// 0~9까지의 랜덤한 값을 만들기 위한 값
	value a, b,c,d,e;	//touch센서의 값을 저장	
	
	fndDisp(0 , 0); //fnd를 0으로 초기화
    ledLibInit();	//led On
    
	srand(time(NULL));	//랜덤함수를 사용하기 위함 함수
   
	linenum1("");	//textlcdlinenum1 초기화
	linenum2("");	//textlcdlinenum1 초기화
	Pic("start1.bmp");	// 스크린에 start1.bmp의 그림을 표시
   
	ledOnOff(correct,1);	// led 초기화
   
	while(1){	//터치의 값이 읽힐때까지 반복
		a = touch();	//touch 실행
		if((a.x > 480 && a.x < 630) && (a.y > 250 && a.y < 310)){	
			break;	//start 위치에 값이 읽히면 종료
			}
		else{
			continue;	//start위치에 값이 아니라면 반복
		}
	}
	
	Pic("mode.bmp");	// 스크린에 mode.bmp의 그림을 표시
	
	while(1){
		call = button();	//mode를 선택하기 위한 button입력
		 
		if(call==2||call == 3 || call == 4){
      Pic("help.bmp");
      linenum1("Manual");
      linenum2("3second wait");
      usleep(1000*3000);
   }
   
   if(call == 2){
      timer = 0;
      num = 3;
      linenum1("practice");
      linenum2("mode");
      
      pthread_create (&fnd, NULL,  &fndcount, NULL);
      
      while(1){
         ledOnOff(correct,1);
        
            int random = rand()%n;   //난수 생성
            
            switch(random){
            case 0 : 
               Pic("search.bmp");
               searchp:
               ledOnOff(correct,1);
               call = button();
            
               if(call == 1){
                  goto home;
               }
               else if(call == 2){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("search.bmp");
                  goto searchp;
               }
               else if(call == 3){
                  printf("%d\n", call);
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto searchp;
               }
            
            case 1 : 
               Pic("menu.bmp");
               menup :
               ledOnOff(correct,1);
               call = button();
            
               if(call == 1){
                  goto home;
               }
               else if(call == 2){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("menu.bmp");
                  goto menup;
               }
               else if(call == 4){
                  printf("%d\n", call);
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto menup;
               }
            
            case 2 :
               Pic("volup.bmp");
               volupp:
               ledOnOff(correct,1);
               call = button();
            
               if(call == 1){
                  goto home;
               }
               else if(call == 2){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("volup.bmp");
                  goto volupp;
               }
               else if(call == 5){
                  printf("%d\n", call);
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto volupp;
               }
         
            case 3 :
               Pic("voldown.bmp"); 
               voldownp :
               ledOnOff(correct,1);
               call = button();
            
               if(call == 1){
                  goto home;
               }   
               else if(call == 2){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("voldown.bmp"); 
                  goto voldownp;
               }
               else if(call == 6){
                  printf("%d\n", call);
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto voldownp;
               }
            
            case 4 :
               Pic("up.bmp"); 
               upp :
               ledOnOff(correct,1);
               b = touch();
            
               if((b.x > 0 && b.x < 200) && (b.y > 400 && b.y < 600)){
                  goto home;
               }   
               else if((b.x > 0 && b.x < 200) && (b.y > 0 && b.y < 200)){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("up.bmp"); 
                  goto upp;
               }
               else if((b.x > 824 && b.x < 1024) && (b.y > 150 && b.y < 300)){
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto upp;
               }
            
            case 5 :
               Pic("down.bmp"); 
               downp:
               ledOnOff(correct,1);
               c = touch();
            
               if((c.x > 0 && c.x < 200) && (c.y > 400 && c.y < 600)){
                  goto home;
               }   
               else if((c.x > 0 && c.x < 200) && (c.y > 0 && c.y < 200)){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("down.bmp"); 
                  goto downp;
               }
               else if((c.x > 824 && c.x < 1024) && (c.y > 300 && c.y < 450)){
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto downp;
               }
            
            case 6 :
               Pic("left.bmp"); 
               leftp :
               ledOnOff(correct,1);
               d = touch();
            
               if((d.x > 0 && d.x < 200) && (d.y > 400 && d.y < 600)){
                  goto home;
               }   
               else if((d.x > 0 && d.x < 200) && (d.y > 0 && d.y < 200)){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("left.bmp"); 
                  goto leftp;
               }
               else if((d.x > 824 && d.x < 1024) && (d.y > 450 && d.y < 600)){
                  correct++; 
                  continue;
               }   
               else{
                  wrong();
                  goto leftp;
               }
         
            case 7 :
               Pic("right.bmp"); 
               rightp :
               ledOnOff(correct,1);
               e = touch();
         
               if((e.x > 0 && e.x < 200) && (e.y > 400 && e.y < 600)){
                  goto home;
               }   
               else if((e.x > 0 && e.x < 200) && (e.y > 0 && e.y < 200)){
                  Pic("pause.bmp");
                  usleep(1000*5000);
                  Pic("right.bmp"); 
                  goto rightp;
               }
               if((e.x > 824 && e.x < 1024) && (e.y > 0 && e.y < 150)){
                  correct++; 
                  continue;
               }
               else{
                  wrong();
                  goto rightp;
               }
            
            case 8 :
               Pic("ball.bmp");
               Gyro();
               correct++; 
               ledOnOff(correct,1);
            }   //switch
      }   //for
   }
   
   if(call == 3){
      timer = 0;
      num = 3;
      ledOnOff(correct,1);
      
      pthread_create (&fnd, NULL,  &fndcount, NULL);
      
      for(int i=0; i < 10; i++){
         linenum1("stage1");
            linenum2("start!!!!");
		
