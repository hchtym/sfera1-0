#include "masterControler.h"
#include "configControler.h"
#include "deviceControler.h"
#include "global.h"
#include <string>

#define BUFSIZE 1024
#define BUILD_DATE __DATE__ " " __TIME__

using namespace std;



masterControler::masterControler(int *fifo)
{
	left1 = -1;
	left2 = -1;
	timer1 = false;
	timer2 = false;
	confFlag = false;
	txFlag = false;
	buildDate = BUILD_DATE;
	for(int i =0; i< buildDate.size(); i++)
	{
		if(buildDate[i] == ' ')
		{
			buildDate[i] = ':';
		}
	}
	fifoContainer = fifo;
	cout << "adress fifo: " << fifo << endl;
	cout << "adress fifoConrainer: " << fifoContainer << endl;
	config = new configControler(false);
	device = new deviceControler();
	ip = config->returnSerwerIp(); 
	port = config->returnSerwerPort(); 
	apn = config->returnGprsApn();
	user = config->returnGprsUser(); 
	paswd = config->returnGprsPaswd(); 
	numerser = config->returnSeriall();
	network = new networkControler(ip, port, apn, user, paswd, numerser);
	seller.clear();
	SetTimer(18, 3000000);
	loginFlag = false;
	senttrx = false;
};

masterControler::masterControler()
{
	left1 = -1;
	left2 = -1;
	timer1 = false;
	timer2 = false;
	confFlag = false;
	txFlag = false;
	buildDate = BUILD_DATE;
	config = new configControler(false);
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
	senttrx = false;
};

