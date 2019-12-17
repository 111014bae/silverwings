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
#include <dirent.h>
#include "textlcd.h"

#define TEXTLCD_DRIVER_NAME "/dev/peritextlcd"
static int fd = 0;
static stTextLCD  stlcd;

int textlcdLibInit(void) 
{
	memset(&stlcd,0,sizeof(stTextLCD));
	fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver (//dev//peritextlcd) open error.\n");
		return 1;
	}
}
void textlcdLine(unsigned int linenum, int len,char input[])
{
	if ( linenum == 1) // firsst line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	}
	else if ( linenum == 2) // second line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	}
	else 
	{
		return 1; 
	}
		if ( len > COLUMN_NUM)
	{
		memcpy(stlcd.TextData[stlcd.cmdData - 1],input,COLUMN_NUM);
	}
	else
	{
		memcpy(stlcd.TextData[stlcd.cmdData - 1],input,len);
	}
	stlcd.cmd = CMD_WRITE_STRING;
	write(fd,&stlcd,sizeof(stTextLCD));
}

int textlicdLibExit(void)
{
	close(fd);
}
