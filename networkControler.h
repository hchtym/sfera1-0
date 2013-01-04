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

extern "C"{
#include "all_headers.h"	
}

using namespace std;


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
	int fileSize();
	//Funkcja parsujaca pobrany config ! 
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	
	//zmienne do eth/gprs
	string ip, port, apn, user, password, serialN;
	//zmienna kanalu w jakim dziala gprs
	static const int chanell = 115200;
	static const int socket0 = 0;
	bool gprs_apnConnected;
	bool gprs_serverConnected;

	struct Date {
			long Year;
			long Month;
			long Day;
			long Hour;
			long Minute;
			long Second;
	};
	
	struct Transaction {
	    BYTE factorySN[20]; //15 znakow i 0
	    BYTE sellerLogin[6];  // 5 znakow i 0
	    BYTE cid[25];
	    unsigned long amount; // 4
	    long points; // 4
	    unsigned long extra; // 4
	    Date datetime; // 7
	    BYTE type; // 0 - BUY, 1 - RETURN
	};
	Date acttime;
	Transaction tx;
	
public:
	networkControler(string &ipr, string &portr, string &apnr, string &userr, string &passwordr, string &serialNr);
	~networkControler();
	int startConf(int type);
	int gprsConnect();
	int sendTrx();
	int sendTransaction();
	int updClk();
	int updConf();
	int updSoft();
	int sendLogs();
	int wrtConLogs();
	int checkSignalStr();
	int checkTotalPoints();

	
};




#endif