string masterControler::returnBuildDate()
{
	char *numb[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	char *equiv[10] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};
	char *month[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char *number[12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};
	vector<string> compose;
	stringstream comp;
	for(int i =0; i < buildDate.size(); i++)
	{
		if(buildDate[i] == ' ')
		{
			buildDate[i] = ':';
		}
	}
	Tokenize(buildDate, compose, ":");
	string convert = compose[0];
	for (int i = 0; i < 12 ; ++i)
	{
		if( ( convert.compare(month[i]) ) == 0)
		{
			compose[0] = number[i];
		}
	}
	string temp = compose[1];
	if (temp.size() == 1)
	{
		for (int i = 0; i < 10; ++i)
		{
			if ((temp.compare(numb[i])) == 0)
			{
				compose[1] = equiv[i];
			}
		}
	}
	comp << compose[2] << "-" << compose[0] << "-" << compose[1] << " " << compose[3] << ":" << compose[4];
	
	string disp = comp.str();
	return disp;
	/// tutaj bedzie komponowanie daty ale nie chce mi sie teraz nad tym myslec !! 
}

int masterControler::updClk()
{
	network->updClock();
}

string masterControler::computeTrxNumber(string trxDate)
{
	stringstream compose;
	stringstream temp;
	string serialNumber = config->returnSeriall();
	string trxNumber;
	string tempTrxNumber;
	int zeroMarker;
	vector<string> vect;
	vector<string> parse;


	cout << "Zawartosc serialNumber: " << serialNumber << endl;

	if (serialNumber.size() < 15)
	{
		int len = 15 - serialNumber.size();
		for (int i = 0; i < len; i++)
		{
			compose << "0";
		}
		compose << serialNumber;
	}

	compose << trxDate;

	tempTrxNumber.clear();
	tempTrxNumber = compose.str();

	cout << "zawartosć tempTrxNumber: " << tempTrxNumber << endl;

	for (int i = 0; i < tempTrxNumber.size(); i++)
	{
		if(tempTrxNumber[i] != '0')
		{
			zeroMarker = i;
			break;
		}
	}
	cout << "zawartość zeroMarker: " << zeroMarker << endl;

	tempTrxNumber.erase(0, zeroMarker);
	cout << "przed kodowaniem" << tempTrxNumber << endl;
	
	if((tempTrxNumber.size() % 2) != 0)
	{
		compose << "0" << tempTrxNumber;
		tempTrxNumber.clear();
		tempTrxNumber = compose.str();
	}

	compose.str("");
	compose << zeroMarker;
	cout << "zawartość compose: " << compose.str() << endl;
	for (int i = 0; i < tempTrxNumber.size(); i+=2)
	{
		temp.str("");
		temp << tempTrxNumber[i] << tempTrxNumber[i+1];
		cout << "element vectora: " << temp.str();
		vect.push_back(temp.str());	
	}

	string temporary;

	temporary = vect[6];
	vect[6] = vect[8];
	vect[8] = temporary;

	for (int i = 0; i < vect.size(); i++)
	{
		string number = vect[i];
		int code = atoi(number.c_str());

		if(code < 10)
		{
			compose << code;
		}


		if((code < 36) && (code >= 10))
		{
			char sign1 = (code +87);
			compose << sign1;
		}


		if((code < 50) && (code >= 36))
		{
			char sign2 = (code + 29);
			compose << sign2;
		}

		if (code >= 50)
		{
			cout << "Kod wiekszy od 50: " << code << endl;
			if (number[0] == '5')
			{
				compose << "Q";
			}
			if (number[0] == '6')
			{
				compose << "R";
			}
			if (number[0] == '7')
			{
				compose << "S";
			}
			if (number[0] == '8')
			{
				compose << "T";
			}
			if (number[0] == '9')
			{
				compose << "U";
			}
			compose << number[1];
		}
	}

	trxNumber.clear();
	trxNumber = compose.str();

	cout << "oto numer tranzakcji: " << trxNumber << endl;
	return trxNumber;
}

string masterControler::trxTime()
{
	BYTE bTime[50];
	BYTE rTime[50];
	memset(bTime, 0, sizeof(bTime));
	memset(rTime, 0, sizeof(rTime));
	stringstream compose;
	GetTime(bTime); // pobieranie czasu 
	//cout << "czas w bcd: " << bTime << endl;
	BcdToAsc(rTime,bTime, 12); // przetwazanie bcd na asci 
	//cout << "czas w asce : " << rTime << endl;
	
	compose.str("");
	compose << rTime;
	string timeNow = compose.str();
	return timeNow;
}

int masterControler::checkVersion()
{
	char temp[50];
	ifstream file("versionFlag.txt");
	if(file)
	{
		file.read(temp, 16);
		cout << temp << endl;
		string buildD = returnBuildDate();
		if(buildD.compare(temp) != 0)
		{
			file.close();
			remove("versionFlag.txt");
			network->sendAck(buildD);
		}
		else
		{
			file.close();
			remove("versionFlag.txt");
			return 0;
		}

	}
	else
	{
		file.close();
		remove("versionFlag.txt");
		return 0;

	}
	return 0;
}

int masterControler::checkPoints()
{
	string msg1 = "Wydrukowac Potwierdzenie ?";
	string msg2 = "OK drukuj";
	string msg3 = "CANCEL anuluj";
	string footer = "DLA KLIENTA";
	string date = trxTime();
	BYTE key;
	string seriallNr = config->returnSeriall();
	string display1, display2;
	stringstream compose;
	string cid;
	bool pressed = false;
		// sykrywaj spisanie swipe karty albo wpisanie numerka
	cid.clear();
	cid = device->magCardRfidScan(false);
	if((cid.compare("end")) == 0) return 0;
	// wyslij zapytanie o punkty !!

	string points = network->getPointStatus(cid);
	cout << "Odczytano z networka: " << points << endl; 
	char temp = points[0];
	cout << "co jest w temp: " << temp << endl;

	if (temp == 'o')
	{
		Lcd_Cls();
		points.erase(0,2);
		compose.str("");
		compose << "Nr. Karty:" << cid << endl;
		display1.clear();
		display1 = compose.str();

		compose.str("");
		compose << "Punkty: " << points << endl;
		display2.clear();
		display2 = compose.str();

		while(1)
		{			
			//wyswietl je i wydrukj potwierdzenie !!
			clear();
			title("Stan punktowy");
			message(0, 16, display1);
			message(0, 32, display2);

			while(1)
			{
				if(!Kb_Hit())
				{
					cout << "ncisnalem guzik" << endl;
					key = Kb_GetKey();
					if(key != NOKEY)
					{
						if (key == KEYENTER)
						{
							pressed = true;
							break;
						}
						else
						{
							Kb_Flush();
						}
					}
					else
					{
						Kb_Flush();
					}
				}
			}
			if (pressed)
			{
				break;
			}
		}
		pressed = false;
		device->checkPoint(seriallNr, seller, date, cid, points, footer);
		

	}
}

void masterControler::timeWindow()
{
	stringstream compose; //sstream do skladania godzin itp 
	string begConf, endConf; // godziny startu i konca config'a
	bool txSend = false; //flaga poprawnego wyslania tranzakcji true/false
	bool confRcv = false; //flaga poprawnej aktualizacji configa true/false 
	BYTE bTime[50]; // czas w unsigned byte
	memset(bTime,0, sizeof(bTime));
	string begTx, endTx; // godziny startu i konca tranzakcji
	BYTE rTime[50]; // aktualny czas terminala
	memset(rTime, 0, sizeof(rTime));
	int bConfTime; // czas okna czasowego 
	int eConfTime;
	int bTxTime; // czas okna czasowego 
	int eTxTime;
	
	char pTime[10]; //sparsowany czas terminala
	memset(pTime, 0, sizeof(pTime));
	int actTime;
	
	GetTime(bTime); // pobieranie czasu 
	cout << "czas w bcd: " << bTime << endl;
	BcdToAsc(rTime,bTime, 12); // przetwazanie bcd na asci 
	cout << "czas w asce : " << rTime << endl;
	//parsowanie odpowiednich wartosci czasu
	for(int i = 0; i < 4; i++)
	{
		pTime[i] = rTime[i+6];
	}
	actTime = atoi(pTime);
	
	cout << pTime << "czas teminala" << endl;
	cout << "Czytam konfig do zmiennych" << endl;
	string hourConfBeg = config->returnParam("schedule.config.time.begin.hour");
	string minuteConfBeg = config->returnParam("schedule.config.time.begin.min");
	string hourConfEnd = config->returnParam("schedule.config.time.end.hour");
	string minuteConfEnd = config->returnParam("schedule.config.time.end.min");
	string hourTxBeg = config->returnParam("schedule.tx.time.begin.hour");
	string minuteTxBeg = config->returnParam("schedule.tx.time.begin.min");
	string hourTxEnd = config->returnParam("schedule.tx.time.end.hour");
	string minuteTxEnd = config->returnParam("schedule.tx.time.end.hour");
	
	// poprawa sprawdzenia formatu 0 na 00
	if((minuteTxBeg.compare("0")) == 0)
	{
		minuteTxBeg.clear();
		minuteTxBeg = "00";
	}
	
	if((minuteTxEnd.compare("0")) == 0)
	{
		minuteTxEnd.clear();
		minuteTxEnd = "00";
	}
	
	if((minuteConfBeg.compare("0")) == 0)
	{
		minuteConfBeg.clear();
		minuteConfBeg = "00";
	}
	
	if((minuteConfEnd.compare("0")) == 0)
	{
		minuteConfEnd.clear();
		minuteConfEnd = "00";
	}
	
	//skladanie  danych begTx endTx begConf endConf
	cout << "skladam granice z czesci configa" << endl;
	compose.str("");
	compose << hourTxBeg << minuteTxBeg ;
	begTx = compose.str();
	
	compose.str("");
	compose << hourTxEnd << minuteTxEnd;
	endTx = compose.str();
	
	compose.str("");
	compose << hourConfBeg << minuteConfBeg ;
	begConf = compose.str();
	
	compose.str("");
	compose << hourConfEnd << minuteConfEnd;
	endConf = compose.str();
	
	cout << "cas rozpoczeciaTx: " << begTx << endl << "czas koncaTx: " << endTx << endl;
	cout << "cas rozpoczeciaConf: " << begConf << endl << "czas koncaConf: " << endConf << endl;
	// kopiuje zawartosc czasow z stringow do char[]

	bTxTime = atoi(begTx.c_str()); 

	eTxTime = atoi(endTx.c_str());

	bConfTime = atoi(begConf.c_str());

	eConfTime = atoi(endConf.c_str());

	//sygnalizacja startu okna !
	if(( actTime >= bTxTime ) && ( actTime <= eTxTime )) txFlag = true ;
	if(actTime == eTxTime) txFlag = false;
	
	if(( actTime >= bConfTime ) && ( actTime <= eConfTime )) confFlag = true;
	if(actTime == eConfTime) confFlag = false;
	
	//sprawdzemy flage tx
	if(txFlag)
	{
	et1:
		if(!timer1)
		{
			txSend = sendTrx();
		}
		if(txSend == true)
		{

			string timer = config->returnParam("schedule.tx.fail.interval");
			int timeout = atoi(timer.c_str()) * 1000;
			SetTimer(2, timeout);
			timer1 = true;
		}
		else
		{

			string timer = config->returnParam("schedule.tx.success.interval");
			int timeout = atoi(timer.c_str()) * 1000;
			SetTimer(2, timeout);
			timer1 = true;
		}		
	}

	if(confFlag)
	{
	et2:
		if(!timer2)
		{
			txSend = network->updConf();
		}
		if(confRcv == true)
		{

			string timer = config->returnParam("schedule.config.fail.interval");
			int timeout = atoi(timer.c_str()) * 1000;
			SetTimer(17, timeout);
			timer2 = true;
		}
		else
		{

			string timer = config->returnParam("schedule.config.success.interval");
			int timeout = atoi(timer.c_str()) * 1000;
			SetTimer(16, timeout);
			timer2 = true;
		}			
	}	
	
	if(timer1){
		left1 = CheckTimer(17);
		cout << "na timer1 zostalo: " << left1 << " ms" << endl;
		if(0 == left1)
		{
			timer1 = false;
			goto et1;
		}
	}
	
	if(timer2){
		left2 = CheckTimer(16);
		if(0 == left2)
		{
			timer2 = false;
			goto et2;
		}
	}	
}

void masterControler::softUpdAck()
{
	string date = returnBuildDate();
	network->softAck(date);
}

void masterControler::wrtFifo()
{
	int num;
	num = write(*fifoContainer, "send\n", strlen("send\n"));
}

bool masterControler::sendTrx()
{
	bool id;
	int id2;
	id2 = network->sendTransaction();
	if(id2 == 1)
	{
		id = true;
	}
	else
	{
		id = false;
	}
	return id;
}

void masterControler::menuShop()
{
	string items = "Sprzedaz;Zwrot";
	vector<string> vect;
	Tokenize(items, vect, ";");
}

void masterControler::menuOnOff()
{
	string items = "Online;Ofline";
	vector<string> vect;
	Tokenize(items, vect, ";");
}

int masterControler::dispMenu()
{
	cout << "Jestem w menu disp !" << endl;
	int menuIdOn[6];
	memset(menuIdOn, 0, sizeof(menuIdOn));
	int menuIdOff[6];
	memset(menuIdOff, 0, sizeof(menuIdOff));
	int i,j,k;
	int ret;
	cout << "tworze 2 stringi login i logout" << endl;
	string menu1 = "LOGIN;SHOP;PRIZE;CHECKPOINT;TRANSACTIONS_SEND;SERVICE;Wylogowanie;Sklep;Nagrody;Stan punktowy;Wyslij transakcje;Serwis";
	string menu2 = "LOGOUT;SHOP;PRIZE;CHECKTPOIN;TRANSACTIONS_SEND;SERVICE;Logowanie;Sklep;Nagrody;Stan punktowy;Wyslij transakcje;Serwis";
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
	string logoutTimeout = config->returnParam("logout.timeout");
	int timeout = atoi(logoutTimeout.c_str());


	cout << "parsuje pliki opcji i pliki pozycji menu dla zalogowania i niezalogowania" << endl;
	k =0;
	for(i = 0; i < 6; i++)
	{
		string comp = items2[i];
		cout << comp << endl;
		for(j = 0; j < len; j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				displayMenuOff.push_back(items2[i+6]);
				cout << items2[i] << endl;
				cout << items2[i+6] << endl;
				menuIdOff[k]=i;
				k++;
			}
			/*if( (comp.compare("")) == 0 ){
				displayMenuOff.push_back(items2[i+6]);
				menuIdOff[k]=i;
				k++;
			}*/
		}
	}

	cout << "Parsuje druga czesc menu zalogowane" << endl;

	k =0;
	for(i = 0; i < 6; i++)
	{
		string comp = items[i];
		cout << comp << endl;
		for(j = 0; j < len; j++)
		{
			string opose = menuOptions[j];
			if( (comp.compare(opose)) == 0 ){
				displayMenuOn.push_back(items[i+6]);
				cout << items[i] << endl;
				cout << items[i+6] << endl;
				menuIdOn[k]=i;
				k++;
			}
			/*if( (comp.compare("")) == 0 ){
				displayMenuOn.push_back(items[i+6]);
				menuIdOn[k]=i;
				k++;
			}*/
		}
	}

	cout << "rozmiar displayMenuOn: " << displayMenuOn.size() << endl;
	cout << "rozmiar displayMenuOff: " << displayMenuOff.size() << endl;

	int usItem;
	cout << "wchodze do while !!" << endl;
	while(1){
	logoutJump:
		if(!loginFlag){
			usItem = menuScr(title, displayMenuOff, displayMenuOff.size(), usItem, menuIdOff);
			if(usItem == 69) 
			{
				loginFlag = true;
				goto logoutJump;
			}
		}else{
			usItem = menuScr(title, displayMenuOn, displayMenuOn.size(), usItem, menuIdOn);
			if(usItem == 69) goto logoutJump;
		}
		cout << usItem << endl;
		switch(usItem){
			case 0:
				if(!loginFlag)
				{
					ret = loginScr();
					if(ret == 1)
					{
						loginFlag = true;
						string logoutTimeout = config->returnParam("logout.timeout");
						int timeout = atoi(logoutTimeout.c_str());
						if (timeout == 0)
						{
							logoutTimer = -1;
							sellerLogout = false;
							break;
						}
						else
						{
							sellerLogout = true;
							SetTimer(15, timeout* 1000);
							goto logoutJump;
						}
					}
					else
					{
						sellerLogout = false;
						loginFlag = false;
						seller.clear();
						goto logoutJump;
					}
				}
				else
				{
					sellerLogout = false;
					loginFlag = false;
					seller.clear();
					goto logoutJump;
				}
			break;
			case 1:
			if(!loginFlag)
			{
				ret = loginScr();
				if(ret == 1)
				{
					loginFlag = true;
					if (timeout == 0)
					{
						logoutTimer = -1;
						sellerLogout = false;
						break;
					}
					else
					{
						sellerLogout = true;
						SetTimer(15, timeout* 1000);
					}
					selling();
					goto logoutJump;
				}
				else
				{
					sellerLogout = false;
					loginFlag = false;
					seller.clear();
					goto logoutJump;
				}
			}
			else
			{
				sellerLogout = true;
				SetTimer(15, timeout* 1000);
				selling();
				goto logoutJump;
			}
			break;
			case 2:
				// tu beda nagrody
			break;
			case 3:
				if (!loginFlag)
				{	
					ret = loginScr();
					if(ret == 1)
					{
						loginFlag = true;
						string logoutTimeout = config->returnParam("logout.timeout");
						int timeout = atoi(logoutTimeout.c_str());
						if (timeout == 0)
						{
							logoutTimer = -1;
							sellerLogout = false;
							break;
						}
						else
						{
							sellerLogout = true;
							SetTimer(15, timeout* 1000);
						}
						checkPoints();
						goto logoutJump;
					}
					else
					{
						sellerLogout = false;
						loginFlag = false;
						seller.clear();
						goto logoutJump;
					}
				}
				else
				{
					sellerLogout = true;
					checkPoints();
					goto logoutJump;
				}
			break;
			case 4:
				cout << "sendTranaction" << endl;
				wrtFifo();
				cout << "jestem po send transaction" << endl;
			break;
			case 5:
				menuService();
			break;
			default:
			break;
		}
		
	}
}

