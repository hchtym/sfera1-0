#ifndef _MASTER_CONTROLER_H
#define _MASTER_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "configControler.h"
#include "deviceControler.h"
#include "networkControler.h"

extern "C"{
#include "all_headers.h"	
}

using namespace std;

class masterControler{
private:
	string seller; // id sprzedawcy ustawiane przy zalogowaniu sie sprzedawcy do terminala
	int dispMenu2(); // drugie menu do wyswietlania serwisow i innych bajerow  
	int dispCPartFile(); // wyswietlanie pliku konfiguracyjnego
	int screenSaver(); // wygaszacz ekranu
	int dispInfo(); // wyswietlanie informacji
	void wrtErrLogs(); // zapisywanie logow 
	int loginWindow(); // okno logowania 
	int sumInput(string &payment);
	int pointComp(string &id, string &payment, string &pnt, string &ext);
	string sum; // suma zakupow
	string points; // suma pkt za zakupy up 
	configControler* config;
	deviceControler* device;
	networkControler* network;
	string numerser;
	string ip;
	string port;
	string apn;
	string user;
	string paswd;
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	int menuScr(const string &title,vector<string> &vect, int size, int index, int *menuid);
	int menuScrOther(string &title);vector<string> &vect, int size, int index, int *menuid);
	
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
	
	struct Date {
			long Year;
			long Month;
			long Day;
			long Hour;
			long Minute;
			long Second;
	};
	
	Date acttime;
	Transaction tx;
	
	bool loginFlag;
	int fileSave(string &sn, string &seler, string &client, string &pay, string &point, string &extrapoint, char type,string &date);
	int title(string str);
	int clear();
	int infoMSG(string &msg);
	int message(int x, int y, string &str);
	
public:
	masterControler();
	int dispMenu();
	int loginScr();
	int selling();
	
};




#endif
