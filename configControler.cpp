#include "configControler.h"
#include "networkControler.h"

using namespace std;

configControler::configControler(bool conf)
{
	if (conf)
	{
		cout << "sprawdzam istnienie pliku" << endl;
		ifstream file("config.txt");
		if(file)
		{
			file.close();
			cf = new ConfigFile("config.txt");
			sn = new ConfigFile("seriall.txt");
		}
		else
		{
			cout << "przechodze do generatora" << endl;
			configGenerator();
		}
	}
	else
	{
		cf = new ConfigFile("config.txt");
		sn = new ConfigFile("seriall.txt");
	}
}

void configControler::gprsConfigGenerator(string apn, string user, string password)
{

	ofstream gprsConf;

	gprsConf.open("ppp-on-dialer-m72", ios_base::in | ios_base::trunc | ios_base::ate );

	cout << "gen_dialer_script" << endl;
	gprsConf << "#!/bin/sh" << endl;
	gprsConf << "execchat-v" << endl;
	gprsConf << "\tABORT\t\t'BUSY'\t\t\\" << endl;
	gprsConf << "\tABORT\t\t'NO ANSWER'\t\t\\" << endl;
	gprsConf << "\tABORT\t\t'VOICE'\t\t\\" << endl; 
	gprsConf << "\tABORT\t\t'NO DIALTONE'\t\t\\" << endl; 
	gprsConf << "\tABORT\t\t'NO DIAL TONE'\t\t\\" << endl; 
	gprsConf << "\tABORT\t\t'DELAYED'\t\t\\" << endl; 
	gprsConf << "\tABORT\t\t'+CMEERROR:100'\t\t\\" << endl; 
	gprsConf << "\t''\t\t'AT'\t\t\\" << endl;
	gprsConf << "\tTIMEOUT\t\t5\t\t\\" << endl;
	gprsConf << "\tOK\t\t'ATE0V1'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'AT+QTUNBUF=2048,3584,500,500'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'AT'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'AT+CGDCONT=1,\"IP\",\"" << apn << "\"'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'ATS0=0'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'AT'\t\t\\" << endl;
	gprsConf << "\tOK\t\t'AT'\t\t\\" << endl;
	gprsConf << "\tOK-AT-OK\t'ATDT*99***1#'\t\t\\" << endl;
	gprsConf << "\tCONNECT\t\t'\\d\\c'\t\t\\" << endl;
	if( user.size() > 0)
	{
		gprsConf << "\togin:--ogin:\t'\"" << user << "\"'\t\t\\" << endl;
	}
	if( password.size() > 0)
	{
	gprsConf << "\tassword:\t'\"" << password << "\"'\t\t\\" << endl;;
	}
	gprsConf.close();

}

void configControler::reloadConfig()
{
	delete(cf);
	delete(sn);
	cf = new ConfigFile("config.txt");
	sn = new ConfigFile("seriall.txt");
}

string configControler::returnParam(string part)
{
	string param = cf->Value("ok", part);
	return param;
}

string configControler::returnExtraMultiplier()
{
	string param = cf->Value("extra", "epc.multiplier");
	return param;
}

string configControler::returnExtraMode()
{
	string param = cf->Value("extra", "epc.type");
	return param;
}

string configControler::returnExtraActive()
{
	string param = cf->Value("extra", "epc.active");
	return param;
}

string configControler::returnExtraPoints()
{
	string param = cf->Value("extra", "epc.points");
	return param;
}

string configControler::returnExtraRange()
{
	string param = cf->Value("extra","extraRange");
	return param;
}

string configControler::returnScreenSaverTimeout()
{
	string param = cf->Value("ok", "screensaver.timeout");
	return param;
}

int configControler::returnExtraRangeValue(vector<string> &vec, int range)
{
	string part;
	stringstream compose;
	for(int i = 0; i < range; i++)
	{
		compose.str("");
		compose << "epc.range" << i;
		part.clear();
		part = compose.str();
		string parameter = cf->Value("extra", part);
		vec.push_back(parameter);

	}
	return 0;
}