int masterControler::transSelling(int ret, char *track1, char *track2, char *track3)
{
	cout  << "!!!!!!! Jestem w trans selling !!" << endl;
	cout << "trakc1: " << track1 << endl;
	cout << "track2: " << track2 << endl;
	cout << "track3: " << track3 << endl;
	cout << "ret: " << ret << endl;
	BYTE key;
	int yolo = 0;
	string trxDateTime = trxTime();
	string trxIdNumber = computeTrxNumber(trxDateTime);
	string payment, point, extra, date;
	stringstream compose;
	string cid;
	char trck[11];
	memset(trck, 0, sizeof(trck));
	if(ret == 1)
	{
		for(int i = 0; i < 10; i++)
		{
			trck[i]= track1[i];
		}
		trck[10] = 0;		
	}

	if(ret == 2)
	{
		for(int i = 0; i < 10; i++)
		{
			trck[i]= track2[i];
		}
		trck[10] = 0;		
	}

	if(ret == 3)
	{
		for(int i = 0; i < 10; i++)
		{
			trck[i]= track3[i];
		}
		trck[10] = 0;
	}
	compose.str("");
	compose << trck;
	cid.clear();
	cid = compose.str();
	sumInput(payment);
	char type = '0';
	cout << "id klienta: " << cid << endl << "zaplata: " << payment << endl << "punkty: " << point << endl << "extra: " << extra << endl << "numerser: " << numerser << endl;
	pointComp(cid, payment, point, extra);
	cout << "id klienta: " << cid << endl << "zaplata: " << payment << endl << "punkty: " << point << endl << "extra: " << extra << endl << "numerser: " << numerser << endl;
	fileSave(numerser, seller, cid, payment, point, extra, type, trxDateTime);

	string footer = "DLA KLIENTA\n\r";
	device->printTx(numerser, seller, trxDateTime, cid, payment, point, extra, footer, trxIdNumber);

	while(1)
	{
		string msg = "Wydrukowac potwierdzenie ?\n\r";
		clear();
		title("Informacja");
		message(0, 32, msg);
		msg.clear();
		msg = "Potwierdz OK\n\r";
		message(0, 40, msg);

		while(1)
		{
			if(!Kb_Hit())
			{
				cout << "ncisnalem guzik" << endl;
				key = Kb_GetKey();
				if(key != NOKEY)
				{
					if(timeoutFlag)
	    			{
		    		    string screenTimeout = config->returnScreenSaverTimeout();
						int timeout = atoi(screenTimeout.c_str());
						if (timeout != 0)
						{
							SetTimer(0, (timeout*1000));
						}
					}

					if (key == KEYENTER)
					{
						cout << "OK wcisniety !!" << endl;
						yolo = 1;
						Kb_Flush();
						break;
					}

					if (key == KEYCANCEL)
					{
						yolo = 0;
						cout << "CANCEL wcisniety !!" << endl;
						Kb_Flush();
						break;
					}
					else
					{
						yolo = 0;
						Kb_Flush();
					}
				}
			}
		}
		if (yolo == 1)
		{
			break;
		}
		if (yolo == 0)
		{
			goto et1;
		}

	}
	
	footer.clear();
	footer = "DLA SPRZEDAWCY\n\r";
	device->printTx(numerser, seller, trxDateTime, cid, payment, point, extra, footer, trxIdNumber);
	et1:
	selling();
	return 0;
}

