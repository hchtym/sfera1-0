#include "masterControler.h"
#include "configControler.h"
#include "deviceControler.h"
#include <string>

using namespace std;



masterControler::masterControler(){
	config = new configControler();
	device = new deviceControler();
	
};

int masterControler::test(){
	unsigned char input[40];
	//string id;
	memset(input, 0 , sizeof(input));
	stringstream compose;
	string str,conv;
		
	while(1){
		cout << "tu doszedlem " << endl;
		char id = device->magCardScan(true);
		//str = device->magCardScan(true);
		cout << device->magCardScan(false) << endl;
		cout << "Co miesci sie w id: " << id << endl;
		compose << id; 
		//compose << device->magCardScan(false);
		//str = id; //compose.str();
		str = compose.str();
		compose.str("");
		//cout << "tu jestem trololololololololo " << compose.str() << endl;
		cout << "tu jestem trololololololololo " << str << " : " << str.size() << endl;
		if(str.size() > 0 ){
			if(str.size()>6 && str.size() < 30){
				break;
			}
		}
	}
	cout << "przed suminput" << endl;
	sumInput();
	pointComp(str);

	
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
			cout << "nacisnalem enterem" << endl;
			if(str2.size() > 0){
				cout << "str2 jest wieksze od sera i przepisuje jego wartosc do sum !" << endl;
			sum = str2;
				cout << " przechodze do returna !!!! wowwwwww" << endl;
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

}

int masterControler::pointComp(string &id){
	int sumapkt;
	BYTE key = NOKEY;
	stringstream compo, compo1;
	//string flag = config->pointComp(); 
	string flag = "off";
	string str2 = "0";
	string temp;
	string pointsExtra;
	string pointsTotal;
	Kb_Flush();
		if((flag.compare("off")) == 0){
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
					goto obliczenia;
				}else{
					return 0;
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
		obliczenia:
		cout << "jestem sobie w obliczeniach muahahahah" << endl;
		vector<string> vect;
		config->confParse(vect, "comp");
		for(int i = 0; i < vect.size(); i++){
			vector<string> compos;
			string part = vect[i];
			cout << part << endl;
			config->Tokenize(part, compos, ":");
			compo1.str("");
			cout << "rozmiar kodu zczytanego z karty !: " <<  id.size() << endl;
			if(id.size() == 10)
			{
				// sprawdz pierwsze 3 cyfry i porownaj je z konfiguracja....
				string type;
				for(int i = 0; i < 3; i++)
				{
					compo1 << id[i];
					type = compo1.str();
				}
				string opt = vect[0];
				cout << "opcja do porownania ! : " << opt << endl;
				if((type.compare(opt))==0){
					//karta jest taka sama jak ta w konfiguracji basta ! 
					if((sum.size()-2) > 1){
						string interval = vect[3];
						interval.erase(interval.size()-1);
						interval.erase(interval.size()-2);
						string minimum = vect[1];
						minimum.erase(minimum.size()-1);
						minimum.erase(minimum.size()-2);
						string sumary = sum;
						sumary.erase(sumary.size()-1);
						sumary.erase(sumary.size()-2);
						int equation = atoi(sumary.c_str());
						string multip = vect[2];
						int mult = atoi(multip.c_str());
						int inter = atoi(interval.c_str());
						int min = atoi(minimum.c_str());
						string option = vect[4];
						if((option.compare("1"))==0){
							sumapkt = (equation/inter)*mult;
						}else{
							sumapkt = ( (equation - min) /inter)*mult;
						}
					}
					compo1.str("");
					compo1 << sumapkt;
					cout << sumapkt;
					points.clear();
					points = compo1.str();	
				}
			}else{
				// tutaj bedzie bezposredni zapis tranzakcji do pliku i tyle !!
			}
	
		}
		

	}else{
		//tu bedzie automatyczne liczenie pkt na podstawie zakresu sumy albo cos takiego sie jeszcze zobaczy 
	}

}
