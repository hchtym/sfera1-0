#include "masterControler.h"

using namespace std;

masterControler::masterControler(){
	configControler* config = new configControler();
	deviceControler* device = new deviceControler();
	
};

masterControler::test(){
	BYTE input[31];
	memset(input, 0 , siezefo(input));
	stringstream compose;
	string str;
	Lcd_Cls();
	Lcd_Printxy(0,0,0, "Przeciagnij karte");	
	while(1){

		device->magCardScan(compose);
		str = compose.str();
		compose.str("");
		if(str.size() > 0 ){
			break;
		}
		Kb_GetStr(0, 24, input, 0, 30, 0, 120);
		string conv = (char* )input;
		if(conv.size()>6 && conv.size() < 30){
			break;
		}
	}
	if(str.size() > 0){
		pointEqu(str);
	}else{
		if(conv.size() > 0){
			pointEqu(conv);
		}
	}
	
}

masterControler::pointEqu(string &str){
	
	stringstream compo;
	string str2,temp;
	while(1){
		Lcd_Cls();
		Lcd_Printxy(0,0,0, "Podaj wartosc zakupu:");
		while(1){
			if(Kb_Hit){
				key = Kb_GetKey();
				if(key != NOKEY){
					break;
				}
			}
		}
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

			break;
			case KEYCANCEL:
			return 0;
			break;
			case KEYBACKSPACE:
//				compo.seekg(0, ios::end);
			temp.clear();
			temp = compo.str();
//				temp[temp.size()-1] = "\0";
			int len = temp.size();
			temp = temp.erase(len-1);
			compo.str("");
			compo.clear();
			compo << temp;
			str2.clear();
			str2 = temp;
			break;
			default:
			break;
		}
		if(str2.size() < 2){
			switch(str2.size()){
				case 1:
				compo.str("");
				compo << "0.0" << str2;
				str2.clear();
				str2 = compo.str();
				break;
				
				case 2:
				compo.str("");
				compo << "0." << str2;
				str2.clear();
				str2 - compo.str();
				break;
				default:
				compo.str("");
				compo << "0.00";
				str2.clear();
				str2 = compo.str();
				break;
			}
			
		}else{
			for(int i=0; i < str2.size(); i++){
				if(i == str2.size()-2)
				{	
					compo << "." << str2[i];					
				}else{
					compo << str2[i];
				}	
			}
			
		}
		Lcd_Printxy(0,32,0, const_cast<char *>(str2.c_str()) );

	}
	
	
	
	
}