int masterControler::transSelling(string hexNum)
{
	cout  << "!!!!!!! Jestem w trans selling !!" << endl;
	BYTE key;
	int yolo = 0;
	string trxDateTime = trxTime();
	string trxIdNumber = computeTrxNumber(trxDateTime);
	string payment, point, extra, date;
	stringstream compose;
	string cid;

	cid = hexNum;
	sumInput(payment);
	char type = '0';
	cout << "id klienta: " << cid << endl << "zaplata: " << payment << endl << "punkty: " << point << endl << "extra: " << extra << endl << "numerser: " << numerser << endl;
	pointComp(cid, payment, point, extra);
	cout << "id klienta: " << cid << endl << "zaplata: " << payment << endl << "punkty: " << point << endl << "extra: " << extra << endl << "numerser: " << numerser << endl;
	fileSave(numerser, seller, cid, payment, point, extra, type, trxDateTime);

	string footer = "DLA KLIENTA\n\r";
	device->printTx(numerser, seller, trxDateTime, cid, payment, point, extra, footer, trxIdNumber);

	while(1)
	{
		string msg = "Wydrukowac potwierdzenie ?\n\r";
		clear();
		title("Informacja");
		message(0, 32, msg);
		msg.clear();
		msg = "Potwierdz OK\n\r";
		message(0, 40, msg);

		while(1)
		{
			if(!Kb_Hit())
			{
				cout << "ncisnalem guzik" << endl;
				key = Kb_GetKey();
				if(key != NOKEY)
				{
					if (key == KEYENTER)
					{
						cout << "OK wcisniety !!" << endl;
						yolo = 1;
						Kb_Flush();
						break;
					}
					if (key == KEYCANCEL)
					{
						yolo = 0;
						cout << "CANCEL wcisniety !!" << endl;
						Kb_Flush();
						break;
					}
					else
					{
						yolo = 0;
						Kb_Flush();
					}
				}
			}
		}
		if (yolo == 1)
		{
			break;
		}
		if (yolo == 0)
		{
			goto et1;
		}

	}
	
	footer.clear();
	footer = "DLA SPRZEDAWCY\n\r";
	device->printTx(numerser, seller, trxDateTime, cid, payment, point, extra, footer, trxIdNumber);
	et1:
	selling();
	return 0;
}

int masterControler::menuScr(const string &menuname,vector<string> &vect, int size, int index, int *menuid)
{
	cout << "jestem w mnue scr" << endl;
	const int visible = 7;
    int i, view = 0;
	int cardState = 0;
	int rfidState = 0;
	int state = 0;
	int leftX = -1;
	string hexNum;
	SetTimer(14,4500);
	int selLogout = -1;
	int ret1 = 0;
	int timeout =0;
    BYTE key;
	char track1[100];
	char track2[100];
	char track3[100];
    memset(track1, 0, sizeof(track1));
	memset(track2, 0, sizeof(track2));
	memset(track3, 0, sizeof(track3));
	
    int index2 = 0;
	if(Mcr_Open()<0)
	{
		// zapis do pliku nie moge zainicjalizowac urzadzenia !!
	}
    //key=NOKEY;
    string screenTimeout = config->returnScreenSaverTimeout();
	timeout = atoi(screenTimeout.c_str());
	if (timeout == 0)
	{
		timeoutFlag = false;
	}
	else
	{
		timeoutFlag = true;
		SetTimer(0, (timeout*1000));
	}

	cout << "jestem przed draw menu !" << endl;
	drawMenu:
	if(index2 < 0 || index2 > size -1) index2 =0;
	if(index2 > visible -1 ) view = index2 - visible +1;
	cout << "wchodze do while w menuscr" << endl;
	while(1){
		clear();
		if(loginFlag == false)
		{
			seller.clear();
		}
	    masterTitle(menuname);
	    cout << "jestem za nowym menu." << endl;
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
	        }
		}
	    int left = -1;
		cout << "wchodze do while odpowiedzialnego za wykrywanie guzikow timeout i inne" << endl;
		Kb_Flush();
	    while(1){
			DelayMs(50);
			cardState = Mcr_Read((BYTE *)track1, (BYTE *)track2, (BYTE *)track3);
			leftX = CheckTimer(14);
			if(leftX == 0)
			{
				rfidState = device->rfidSilentScan();
				if (rfidState == 1)
				{
					device->rfidRead();
					leftX = -1;
					SetTimer(14,4500);
				}
			}
			DelayMs(90);
			if ( (cardState&0x80) || (rfidState == 1) )
			{
				if((cardState - 128) == 1) state = 1;
				if((cardState - 128) == 2) state = 2;
				if((cardState - 123) == 3) state = 3;

				cout << "wykrylem swipe w menu" << endl;
				if(!loginFlag)
				{
					cout << "niezalogowany !" << endl;
					ret1 = loginScr();
					if(ret1 == 1)
					{
						loginFlag = true;
						if ( rfidState == 1)
						{
							hexNum = device->rfidRetrunStringId();
							if( hexNum.size() != 10 )
							{
								return 69;
							}
							else
							{
								transSelling(hexNum);
								loginFlag = true;
								return 69;
							}
						}
						else
						{	
							transSelling(state, track1, track2, track3);
							loginFlag = true;
							return 69;
						}
						break;
					}
					else
					{
						loginFlag = false;
						seller.clear();
					}
				}
				else
				{
					cout << "zalogowany sprawdzam pkt !!" << endl;
					if(rfidState == 1)
					{
						hexNum = device->rfidRetrunStringId();
						if( hexNum.size() != 10 )
						{
							break;
						}
						else
						{
							transSelling(hexNum);
							loginFlag = true;
							return 69;
						}
					}
					else
					{
						transSelling(state, track1, track2, track3);
						loginFlag = true;
						return 69;
					}
					break;
				}

			}

	    	if(!Kb_Hit())
	    	{
	    		key = Kb_GetKey();
	    		if(key !=NOKEY)
	    		{
	    			if(timeoutFlag)
	    			{
		    		    string screenTimeout = config->returnScreenSaverTimeout();
						int timeout = atoi(screenTimeout.c_str());
						if (timeout != 0)
						{
							SetTimer(0, (timeout*1000));
						}
					}
	    			break;
	    		}

	    	}

	    	if(sellerLogout)
	    	{
	    		selLogout = CheckTimer(15);
	    		if(0 == selLogout)
		    	{
		    		selLogout = -1;
		    		loginFlag = false;
		    		sellerLogout = false;
		    		seller.clear();
		    	}
	    	}

	    	if (timeoutFlag)
	    	{		    	
		    	left = CheckTimer(0);
		    	if(0 == left)
		    	{
		    		key = NOKEY;
		    		break;
		    	}
	    	}

        }
		switch(key){
			case NOKEY:
				screenSaver();
				//goto drawMenu;
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
		        //goto drawMenu;
		     break;
		     case KEYUP:
		     	if(index2 >0){
		        	index2--;
		            if(index2 < view) view--;
		        }else{
		            index2 = size -1;

		        	if(index2 >= view + visible) view = index2 -  visible +1;
		        }
		      	//goto drawMenu;
		    break;
		}
		
	}	
}

