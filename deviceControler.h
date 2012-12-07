#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class configControler{
	
	
public:
	deviceControler();
	void screenInit();
	int printRecipe();
	int rfidScan();
	int rfidMemrite();
	int ertErrDevLogs();
	int magCardScan();
	
}




#endif
