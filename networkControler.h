#ifndef _NETWORK_CONTROLER_H
#define _NETWORK_CONTROLER_H

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <unistd.h>
#include "configControler.h"
//#include "ftplib.h"

extern "C"{
#include "all_headers.h"	
}

using namespace std;

struct Date {
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
	unsigned char GTM;
		//short Year;
		//short Month;
		//short Day;
		//short Hour;
		//short Minute;
		//short Second;
		//short GTM;
};

struct Transaction {
    unsigned char factorySN[20]; //15 znakow i 0
    unsigned char sellerLogin[6];  // 5 znakow i 0
    unsigned char cid[25];
    unsigned long amount; // 4
    long points; // 4
    unsigned long extra; // 4
    Date datetime; // 7
    unsigned char type; // 0 - BUY, 1 - RETURN
};


class networkControler{
private:

	// zmienna zawieajaca dane do pobieranie konfiguracji wykorzystywane w 2 metodach;
	static const char* const configs[2][6];
	// funkcje wykorzystywane tylko raz przy pierwszym pobieraniu konfiguracji 
	int gprsCon();
	int ethConf();
	int sockfd;

	// inicjator karty sim, oraz kanalu
	void gprsInit();
	int connectAllQuiet();
	int disconnectAllQuiet();
	int fileSize(string fileName);
	//Funkcja parsujaca pobrany config ! 
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	void catFile();
	int softUpdate(string data);
	//zmienne do eth/gprs
	string ip, port, apn, user, password, serialN;
	//zmienna kanalu w jakim dziala gprs
	static const int chanell = 115200;
	static const int socket0 = 0;
	bool gprs_apnConnected;
	bool gprs_serverConnected;
	configControler* config;
	//ftplib * ftp;
	Date acttime;
	Transaction tx;
	
public:
	networkControler(string &ipr, string &portr, string &apnr, string &userr, string &passwordr, string &serialNr);
	~networkControler();
	int softAck(string date);
	int sendAck(string date);
	int startConf(int type);
	int updConf();
	int updClock();
	int sendTrx();
	int updClk();
	int updSoft();
	int sendLogs();
	int wrtConLogs();
	int gprsConnect();
	int getPointState();
	int checkSignalStr();
	int sendTransaction();
	int checkTotalPoints();
	int getPointStatus(string cid);

	
};




#endif