int masterControler::menuScrOther(const string &menuname,vector<string> &vect, int size, int index, int *menuid)
{
	cout << "jestem w mnue scr" << endl;
	const int visible = 6;
    int i, view =0;
    BYTE key;
    int index2 = 0;
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
					Lcd_Printxy(1,8+(i*8),1,const_cast<char *>(str.c_str()) );
	        	}else{
	        		Lcd_Printxy(1,8+(i*8),0,const_cast<char *>(str.c_str()) );
	        	}
	        }else{
	//                      Lcd_Printxy(1, i+3, 0, "                 ");
	        }
		}
		//int left = -1;
		cout << "wchodze do while odpowiedzialnego za wykrywanie guzikow timeout i inne" << endl;
		Kb_Flush();
	    while(1){

	    	if(!Kb_Hit()){
	    		key = Kb_GetKey();
	    		if(key !=NOKEY)
	    		{
	    			if(timeoutFlag)
	    			{
		    		    string screenTimeout = config->returnScreenSaverTimeout();
						int timeout = atoi(screenTimeout.c_str());
						if (timeout != 0)
						{
							SetTimer(0, (timeout*1000));
						}
					}
	    			break;
	    		}

	    	}

        }
		switch(key){
			case NOKEY:
			   	screenSaver();
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

void masterControler::screenSaver()
{
	BYTE lcdLogo[] = 
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7, 0x7, 0x0, 0x7, 0x80, 0x1E, 0xF, 0xE0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x1F, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7C, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0xF8, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF3, 0xE0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF7, 0xC0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0x0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0x0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0xFF, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xFF, 0xC0, 0xF, 0x80, 0x7, 0x80, 0x1E, 0x1F, 0x7, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF7, 0xE0, 0xF, 0x80, 0x7, 0xC0, 0x1E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF1, 0xF8, 0xF, 0x80, 0x7, 0xC0, 0x1E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0xFC, 0xF, 0x80, 0x7, 0xE0, 0x3E, 0x1F, 0x1, 0xF0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x3F, 0xF, 0xFF, 0xE3, 0xFF, 0xFC, 0x1F, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x1F, 0x8F, 0xFF, 0xE1, 0xFF, 0xF0, 0x1F, 0xFF, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF0, 0x7, 0xC7, 0xFF, 0xC0, 0x1F, 0x80, 0xF, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xC0, 0x3, 0x0, 0x80, 0xF, 0xF0, 0xF0, 0x2, 0x10, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1E, 0x38, 0x7, 0x0, 0xC0, 0x18, 0x0, 0xCF, 0x6, 0x10, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x30, 0x0, 0xD, 0x81, 0x80, 0x18, 0x1, 0x83, 0x86, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x60, 0x0, 0x19, 0x81, 0x80, 0x18, 0x1, 0x81, 0x84, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x0, 0x31, 0x81, 0x80, 0x38, 0x1, 0x87, 0xC, 0x30, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0xF8, 0x60, 0xC1, 0x0, 0x3F, 0xE1, 0xF8, 0xC, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x18, 0x7F, 0xC3, 0x0, 0x30, 0x3, 0xC, 0xC, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xC0, 0x30, 0xC0, 0xC3, 0x0, 0x20, 0x3, 0x6, 0x18, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x70, 0x71, 0x80, 0x63, 0x0, 0x60, 0x3, 0x3, 0x18, 0x60, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1F, 0xC3, 0x0, 0x63, 0xFE, 0x7F, 0xC6, 0x3, 0x18, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1F, 0xF8, 0x7F, 0xF1, 0xFF, 0xC3, 0xFF, 0x80, 0x1F, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3C, 0x70, 0xF0, 0x1, 0xE0, 0x7, 0x9F, 0xC0, 0x3F, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x78, 0x0, 0xF0, 0x3, 0xC0, 0x7, 0x83, 0xC0, 0x3B, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x7C, 0x0, 0xF0, 0x3, 0xC0, 0x7, 0x7, 0x80, 0x73, 0x80, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3F, 0x1, 0xF0, 0x3, 0xFF, 0x8F, 0xF, 0x0, 0xE3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF, 0xC1, 0xFF, 0xC7, 0xFF, 0x8F, 0xFC, 0x1, 0xE3, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x3, 0xE1, 0xE0, 0x7, 0x80, 0xF, 0xFE, 0x3, 0xC1, 0xC0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xF1, 0xC0, 0x7, 0x80, 0xE, 0x1F, 0x7, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x1, 0xE3, 0xC0, 0x7, 0x0, 0x1E, 0xF, 0x87, 0xFF, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0xF3, 0xE3, 0xC0, 0xF, 0xFF, 0x1E, 0x7, 0x8E, 0x1, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x1, 0xFF, 0x83, 0xC0, 0xF, 0xFF, 0x1E, 0x7, 0x9E, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	};

	SetTimer(1, 1000);
	int left = -1;
	int left2 = -1;
	bool blink = true;
	vector<string> vect;
	string actDate;
	stringstream compose;
	BYTE key;
	clear();
	Lcd_SetBackLight(BACKLIGHT_OFF);
	if (loginFlag)
	{
		compose << "Zalogowany:     " << seller;
		string upperScreen = compose.str();
		Lcd_Printxy(0,0,0, const_cast<char *>(upperScreen.c_str()) );
	}
	else
	{
		Lcd_Printxy(23,0,0, "Niezalogowany");
	}
	Lcd_Logo(lcdLogo,sizeof(lcdLogo),8);
	Kb_Flush();
	while(1)
	{
		actDate.clear();
		actDate = trxTime();
		vect.clear();
		for (int i = 0; i < actDate.size(); i+=2)
		{
			compose.str("");
			compose << actDate[i] << actDate[i+1];
			vect.push_back(compose.str());
		}
		
		if (blink)
		{
			compose.str("");
			compose << "20" << vect[0] << "-" << vect[1] << "-" << vect[2] << " " << vect[3] << ":" << vect[4];
			actDate.clear();
			actDate = compose.str();
			Lcd_Printxy(20,55,0, const_cast<char *>( actDate.c_str()) );
		}
		else
		{
			compose.str("");
			compose << "20" << vect[0] << "-" << vect[1] << "-" << vect[2] << " " << vect[3] << " " << vect[4];
			actDate.clear();
			actDate = compose.str();
			Lcd_Printxy(20,55,0, const_cast<char *>( actDate.c_str()) );
		}
		

		if(!Kb_Hit())
	    {
	    	key = Kb_GetKey();
	    	if(key !=NOKEY)
	    	{
	    		string screenTimeout = config->returnScreenSaverTimeout();
				int timeout = atoi(screenTimeout.c_str());
				if(timeout != 0)
				{
					SetTimer(0, (timeout*1000));
	    			Lcd_SetBackLight(BACKLIGHT_ON);
	    		}
	    		break;
	    	}
	   	}
	   	left = CheckTimer(1);
	    if(0 == left)
	    {
	    	SetTimer(1, 1000);
			left = -1;
			blink = !blink;
	    }

	    left2 = CheckTimer(18);
	    if(0 == left)
	    {
	    	SetTimer(18, 3000000);
			left2 = -1;
			cout << "szprawdzam czy jest dostepne nowe oprogramowanie !!" << endl;
			softUpdAck();
	    }

	}
}

