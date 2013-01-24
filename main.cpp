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

using namespace std;

int main(){
	SystemInit();
	Prn_Init();
//	Wls_Init();
	SYS_LoadDriver(DRIVER_CONTACT_MEMORY_CARD);
	SYS_LoadDriver(DRIVER_CONTACT_CPU_CARD);
	Lcd_Cls();
	Lcd_SetGray(50);
	Lcd_SetFont(1,0,0);
//	configControler* config = new configControler();
	//master->selling();
	//char c;	
	//int fd;
//	networkControler* network = new networkControler();
//	deviceControler* device = new deviceControler();
//	device->rfidScan();
//	device->atc24Read();
//	device->sleRead();
//	device->magCardScan();
	//pid_t childpid;
//	clone(master->masterBackground(), child_stack, CLONE_VM, NULL);
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
		while(1)
		{
		
			sleep(20);
			cout << "skonczylem czekac sprawdzem co jest w fifo." << endl;
			//fifo.open(FIFO_NAME, ios::out | ios::turnc);
			do
			{	
				num = read(fd, bufer, 6);
				//fifo.read();
				if(strcmp(bufer, "send\n") == 0)
				{
					cout << "Udalo sie str jest taki sam wysylam." << endl;
					master->masterBackground();	
				}
				memset(bufer, 0, sizeof(bufer));
				//fifo.close();
			}while(num > 0);
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

		cout << "(rodzic) adres FD: " << &fd << endl;
		cout << "(rodzic)zawartosc FD: " << fd << endl;
		while(1)
		{
				master->dispMenu();
		}
	}
	
	return 0;
}
