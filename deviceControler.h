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
	void hexToString(char *str, BYTE* buf, int len);
	
public:
	deviceControler();
	int printRecipe();
	int rfidScan();
//	int rfidMemWrite();
	int atc24Read();
	int sleRead();
	int chipCardScan();
	int ertErrDevLogs();
	char magCardScan(bool kbd, char btrck);
	
};




#endif