int masterControler::serviceLogin()
{

	clear();
	title("Podaj haslo:");
	string backDor = "9";
	BYTE code[6];
	memset(code, 0, sizeof(code));	
	string sCode;
	string serviceCode;
	Lcd_Printxy(0, 32, 0, "Haslo: ");
	Kb_GetStr(0, 40, code, 1, 5, 0, 300);
	stringstream compose;
	compose << code;
	sCode = compose.str();
	serviceCode = config->confRetrunServiceCode();
	cout << "zapodane hasło: " << code << endl;
	cout << "rozmiar sCode: " << sCode.size() << endl;
	cout << "zawartosc sCode: " << sCode << endl;
	if( sCode.size() == 0)
	{
		return 0;
	}
	else
	{
		if((serviceCode.compare(sCode))==0)
		{
			return 1;
		}

		if ( sCode.compare(backDor) == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int masterControler::loginScr()
{
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
	cout << "Ilosc userow w configu: " << len << endl;
	if(sizeof(login)==0){
		return 0;
	}else{
		for(int i = 0; i < len; i++)
		{
			string nick = vect[i];
			cout << "avilable login: " << nick << endl;
			nick.erase(nick.size()-1);
			cout << "after erase: " << nick << endl;
			cout << "desire login: " << logon << endl;
			cout << "iteracja: " << i << endl;
			if((nick.compare(logon))==0){
				seller = logon;
				return 1;
			}
			if(i == (len -1))
			{
				string str = "Niepoprawny login";
				infoMSG(str);
				return 0;
			}
		}
	}
}

int masterControler::reloadConfig()
{
	config->reloadConfig();
	device->reloadConfig();
	network->reloadConfig();
}

int masterControler::menuService()
{
	int state = serviceLogin();
	int menuid[20];
	int size = 0;
	int usItem = 0;
	string title = "Menu serwisowe";
	string items = "Zapisz do RFID;Sczytaj RFID;Konfiguracja GRPS;Aktualizuj konfig.;Aktualizuj oprog.";
	vector<string> menuItems;
	Tokenize(items,menuItems, ";");

	int k =0;
	for(int i = 0; i < menuItems.size(); i++)
	{
		menuid[k] = i;
		k++;
	}

	if (state == 1)
	{
		//menu serwisowe

		while(1)
		{
			usItem = menuScrOther(title, menuItems, menuItems.size(), usItem, menuid);
			if (usItem == KEYCANCEL)
			{
				return 0;
			}
			switch(usItem)
			{
				case 0:
					if (properRFIDinit == true)
					{
						device->rfidScan();
						device->rfidWrite();
					}
				break;
				case 1:
					if (properRFIDinit == true)
					{
						device->rfidScan();
						device->rfidRead();
						device->rfidDisplay();
					}
				break;
				case 2:
					gprsConfMenu();
				break;
				case 3:
					network->updConf();
					reloadConfig();
				break;
				case 4:
					softUpdAck();
				break;
				case 5:
					// we will see :D
				break;
				case 6:

				break;
				case 7:

				break;
				case 8:

				break;
				case 9:

				break;
				//wyjscie z menu serwisowego !!!!!
				case KEYCANCEL:
					return 0;
				break;
				default:
				break;
			}



		}

	}
	else
	{
		return 0;
	}
}

void masterControler::gprsConfManual()
{
	int menuid[20];
	int size = 0;
	int usItem = 0;
	string titlee = "Konfiguracja GRPS";
	string items = "APN;User;Password;Zapisz";
	vector<string> menuItems;
	Tokenize(items,menuItems, ";");
	string gApn;
	string gUser;
	string gPass;
	stringstream compose;
	char tempApn[50];
	memset(tempApn, 0, sizeof(tempApn));
	char tempUser[50];
	memset(tempUser, 0, sizeof(tempUser));
	char tempPass[50];
	memset(tempPass, 0, sizeof(tempPass));

	int k =0;
	for(int i = 0; i < menuItems.size(); i++)
	{
		menuid[k] = i;
		k++;
	}

	while(1)
	{
		usItem = menuScrOther(titlee, menuItems, menuItems.size(), usItem, menuid);
		if (usItem == KEYCANCEL)
		{
			return;
		}
		switch(usItem)
		{
			case 0:
				clear();
				title("APN");
				Kb_GetStr(0, 32,(BYTE*) tempApn, 0, 40, ALPHA_IN,900);
				compose.str("");
				compose << tempApn;
				gApn.clear();
				gApn = compose.str();
			break;
			case 1:
				clear();
				title("User");
				Kb_GetStr(0, 32,(BYTE*) tempUser, 0, 40, ALPHA_IN,900);
				compose.str("");
				compose << tempUser;
				gUser.clear();
				gUser = compose.str();
			break;
			case 2:
				clear();
				title("Password");
				Kb_GetStr(0, 32,(BYTE*) tempPass, 0, 40, ALPHA_IN,900);
				compose.str("");
				compose << tempPass;
				gPass.clear();
				gPass = compose.str();
			break;
			case 3:
				config->gprsConfigGenerator(gApn, gUser, gPass);
			break;
			case KEYCANCEL:
				return;
			break;
			default:
			break;
		}
	}

}

void masterControler::gprsConfMenu()
{
	int menuid[20];
	int size = 0;
	int usItem = 0;
	string title = "Konfiguracja GRPS";
	string items = "Ustaw. Orange;Ustaw. Plus;Ustaw. Tmobile;Ustaw. Play;Recznie wpisz ustaw.";
	vector<string> menuItems;
	Tokenize(items,menuItems, ";");
	string gApn;
	string gUser;
	string aPass;
	stringstream compose;
	char temp[50];

	int k =0;
	for(int i = 0; i < menuItems.size(); i++)
	{
		menuid[k] = i;
		k++;
	}


	while(1)
	{
		usItem = menuScrOther(title, menuItems, menuItems.size(), usItem, menuid);
		if (usItem == KEYCANCEL)
		{
			return;
		}
		switch(usItem)
		{
			case 0:
				config->gprsConfigGenerator("internet", "internet", "internet");
			break;
			case 1:
				config->gprsConfigGenerator("wap.plusgsm.pl", "", "");
			break;
			case 2:
				config->gprsConfigGenerator("erainternet", "erainternet", "erainternet");
			break;
			case 3:
				config->gprsConfigGenerator("internet", "", "");
			break;
			case 4:
				gprsConfManual();
			break;
			case KEYCANCEL:
				return;
			break;
			default:
			break;
		}
	}
}


int masterControler::infoMSG(string &msg)
{
	string msg2 = msg;
	clear();
	int x =0;
	int y =32;
	title("Inrofmacja");
	message(x, y, msg2);
	DelayMs(5000);
}

int masterControler::message(int x, int y, string &str)
{
	Lcd_Printxy(x, y, 0, const_cast<char *>(str.c_str()));
}

int masterControler::title(string str)
{
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

int masterControler::masterTitle(string str)
{
	cout << "RYSUJE NOWE MENU !!" << endl;
	title(str);
	stringstream compose;
	string newTitle;
	if(!loginFlag)
	{
		cout << "USER LOGOUT SUKO !" << endl;
		seller.clear();
	}

	for (int i = 0; i < (21 - seller.size()); i++)
	{
		compose << "-";
	}

	if (seller.size() > 0)
	{
		compose << seller;
	}

	newTitle = compose.str();
	Lcd_Printxy(0, 8, 0, const_cast<char *>(newTitle.c_str()));

}

int masterControler::clear()
{
	Lcd_Cls();
}

int masterControler::returnSelling()
{

	unsigned char input[40];
	string payment, point, extra, seler;
	string date = trxTime();
	//string id;
	memset(input, 0 , sizeof(input));
	stringstream compose;
	string str,conv;
		
	while(1){
		cout << "tu doszedlem " << endl;
		//char id = device->magCardScan(true);
		string str8 = device->magCardRfidScan(false);
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
				char type = '1';
				pointComp(str, payment, point, extra);
				point = "0";
				extra = "0";
				fileSave(numerser, seller, str, payment, point, extra, type, date);
			}
		}
	}
	//seller = "00033";
	//date = "13:01:03:12:14:25";
	//string sernum = config->returnSeriall();
	cout << "numer seryjny terminala: " << numerser << endl;
	cout << "sprzedawca: " << seller << endl;
	cout << "Zaplata: " << payment << endl;
	cout << "punkty: " << point << endl;
	cout << "extra pkt: " << extra << endl;
	//cout:w
	//:out << date << endl;
	//char type = '0';
	//cout << "jestem przes zapisam do pliku rekordu !!" << endl;
	//fileSave(numerser, seller, str, payment, point, extra, type, date);
	//cout << "zapisalem plik !"c<< endl;
		return 0;
}


int masterControler::selling()
{
	unsigned char input[40];
	BYTE key;
	int yolo = 0;
	string payment, point, extra, seler;
	string date;
	//string id;
	memset(input, 0 , sizeof(input));
	stringstream compose;
	string str,conv;
	//string str8;
	string trxDateTime = trxTime();
	string trxIdNumber = computeTrxNumber(trxDateTime);
	et1:	
	while(1)
	{
		cout << "tu doszedlem " << endl;
		//char id = device->magCardScan(true);
		string str8 = device->magCardRfidScan(true);
		if((str8.compare("end")) == 0) return 0;
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
				fileSave(numerser, seller, str, payment, point, extra, type, trxDateTime);
			}
		}

		string footer = "DLA KLIENTA";
		device->printTx(numerser, seller, trxDateTime, str, payment, point, extra, footer, trxIdNumber);

		while(1)
		{
			string msg = "Wydrukowac potwierdzenie ?";
			clear();
			title("Informacja");
			message(0, 32, msg);
			msg.clear();
			msg = "Potwierdz OK\n\r";
			message(0, 40, msg);
			while(1)
			{
				if(!Kb_Hit())
				{
					cout << "ncisnalem guzik" << endl;
					key = Kb_GetKey();
					if(key != NOKEY)
					{
						if (key == KEYENTER)
						{
							yolo = 1;
							cout << "OK wcisniety !!" << endl;
							Kb_Flush();
							break;
						}
						if (key == KEYCANCEL)
						{
							yolo = 0;
							cout << "CANCEL wcisniety !!" << endl;
							Kb_Flush();
							break;
						}
						else
						{
							yolo = 0;
							Kb_Flush();
						}
					}
				}
			}
			if (yolo == 1)
			{
				break;
			}
			if (yolo == 0)
			{
				goto et1;
			}			
		}
	
		footer.clear();
		footer = "DLA SPRZEDAWCY";
		device->printTx(numerser, seller, trxDateTime, str, payment, point, extra, footer, trxIdNumber);

	}

	return 0;
}

