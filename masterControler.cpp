#include "masterControler.h"
#include "configControler.h"
#include "deviceControler.h"
#include <string>

using namespace std;



masterControler::masterControler(){
	config = new configControler();
	device = new deviceControler();
	ip = config->returnSerwerIp(); 
	port = config->returnSerwerPort(); 
	apn = config->returnGprsApn();
	user = config->returnGprsUser(); 
	paswd = config->returnGprsPaswd(); 
	numerser = config->returnSeriall();
	network = new networkControler(ip, port, apn, user, paswd, numerser);
	
	seller.clear();
	loginFlag = false;
	//vheiu
};

int masterControler::dispMenu(){
	// to tutaj bedzie odpowiedzialne za wyswietlenie menu z logowaniem sie i innymi gownami ! 
	string menu1 = "LOGIN;SHOP;PRIZE;CHECKPOINT;TRANSACTIONS_SEND;SERVICE;;Logowanie;Sklep;Nagrody;Stan punktowy;Wyslij tranzakcje;Serwis";
	string menu2 = "LOGOUT;SHOP;PRIZE;CHECKTPOIN;TRANSACTIONS_SEND;SERVICE;;Wylogowanie;Sklep;Nagrody;Stan punktowy;Wyslij tranzakcje;Serwis";
	vector<string> items;
	vector<string> items2;
	Tokenize(menu1,items, ";");
	Tokenize(menu2,items2, ";");
	
	vector<string> menuOptions;
	config->returnMenu(menuOptions);
	vector<string> displayMenuOn;
	vector<string> displayMenuOff;
// parsowanie menu bez usera
	for(int i = 0; i < 6; i++)
	{
		string comp = items2[i]
		for(int j = 0; j < menuOptions.size(); j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				diaplayMenuOff.push_back(items2[i+6]);
			}
			if( (comp.compare("")) == 0 ){
				diaplayMenuOff.push_back(items2[i+6]);
			}
		}
	}
// parsowanie menu z userem
	for(int i = 0; i < 6; i++)
	{
		string comp = items[i]
		for(int j = 0; j < menuOptions.size(); j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				diaplayMenuOn.push_back(items[i+6]);
			}
			if( (comp.compare("")) == 0 ){
				diaplayMenuOn.push_back(items[i+6]);
			}
		}
	}
	int usItem;
	while(1){
		usItem = menuScr(title, displayMenuOff, 6, usItem);
//		switch(usItem){
			
//		}
		
	}
}

int mesterControler::menuScr(&title, &vect, int size, int index){
	
	const int visible = 6;
    int i, j, view =0;
    BYTE key;
    char *str[40];
    char str2[40];
    int rtn;
    int usTk1len, usTk2len, usTk3len;
    BYTE baTk1Buf[BUFSIZE], baTk2Buf[BUFSIZE], baTk3Buf[BUFSIZE]; //Track 1,2,3 data buffer //
    BYTE baTk1Err, baTk2Err, baTk3Err; //Track 1,2,3 individual status //
    vector<string> items2[20]; // tablica nowych elementow menu
    int idx[20]; // tablica indeksow ze starej listy elementow
    int index2 = 0; // indeks elementu z listy items2       
    int tick = 0;

    key=NOKEY;
    
drawMenu:
	if(index2 < 0 || index2 > size -1) index2 =0;
	if(index2 > visible -1 ) view = index2 - visible +1;

	while(1){
		clear();
	    title(title);

		for(i=0; i < visible; i++){

			if(view +i < size){
				string str = items2[view +i];
				int k,j=21;
	            int len = str.size();
	            for(k=0; k < k-len; k++){
	            	str += " ";
	            }
				if(index2 == view +i){
					Lcd_Printxy(1,16+(i*8),1,str.c_str());
	        	}else{
	        		Lcd_Printxy(1,16+(i*8),0,str.c_str());
	        	}
	        }else{
	//                      Lcd_Printxy(1, i+3, 0, "                 ");
	        }
		}
	    SetTimer(0, 3000);
	    int left = -1;
	    while(1){
	    	if(Kb_Hit){
	    		key = Kb_GetKey();
	    		if(key !=NOKEY){
	    			break;
	    		}else{
	    			key = NOKEY;
	    		}

	    	}
	    	left = CheckTimer(0);
	    	if(0 == left){
	    		key = NOKEY;
	    		break;
	    	}
        }
		switch(key){
			case NOKEY:
		    	creenBlank();
				goto drawMenu;
		    break;
		    case KEYCANCEL:
		    case KEYBACKSPACE:
		        return KEYCANCEL;
		    case KEYENTER: return idx[index2];

		    case KEYDOWN:
		        if(index2 < size -1){
		        	index2++;
		            if(index2 >= view + visible)view++;
		        }else{
		            index2 =0;
		            view = 0;
		        }
		     break;
		     case KEYUP:
		     	if(index2 >0){
		        	index2--;
		            if(index2 < view) view--;
		        }else{
		            index2 = size -1;
		                        //      view = size -1;
		        	if(index2 >= view + visible) view = index2 -  visible +1;
		        }
		//      goto drawMenu;
		        break;
		}
		
	}
	
}

