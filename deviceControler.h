#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include "masterConrtoler"
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
	int lineWidth;
	void hexToString(char *str, BYTE* buf, int len);

	bool isPrinterReady();

	void center(char* to, const char* from);

	void justify(char* to, const char* left, const char* right);

	void printerInit();

	char* formatAmount(unsigned long a, char *temp);

	void printLines(int amount);



public:
	deviceControler();
	void printTotal(char *cid, int points, bool copy);

	void printTx();
	void printSend();
	void checkPoint();
	void printerHeader(string seriallNr, string sellerId, string date, string cid);
	void printTrx();
	void 
	int printRecipe();
	int rfidScan();
	int atc24Read();
	int sleRead();
	int chipCardScan();
	int ertErrDevLogs();
	int printerDev();
	string magCardScan(bool kbd);
	
};




#endif
