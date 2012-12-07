#include "configControler.h"
#include "networkControler.h"

//definicje guzikow klawiatury

#define KEYCANCEL  0x1B
#define KEYENTER  0x0D 
#define KEYUP  0x3C 
#define KEYDOWN  0x3E
#define NOKEY 0xFE
#define KEYFUN 0x12

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
			item = miniScreen(&title, 2, true);
			switch(item){
				case 0:
				flag = "eth";
				ret = miniScreen(&title, 2, false);
				break;
				case 1:
				flag = "gprs";
				ret = miniScreen(&title, 5, false);
				break;
			}
	}
	string apnc = "erainternet";
	string userc = "erainternet";
	string passwordc = "";
	networkControler* conector = new networkControler(ip, port, apnc, userc, passwordc);
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

int configControler::miniScreen(const *string title, int size, bool opt){
	int	marked =0;
	BYTE key;
	if(opt == true){
		while(1){
			key = NOKEY;
			Lcd_Cls();
			Lcd_Printxy(0, 0, 1, const_cast<char *>(title->c_str())); //tytul
			// wyswietlanie menu
			for(int i=0; i < size; i++){
				string str = options[i];
				if(marked == 0){
					Lcd_Printxy(0, (i*8)+8, 1,const_cast<char *>(str.c_str()));
				}else{
					Lcd_Printxy(0, (i*8)+8, 0, const_cast<char *>(str.c_str()));
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
					if(marked > 0) {
						marked++;
						if(marked > 1) marked =0;
					}else{
						marked++;
					}
				break;
				case KEYUP:
					if(marked <1 ){
						marked--;
						if(marked < 0) marked =1;
					}else{
						marked--;
					}
				break;
				case KEYENTER:
					return marked;
				break;
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
			Lcd_Printxy(0, 0, 1, const_cast<char *>(title->c_str())); //tytul
			// wyswietlanie menu
			for(int i= 0; i < size ; i++){
				string str = confOptions[i];
				if(marked == 0){
					Lcd_Printxy(0, (i*8)+8, 1, const_cast<char *>(str.c_str()) );
				}else{
					Lcd_Printxy(0, (i*8)+8, 0, const_cast<char *>(str.c_str()) );
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
					if(marked > 0) {
						marked++;
						if(marked > size-1) marked =0;
					}else{
						marked++;
					}
				break;
				case KEYUP:
					if(marked <1 ){
						marked--;
						if(marked < 0) marked =size -1;
					}else{
						marked--;
					}
				break;
				case KEYENTER:
					enter = true;
				break;
				case KEYFUN:
					return 10;
				break;
				default:
					key=NOKEY;
				break;
			}
			if(enter == true){
				switch(marked){
					case 0:
						miniInput();
					break;
					case 1:
						miniInput();
					break;
					default:
					break;
				}
			}
			enter = false;
			
		}		
	
	
		
	}
	
}

int configControler::miniInput(){
	
	
	
}

int configControler::configReload(){

	
	
}