int masterControler::loginScr(){
	clear();
	title("Prosze zaloguj sie");
	BYTE login[6];	
	string logon;
	Lcd_Printxy(0, 32, 0, "Login:");
	Kb_GetStr(0, 40, login, 5, 5, 0, 300);
	stringstream compose;
	compose << login;
	logon = compose.str();
	vector<string> vect;
	config->confParse(vect, "seller");
	int len = vect.size();
	if(sizeof(login)==0){
		return 0;
	}else{
		for(int i = 0; i < len; i++)
		{
			string nick = vect[i];
			nick.erase(nick.size()-1);
			if((nick.compare(logon))==0){
				seller = logon;
				return 0;
			}else{
				string str = "Niepoprawny login";
				infoMSG(str);
				return 0;
			}
		
		}
	}
}

int masterControler::infoMSG(string &msg){
	string msg2 = msg;
	clear();
	int x =0;
	int y =32;
	title("Inrofmacja");
	message(x, y, msg2);
}

int masterControler::message(int x, int y, string &str){
	Lcd_Printxy(x, y, 0, const_cast<char *>(str.c_str()));
}

int masterControler::title(string str){
	int len = (21 - str.size());
	stringstream compose;
	compose.str("");
	compose << str;
	for(int i = 0; i < len; i++)
	{
		compose << " ";
	}
	str.clear();
	str = compose.str();
	Lcd_Printxy(0,0,1, const_cast<char *>(str.c_str()));
	
}

int masterControler::clear(){
	Lcd_Cls();
}

int masterControler::selling(){
	unsigned char input[40];
	string payment, point, extra, seler;
	string date;
	//string id;
	memset(input, 0 , sizeof(input));
	stringstream compose;
	string str,conv;
		
	while(1){
		cout << "tu doszedlem " << endl;
		//char id = device->magCardScan(true);
		string str8 = device->magCardScan(true);
		//char id = device->magCardScan(true);
		cout << "to jest id tuz za magCardScan: " << str8 << endl; 
		//cout << device->magCardScan(false) << endl;
		cout << "Co miesci sie w id: " << str8 << endl;
		compose << str8; 
		//compose << device->magCardScan(false);
		//str = id; //compose.str();
		str = compose.str();
		compose.str("");
		//cout << "tu jestem trololololololololo " << compose.str() << endl;
		cout << "tu jestem trololololololololo " << str << " : " << str.size() << endl;
		if(str.size() > 0 ){
			if(str.size() > 5 && str.size() < 30){
				break;
			}
		}
	}
	cout << "przed suminput" << endl;
	sumInput(payment);
	cout << str << endl << payment << endl << point << endl << extra << endl << numerser << endl;
	pointComp(str, payment, point, extra);
	seller = "00033";
	date = "13:01:03:12:14:25";
	//string sernum = config->returnSeriall();
	cout << "numer seryjny terminala: " << numerser << endl;
	cout << "sprzedawca: " << seller << endl;
	cout << "Zaplata: " << payment << endl;
	cout << "punkty: " << point << endl;
	cout << "extra pkt: " << extra << endl;
	cout << date << endl;
	char type = '0';
	
	fileSave(numerser, seller, str, payment, point, extra, type, date);
	return 0;
}

