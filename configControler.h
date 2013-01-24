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
	ConfigFile *cf;
	ConfigFile *sn;
	string flag;
	vector<string> options;
	vector<string> confOptions;
	//koniec ekranow
	int confCounter(char *str);
	//zmienne glopbalne obieku (dostepne tylko dla obieku !)
	string ip, port, apn, user, password, seriallnumber;
	///
	
public: 
	configControler(); 
	int configReload(); 
	int getConfig(); 
	int returnMenu(vector<string> &vec); 
	int returnPrize(vector<string> &vec); 
	int returnSeller(vector<string> &vec); 
	int returnLCard(vector<string> &vec); 
	int returnComputation(vector<string> &vec);
	string returnParam(char part);
	string returnGprsApn(); 
	string returnGprsUser();
	string returnGprsPaswd();
	string returnSerwerIp();
	string returnSerwerPort(); 
	int returnTime(); 
	int returnVersion(); 
	int returnServiceCode();
	string returnSeriall();
	int confParse(vector<string> &vect, string section);
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	
};




#endif
