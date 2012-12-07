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
	char * configs[][]={
	{
	"par.podst.", "zab.karty",
	"opcje menu", "nagrody",
	"sprzedawcy", "obliczenia"
	},{
	"ok", "lc",
	"menu", "prize",
	"seller", "comp"
	}
	};
	// funkcje wykorzystywane tylko raz przy pierwszym pobieraniu konfiguracji 
	int gprsCon();
	int ethCon();
	// inicjator karty sim, oraz kanalu
	void gprsInit();
	
	//Funkcja parsujaca pobrany config ! 
	void Tokenize();
	
	
public:
	networkControler(string *ip, string *port, string *apn, string *user, string *password);
	~networkControler();
	int startConf(int type);
	int sendTransaction();
	int updClk();
	int updConf();
	int updSoft();
	int sendLogs();
	int wrtConLogs();
	int checkSignalStr();
	int checkTotalPoints();
	
}




#endif