int masterControler::sumInput(string &payment){
	BYTE key = NOKEY;
	stringstream compo,compo1;
	string temp;
	string str2 = "0.00";
	string str3;
	str3.clear();
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
		clear();
		title("Podaj wartosc zakupu:");
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
				str3.clear();
				str3 = compo.str();
			break;
			case KEY1:
				compo << "1";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
			case KEY2:
				compo << "2";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
			case KEY3:
				compo << "3";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
			case KEY4:
				compo << "4";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
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
				str3.clear();
				str3 = compo.str();
			break;
			case KEY7:
				compo << "7";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
			case KEY8:
				compo << "8";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
				case KEY9:
				compo << "9";
				str2.clear();
				str2 = compo.str();
				str3.clear();
				str3 = compo.str();
			break;
			case KEYENTER:
			cout << "nacisnalem enterem" << endl;
			if(str2.size() > 0){
				cout << "str2 jest wieksze od sera i przepisuje jego wartosc do sum !" << endl;
				payment = str3;
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

int masterControler::fileSave(string &sn, string &seler, string &client, string &pay, string &point, string &extrapoint, char type, string &date){
	cout << "otwieram trx.txt" << endl;
	ofstream trx("trx.txt", ios_base::app);
	cout << "otwieram tranz.bin" << endl;
	ofstream outbin("tranz.bin", ofstream::binary);
	ofstream test("tranzakcje.txt", ios_base::app);
	cout << "tworze string stream compose)" << endl;
	stringstream compose;
	cout << "tworze stringacza" << endl;
	string total;
	cout << "czyszcze string strema" << endl; 
	compose.str("");
	cout << "wypelniam string tym: " << endl << sn << ";" << seler << ";" << client << ";" << pay << ";" << point << ";" << extrapoint << ";" << type << ";" << date << endl;
	compose << sn << ";" << seler << ";" << client << ";" << pay << ";" << point << ";" << extrapoint << ";" << type << ";" << date << endl;
	acttime.Year = 13;
	acttime.Month = 01;
	acttime.Day = 04;
	acttime.Hour = 12;
	acttime.Minute = 05;
	acttime.Second = 55;
	unsigned char factory[20];
	memset(factory, 0, sizeof(factory));
	strncpy( (char *)factory, sn.c_str(), sizeof(factory) );
	memcpy(tx.factorySN, factory, sizeof(factory));
	unsigned char selery[6];
	memset(selery, 0, sizeof(selery));
	strncpy( (char *)selery, seler.c_str(), sizeof(selery) );
	memcpy(tx.sellerLogin, selery, sizeof(selery));
	unsigned char cliente[25];
	memset( cliente, 0, sizeof(cliente));
	strncpy( (char *)cliente, client.c_str(), sizeof(cliente) );
	memcpy(tx.cid, cliente, sizeof(cliente));
	tx.amount = atoi(pay.c_str());
	tx.points = atoi(point.c_str());
	tx.extra = atoi(extrapoint.c_str());
	memcpy(&tx.datetime, &acttime, sizeof(tx.datetime));
	//memcpy((void *)tx.type, (void *)typecpy, sizeof(tx.type));
	//tx.type = typecpy;
	tx.type = type;
	total = compose.str();
	outbin.write( (char *)&tx, sizeof(struct Transaction));
	test.write( (char *)&tx, sizeof(struct Transaction));
	cout << sizeof(struct Transaction) << endl;
	cout << "wciepnujs to szystko do pliku" << endl;
	trx << total;
	cout << "camykom plik " << endl;
	trx.close();
	outbin.close();
	cout << "zamknal zech plik ciupia dalej" << endl;
	
	return 0;
}

int masterControler::pointComp(string &id, string &payment, string &pnt, string &ext){
	cout << "jestem w pointComp, oto id karty !: " << id << endl;
	int sumapkt;
	BYTE key = NOKEY;
	stringstream compo, compo1;
	//string flag = config->pointComp(); 
	string flag = "off";
	string str2 = "0";
	string temp;
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
			clear();
			title("Podaj punkty extra");
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
			// sprawdz pierwsze 3 cyfry i porownaj je z konfiguracja....
			string type;
			for(int i = 0; i < 3; i++)
			{
				compo1 << id[i];
				type = compo1.str();
			}
			if(id.size() == 10)
			{	
				cout << "typ karty: " << type << endl;
				string opt = compos[0];
				cout << "opcja do porownania ! : " << opt << endl;
				if((type.compare(opt))==0){
					cout << "karta zgodna z opcja konfiguracji! " << endl;
					//karta jest taka sama jak ta w konfiguracji basta ! 
					if((payment.size()-2) > 1){
						string interval = compos[3];
						interval.erase(interval.size()-1);
						interval.erase(interval.size()-1);
						string minimum = compos[1];
						minimum.erase(minimum.size()-1);
						minimum.erase(minimum.size()-1);
						string sumary = payment;
						sumary.erase(sumary.size()-1);
						sumary.erase(sumary.size()-1);
						int equation = atoi(sumary.c_str());
						cout << "equation: " << equation << endl;
						string multip = compos[2];
						int mult = atoi(multip.c_str());
						cout << "multip: " << mult << endl;
						int inter = atoi(interval.c_str());
						cout << "inter: " << inter << endl;
						int min = atoi(minimum.c_str());
						cout << "min: " << min << endl;
						int extra = atoi(str2.c_str());
						string option = compos[4];
						cout << "option: " << option << endl;
						if((type.compare(opt))==0){
							if((option.compare("1"))==0){
								sumapkt = ((equation/inter)*mult)+extra;
								cout << "sumapkt, option 1: " << sumapkt << endl;
								stringstream temp;
								temp << sumapkt;
								pnt = temp.str();
								temp.str("");
								temp << extra;
								ext = temp.str();
								return 0;
								break;
							}else{
								sumapkt = (( (equation - min) /inter)*mult)+extra;
								cout << "sumapkt, option 0: " << sumapkt << endl;
								stringstream temp;
								temp << sumapkt;
								pnt = temp.str();
								temp.str("");
								temp << extra;
								ext = temp.str();
								return 0;
								break;
							}
						}
					}
				}
			}else{
				// tutaj bedzie bezposredni zapis tranzakcji do pliku i tyle !!
			}
	
		}
		

	}else{
		//tu bedzie automatyczne liczenie pkt na podstawie zakresu sumy albo cos takiego sie jeszcze zobaczy 
	}
	return 0;
}

void masterControler::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " "){
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos){
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}

}
