#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
//#include "masterControler.h"
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

	void center(string &cent);

	void justify(char* to, const char* left, const char* right);

void printerInit(int size);

	char* formatAmount(unsigned long a, char *temp);

	void printLines(int amount);
	void printBold(int mode);



public:
	deviceControler();
	void printTotal(char *cid, int points, bool copy);

	void printTx(string seriallNr, string sellerId, string date, string cid, string sum, string point, string extra, string footer, string trxNumber);
	void printSend(string seriallNr, string sellerId, string date, string cid);
	void checkPoint();
	void printerHeader(string seriallNr, string sellerId, string date, string cid);
	void printRecipe();
	void printerSetFont(int size);
	int rfidScan();
	int atc24Read();
	int sleRead();
	int chipCardScan();
	int ertErrDevLogs();
	int printerDev();
	string magCardScan(bool kbd);
	
};




#endif
