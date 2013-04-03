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
//private:
	int left1, left2;
	bool confFlag;
	bool txFlag;
	bool timer1;
	bool timer2;
	int logoutTimer;
	bool sellerLogout;
	int *fifoContainer;
	string buildDate; //data kompilacji jednoczesnie wersja softu !
	string seller; //id sprzedawcy ustawiane przy zalogowaniu sie sprzedawcy do terminala
	int dispMenu2(); // drugie menu do wyswietlania serwisow i innych bajerow  
	int dispCPartFile(); // wyswietlanie pliku konfiguracyjnego
	int dispInfo(); // wyswietlanie informacji
	void wrtErrLogs(); // zapisywanie logow 
	int loginWindow(); // okno logowania 
	int sumInput(string &payment);
	int pointComp(string &id, string &payment, string &pnt, string &ext);
	void recipePrint(bool recipeCopy);
	void menuOnOff();
	void menuShop();
	void wrtFifo();
	bool senttrx;
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
	int menuScrOther(const string &title, vector<string> &vect, int size, int index, int *menuid);
	void screenSaver();
	
	
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
	
	struct Date {
			unsigned char Second;
			unsigned char Minute;
			unsigned char Hour;
			unsigned char Day;
			unsigned char Month;
			unsigned char Year;
			unsigned char GTM;
	};
	
	Date acttime;
	Transaction tx;
	
	bool loginFlag;
	bool timeoutFlag;
	int fileSave(string &sn, string &seler, string &client, string &pay, string &point, string &extrapoint, char type,string &date);
	int title(string str);
	int masterTitle(string str);
	int clear();
	int infoMSG(string &msg);
	int message(int x, int y, string &str);
	int checkPoints();
	int transSelling(int ret, char *track1, char *track2, char *track3);
	int transSelling(string hexNum);
	int serviceLogin();
	int selling();
	int returnSelling();
	int menuService();
	void gprsConfMenu();
	string computeTrxNumber(string trxDate);
	string trxTime();
	
public:
	masterControler(int *);
	masterControler();
	void gprsConfManual();
	string returnBuildDate();
	int checkVersion();
	int reloadConfig();
	bool sendTrx();
	int dispMenu();
	int updClk();
	int loginScr();
	void timeWindow();
	void softUpdAck();
	void masterBackground();
	
};




#endif
