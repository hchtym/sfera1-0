#include "deviceControler.h"
#include "masterControler.h"
#include "networkControler.h"
#include "configControler.h"

using namespace std;

int main(){
	SystemInit();
	Prn_Init();
//	Wls_Init();
	Lcd_Cls();
	Lcd_SetGray(50);
	Lcd_SetFont(1,0,0);
//	configControler* config = new configControler();

//	networkControler* network = new networkControler();
	deviceControler* device = new deviceControler();
//		device->rfidScan();
	device->atc24read();
	while(1){


		
	}
	
	return 0;
}
