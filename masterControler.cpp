#include "masterControler.h"
#include "configControler.h"
#include "deviceControler.h"
#include <string>

using namespace std;



masterControler::masterControler(){
//	config = new configControler();
	device = new deviceControler();
	
};

int masterControler::test(){
	unsigned char input[40];
	string id;
	memset(input, 0 , sizeof(input));
	stringstream compose;
	string str,conv;
	
	Lcd_Cls();
	Lcd_Printxy(0,0,0, "Przeciagnij karte");	
	while(1){
		cout << "tu doszedlem " << endl;
		//id = device->magCardScan();
		//compose << id; 
		cout << "tu jestem trololololololololo" << endl << compose.str() << endl;
		//str = id; //compose.str();
		cout << str << endl;
		compose.str("");
		if(str.size() > 0 ){
			break;
		}
		cout << "jestem przed Kb_GetStr" << endl;
		Kb_GetStr(0, 24, input, 0, 30, 0, 120);
		cout << "jestem za Kb_GetStr" << endl;
		//conv = (char* )input;
		char *strg = (char *)input;
		conv = strg;
		if(conv.size()>6 && conv.size() < 30){
			break;
		}
	}
	cout << "przed suminput" << endl;
	sumInput();
//	pointComp();
	delete compose;
	delete &str;
	delete &conv;
	delete &id;
	
}

int masterControler::sumInput(){
	BYTE key = NOKEY;
	stringstream compo,compo1;
	string temp;
	string str2 = "0.00";
	cout << "wszedlem do pointEqu" << endl;
	while(1){
		compo1.str("");
		for(int i = 0; i < (21 - str2.size()); i++)
		{
			compo1 << " ";
		}
		compo1 << str2;
		str2.clear();
		str2 = compo1.str();
		Lcd_Cls();
		Lcd_Printxy(0,0,0, "Podaj wartosc zakupu:");
		Lcd_Printxy(0,32,0, const_cast<char *>(str2.c_str()) );
		cout << "sprawdzam klawisz ktory wcisnalem" << endl;
		while(1){
			if(Kb_Hit){
				key = Kb_GetKey();
				if(key != NOKEY){
					break;
				}
			}
		}
		cout << "jestem przed switchem klawiszy" << endl;
		switch(key){
			case KEY0:
				compo << "0";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY1:
				compo << "1";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY2:
				compo << "2";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY3:
				compo << "3";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY4:
				compo << "4";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY5:
				compo << "5";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY6:
				compo << "6";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY7:
				compo << "7";
				str2.clear();
				str2 = compo.str();
			break;
			case KEY8:
				compo << "8";
				str2.clear();
				str2 = compo.str();
			break;
				case KEY9:
				compo << "9";
				str2.clear();
				str2 = compo.str();
			break;
			case KEYENTER:
			if(str2.size() > 0){
				sum = str2;
			return 0;
			}
			break;
			case KEYCANCEL:
			return 0;
			break;
			case KEYBACKSPACE:
			temp.clear();
			temp = compo.str();
//				temp[temp.size()-1] = "\0";
			int len = temp.size();
			if(len == 0){
				str2.clear();
			//string ma dlugosc 0 i dupa ! 
				break;
			}else{
			temp = temp.erase(len-1);
			compo.str("");
			compo.clear();
			compo << temp;
			str2.clear();
			str2 = temp;
			}
			break;
			default:
			break;
		}
		cout << str2 << endl;
		cout << "jestem za switchem sprawdzam dlugosc !" << endl;
		if(str2.size() <= 2){
			switch(str2.size()){
				case 1:
				compo1.str("");
				compo1 << "0.0" << str2;
				str2.clear();
				str2 = compo1.str();
				break;
				case 2:
				compo1.str("");
				compo1 << "0." << str2;
				str2.clear();
				str2 = compo1.str();
				break;
				default:
				compo1.str("");
				compo1 << "0.00";
				str2.clear();
				str2 = compo1.str();
				break;
			}
		}else{
			cout << "iteruje zapodane dane !" << endl;
			compo1.str("");
			int len = str2.size();
			for(int i=0; i < len; i++){
				if(i == len-2)
				{	
					compo1 << "." << str2[i];					
				}else{
					compo1 << str2[i];
				}	
			}
			cout << compo1.str();
			str2 = compo1.str();
		}
	}	
	delete compo;
	delete compo1;
	delete &temp;
	delete &str2;

}

int masterControler::pointComp(){
	BYTE key = NOKEY;
	stringstream compo, compo1;
	//string flag = config->pointComp(); 
	string flag = "off";
	string str2 = "0";
	string temp;
	string pointsExtra;
	string pointsTotal;
	if(flag.compare("off")){
		//reczne podawanie pkt !! 
		while(1){
			compo1.str("");
			for(int i = 0; i < (21 - str2.size()); i++)
			{
				compo1 << " ";
			}
			compo1 << str2;
			str2.clear();
			str2 = compo1.str();
			Lcd_Cls();
			Lcd_Printxy(0,0,0, "Podaj punkty extra");
			Lcd_Printxy(0,32,0, const_cast<char *>(str2.c_str()) );
			cout << "sprawdzam klawisz ktory wcisnalem" << endl;
			while(1){
				if(Kb_Hit){
					key = Kb_GetKey();
					if(key != NOKEY){
						break;
					}
				}
			}
			cout << "jestem przed switchem klawiszy" << endl;
			switch(key){
				case KEY0:
					compo << "0";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY1:
					compo << "1";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY2:
					compo << "2";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY3:
					compo << "3";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY4:
					compo << "4";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY5:
					compo << "5";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY6:
					compo << "6";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY7:
					compo << "7";
					str2.clear();
					str2 = compo.str();
				break;
				case KEY8:
					compo << "8";
					str2.clear();
					str2 = compo.str();
				break;
					case KEY9:
					compo << "9";
					str2.clear();
					str2 = compo.str();
				break;
				case KEYENTER:
				if(str2.size() > 0){
					pointsExtra = str2;
				}
				break;
				case KEYCANCEL:
				return 0;
				break;
				case KEYBACKSPACE:
				temp.clear();
				temp = compo.str();
				int len = temp.size();
				if(len == 0){
					str2 = "0";
					break;
				}else{
				temp = temp.erase(len-1);
				compo.str("");
				compo.clear();
				compo << temp;
				str2.clear();
				str2 = temp;
				}
				break;
				default:
				break;
			}
		
		}
	}else{
		//tu bedzie automatyczne liczenie pkt na podstawie zakresu zumy albo cos takiego sie jeszcze zobaczy 
	}
	
	delete compo;
	delete compo1;
	delete &temp;
	delete &str2;
	delete &pointsExtra;
	delete &pointsTotal;
	delete &flag;
}