string configControler::returnExtraValue()
{
	string param = cf->Value("extra", "epc.value");
	return param;
}

string configControler::returnExtraFrom()
{
	string param = cf->Value("extra", "epc.from");	
	return param;
}

string configControler::returnExtraTo()
{
	string param = cf->Value("extra", "epc.to");	
	return param;
}

string configControler::returnConfVer()
{
	string  confVer = cf->Value("ok", "version.date");
	return confVer;
}

int configControler::returnMenu(vector<string> &vec)
{
	confParse(vec, "menu");
}

int configControler::returnPrize(vector<string> &vec)
{
	confParse(vec, "prize");
}

int configControler::returnSeller(vector<string> &vec)
{
	confParse(vec, "seller");
}

int configControler::returnLCard(vector<string> &vec)
{
	confParse(vec, "lc");
}

int configControler::returnComputation(vector<string> &vec)
{
	confParse(vec, "comp");
}

int configControler::confParse(vector<string> &vect, string section)
{
	stringstream compose;
	compose.str("");
	int size = confCounter(const_cast<char *>(section.c_str()));
	
	for(int i = 0; i < size; i++)
	{
		compose << "No" << i;
		string part = compose.str();
		compose.str("");
		string item	= cf->Value(section,part);
		vect.push_back(item);
	}	
}

int configControler::confCounter(char *str)
{
	stringstream compose,compose1;
	string pos3;
	int pos2 =0;
	int pos =0;
	int itemss=0;
	compose << "[" << str << "]";
	compose1 << "[/" << str << "]";
	ifstream file("config.txt");
	string seek, line;
	seek = compose.str();
	 while(!file.eof() ){
		getline(file,line);
		++pos;
		if( line.compare(0,seek.size(),seek) == 0){
			break;
		}
	}
	file.clear();
	seek = compose1.str();
	 while(!file.eof() ){
		getline(file,line);
		if( line.compare(0,seek.size(),seek) == 0){
			break;
		}
			pos2++;
	}
	file.close();

	return pos2;
}

int configControler::configGenerator()
{
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
				cout << flag << endl;
				ret = miniScreen(title, 2, false);
				break;
				case 1:
				flag = "gprs";
				cout << flag << endl;
				ret = miniScreen(title, 2, false);
				break;
				case 2:
					miniInput(options[2],seriallnumber);
				break;
				default:
				break;
			}
	}
	string apnc = "internet";
	string userc = "";
	string passwordc = "";
	ofstream loger("log.txt");
	loger << "Tworze plik seriall.txt" << endl;
	
	ofstream file("seriall.txt");
	file << "[seriall]" << endl;
	file << "SN = " << seriallnumber.c_str() << endl;
	file.close();
	loger << "Stworzylem plik tworze obiekt!" << endl;
	cout << "konfiguracja przed przekazaniem danych to konstruktora !" << endl;
	cout << ip << endl << port << endl << apnc << endl << userc << endl << passwordc << endl << seriallnumber << endl;
	networkControler* conector = new networkControler(ip, port, apnc, userc, passwordc, seriallnumber);
	cout << "dane zostaly przekazana do obiektu !" << endl;
	loger << "sprawdzam  flage" << endl;
	cout << flag << endl;
	if(flag.compare("eth") ==0){
		loger << "sprawdzilem flage i odpalam eth !)" << endl;
		cout << "odpalam opcje dla eth" << endl;
		int type = 1;
		conector->startConf(type);
	}else{
		if(flag.compare("gprs") == 0){
			loger << "sprawdzialem flage i odpalam gprs" << endl;
			cout << "odalam dla gprs" << endl; 
			conector->startConf(0);
		}else{
			// loguj nieznany typ !
		}
	}
	//Lcd_Cls();
	//Lcd_Printxy(0,0,0, "Udalo sie ! chyba ?");
	//cout << "cipa max pizda 56600" << endl;
	delete conector;	
}

