#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "deviceControler.h"
#include "masterControler.h"
#include "networkControler.h"
#include "configControler.h"

#define FIFO_NAME "fifo_file"

#define BUILD_DATE __DATE__ " " __TIME__

using namespace std;

int main(){
	SystemInit();
	Prn_Init();
	SYS_LoadDriver(DRIVER_CONTACT_MEMORY_CARD);
	SYS_LoadDriver(DRIVER_CONTACT_CPU_CARD);
	Lcd_Cls();
	Lcd_SetGray(50);
	Lcd_SetFont(1,0,0);

	//cout << "Build date: " << BUILD_DATE << endl;
	
	configControler* config = new configControler();
	delete(config);

	char bufer[6];
	pid_t pID = fork();
	if(pID == 0) /* child */
	{
		//ifstream fifo;
		mknod(FIFO_NAME, S_IFIFO | 0666, 0);
		int num;
		int fd;
		char bufer[6];
		fd = open(FIFO_NAME, O_RDONLY | O_NDELAY);
		//fifo.open(FIFO_NAME, ios::out | ios::turnc);
		masterControler* master = new masterControler();
		cout << "Child process is on the go" << endl;
		master->updClk();
		master->checkVersion();
		while(1)
		{

			sleep(20);
			do
			{	
				num = read(fd, bufer, 6);
				if(strcmp(bufer, "send\n") == 0)
				{
					master->masterBackground();	
				}
				memset(bufer, 0, sizeof(bufer));

			}while(num > 0);
			cout << "jestem przed timeWindow" << endl;
			master->timeWindow();
		}
	}
	else if (pID < 0)/* failed */
	{
		cout << "failed to rok !" << endl;
	}
	else /*parent */
	{
		int fd;
		mknod(FIFO_NAME, S_IFIFO | 0666, 0);
		sleep(3);
		fd = open(FIFO_NAME, O_WRONLY | O_NDELAY);

	masterControler* master = new masterControler(&fd);
	string dupa = master->returnBuildDate();
	cout << dupa << endl;
		while(1)
		{
				master->dispMenu();
		}
	}
	
	return 0;
}
