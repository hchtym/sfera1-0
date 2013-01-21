#include <sched.h>
#include "deviceControler.h"
#include "masterControler.h"
#include "networkControler.h"
#include "configControler.h"

using namespace std;

int main(){
	void *child_stack;
	child_stack = malloc(16384);
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
	
	clone(masterControler->masterBackground(), child_stack, CLONE_VM, NULL);
	
	while(1){
			master->dispMenu();

		
	}
	
	return 0;
}