int configControler::miniScreen(string &title, int size, bool opt)
{
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
				case KEYFUN:
				return 10;
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
			Lcd_Printxy(0, 0, 1, const_cast<char *>(title.c_str())); //tytul
			Lcd_Printxy(0, 8, 0, "---------------------");
			// wyswietlanie menu
			for(int i= 0; i < size ; i++){
				stringstream compo;
				compo << confOptions[i];
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

int configControler::miniInput(string& title, string& variable)
{
	stringstream compo;
	BYTE key = NOKEY;
	bool arg  =true;
	string temp;
	unsigned char input[20];
	memset(input, 0, sizeof(input));
	compo << title;
	compo.seekg(0, ios::end);
	int len = compo.tellg();
	for(int i=0; i<21-len; i++){
		compo << " ";
	}
	string str = compo.str();
	string str2, str3;
	str2.clear();
	str3.clear();
	Lcd_Cls();
	Lcd_Printxy(0, 0, 1, const_cast<char *>(str.c_str()) );
	compo.str("");
	compo.clear();
	if(title.compare("Ip") == 0)compo << "Ip:" << ip;
	if(title.compare("Port") == 0)compo << "Port:" << port;
	if(title.compare("Seriall Number") == 0)compo << "SN:" << seriallnumber;
	str3 = compo.str();
	compo.str("");
	compo.clear();
	if(title.compare("Ip") == 0)
	{
		while(arg)
		{
			Lcd_Cls();
			Lcd_Printxy(0, 0, 1, const_cast<char *>(str.c_str()) );
			Lcd_Printxy(1, 11, 0, const_cast<char *>(str3.c_str()) );
			Lcd_Printxy(0, 24, 0, const_cast<char *>(str2.c_str()) );
			Lcd_Printxy(1, 55, 0, "AlPha = .");
			while(1)
			{
				if(Kb_Hit)
				{
					key = Kb_GetKey();
					if(key != NOKEY)
					{
						break;
					}
				}
			}
			switch(key)
			{
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
					//int dl = str2.size();
					//if(dl < 15){
					arg = false;
					//}else{
						Lcd_Printxy(0, 47, 0, "Za dlugi IP");	
					//}
				break;
				case KEYALPHA:
					compo << ".";
					str2.clear();
					str2 = compo.str();
				break;
				case KEYCANCEL:
				return 0;
				break;
				case KEYBACKSPACE:
				temp.clear();
				temp = compo.str();
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
			
		
		}
	}
	else
	{
		Lcd_Printxy(1, 11, 0, const_cast<char *>(str3.c_str()) );
		Kb_GetStr(0, 24, input, 4, 19, 0, 120);
	}
		if(str2.size() != 0)
		{
			if(title.compare("Ip") == 0) ip = str2; //const_cast<char *>(input);
		}
		
		string conv = (char *)input;

		if(conv.size() != 0)
		{
	
			if(title.compare("Port") == 0) port = conv; //const_cast<char *>(input);
	
			if(title.compare("Seriall Number") == 0) seriallnumber = conv; //const_cast<char *>(input);
		}
}

string configControler::returnSeriall()
{
	string seriall = sn->Value("seriall","SN");
	return seriall;	
}

string configControler::returnSerwerIp()
{
	string adr = cf->Value("ok", "gprs.address");
	return adr;
}

string configControler::returnSerwerPort()
{
	string prt = cf->Value("ok", "gprs.port");
	return prt;
}


string configControler::confRetrunServiceCode()
{
	string prt = cf->Value("ok", "service.code");
	return prt;
}


string configControler::returnGprsApn()
{
	string gapn = cf->Value("ok", "gprs.apn");
	return gapn;
}

string configControler::returnGprsUser()
{
	string guser = cf->Value("ok", "gprs.login");
	return guser;
}

string configControler::returnGprsPaswd()
{
	string gpaswd = cf->Value("ok", "gprs.password");
	return gpaswd;
}

void configControler::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos){
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}
}

