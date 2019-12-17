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
		
