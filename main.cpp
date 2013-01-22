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
	struct Transaction {
        unsigned char factorySN[20]; //15 znakow i 0
        unsigned char sellerLogin[6];  // 5 znakow i 0
        unsigned char cid[25];
        unsigned long amount; // 4
        long points; // 4
        unsigned long extra; // 4
        Date datetime; // 7
        unsigned char type; // 0 - BUY, 1 - RETURN
    };
                                                                                                                                                                                   
    struct Date {
        unsigned char Second;
        unsigned char Minute;
        unsigned char Hour;
        unsigned char Day;
        unsigned char Month;
        unsigned char Year;
        unsigned char GTM;
    };

    cout << "rozmiar struct Date: " << sizeof(struct Date) << endl;
    cout << "rozmiar strucn transaction: " << sizeof(struct Transaction) << endl;

    cout << "rozmiar int: " << sizeof(int) << endl;
    cout << "rozmiar long: " << sizeof(long) << endl;
    cout << "rozmiar long int: " << sizeof(long int) << endl;
	cout << "rozmiar unsigned long: " << sizeof(unsigned long) << endl;
	cout << "rozmiar unsigned long int" << sizeof(unsigned long int) << endl;
    cout << "rozmiar short: " << sizeof(short) << endl;
    cout << "rozmiar short int: " << sizeof(short int) << endl;
	cout << "roamiar unsigned short int: " << sizeof(unsigned short int) << endl;
	cout << "rozmiar unsigned int: " << sizeof(unsigned int) << endl;
	cout << "rozmiar unsigned char: " << sizeof(unsigned char) << endl;
	
	
	
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
		//memset(data, 0, sizeof(data));
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
	cout << "adres data dziecka: " << &data << endl;
	cout << "Jestem przed while za shmat..." << endl;
		while(1)
		{	
			cout << "sleep na 3min" << endl;
			sleep(180);
			cout << "Sprawdzam " << endl;
				memset(bufer, 0, sizeof(bufer));
				int i =0;
				cout << sizeof(data) << endl;
				int len = strlen((char *)data);
				cout << len << endl;
			if( len > 0){
				for (s = (char *) data; *s != '\n'; s++)
				{
					cout << "czytam z rury" << endl;
					bufer[i] = *s;
					cout << bufer[i] << endl;
					i++;
				}  
			}
				string msg = bufer;
				cout << "tak wyglada msg: " << msg << endl;
				master->masterBackground(msg);
			//	memset(data, 0, sizeof(data));
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
		cout << "adres data rodzica: " << &data << endl;
		
		while(1)
		{
				master->dispMenu();	
		}
	}
	
	return 0;
}
