#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

extern "C"{
#include "all_headers.h"	
}

using namespace std;

class deviceControler{
private:
	
	
public:
	deviceControler();
	void screenInit();
	int printRecipe();
	int rfidScan();
	int rfidMemrite();
	int ertErrDevLogs();
	int magCardScan();
	
};




#endif
