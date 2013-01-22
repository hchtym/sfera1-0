#include <sched.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "deviceControler.h"
#include "masterControler.h"
#include "networkControler.h"
#include "configControler.h"

#define SHMSZ     27

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
	int shmid;
	key_t klucz;
	char *shm, *s;
	void *data;
//	networkControler* network = new networkControler();
//	deviceControler* device = new deviceControler();
//	device->rfidScan();
//	device->atc24Read();
//	device->sleRead();
//	device->magCardScan();
	//pid_t childpid;
//	clone(master->masterBackground(), child_stack, CLONE_VM, NULL);
	char bufer[6];
	masterControler* master = new masterControler(*&shm);
	pid_t pID = fork();
	if(pID == 0)
	{
		sleep(5);
		cout << "Jestem w procesie dziecku" << endl;
		klucz = 5678;
		cout << "klucz: " << klucz << endl;
	if ((shmid = shmget(klucz, SHMSZ, 0666)) < 0) 
	{
	        perror("shmget");
			cout << "errr w shmid !!" << endl;
	        exit(1);
	}
	cout << "jestem za shmid czy cus" << endl;
	if ((data = shmat(shmid, NULL, 0)) == (char *) -1) 
	{
		cout << "error w shmat !" << endl;
	        perror("shmat");
	        exit(1);
	}
	cout << "Jestem przed while za shmat..." << endl;
		while(1)
		{
			cout << "sleep na 3min" << endl;
			sleep(3000);
			cout << "Sprawdzam " << endl;
				memset(bufer, 0, sizeof(bufer));
				int i =0;
			for (s = (char *) data; *s != '\n'; s++)
			{
				cout << "czytam z rury" << endl;
				bufer[i] = *s;
				cout << bufer[i] << endl;
				i++;
			}  
				string msg = bufer;
				cout << "tak wyglada msg: " << msg << endl;
				master->masterBackground(msg);
			
		}
	}
	else if (pID < 0)
	{
		cout << "failed to rok !" << endl;
	}
	else
	{
		klucz = 5678;
		
		if ((shmid = shmget(klucz, SHMSZ, IPC_CREAT | 0666)) < 0) {
	        perror("shmget");
	        exit(1);
	    }

	    if ((data = shmat(shmid, NULL, 0)) == (char *) -1) {
	        perror("shmat");
	        exit(1);
	    }
		s = shm;
		
		
		while(1)
		{
				master->dispMenu();	
		}
	}
	
	return 0;
}
