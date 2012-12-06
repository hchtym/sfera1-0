#ifndef _CONFIG_CONTROLER_H
#define _CONFIG_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>

class configControler{ 
	int configGenerator();
	// Ekrany uzywane do konfiguracji przy pierwszym wlaczeniu terminala 
	int miniScreen();
	int miniInput();
	string flag;
	//koniec ekranow

	//zmienne glopbalne obieku (dostepne tylko dla obieku !)
	string ip, port, apn, user, passrod;

	
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
}




#endif