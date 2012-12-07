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
	int miniScreen(const *string title, int size, bool opt);
	int miniInput();
	string flag;
	vector<string> options;
	vector<string> confOptions;
	//koniec ekranow

	//zmienne glopbalne obieku (dostepne tylko dla obieku !)
	string ip, port, apn, user, password;

	
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
