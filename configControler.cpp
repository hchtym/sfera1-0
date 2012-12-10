#include "configControler.h"
#include "networkControler.h"

//definicje guzikow klawiatury

//#define KEYCANCEL  0x1B
//#define KEYENTER  0x0D 
//#define KEYUP  0x3C 
//#define KEYDOWN  0x3E
//#define NOKEY 0xFE
//#define KEYFUN 0x12

using namespace std;

configControler::configControler(){
	ifstream file("config.txt");
	if(file)
	{
		file.close();
		ConfigFile cf("config.txt");
		// dalszy uber magick z parsowanie plikow 
	
	
	
	
	
	
	}else{
		configGenerator();
	}
	
}

int configControler::configGenerator(){
	int item;
	string name = "Konfiguracja wstepna"; 
	stringstream compose;
	compose << name;
	options.push_back("Ethernet");
	options.push_back("GPRS");
	options.push_back("Seriall Number");
	confOptions.push_back("Ip");
	confOptions.push_back("Port");
	int len = name.size();
	for(int i=0; i<21-len; i++){
		compose << " ";
	}
	string title = compose.str();
	int ret;
	while(1){
		if(ret == 10) break;
		if(item == 10) break;
			item = miniScreen(title, 3, true);
			switch(item){
				case 0:
				flag = "eth";
				ret = miniScreen(title, 2, false);
				break;
				case 1:
				flag = "gprs";
				ret = miniScreen(title, 2, false);
				break;
				case 2:
					miniInput(options[2],seriallnumber);
				break;
				default:
				break;
			}
	}
	string apnc = "erainternet";
	string userc = "erainternet";
	string passwordc = "";
	networkControler* conector = new networkControler(&ip, &port, &apnc, &userc, &passwordc);
	if(flag.compare("eth") ==0){
		conector->startConf(0);
	}else{
		if(flag.compare("gprs") == 0){
			conector->startConf(1);
		}else{
			// loguj nieznany typ !
		}
	}
	delete conector;
	
}

int configControler::miniScreen(string &title, int size, bool opt){
	int	marked =0;
	BYTE key;
	bool ends;
	if(opt == true){
		while(1){
			key = NOKEY;
			Lcd_Cls();
			Lcd_Printxy(0, 0, 1, const_cast<char *>(title.c_str())); //tytul
			Lcd_Printxy(0, 8, 0, "---------------------");
			// wyswietlanie menu
			for(int i=0; i < size; i++){
				stringstream compo;
				compo << options[i];
				compo.seekg(0, ios::end);
				int len = compo.tellg();
				for(int i=0; i<21-len; i++){
					compo << " ";
				}
				string str = compo.str();				
				if(marked == i){
					Lcd_Printxy(0, (i*8)+16, 1,const_cast<char *>(str.c_str()));
				}else{
					Lcd_Printxy(0, (i*8)+16, 0, const_cast<char *>(str.c_str()));
				}
			}
			while(1){
				if(Kb_Hit){
					key = Kb_GetKey();
					if(key != NOKEY){
						break;
					}
				}
			}
			switch(key){
				case KEYDOWN:
					if(marked < size -1) {
						marked++;
						if(marked > size-1) marked =0;
					}else{
						marked = 0;
					}
				break;
				case KEYUP:
					if(marked > 0 ){
						marked--;
						if(marked < 0) marked =size -1;
					}else{
						marked = size -1;
					}
				break;
				case KEYENTER:
					return marked;
				break;
				break 
				default:
					key=NOKEY;
				break;
			}
		}
	}else{
	bool enter = false;		
		while(1){
			key = NOKEY;
			Lcd_Cls();
			Lcd_Printxy(0, 0, 1, const_cast<char *>(title.c_str())); //tytul
			Lcd_Printxy(0, 8, 0, "---------------------");
			// wyswietlanie menu
			for(int i= 0; i < size ; i++){
				stringstream compo;
				compo << options[i];
				compo.seekg(0, ios::end);
				int len = compo.tellg();
				for(int i=0; i<21-len; i++){
					compo << " ";
				}
				string str = compo.str();
				if(marked == i){
					Lcd_Printxy(0, (i*8)+16, 1, const_cast<char *>(str.c_str()) );
				}else{
					Lcd_Printxy(0, (i*8)+16, 0, const_cast<char *>(str.c_str()) );
				}
			}
			while(1){
				if(Kb_Hit){
					key = Kb_GetKey();
					if(key != NOKEY){
						break;
					}
				}
			}
			switch(key){
				case KEYDOWN:
					if(marked < size -1) {
						marked++;
						if(marked > size-1) marked =0;
					}else{
						marked = 0;
					}
				break;
				case KEYUP:
					if(marked > 0 ){
						marked--;
						if(marked < 0) marked =size -1;
					}else{
						marked = size -1;
					}
				break;
				case KEYENTER:
					enter = true;
				break;
				case KEYFUN:
					return 10;
				break;
				case KEYCANCEL:
				case KEYBACKSPACE:
					ends = true;
					enter = false;
				break;
				default:
					key=NOKEY;
				break;
			}
			if(enter == true){
				switch(marked){
					case 0:
						miniInput(confOptions[0], ip);
					break;
					case 1:
						miniInput(confOptions[1], port);
					break;
					default:
					break;
				}
			}
			enter = false;
			if(ends == true) break;
		}		
	
	
		
	}
	
}

int configControler::miniInput(string& title, string& variable){
	stringstream compo;
	unsigned char *input;
	compo << title;
	compo.seekg(0, ios::end);
	int len = compo.tellg();
	for(int i=0; i<21-len; i++){
		compo << " ";
	}
	string str = compo.str();
	
	Lcd_Cls();
	Lcd_Printxy(0, 0, 1, const_cast<char *>(str.c_str()) );
	if(title.compare("Ip") == 0){
		Lcd_Printxy(1, 11, 0, "Podawaj same cyfry.");
		Kb_GetStr(0, 20, input, 4, 12, 0, 60);
	
	}else{
		Kb_GetStr(0, 20, input, 4, 12, 0, 60);
	}
	
	if(title.compare("Ip") == 0) ip = input;
	
		if(title.compare("Port") == 0) port = input;
	
	if(title.compare("Seriall Number") == 0) seriallnumber = input;
	
	
}

int configControler::configReload(){

	
	
}
