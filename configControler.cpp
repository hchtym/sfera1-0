#include "configControler.h"
#include "networkControler.h";

//definicje guzikow klawiatury

#define KEYCANCEL  0x1B
#define KEYENTER  0x0D 
#define KEYUP  0x3C 
#define KEYDOWN  0x3E
#define NOKEY 0xFE
#define KEYFUN 0x12

typedef unsigned char BYTE;

configControler::configControler(){
	
	
}

int configControler::configGenetaror(){
	int item;
	string name = "Konfiguracja wstepna"; 
	stringstream compose;
	compose << name;
	int select 
	options.push_back("Ethernet");
	options.push_back("GPRS");
	confOptions.push_back("Ip");
	confOptions.push_back("Port");
	confOptions.push_back("Apn");
	confOptions.push_back("User");
	confOptions.push_back("Password");
	for(int i=0 i<21-name.size();i++){
		compose << " ";
	}
	string title = compose;
	int ret;
	while(1){
		if(ret == 10){
			break;
			item = miniScreen(title.c_str(), 2, true);
			switch(item){
				case 0:
				ret = miniScreen(title.c_str(), 2, false);
				break;
				case 1:
				ret = miniScreen(title.c_str(), 5, false);
				break;
			}
		}
	}
	

}

int configControler::miniScreen(char *title, int size, bool opt){
	int	marked =0;
	BYTE key;
	it(opt == true){
		while(1){
			key = NOKEY;
			Lcd_Cls();
			Lcd_Printxy(0,0,1,title); //tytul
			// wyswietlanie menu
			for(int i=0; i < size; i++){
				string str = options[i];
				if(marked == 0){
					Lcd_Printxy(0, (i*8)+8, 1, str.c_str());
				}else{
					Lcd_Printxy(0, (i**)+8, 0, str.c_str());
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
	}
	else{
	bool enter = false;	
	int marked =0;
	int chosen =0;
	if(size == 2)
	{
		chosen = 6;
		flag = "eth";
	}
	else{
		flag = "gprs";
	}
		while(1){
			key = NOKEY;
			Lcd_Cls();
			Lcd_Printxy(0,0,1,title); //tytul
			// wyswietlanie menu
			for(int i= 0; i < size ; i++){
				if(size == 2)
				{
					chosen = chosen +marked;
				}else{
					chosen = marked;
				}
				string str = confOptions[i];
				if(marked == 0){
					Lcd_Printxy(0, (i*8)+8, 1, str.c_str());
				}else{
					Lcd_Printxy(0, (i**)+8, 0, str.c_str());
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
				case KEYUN;
					return 10;
				break;
				default:
					key=NOKEY;
				break;
			}
			if(enter == true){
				switch(chosen){
					case 0:
						miniInput();
						enter = false;
					break;
					case 1:
						miniInput();
						enter = false;
					break;
					case 2:
						miniInput();
						enter = false;
					break;
					case 3:
						miniInput();
						enter = false;
					break;
					case 4:
						miniInput();
						enter = false;
					break;
					case 5:
						miniInput();
						enter = false;
					break;
					default:
					break;
				}
			}
			
		}		
	
	
		
	}
	
}

int configControler::miniInput(){
	
	
	
}

int configControler::configReload(){

	
	
}