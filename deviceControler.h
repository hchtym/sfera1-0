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
	int lineWidth;
	void hexToString(char *str, BYTE* buf, int len);

	bool isPrinterReady();

	void center(char* to, const char* from);

	void justify(char* to, const char* left, const char* right);

	void formatCardNumber(char *cid, char *temp);
	char* formatAmount(unsigned long a, char *temp);

	void printLines(int amount);
	void printTicketHeader(const char* datetime, char *terminalId, char *sellerId, char *cardNumber1, char *cardNumber2,
								char *id, char *pin);
	void printTicketHeaderNoLogo(const char* datetime, char *terminalId, char *sellerId, char *cardNumber1,
											char *cardNumber2, char *id, char *pin);
	void printTicketFooter(bool copy);


public:
	deviceControler();
	void printTotal(char *cid, int points, bool copy);

	void printTxSentRaport(unsigned long txCount, unsigned long txSell);
	void printFailRaport(char *str1, char *str2, char *str3);
	void printScheduleRaport(
            const char* confWindowOpenedTime,
            const char* confWindowClosedTime,
            const char* txWindowOpenedTime,
            const char* txWindowClosedTime,
            unsigned int confTriesCount,
            unsigned int txTriesCount,
            unsigned int confSuccessCount,
            unsigned int txSuccessCount
            );

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
