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

int main()
{
	unsigned char lcdLogo[] = 
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7, 0x7, 0x0, 0x7, 0x80, 0x1E, 0xF, 0xE0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x1F, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7C, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0xF8, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF3, 0xE0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF7, 0xC0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0x0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0x0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0xC0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x7, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF7, 0xE0, 0xF, 0x80, 0x7, 0xC0, 0x1E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF1, 0xF8, 0xF, 0x80, 0x7, 0xC0, 0x1E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0xFC, 0xF, 0x80, 0x7, 0xE0, 0x3E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x3F, 0xF, 0xFF, 0xE3, 0xFF, 0xFC, 0x1F, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x1F, 0x8F, 0xFF, 0xE1, 0xFF, 0xF0, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7, 0xC7, 0xFF, 0xC0, 0x1F, 0x80, 0xF, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xC0, 0x3, 0x0, 0x80, 0xF, 0xF0, 0xF0, 0x2, 0x10, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1E, 0x38, 0x7, 0x0, 0xC0, 0x18, 0x0, 0xCF, 0x6, 0x10, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x30, 0x0, 0xD, 0x81, 0x80, 0x18, 0x1, 0x83, 0x86, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x60, 0x0, 0x19, 0x81, 0x80, 0x18, 0x1, 0x81, 0x84, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x0, 0x31, 0x81, 0x80, 0x38, 0x1, 0x87, 0xC, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0xF8, 0x60, 0xC1, 0x0, 0x3F, 0xE1, 0xF8, 0xC, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x18, 0x7F, 0xC3, 0x0, 0x30, 0x3, 0xC, 0xC, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x30, 0xC0, 0xC3, 0x0, 0x20, 0x3, 0x6, 0x18, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x70, 0x71, 0x80, 0x63, 0x0, 0x60, 0x3, 0x3, 0x18, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1F, 0xC3, 0x0, 0x63, 0xFE, 0x7F, 0xC6, 0x3, 0x18, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1F, 0xF8, 0x7F, 0xF1, 0xFF, 0xC3, 0xFF, 0x80, 0x1F, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3C, 0x70, 0xF0, 0x1, 0xE0, 0x7, 0x9F, 0xC0, 0x3F, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x78, 0x0, 0xF0, 0x3, 0xC0, 0x7, 0x83, 0xC0, 0x3B, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x7C, 0x0, 0xF0, 0x3, 0xC0, 0x7, 0x7, 0x80, 0x73, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3F, 0x1, 0xF0, 0x3, 0xFF, 0x8F, 0xF, 0x0, 0xE3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF, 0xC1, 0xFF, 0xC7, 0xFF, 0x8F, 0xFC, 0x1, 0xE3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3, 0xE1, 0xE0, 0x7, 0x80, 0xF, 0xFE, 0x3, 0xC1, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xF1, 0xC0, 0x7, 0x80, 0xE, 0x1F, 0x7, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xE3, 0xC0, 0x7, 0x0, 0x1E, 0xF, 0x87, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF3, 0xE3, 0xC0, 0xF, 0xFF, 0x1E, 0x7, 0x8E, 0x1, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x1, 0xFF, 0x83, 0xC0, 0xF, 0xFF, 0x1E, 0x7, 0x9E, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	};


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
	Lcd_Logo(lcdLogo,sizeof(lcdLogo),8);
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
		DelayMs(50);
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
	Lcd_Logo(lcdLogo,sizeof(lcdLogo),8);
	Lcd_Printxy(0,55,0,"Prosze czekac...");
	sleep(20);

	cout << "kiluje obiekt" << endl;
	//sleep(20);


	delete(config);
	RF_Close();
	RF_Init();
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
			if(false)
			{
				
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
	SetTimer(2, 2400000);
	string dupa = master->returnBuildDate();
	cout << dupa << endl;
		while(1)
		{
				master->dispMenu();
		}
	}
	
	return 0;
}
