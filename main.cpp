#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	int fd;
//	networkControler* network = new networkControler();
//	deviceControler* device = new deviceControler();
//	device->rfidScan();
//	device->atc24Read();
//	device->sleRead();
//	device->magCardScan();
	//pid_t childpid;
//	clone(master->masterBackground(), child_stack, CLONE_VM, NULL);
	char bufer[6];
	masterControler* master = new masterControler(&fd);
	pid_t pID = fork();
	if(pID == 0) /* child */
	{
		while(1)
		{
			int num;
			char bufer[6];
			fd = open(FIFO_NAME, O_RDONLY | O_NDELAY);
			
			cout << "Jestem w procesie dziecku" << endl;
			cout << "adres FD: " << &fd << endl;
			cout << "zawartosc FD" << fd << endl;
		
			sleep(5);
			cout << "skonczylem czekac sprawdzem co jest w fifo." << endl;
			do
			{	
				num = read(fd, bufer, 6);
				if(strcmp(bufer, "send\n") == 0)
				{
					cout << "Udalo sie str jest taki sam wysylam." << endl;
					master->masterBackground();	
				}
			}while(num > 0);
		}
	}
	else if (pID < 0)/* failed */
	{
		cout << "failed to rok !" << endl;
	}
	else /*parent */
	{
		sleep(2);
		fd = open(FIFO_NAME, O_WRONLY | O_NDELAY);
		while(1)
		{
				master->dispMenu();
		}
	}
	
	return 0;
}
