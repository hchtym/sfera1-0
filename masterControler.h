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
	bool loginFlag;
	int fileSave();
	void title(string str);
	void clear();
	int infoMSG();
	void msg();
	
public:
	masterControler();
	int dispMenu();
	int loginScr();
	int selling();
	
};




#endif
