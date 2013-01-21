#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include "deviceControler.h"
#include "masterControler.h"
#include "networkControler.h"
#include "configControler.h"

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
	masterControler* master = new masterControler();
	//master->selling();

//	networkControler* network = new networkControler();
//	deviceControler* device = new deviceControler();
//	device->rfidScan();
//	device->atc24Read();
//	device->sleRead();
//	device->magCardScan();
	
//	clone(master->masterBackground(), child_stack, CLONE_VM, NULL);
	
	pid_t pID = fork();
	if(pID == 0)
	{
		master->masterBackground();
	}
	else if (pID < 0)
	{
		cout << "failed to rok !" << endl;
	}
	else
	{
		while(1){
				master->dispMenu();	
		}
	}
	
	return 0;
}
