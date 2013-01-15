#include "masterControler.h"
#include "configControler.h"
#include "deviceControler.h"
#include <string>

#define BUFSIZE 1024

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

void masterControler::menuShop(){
	string items = "Sprzedaz;Zwrot";
	vector<string> vect;
	Tokenize(items, vect, ";");
	int menuId[2] = {1,2};
}

void masterControler::menuOnOff(){
	string items = "Online;Ofline";
	vector<string> vect;
	Tokenize(items, vect, ";");
	int menuId[2] = {1,2};
}

int masterControler::dispMenu(){
	// to tutaj bedzie odpowiedzialne za wyswietlenie menu z logowaniem sie i innymi gownami ! 
	cout << "Jestem w menu disp !" << endl;
	int menuIdOn[6];
	memset(menuIdOn, 0, sizeof(menuIdOn));
	int menuIdOff[6];
	memset(menuIdOff, 0, sizeof(menuIdOff));
	int i,j,k;
	int ret;
	cout << "tworze 2 stringi login i logout" << endl;
	string menu1 = "LOGIN;SHOP;PRIZE;CHECKPOINT;TRANSACTIONS_SEND;SERVICE;;Wylogowanie;Sklep;Nagrody;Stan punktowy;Wyslij tranzakcje;Serwis";
	string menu2 = "LOGOUT;SHOP;PRIZE;CHECKTPOIN;TRANSACTIONS_SEND;SERVICE;;Logowanie;Sklep;Nagrody;Stan punktowy;Wyslij tranzakcje;Serwis";
	cout << "tworze 2 vectory to przechowywania pozycji menu" << endl;
	vector<string> items;
	vector<string> items2;
	cout << "tokenizuje" << endl;
	Tokenize(menu1,items, ";");
	Tokenize(menu2,items2, ";");
	string title = "Menu";
	cout << "tworze vector menuoptions i wypelniam go opcjami z configa" << endl;
	vector<string> menuOptions;
	config->returnMenu(menuOptions);
	int len = menuOptions.size();
	cout << "tworze vectory dusplay menu on/off" << endl;
	vector<string> displayMenuOn;
	vector<string> displayMenuOff;
// parsowanie menu bez usera
	cout << "passuje pliki opcji i pliki pozycji menu dla zalogowania i niezalogowania" << endl;
	k =0;
	for(i = 0; i < 6; i++)
	{
		string comp = items2[i];
		for(j = 0; j < len; j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				displayMenuOff.push_back(items2[i+6]);
				menuIdOff[k]=i;
				k++;
			}
			if( (comp.compare("")) == 0 ){
				displayMenuOff.push_back(items2[i+6]);
				menuIdOff[k]=i;
				k++;
			}
		}
	}
// parsowanie menu z userem
	k =0;
	for(i = 0; i < 6; i++)
	{
		string comp = items[i];
		for(j = 0; j < len; j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				displayMenuOn.push_back(items[i+6]);
				menuIdOn[k]=i;
				k++;
			}
			if( (comp.compare("")) == 0 ){
				displayMenuOn.push_back(items[i+6]);
				menuIdOn[k]=i;
				k++;
			}
		}
	}
	int usItem;
	cout << "wchodze do while !!" << endl;
	while(1){
		if(!loginFlag){
			usItem = menuScr(title, displayMenuOff, displayMenuOff.size(), usItem, menuIdOff);
		}else{
			usItem = menuScr(title, displayMenuOn, displayMenuOff.size(), usItem, menuIdOn);
		}
		cout << usItem << endl;
		switch(usItem){
			case 0:
				if(!loginFlag){
					ret = loginScr();
					if(ret == 1){
						loginFlag = true;
					}else{
						loginFlag = false;
					}
				}else{
					loginFlag = false;
				}
			break;
			case 1:
			if(!loginFlag){
				ret = loginScr();
				if(ret == 1){
					loginFlag = true;
				}else{
					loginFlag = false;
				}
			}else{
				// tu bedzie przejscie do sklepu in the future :D jeeeeeee
			}
			break;
			case 2:
				// tu bede nagrody
			break;
			case 3:
				//tu bedzie spr pkt
			break;
			case 4:
				cout << "sendTranaction" << endl;
				network->sendTransaction();
			break;
			case 5:
				// tu bedzie serwis !!
			break;
			default:
			break;
		}
		
	}
}

