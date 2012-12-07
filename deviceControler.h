#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>

extern "C"{
#include "all_headers.h"	
}

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
