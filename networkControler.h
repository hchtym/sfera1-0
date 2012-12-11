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
	int ethCon();
	// inicjator karty sim, oraz kanalu
	void gprsInit();
	
	//Funkcja parsujaca pobrany config ! 
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	
	//zmienne do eth/gprs
	string ip, port, apn, user, password, serialN;
	
	
public:
	networkControler(string &ipr, string &portr, string &apnr, string &userr, string &passwordr, string &serialNr);
	~networkControler();
	int startConf(int *type);
	int sendTransaction();
	int updClk();
	int updConf();
	int updSoft();
	int sendLogs();
	int wrtConLogs();
	int checkSignalStr();
	int checkTotalPoints();
	int dummy();
	
};




#endif