int masterControler::menuScr(const string &menuname,vector<string> &vect, int size, int index, int *menuid){
	cout << "jestem w mnue scr" << endl;
	const int visible = 6;
    int i, j, view =0;
	int ret;
	int ret1;
    BYTE key;
	char track1[100];
	char track2[100];
	char track3[100];
    memset(track1, 0, sizeof(track1));
	memset(track1, 0, sizeof(track2));
	memset(track1, 0, sizeof(track3));
	
    int index2 = 0;      
    int tick = 0;
	if(Mcr_Open()<0)
	{
		// zapis do pliku nie moge zainicjalizowac urzadzenia !!
	}
    //key=NOKEY;
	cout << "jestem przed draw menu !" << endl;
drawMenu:
	if(index2 < 0 || index2 > size -1) index2 =0;
	if(index2 > visible -1 ) view = index2 - visible +1;
	cout << "wchodze do while w menuscr" << endl;
	while(1){
		clear();
	    title(menuname);

		for(i=0; i < visible; i++){

			if(view +i < size){
				string str = vect[view +i];
				int k,j=21;
	            int len = str.size();
	            for(k=0; k < j-len; k++){
	            	str += " ";
	            }
				if(index2 == view +i){
					Lcd_Printxy(1,16+(i*8),1,const_cast<char *>(str.c_str()) );
	        	}else{
	        		Lcd_Printxy(1,16+(i*8),0,const_cast<char *>(str.c_str()) );
	        	}
	        }else{
	//                      Lcd_Printxy(1, i+3, 0, "                 ");
	        }
		}
	    SetTimer(0, 30000);
	    int left = -1;
		cout << "wchodze do while odpowiedzialnego za wykrywanie guzikow timeout i inne" << endl;
		Kb_Flush();
	    while(1){
		
			DelayMs(50);
			ret = Mcr_Read((BYTE *)track1, (BYTE *)track2, (BYTE *)track3);
			if (ret&0x80){
				cout << "wykrylem swipe w menu" << endl;
				if(!loginFlag){
					cout << "niezalogowany !" << endl;
					ret1 = loginScr();
					if(ret1 == 1){
						loginFlag = true;
						selling();
						break;
					}else{
						loginFlag = false;
					}
				}else{
					cout << "zalogowany sprawdzam pkt !!" << endl;	
					selling();
					break;
				}

			}

	    	if(!Kb_Hit()){
	    		key = Kb_GetKey();
	    		if(key !=NOKEY){
	    			break;
	    		}

	    	}

	    	//left = CheckTimer(0);
	    	//if(0 == left){
	    	//	key = NOKEY;
	    	//	break;
	    	//}
        }
		switch(key){
			case NOKEY:
		 //   	creenBlank();
				goto drawMenu;
		    break;
		    case KEYCANCEL:
		    case KEYBACKSPACE:
		        return KEYCANCEL;
			break;
		    case KEYENTER: 
				return menuid[index2];
			break;
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

int masterControler::menuScrOther(const string &menuname,vector<string> &vect, int size, int index, int *menuid){
	cout << "jestem w mnue scr" << endl;
	const int visible = 6;
    int i, j, view =0;
	int ret;
	int ret1;
    BYTE key;
    int index2 = 0;
    int tick = 0;
	if(Mcr_Open()<0)
	{
		// zapis do pliku nie moge zainicjalizowac urzadzenia !!
	}
    //key=NOKEY;
	cout << "jestem przed draw menu !" << endl;
drawMenu:
	if(index2 < 0 || index2 > size -1) index2 =0;
	if(index2 > visible -1 ) view = index2 - visible +1;
	cout << "wchodze do while w menuscr" << endl;
	while(1){
		clear();
	    title(menuname);

		for(i=0; i < visible; i++){

			if(view +i < size){
				string str = vect[view +i];
				int k,j=21;
	            int len = str.size();
	            for(k=0; k < j-len; k++){
	            	str += " ";
	            }
				if(index2 == view +i){
					Lcd_Printxy(1,16+(i*8),1,const_cast<char *>(str.c_str()) );
	        	}else{
	        		Lcd_Printxy(1,16+(i*8),0,const_cast<char *>(str.c_str()) );
	        	}
	        }else{
	//                      Lcd_Printxy(1, i+3, 0, "                 ");
	        }
		}
	    SetTimer(0, 30000);
	    int left = -1;
		cout << "wchodze do while odpowiedzialnego za wykrywanie guzikow timeout i inne" << endl;
		Kb_Flush();
	    while(1){

	    	if(!Kb_Hit()){
	    		key = Kb_GetKey();
	    		if(key !=NOKEY){
	    			break;
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
		 //   	creenBlank();
				goto drawMenu;
		    break;
		    case KEYCANCEL:
		    case KEYBACKSPACE:
		        return KEYCANCEL;
			break;
		    case KEYENTER: 
				return menuid[index2];
			break;
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

void masterControler::screenSaver(){
	
}

int masterControler::loginScr(){
	clear();
	title("Prosze zaloguj sie");
	BYTE login[6];
	memset(login, 0, sizeof(login));	
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
				return 1;
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
	DelayMs(5000);
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
		if((str8.compare("end")) == 0) break;
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
				cout << "przed suminput" << endl;
				sumInput(payment);
				char type = '0';
				//cout << str << endl << payment << endl << point << endl << extra << endl << numerser << endl;
				pointComp(str, payment, point, extra);
				fileSave(numerser, seller, str, payment, point, extra, type, date);
			}
		}
	}
	//seller = "00033";
	//date = "13:01:03:12:14:25";
	//string sernum = config->returnSeriall();
	//cout << "numer seryjny terminala: " << numerser << endl;
	//cout << "sprzedawca: " << seller << endl;
	//cout << "Zaplata: " << payment << endl;
	//cout << "punkty: " << point << endl;
	//cout << "extra pkt: " << extra << endl;
	//cout:w
	//:out << date << endl;
	//char type = '0';
	//cout << "jestem przes zapisam do pliku rekordu !!" << endl;
	//fileSave(numerser, seller, str, payment, point, extra, type, date);
	//cout << "zapisalem plik !"c<< endl;
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
	ofstream out("test.dat");
	ofstream trx("trx.txt", ios_base::app);
	cout << "otwieram tranz.bin" << endl;
	ofstream outbin("tranz.bin", ofstream::binary);
	ofstream test("tranzakcje.txt", ios_base::binaryi|ios::app);
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
	out.write(reinterpret_cast<char *>(&tx), sizeof(tx));
	test.write( reinterpret_cast<char *>(&tx), sizeof(struct Transaction));
	
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

void masterControler::recipePrint(bool recipeCopy){
	
}




