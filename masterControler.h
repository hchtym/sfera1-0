#ifndef _MASTER_CONTROLER_H
#define _MASTER_CONTROLER_H

#include "ConfigFile.h"
#include "Chameleon.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

extern "C"{
#include "all_headers.h"	
}

using namespace std;

class masterControler{
private:
	int dispMenu2(); // drugie menu do wyswietlania serwisow i innych bajerow  
	int dispCPartFile(); // wyswietlanie pliku konfiguracyjnego
	int screenSaver(); // wygaszacz ekranu
	int dispInfo(); // wyswietlanie informacji
	void wrtErrLogs(); // zapisywanie logow 
	int loginWindow(); // okno logowania 
	int PointEqu(string &str);
public:
	masterControler();
	int dispMenu();
	int test();
	
};




#endif
