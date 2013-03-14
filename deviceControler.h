#ifndef _DEVICE_CONTROLER_H
#define _DEVICE_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include "configControler.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
//#include "internal.h"

extern "C"{
#include "all_headers.h"	
}

using namespace std;

class deviceControler{
private:
	char rfidId[20];
	configControler *config;
	string rfidPasswd;
	BYTE rfidPass[20]; 
	BYTE rfidSerialNo[100];
	BYTE rfidIdBufer[100];
	BYTE rfidData[100];
	int lineWidth;
	void hexToString(char str, BYTE* buf, int len);
	string rfidType;
	BYTE rfidBuffer[16];

	bool isPrinterReady();

	void center(string &cent);

	void justify(char* to, const char* left, const char* right);

	void printerInit(int size);

	char* formatAmount(unsigned long a, char *temp);

	void printLines(int amount);
	void printBold(int mode);
	void decHex(char *buf[100], char *hex[100]);
	void hexDec(char *buf[100], char *hex[100]);





public:
	deviceControler();
	void printTotal(char *cid, int points, bool copy);
	int reloadConfig();
	void printTx(string seriallNr, string sellerId, string date, string cid, string sum, string point, string extra, string footer, string trxNumber);
	void printSend(string seriallNr, string date, string sendTrx, string footer);
	void checkPoint(string seriallNr, string sellerId, string date, string cid, string point,string footer);
	void printerHeader(string seriallNr, string sellerId, string date, string cid);
	void printerHeaderLesser(string seriallNr, string date);
	void printRecipe();
	void printerSetFont(int size);
	int tagScan();
	int rfidSilentScan();
	int rfidScan();
	int rfidWrite(string input);
	int rfidWrite();
	int rfidDisplay();
	int rfidRead();
	int atc24Read();
	int sleRead();
	int chipCardScan();
	int ertErrDevLogs();
	int printerDev();
	string magCardRfidScan(bool kbd);
	string rfidRetrunStringId();
	
};

#endif
