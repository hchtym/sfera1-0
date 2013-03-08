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
	cout << "tworze configControler" << endl;
	configControler* config = new configControler(true);
	Lcd_Cls();
	Lcd_Printxy(0,0,0,"Ladowanie systemu");
	Lcd_Printxy(0,55,0,"Prosze czekac...");
	//Lcd_Logo();

	// ustalam ikonke zasiegu 
	for(int i = 0; i < 100; i++)
	{
		int ret,sig;
		sig = -1;
		ret = Wls_CheckSignal(&sig);
		switch(sig){
			case NO_SIGNAL:
			Lcd_Icon(1, ICON_ON, 1);
			break;
			case SIGNAL_VERY_WEAK:
			Lcd_Icon(1, ICON_ON, 2);
			break;
			case SIGNAL_NORMAL:
			Lcd_Icon(1, ICON_ON, 3);
			break;
			case SIGNAL_STRONG:
			Lcd_Icon(1, ICON_ON, 4);
			break;
			case SIGNAL_VERY_STRONG:
			Lcd_Icon(1, ICON_ON, 5);
			break;
			default:
			Lcd_Icon(1, ICON_OFF, 1);
			break;
		}
		DelayMs(250);
	}

	//nawizauje polaczenie z siecia 
	if(Wls_CheckPPPLink(45))
	{
		Lcd_Cls();
		Lcd_Printxy(0,8,0, "Brak połączenia z internetem.");
		Lcd_Printxy(0,32,0, "Wszystkie funkcje sieciowe zostana odłączone.");
	}

	Lcd_Cls();
	Lcd_Printxy(0,0,0,"Ladowanie systemu");
	Lcd_Printxy(0,55,0,"Prosze czekac...");
	

	cout << "kiluje obiekt" << endl;
	sleep(20);


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


		//master->updClk();
		master->checkVersion();

		while(1)
		{
			if(Wls_CheckPPPLink(45))
			{
				sleep(45);
			}
			else
			{
				sleep(20);
				do
				{	
					num = read(fd, bufer, 6);
					if(strcmp(bufer, "send\n") == 0)
					{
						cout << "dostalem polecenie sen !!" << endl;
						master->masterBackground();	
					}
					memset(bufer, 0, sizeof(bufer));

				}while(num > 0);
				cout << "jestem przed timeWindow" << endl;
				master->timeWindow();
			}
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