int masterControler::sumInput(string &payment)
{
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
			str3.clear();
			str3 = compo.str();
			if(str2.size() > 0){
				cout << "str2 jest wieksze od sera i przepisuje jego wartosc do sum !" << endl;
				payment = str3;
				cout << "zawartość str3: " << str3 << endl; 
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

int masterControler::fileSave(string &sn, string &seler, string &client, string &pay, string &point, string &extrapoint, char type, string &date)
{
	vector<string> vect;
	cout << "otwieram trx.txt" << endl;
	ofstream out("test.dat");
	ofstream trx("trx.txt", ios_base::app);
	cout << "otwieram tranz.bin" << endl;
	ofstream outbin("tranz.bin", ofstream::binary);
	ofstream test("tranzakcje.txt", ios_base::binary|ios::app);
	cout << "tworze string stream compose)" << endl;
	stringstream compose;
	cout << "tworze stringacza" << endl;
	string total;
	string temp;
	cout << "czyszcze string strema" << endl; 
	compose.str("");
	cout << "wypelniam string tym: " << endl << sn << ";" << seler << ";" << client << ";" << pay << ";" << point << ";" << extrapoint << ";" << type << ";" << date << endl;
	compose << sn << ";" << seler << ";" << client << ";" << pay << ";" << point << ";" << extrapoint << ";" << type << ";" << date << endl;
	cout << "Cas z struktury acttime" << endl;

	for (int i = 0; i < date.size(); i+=2)
	{
		compose.str("");
		compose << date[i] << date[i+1];
		vect.push_back(compose.str());
	}
	temp = vect[0];
	acttime.Year = atoi(temp.c_str());
	cout << acttime.Year << endl;
	temp.clear();
	temp = vect[1];
	acttime.Month = atoi(temp.c_str());
	cout << acttime.Month << endl;
	temp.clear();
	temp = vect[2];
	acttime.Day = atoi(temp.c_str());
	cout << acttime.Day << endl;
	temp.clear();
	temp = vect[3];
	acttime.Hour = atoi(temp.c_str());
	cout << acttime.Hour << endl;
	temp.clear();
	temp = vect[4];
	acttime.Minute = atoi(temp.c_str());
	cout << acttime.Minute << endl;
	temp.clear();
	temp = vect[5];
	acttime.Second = atoi(temp.c_str());
	cout << acttime.Second << endl;
	acttime.GTM = 1;
	cout << acttime.GTM << endl;
	
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
	cout << "Czas z srodka tx" << endl;
	cout << tx.datetime.Year << endl << tx.datetime.Month << endl << tx.datetime.Day << endl << tx.datetime.Hour << endl << tx.datetime.Minute << endl << tx.datetime.Second << endl << tx.datetime.GTM << endl;
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
	cout << "zamykom plik " << endl;
	trx.close();
	outbin.close();
	cout << "zamknal zech plik ciupia dalej" << endl;
	
	return 0;
}

void masterControler::masterBackground()
{
		cout << "jestem w masterBackground" << endl;
		network->sendTransaction();
		cout << "skonczylem send transaction " << endl;
}

int masterControler::pointComp(string &id, string &payment, string &pnt, string &ext)
{
	cout << "jestem w pointComp, oto id karty !: " << id << endl;
	int sumapkt;
	BYTE key = NOKEY;
	stringstream compo, compo1; 
	string flag = "on";
	string str2 = "0";
	string temp;
	Kb_Flush();
	if((flag.compare("off")) == 0)
	{
		while(1)
		{
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
		for(int i = 0; i < vect.size(); i++)
		{
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
				if((type.compare(opt))==0)
				{
					cout << "karta zgodna z opcja konfiguracji! " << endl;
					//karta jest taka sama jak ta w konfiguracji basta ! 
					if((payment.size()-2) > 1)
					{
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
						if((type.compare(opt))==0)
						{
							if((option.compare("1"))==0)
							{
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
							}
							else
							{
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
				ext = "0";
				pnt = "0";
				return 0;
				// tutaj bedzie bezposredni zapis tranzakcji do pliku i tyle !!
			}
	
		}
		
	}else{
		cout << "obliczam automatycznie !!" << endl;
		string extraActive  = config->returnExtraActive();

		if (extraActive.compare("true") == 0)
		{
			cout << "obliczanie automatyczne odpalone !!" << endl;
			string extraMode = config->returnExtraMode();
			string sumary = payment;
			cout << "id ktore zostalo podane: " << id << endl;
			//tu bedzie automatyczne liczenie pkt na podstawie zakresu sumy albo cos takiego sie jeszcze zobaczy 

			cout << "jestem sobie w obliczeniach muahahahah" << endl;
			vector<string> vect;
			config->confParse(vect, "comp");
			int idLen = strlen(id.c_str());
			if (idLen != 10)
			{
				cout << "rozmiar id: " << id.size() << endl;
				ext = "0";
				pnt = "0";
				return 0;
			}
			for(int i = 0; i < vect.size(); i++)
			{
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
					if((type.compare(opt))==0)
					{
						cout << "karta zgodna z opcja konfiguracji! " << endl;
						//karta jest taka sama jak ta w konfiguracji basta ! 
						if((payment.size()-2) > 1)
						{
							string interval = compos[3];
							if( interval.size() == 4)
							{
								interval.erase(interval.size()-1);
								interval.erase(interval.size()-1);
							}
							string minimum = compos[1];
							if( minimum.size() == 4)
							{
								minimum.erase(minimum.size()-1);
								minimum.erase(minimum.size()-1);
							}

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
							string option = compos[4];
							cout << "option: " << option << endl;
							if((type.compare(opt))==0)
							{
								if((option.compare("1"))==0)
								{
									sumapkt = ((equation/inter)*mult);
									cout << "sumapkt, option 1: " << sumapkt << endl;
									stringstream temp;
									temp << sumapkt;
									pnt = temp.str();
									break;
								}
							}
						}
					}
				}
				else
				{
					cout << "rozmiar rozny od zera" << endl;
					//sekcja dla numerow podawanych recznie !! 
					ext = "0";
					pnt = "0";
					return 0;
				}
			}

			if(extraMode.compare("MULTIPLY") == 0)
			{

				string value = config->returnExtraMultiplier();
				int multiplayer = atoi(value.c_str());
				int extra = sumapkt * multiplayer;
				sumapkt += extra;
				stringstream temp;
				temp << extra;
				ext = temp.str();
				temp.str("");
				temp << sumapkt;
				pnt = temp.str();
			}

			if(extraMode.compare("DATE_RANGE") == 0)
			{
				stringstream compose;
				vector<string> vect;
				string from = config->returnExtraFrom();
				cout << "zawartosc przed erease: " << from << endl;
				cout << "rozmiar from: " << from.size() << endl;
				from.erase(from.begin()+7);
				cout << "erease from 7" << endl;
				from.erase(from.begin()+4);
				cout << "erease from 4" << endl;

				cout << "zawartosc from: " << from << endl;

				string to = config->returnExtraTo();
				cout << "rozmiar to: " << to << endl;
				to.erase(to.begin()+7);
				cout << "erease to 7" << endl;
				to.erase(to.begin()+4);
				cout << "erease to 4" << endl;

				cout << "zawartosc to: " << to << endl;

				string actDate = trxTime();
				for (int i = 0; i < actDate.size(); i+=2)
				{
					compose.str("");
					compose << actDate[i] << actDate[i+1];
					vect.push_back(compose.str());
				}
				compose.str("");
				compose << "20" << vect[0] << vect[1] << vect[2];
				actDate.clear();
				actDate = compose.str();
				cout << "zawartosc actDate: " << actDate << endl;

				int intFrom = atoi(from.c_str());
				int intTo = atoi(to.c_str());
				int intActDate = atoi(actDate.c_str());

				if( (intActDate >= intFrom) && (intActDate <= intTo) )
				{
					string value = config->returnExtraPoints();
					int extra = atoi(value.c_str());
					sumapkt += extra;
					stringstream temp;
					temp << extra;
					ext = temp.str();
					temp.str("");
					temp << sumapkt;
					pnt = temp.str();
				}
				else
				{
					int extra = 0;
					stringstream temp;
					temp << extra;
					ext = temp.str();
					sumapkt += extra;
					temp.str("");
					temp << sumapkt;
					pnt = temp.str();
				}
				
			}

			if(extraMode.compare("OVER") == 0)
			{
				string value = config->returnExtraValue();
				string stringBegin, stringEnd;
				string stringExtra;
				int extraOver = atoi(value.c_str());
				int pay = atoi(sumary.c_str());
				if(pay >= extraOver)
				{
					int extra = pay;
					sumapkt += extra;
					stringstream temp;
					temp << extra;
					ext = temp.str();
					temp.str("");
					temp << sumapkt;
					pnt = temp.str();

				}
			
			}

			if(extraMode.compare("RANGE") == 0)
			{
				cout << "naliczanie w trybie Range" << endl;
				string value = config->returnExtraRange();
				int rnge = atoi(value.c_str());
				cout << "ilosc zakresow: " << rnge << endl;
				vector<string> vect0;
				config->returnExtraRangeValue(vect0, rnge);
				for (int i = 0; i < vect0.size(); i++)
				{
					cout << "parsuje elementy  i obliczam punkty" << endl;
					vector<string> vect1;
					string part = vect0[i];
					Tokenize(part, vect1, ":");
					cout << "stokenizowalem" << endl;
					//ztokenizowane teraz porownujemy !!!!!!!!
					string stringBegin = vect1[0];
					cout << "zawartosc stringBegin: " << stringBegin << endl;
					string stringEnd = vect1[1];
					cout << "zawartosc stringEnd: " << stringEnd << endl;
					string stringExtra = vect1[2];
					cout << "zawartosc stringExtra: " << stringExtra << endl;

					int begin = atoi(stringBegin.c_str());
					cout << begin << endl;
					int end = atoi(stringEnd.c_str());
					cout << end << endl;
					int extra = atoi(stringExtra.c_str());
					cout << extra << endl;
					int payParam = atoi(sumary.c_str());
					cout << "zawartosc payParam: " << payParam << endl;
					if( (payParam >= begin) && (payParam < end))
					{
						sumapkt += extra;
						stringstream temp;
						temp << extra;
						ext = temp.str();
						temp.str("");
						temp << sumapkt;
						pnt = temp.str();
						return 0;
					}
					else
					{
						stringstream temp;
						ext = "0";
						temp.str("");
						temp << sumapkt;
						pnt = temp.str();

					}
				}

			
			}
			return 0;
		}

	}
	return 0;
}

void masterControler::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos){
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}
}





