#ifndef _CONFIG_CONTROLER_H
#define _CONFIG_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

extern "C"{
#include "all_headers.h"	
}

using namespace std;

class configControler{ 
private:
	int configGenerator();
	// Ekrany uzywane do konfiguracji przy pierwszym wlaczeniu terminala 
	int miniScreen(string &title, int size, bool opt);
	int miniInput(string &title, string &variable);
	string flag;
	vector<string> options;
	vector<string> confOptions;
	//koniec ekranow
	int confCounter(char *str);
	//zmienne glopbalne obieku (dostepne tylko dla obieku !)
	string ip, port, apn, user, password, seriallnumber;
	// 
	string lcb; //locked card behaviour
	string lcbmsg; // wiadomosc do zablokowanych kart
	string screntimeout; //czas do wygaszacza
	string logtime; // timeout na wylogowanie
	string servcode; //kod serwisowy
	string versiondate; // data wersji configa
	string prntconf; // drukowanie potweirdzenia
	string schconffailint; // interwal failu
	string schconfsucint; // interwal powodzenia
	string schconftimebeghr;
	string schconftimebehmin;
	string schconftimeendht;
	string schconftimeendmin;
	string schtxfailint;
	string schtxsucint;
	string schtxtimebeghr;
	string schtxtimebegmin;
	string schtxtimeendhr;
	string schtxtimeendmin;
	
public: 
	configControler(); 
	int configReload(); 
	int getConfig(); 
	int returnMenu(); 
	int returnPrize(); 
	int returnSeller(); 
	int returnLCard(); 
	int returnCmputation(); 
	int returnGPRSConf(); 
	int returnSerwerConf(); 
	int returnTime(); 
	int returnVersion(); 
	int returnServiceCode();   
};




#endif
