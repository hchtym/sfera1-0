#include "networkControler.h"
#include "configControler.h"
#include "deviceControler.h"

#define buffer (2048)
//#define ERR_OK (0)


using namespace std;

networkControler::networkControler(string &ipr, string &portr, string &apnr, string &userr, string &passwordr, string &serialNr)
{
	device = new deviceControler();
	cout << "konfig z obiektu networkControler przed przekazaniem !!" << endl;
	ip = ipr;
	port = portr;
	apn = apnr;
	user = userr;
	password = passwordr;
	serialN = serialNr;
	config = new configControler(false);
}

networkControler::~networkControler()
{
	delete(device);
}

int networkControler::connectAllQuiet()
{
	cout << "podlaczam sie gdzenie do serwera" << endl;
	ofstream loger("logs.txt", ios_base::app);
	loger << "start ethCon" << endl;
	// konfiguracja socketa !! 
	char pCAPData[buffer*10];
	char download[buffer*10];
	memset(pCAPData, 0, sizeof(pCAPData));
	memset(download, 0, sizeof(download));
	struct sockaddr_in dest_addr;
	cout << "tworze socket" << endl;
	loger << "tworze socket" << endl;
	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		loger << "socket erros" << endl;
		perror("socket"); // ogowanie do pliku !! 
		//exit(1);
	}
		loger << "utworzylam socket" << endl;
	//int ports;
	//atoi(port.c_str());
	dest_addr.sin_family = AF_INET;
	loger << "konfiguruje port" << endl;
	dest_addr.sin_port = htons( atoi(port.c_str())	); // wstawic port 
	loger << "skonfigurowalem port" << endl;
	loger << "konfiguruje ip" << endl;
	dest_addr.sin_addr.s_addr = inet_addr(ip.c_str() ); // wstawic ip
	loger << "skonfigurowalem ip" << endl;
	memset(&(dest_addr.sin_zero), '\0', 8);

	loger << "lacze sie z serwerm" << endl;
	if(connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)) == -1){
		loger << "connect socket error !!" << endl;
		perror("connect"); // zamienic na logowanie do pliku !
		//exit(1);
	}
	loger << "otwarlem socket i polaczylem sie" << endl;
	cout << "otwarlem socket i sie polaczylem " << endl;
	int len,bytes_sent,bytes_recv;
	stringstream compose;
	compose << "nr;" << serialN << endl; 
	string msg = compose.str();
	cout << "wysylam numer seryjny !" << endl;
	len = msg.size();
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
		loger << "send serial error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);
	loger << "otworzylem polaczenie sieciowe !!" << endl;
	cout << "polaczylem sie !" << endl;
	loger.close();
	return 0;
}

int networkControler::updClock()
{
	connectAllQuiet();
	// wysylam zapytanie o clocka :D 
	char pCAPData[buffer*10];
	int ret;
	char bufer[50000];
	memset(bufer, 0, sizeof(bufer));
	char temp[730];
	memset(temp, 0, sizeof(temp));
	int ulLen =0;
	int x =0;
	int ulHandle =0;
	memset(pCAPData, 0, sizeof(pCAPData));
	stringstream compose; 
	string msg;
	int sent;
	string serwTime;
	compose.str("");
	cout << "tworze stream" << endl;
	compose << "clock"; //<< endl;
	msg = compose.str();
	int len,bytes_sent,bytes_recv;
	int resend = 0;
	len = msg.size();
	bytes_sent = 0;
	bytes_recv = 0;
	//send clock request to server
	cout << "wysylam zapytanie" << endl;
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
	{
		//loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	//odbieram dane od serweta
	if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1)
	{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	compose.str("");
	compose << pCAPData;
	serwTime = compose.str();
	cout << "czas z serwera: " << serwTime << endl;
	compose.str("");
	compose << serwTime[2] << serwTime[3] << serwTime[5] << serwTime[6] << serwTime[8] << serwTime[9] << serwTime[11] << serwTime[12] << serwTime[14] << serwTime[15] << serwTime[17] << serwTime[18];
	serwTime.clear();
	serwTime = compose.str();
	cout << "czas po sparsowaniu: " << serwTime << endl;
	BYTE converse[15];
	memset(converse, 0, sizeof(converse));
	BYTE bcdTime[30];
	memset(bcdTime, 0, sizeof(bcdTime));
	
	for(int i =0; i< 12; i++)
	{
		converse[i] = serwTime[i];
	}
	cout << "string w formacie byte: " << converse << endl;
	AscToBcd(bcdTime, converse, strlen((char *)converse));
	cout << "czas po konwersji na BCD: " << bcdTime << endl;
	
	//ustawam zegar !
	ret = SetTime(bcdTime);
	if(ret != ERR_OK)
	{
		cout << "Nieustawilem zegara !!" << endl;
	}
	
	
	// koniec zapytanie o godzine koncze polaczenie (bo po co ma wisiec nie uzywane ...)
	
	disconnectAllQuiet();
}

int networkControler::disconnectAllQuiet()
{
	ofstream loger("logs.txt", ios_base::app);
	int bytes_sent;
	sleep(1);
	loger << "sendin 'bye' and ending connection !" << endl;
    if((bytes_sent = send(sockfd, "bye", 3, 0))== -1){
        perror("send");
        //exit(1);
    }
	// zamykam plik konfiguracyjny !
	// zamykam socket !
	close(sockfd);
	loger.close();
	return 0;
}

int networkControler::sendTransaction()
{
	cout << "jestem w send trx" << endl;
	connectAllQuiet();
	cout << "wchodze do sendtrx" << endl;
	sendTrx();
	cout << "rozlaczam sie" << endl;
	disconnectAllQuiet();
	cout << "Rozlaczem sie !" << endl;
}

int networkControler::softAck(string date)
{
	cout << "jestem w softAck" << endl;
	connectAllQuiet();
	int ret = softUpdate(date);
	disconnectAllQuiet();
	cout << "jestem za disconnectAllQuiet a ret wynosi: " << ret << endl;
	if(ret == 1)
	{	
		cout << "odpalam skrypt instalacyjny !!" << endl;
		system("/bin/upd.sh");
	}
	return 0;
}

int networkControler::softUpdate(string data)
{
	if( remove( "/home/strong_lion/scl_app_new" ) != 0 )
    cout << "Error deleting file" << endl;
  	else
    cout << "File successfully deleted" << endl;
	sleep(1);
	ofstream newApp("/home/strong_lion/scl_app_new", ios_base::binary | ios_base::out | ios_base::app);
	char pCAPData[buffer*10];
	unsigned char downloader[buffer * 10];
	memset(downloader, 0, sizeof(downloader));
	char bufer[50000];
	memset(bufer, 0, sizeof(bufer));
	char temp[730];
	memset(temp, 0, sizeof(temp));
	int ulLen =0;
	int x =0;
	int ulHandle =0;
	memset(pCAPData, 0, sizeof(pCAPData));
	stringstream compose; 
	string msg;
	int sent;
	string info;
	compose.str("");
	cout << "tworze stream" << endl;
	string serNr = config->returnSeriall();
	cout << "Wysylam zapytanie." << endl;
	//compose << "cap;" << serNr << ";" << data << endl; //<< endl;
	compose << "cap;" << serNr << ";" << "2013-02-07 15:02" << endl;
	msg = compose.str();
	int len,bytes_sent,bytes_recv;
	int resend = 0;
	int size1 = 0;
	int size2 = 0;
	bytes_sent = 0;
	bytes_recv = 0;

	len = msg.size();
	cout << "wysylam zapytanie" << endl;
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
	{
		//loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	//sleep(1);

	if((bytes_recv = recv(sockfd, pCAPData,(buffer*10) -1, 0)) == -1)
	{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	cout << "recived message: " << pCAPData << endl;

	compose.str("");
	compose << pCAPData;
	info = compose.str();
	cout << info << endl;
	if(info.compare("ok") == 0)
	{
		cout << "wysylam potwierdzenie 'ok' " << endl;
		msg.clear();
		msg = "ok\n\r";
		if((bytes_sent = send(sockfd, msg.c_str(), msg.size(), 0)) == -1)
		{
		//loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
		}
		cout << "wyslalem: " << msg << endl;
		if((bytes_recv = recv(sockfd, pCAPData,(buffer*10) -1, 0)) == -1)
		{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
		}

		cout << "ostrzymalem rozmiar: " << pCAPData << endl;

		compose.str("");
		compose << pCAPData;
		info.clear();
		info = compose.str();
		cout << "rozmiar z stringa: " << info << endl;
		int downloaded =0;
		int reciveSize = atoi(pCAPData );
		// przygotowuje sie do odebrania softu
		unsigned char * newSoft;
		newSoft = (unsigned char *) malloc(reciveSize);
		memset(newSoft, 0, sizeof(newSoft));
		// odbieram rozmiar
		if(info.size() > 0)
		{
			cout << "wysyłam potwierdzenie otrzymania rozmiaru !!" << endl;
			msg.clear();
			msg = "ok\n\r";
			if((bytes_sent = send(sockfd, msg.c_str(), msg.size(), 0)) == -1)
			{
			//loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
			//exit(1);
			}
			cout << "wyslalem potwierdzenie rozmiaru !!" << endl;
			
			int j = 0;
			cout << "Przygotowuje się do pobierania softu." << endl;
			while(downloaded < reciveSize)
			{
				memset(pCAPData, 0, sizeof(pCAPData));
				bytes_recv = 0;
				if((bytes_recv = recv(sockfd, downloader, (buffer *10), 0)) == -1)
				{
					//loger << "recive error" << endl;
					perror("recive"); // logowanie do pliku !!
					//exit(1);
				}

				len = bytes_recv;

				downloaded += bytes_recv;

				for (int i = 0; i < len; i++)
				{
					newSoft[j] = downloader[i];
					j++;
					if(j == reciveSize) break;
				}

				cout << "Pobrałem: " << reciveSize << " - " <<downloaded << endl;
			}

			for (int i = 0; i < reciveSize; i++)
			{
				newApp << newSoft[i];
			}

			newApp.close();
			free(newSoft);

			cout << "sciagniete: " << downloaded << endl;
			cout << "rozmiar przyslany " << reciveSize << endl;
			if(downloaded == reciveSize)
			{
				cout << "zamykam newApp, tworze i zapisuje plik versionFlag" << endl;
				ofstream vFlag("versionFlag.txt", ios_base::out | ios_base::app);
				vFlag << data;
				vFlag.close();
				return 1;
			}

		}
	}
	else
	{
		if(info.compare("brak") == 0)
		{
			return 0;
		}
	}
}

string networkControler::getPointStatus(string cid)
{
	char recived[100000];
	memset(recived, 0, sizeof(recived));
	stringstream temp;
	string msg;
	int bytes_recv,bytes_sent;

	temp.str("");
	temp << "total;" << cid << endl;
	msg.clear();
	msg = temp.str();

	connectAllQuiet();

	if((bytes_sent = send(sockfd, msg.c_str(), msg.size(), 0)) == -1)
	{
	//loger << "send filetx; error" << endl;
	perror("send"); // logowanie do pliku !
	//exit(1);
	}



	if((bytes_recv = recv(sockfd, recived, sizeof(recived) -1, 0)) == -1)
	{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	temp.str("");
	temp << recived << endl;
	msg.clear();
	msg = temp.str();

	cout << "Odpowiedz na total; : " << msg << endl;

	disconnectAllQuiet();
	return msg;
}


int networkControler::sendAck(string date)
{
	connectAllQuiet();


	char pCAPData[buffer*10];
	char bufer[50000];
	memset(bufer, 0, sizeof(bufer));
	char temp[730];
	memset(temp, 0, sizeof(temp));
	int ulLen =0;
	int x =0;
	int ulHandle =0;
	memset(pCAPData, 0, sizeof(pCAPData));
	stringstream compose; 
	string msg;
	int sent;
	string info;
	compose.str("");
	cout << "tworze stream" << endl;
	string serNr = config->returnSeriall();
	compose << "cap-ack;" << serNr << ";" << date << endl; //<< endl;
	msg = compose.str();
	int len,bytes_sent,bytes_recv;
	int resend = 0;
	int size1 = 0;
	int size2 = 0;
	bytes_sent = 0;
	bytes_recv = 0;

	len = msg.size();
	cout << "wysylam zapytanie" << endl;
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
	{
		//loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);

	if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1)
	{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	compose.str("");
	compose << pCAPData;
	info = compose.str();


	if(info.compare("ok") == 0)
	{
		remove("versionFlag.txt");
		//execl("/bin/rm", "rm", "versionFlag.txt", (char *) 0);
	}

	disconnectAllQuiet();
}

int networkControler::fileSize(string fileName)
{
	int size;
	FILE *pFile = NULL;
	pFile = fopen(fileName.c_str(), "rb" );
	fseek( pFile, 0, SEEK_END );
	size = ftell( pFile );
	fclose( pFile );
	

	return size;
}

int networkControler::reloadConfig()
{
	config->reloadConfig();
}

int networkControler::updConf()
{
	string msg;
	stringstream compose;
	string snumer = config->returnSeriall();
	string confVer = config->returnConfVer();
	char pCAPData[buffer*10];
	char download[buffer*10];
	memset(pCAPData, 0, sizeof(pCAPData));
	memset(download, 0, sizeof(download));
	int len,bytes_sent,bytes_recv;
	connectAllQuiet();

	compose.str("");
	compose << "conf;" << snumer << ";" << confVer;
	cout << "Wysylam zapytsanie o config: " << compose.str() << endl;
	msg.clear();
	msg = compose.str();
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
	{
		//loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		return 0;
	}
	//sleep(1);

	if((bytes_recv = recv(sockfd, pCAPData,(buffer*10) -1, 0)) == -1)
	{
		//loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		return 0;
	}

	compose.str("");
	compose << pCAPData;
	msg.clear();
	msg = compose.str();

	cout << "Otrzymana wiadomosc" << msg << endl;
	if (msg.compare("brak") == 0)
	{
		cout << "Brak Konfiguracji !" << endl;
		return 0;
	}
	if (msg.compare("ok") == 0)
	{	
		cout << "pobieram nowa konfiguracje: " << msg << endl;
		remove("config.txt");

		ofstream file("config.txt", ios_base::app);
		// konfiguracja socketa !! 
		cout << "entering for !" << endl;
		for(int i =0; i<6; i++){
			sleep(1);
		    char str[40];
		    int dataLen;
		    sprintf(str, "%s" ,configs[1][i]);
		    char msg3[50];
		    strcpy(msg3, configs[0][i]);
		    if((bytes_sent = send(sockfd, str, strlen(str), 0)) == -1 ){
			    perror("send"); // logowanie do pliku !
				return 0;
    		}else{
				cout << "Sendet request for data "  << configs[1][i] << endl;
			}
    		memset(pCAPData, 0, sizeof(pCAPData));
    		if((bytes_recv = recv(sockfd, pCAPData, (buffer)-1, 0))== -1){
    			perror("reciv"); // logowanie do pliku !!
    			return 0;
    		}else{
				cout << "reciver dataLen: " << pCAPData << endl;
			}
    	    dataLen = atoi(pCAPData);
			cout << "wysylam potwierdzenie datalen" << endl;
    		if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))== -1){
    			perror("send"); // logowanie do pliku !!
    			return 0;
    		}else{
				cout << "Potwierdzeniew yslane poprawnie !!" << endl;
			}

    		memset(pCAPData, 0, sizeof(pCAPData));
			memset(download, 0, sizeof(download));
        	bytes_recv = 0;

			while(bytes_recv < dataLen){
				int recive = 0;
					if((recive = recv(sockfd, download, dataLen, 0))== -1){
				    		perror("Reciv"); // logowanie do pliku !!
				    		return 0;
			    	}
			    	else
			    	{
						cout << "recived part of this size: " << recive << endl;
					}
					bytes_recv += recive;
					strcat(pCAPData, download);
					memset(download, 0, sizeof(download));		
			}
			// przekazuje pobrane dane to stringa 
        	string dane(pCAPData);
			// tworze vector
        	vector<string> tokens;
			// prasuje ztringa i podaje go do vectora
        	Tokenize(dane, tokens, ";");
			// iteruje vector i wrzucam dane do pliku !!
        	std::vector<string>::iterator j;
			int licz=0;
			file << "[" << configs[1][i] << "]" << endl;
        	for(j=tokens.begin(); j<tokens.end(); ++j)
        	{
				if(configs[1][i]== "ok"){
					file << *j;
					file << endl;
				}else{
					file << "No" << licz << " = " << *j << endl;
					licz++;
				}
    	    }
			file << "[/" << configs[1][i] << "]" << endl;
			file << endl;
    	   // cout << endl;
			memset(pCAPData, 0, sizeof(pCAPData));
		}
		// potwierdzam zakonczenie pobierania danych !!
		sleep(1);

    	if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))==-1){
    	    perror("send");
    	    return 0;
    	}
    	file.close();

	}
	else
	{
		//nastapil blad !!
		return 0;
	}


	disconnectAllQuiet();
	return 1;
}

void networkControler::catFile()
{
	fstream file;
	file.open("tranzakcje.txt", ios_base::in | ios_base::binary);
	char temp[100000];
	int filesize = fileSize("tranzakcje.txt.bckp");
	int len = filesize/sizeof(struct Transaction);
	file.read(temp, len);
	string tempA = temp;
	tempA.erase(0, len);
	file.close();
	
	file.open("sample.txt", ios_base::out | ios_base::trunc | ios_base::binary);
	file << tempA;
	file.close();
}

int networkControler::getPointState()
{
	connectAllQuiet();
	
	
	
	
	disconnectAllQuiet();
}

int networkControler::sendTrx()
{
	// wyznacza godzine tranzakcji
	BYTE bTime[50];
	BYTE rTime[50];
	memset(bTime, 0, sizeof(bTime));
	memset(rTime, 0, sizeof(rTime));
	stringstream compose;
	GetTime(bTime); // pobieranie czasu 
	cout << "czas w bcd: " << bTime << endl;
	BcdToAsc(rTime,bTime, 12); // przetwazanie bcd na asci 
	cout << "czas w asce : " << rTime << endl;
	string trxAmountSize;
	compose.str("");
	compose << rTime;
	string date = compose.str();

	string footer = "DLA SPRZEDAWCY";
	string seriallNr = config->returnSeriall();
	etk1:
	cout << "jestem w send trx" << endl;
	//ofstream loger;
	ifstream filee("tranzakcje.txt");
	if(filee)
	{
	ifstream  src("tranzakcje.txt");
    ofstream  dst("tranzakcje.txt.bckp");

    dst << src.rdbuf();
    src.close();
    dst.close();
	//system("/bin/cp tranzakcje.txt tranzakcje.txt.bckp");
	//execl("/bin/cp", "cp", "tranzakcje.txt", "tranzakcje.txt.bckp", (char *) 0);
	}
	sleep(1);
	//cout << "otworzylem file stream loger" << endl;
	//loger.open("logs.txt", ios_base::app);
	//cout << "otwarlem plik i jest git" << endl;
	char pCAPData[buffer*10];
	char bufer[50000];
	memset(bufer, 0, sizeof(bufer));
	char temp[730];
	memset(temp, 0, sizeof(temp));
	int ulLen =0;
	int x =0;
	int ulHandle =0;
	memset(pCAPData, 0, sizeof(pCAPData));
	string msg;
	int sent = 0;
	string info;
	compose.str("");
	cout << "tworze stream" << endl;
	compose << "filetx;"; //<< endl;
	msg = compose.str();
	int len,bytes_sent,bytes_recv;
	int resend = 0;
	int size1 = 0;
	int size2 = 0;
	//len = msg.size();
	bytes_sent = 0;
	bytes_recv = 0;

	size1 = fileSize("tranzakcje.txt");
	size2 = fileSize("tranzakcje.txt.bckp");

	//ifstream file("tranzakcje.txt");
	
	if(size2 != 0)
	{
	repete2:		//compose.str("");
		compose << size2 << ";" << sizeof(struct Transaction) << endl;
		msg.clear();
		msg = compose.str();
		len = msg.size();
		// send file to the serwer
		cout << "wysylam zapytanie" << endl;
		if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
		{
			//loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
			//exit(1);
		}
	sleep(1);
	ulLen = 720;
	ifstream file("tranzakcje.txt.bckp", ios::in|ios::binary);
	int trxSendAmount = size2/sizeof(struct Transaction);
	compose.str("");
	compose << trxSendAmount;
	trxAmountSize = compose.str();
	while(x < size2)
	{
		if(sent < size2)
		{
			file.read(temp, ulLen);
			sent += ulLen;
		}
		else
		{
			ulLen = size2 - sent;
			file.read(temp,ulLen);
		}
	len = ulLen;
	cout << size2 << "-" << x << endl;

	//len = 720;
	//cout << "długość stringu" << len << endl;

		if((bytes_sent = send(sockfd, temp, len, 0)) == -1)
		{
				//loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
				//exit(1);
		}
		x += bytes_sent;
		memset(temp, 0, sizeof(temp));
		if(x == size2) break;
	}
		memset(pCAPData, 0, sizeof(pCAPData));
		if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1)
		{
			//loger << "recive error" << endl;
			perror("recive"); // logowanie do pliku !!
			//exit(1);
		}
		compose.str("");
		compose << pCAPData;
		info = compose.str();
		if((info.compare("ok")) == 0)
		{

				cout << "sprawdzam rozmiar" << endl;
				size1 = fileSize("tranzakcje.txt");
				cout << "tranzakcje.txt size: " << size1 << endl;
				size2 = fileSize("tranzakcje.txt.bckp");
				cout << "tranzakcje.txt.bckp size: " << size2 << endl;

			if(size1 == size2){
				remove("tranzakcje.txt");
				remove("tranzakcje.txt.bckp");
				device->printSend(seriallNr, date, trxAmountSize, footer);
				return 1;
			}
			else
			{
				//funkcja ktora wycina wyslane dane ! 
				catFile();
				remove("tranzakcje.txt.bckp");
				device->printSend(seriallNr, date, trxAmountSize, footer);
				return 0;
			}
			//loger << "serwer recived msg properly" << endl;
		}
		else
		{
			if(resend > 3)
			{
				file.close();
				return 0;
			}
			resend++;
			goto repete2;
			//loger << "serwer didn't recive message" << endl;
		}	
		
		
		//fclose( pFile );
	}
	else
	{
		//send 0;0;
		compose.str();
		compose << "0;0" << endl;
		msg.clear();
		msg = compose.str();
		len = msg.size();
	repete:		
		if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1)
		{
			//loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
			//exit(1);
		}
		// czekamy na ok
		if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1)
		{
			//loger << "recive error" << endl;
			perror("recive"); // logowanie do pliku !!
			//exit(1);
		}
		compose.str("");
		compose << pCAPData;
		info = compose.str();
		if((info.compare("ok")) == 0)
		{
			return 1;
			//loger << "serwer recived msg properly" << endl;
		}
		else
		{
			//loger << "serwer didn't recive message" << endl;
			if(resend > 3)
			{
				//loger.close(); 
				return 0;
			}
			resend++;
			goto repete;
		}	
	}


	//loger.close();
	return 0;
}

int networkControler::checkSignalStr()
{
	int ret,sig;
	sig = -1;
	ret = Wls_CheckSignal(&sig);
	switch(sig){
		case NO_SIGNAL:
		Lcd_Icon(1, ICON_ON, 1);
		break;
		case SIGNAL_VERY_WEAK:
		Lcd_Icon(1, ICON_ON, 2);
		break;
		case SIGNAL_NORMAL:
		Lcd_Icon(1, ICON_ON, 3);
		break;
		case SIGNAL_STRONG:
		Lcd_Icon(1, ICON_ON, 4);
		break;
		case SIGNAL_VERY_STRONG:
		Lcd_Icon(1, ICON_ON, 5);
		break;
		default:
		Lcd_Icon(1, ICON_OFF, 1);
		break;
	}
}

void networkControler::gprsInit()
{
	Lcd_Cls();
	Wls_InputUidPwd((BYTE *)user.c_str(), (BYTE *)password.c_str() );
	int ret;

	//ret = Wls_Init();
	//DelayMs(500); 
	//if(ret != ERR_OK){
	//	Lcd_Printxy(0,8,0, "Blad modolu gprs");
	//}

	for(int i = 0; i < 100; i++)
	{
	
	checkSignalStr();
	DelayMs(200);
	}
	
	/*ret = Wls_SetBaudrate(115200);
	DelayMs(200);
	if(ret != ERR_OK){
		Lcd_Cls();
		Lcd_Printxy(0,0,0, "Blad przy ustawianiu");
	}*/
	Lcd_Printxy(0,16,0, "Spr obecnosc k.sim");

	ret = Wls_CheckSim();
	DelayMs(500);
	if(ret == ERR_OK)
	{
		Lcd_Printxy(0,24, 0, "SIM = OK");
	}else{
		Lcd_Printxy(0,24, 0, "Brak karty SIM");
	}	
}

int networkControler::gprsConnect()
{
	gprs_apnConnected = false;
	gprs_serverConnected = false;
	int i,j,ret, signal;
	uchar *send_buf;
	bool state;
	int SERVER_CONNECT_TIMEOUT = 30000;
	int APN_CONNECT_TIMEOUT = 20000;
	int retry =0;
	// inicjalizacja danych do logowania !! 
	
	do {
		if(!gprs_apnConnected)
		{
			for(int i = 0; i < 100; i++)
			{
				signal =1;
				j = Wls_CheckSignal(&signal);
				if(signal >= SIGNAL_WEAK && signal <= SIGNAL_VERY_STRONG)
				{
					break;
				}
				DelayMs(200);
			}
			if(i>100)
			{
				Lcd_Cls();
				Lcd_Printxy(0,0,0, "Brak sygnalu");
				DelayMs(30000);
			}
			for(int i = 0; i <2; i++)
			{
				ret = Wls_Dial(const_cast<char *>(apn.c_str()) );
				if(ERR_OK == ret)
				{
					gprs_apnConnected = true;
					break;
				}
				DelayMs(APN_CONNECT_TIMEOUT);		
			}
			if(i>2)
			{
				Lcd_Cls();
				Lcd_Printxy(0,0,0,"Timeout polaczenia z APN");
				return 0;
			}
			
			for(int i = 0; i < 2; i++)
			{
				ret = Wls_MTcpConnect(socket0, const_cast<char *>(ip.c_str()), const_cast<char *>(port.c_str()), SERVER_CONNECT_TIMEOUT);
				if(ERR_OK==ret)
				{
					gprs_serverConnected = true;
					break;
				}
			}
			if(i>2)
			{
				Lcd_Cls();
				Lcd_Printxy(0,0,0,"Blad polaczenia z serwerem");
				return 0;
			}else{
				Lcd_Cls();
				Lcd_Printxy(0,0,0, "Polaczono z serwerem !");
				
			}
		}
		
		retry++;	
	}while( (!gprs_apnConnected || !gprs_serverConnected) && (retry<2) );


		if(gprs_serverConnected)
		{
			int recv_len;
			stringstream compose;
			compose << "nr;" << serialN.c_str();
			string str = compose.str();
			char *przeciep;
			strcpy(przeciep,str.c_str());
			send_buf = (uchar *) przeciep;
			ret = Wls_MTcpSend(socket0, send_buf, str.size());
			if(ERR_OK != ret)
			{
				Lcd_Cls();
				Lcd_Printxy(0,0,0,"Sending failed !");
				Wls_MTcpClose(socket0);
			}else{
				Lcd_Cls();
				Lcd_Printxy(0,0,0,"Sending done properly");
			}
		}	
}

int networkControler::gprsCon()
{
	ofstream file("config.txt", ios_base::app);
	ofstream loger("logs.txt", ios_base::app);
	
	loger << "start gprsCon" << endl;
	// konfiguracja socketa !! 
	char pCAPData[buffer];
	char download[buffer];
	//struct sockaddr_in dest_addr;

	int len,bytes_sent,bytes_recv;
	stringstream compose;
	compose << "nr;" << serialN.c_str() << endl; 
	string msg = compose.str();
	len = msg.size();
	bytes_sent = Wls_MTcpSend(socket0, (uchar *) msg.c_str(), len);
	if(ERR_OK == bytes_sent){
		loger << "send serial error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);
	loger << "przedstawilem sie serwerowi" << endl;
	string msg2 = "conf;000001030100397;2009-06-02 00:00\0";
	len = msg2.size();
	
	bytes_sent = Wls_MTcpSend(socket0, (uchar *) msg2.c_str(), len);
	if(ERR_OK != bytes_sent){
		loger << "send problem" << endl;
		perror("send");
		Wls_MTcpClose(socket0);
	}
	
	bytes_recv = Wls_MTcpRecv(socket0,(uchar *) pCAPData, buffer, 0, 30000);
	if(ERR_OK == bytes_recv){
		loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	loger << "ok sendign" << endl;
	if(strcmp((char*)pCAPData, "ok") != 0){
		loger << "send ok error" << endl;
		perror("nieudane polaczenie"); // logowanie do pliku !!
		//exit(1);
	}
	loger << "entering for !" << endl;
	for(int i =0; i<6; i++){
		//sleep(1);
	    char str[40];
	    int dataLen;
	    sprintf(str, "%s" ,configs[1][i]);
	    char msg3[50];
	    strcpy(msg3, configs[0][i]);
	    len = msg2.size();
		bytes_sent =  Wls_MTcpSend(socket0, (uchar *) str, strlen(str) );
		if(ERR_OK == bytes_sent){
			loger << "sending problem !" << endl;
		    perror("send"); // logowanie do pliku !
		 //   exit(1);
    	}else{
			loger << "Sendet request for data "  << configs[1][i] << endl;
		}
    	memset(pCAPData, 0, sizeof(pCAPData));
		bytes_recv = Wls_MTcpRecv(socket0,(uchar *) pCAPData, buffer, 0, 30*1000);
    	if(ERR_OK == bytes_recv ){
			loger << "recive eror datalen" << endl;
    		perror("reciv"); // logowanie do pliku !!
    	//	exit(1);
    	}
        dataLen = atoi(pCAPData);
		//cout << dataLen << endl;
    	//cout << "Wysylam potwierdzenie." << endl;
		loger << "wysylam potwierdzenie datalen" << endl;
		
		bytes_sent = Wls_MTcpSend(socket0, (uchar *) "ok", strlen("ok"));
    	if(ERR_OK == bytes_sent){
			loger << "error sending ok for data len" << endl;
    		perror("send"); // logowanie do pliku !!
    	//	exit(1);
    	}
    	memset(pCAPData, 0, sizeof(pCAPData));
		memset(download, 0, sizeof(download));
        bytes_recv = 0;

		while(bytes_recv < dataLen){
			int recive = 0;
				bytes_recv = Wls_MTcpRecv(socket0,(uchar *) download, buffer, 0, 30*1000);
				if(ERR_OK == bytes_recv){
					loger << "error reciving data for: " << configs[1][i] << endl;
		    		perror("Reciv"); // logowanie do pliku !!
		    //		exit(1);
		    	}
				bytes_recv += recive;
				//sleep(0.5);
				strcat(pCAPData, download);
				memset(download, 0, sizeof(download));		
		}
		// przekazuje pobrane dane to stringa 
		loger << "copying pCAPData to dane" << endl;
        string dane(pCAPData);
		loger << "creatong vector for: " << configs[1][i] << endl;
		// tworze vector
        vector<string> tokens;
		// prasuje ztringa i podaje go do vectora
		loger << "tokenize data !" << endl;
        Tokenize(dane, tokens, ";");
		// iteruje vector i wrzucam dane do pliku !!
		loger << "creating iterator" << endl;
        std::vector<string>::iterator j;
		int licz=0;
		file << "[" << configs[1][i] << "]" << endl;
		loger << "writing data to file" << endl;
        for(j=tokens.begin(); j<tokens.end(); ++j){
			if(configs[1][i]== "ok"){
				file << *j;
				file << endl;
			}else{
				file << "No" << licz << " = " << *j << endl;
				licz++;
			}
        }
		file << "[/" << configs[1][i] << "]" << endl;
		file << endl;
		loger << "data writen" << endl;
       // cout << endl;
		memset(pCAPData, 0, sizeof(pCAPData));
	}
	// potwierdzam zakonczenie pobierania danych !!
	sleep(1);
	loger << "sending ok that the data was reciver properly" << endl;
	bytes_sent = Wls_MTcpSend(socket0, (uchar *)"ok", strlen("ok"));
    if(ERR_OK == bytes_sent){
        perror("send");
        //exit(1);
    }
	// koncze polaczenie z serwerem !! 
	sleep(1);
	loger << "sendin 'bye' and ending connection !" << endl;
	bytes_sent = Wls_MTcpSend(socket0, (uchar *) "bye", strlen("bye"));
    if(ERR_OK == bytes_sent){
        perror("send");
        //exit(1);
    }
	// zamykam plik konfiguracyjny !
    file.close();
	loger.close();
	// zamykam socket !
	//close(sockfd);	
}

int networkControler::ethConf()
{
	cout << "ehtCon pizda od srodka" << endl;
	// tworze plik konfiguracyjny !
	ofstream file("config.txt", ios_base::app);
	ofstream loger("logs.txt", ios_base::app);
	
	loger << "start ethCon" << endl;
	// konfiguracja socketa !! 
	char pCAPData[buffer*10];
	char download[buffer*10];
	struct sockaddr_in dest_addr;
	
	loger << "tworze socket" << endl;
	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		loger << "socket erros" << endl;
		perror("socket"); // ogowanie do pliku !! 
		//exit(1);
	}
		loger << "utworzylam socket" << endl;
	//int ports;
	//atoi(port.c_str());
	dest_addr.sin_family = AF_INET;
	loger << "konfiguruje port" << endl;
	dest_addr.sin_port = htons( atoi(port.c_str())	); // wstawic port 
	loger << "skonfigurowalem port" << endl;
	loger << "konfiguruje ip" << endl;
	dest_addr.sin_addr.s_addr = inet_addr(ip.c_str() ); // wstawic ip
	loger << "skonfigurowalem ip" << endl;
	memset(&(dest_addr.sin_zero), '\0', 8);

	loger << "lacze sie z serwerm" << endl;
	if(connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)) == -1){
		loger << "connect socket error !!" << endl;
		perror("connect"); // zamienic na logowanie do pliku !
		//exit(1);
	}
	loger << "otwarlem socket i polaczylem sie" << endl;
	
	int len,bytes_sent,bytes_recv;
	stringstream compose;
	compose << "nr;" << serialN.c_str() << endl; 
	string msg = compose.str();
	len = msg.size();
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
		loger << "send serial error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);
	loger << "przedstawilem sie serwerowi" << endl;
	compose.str("");
	compose << "conf;" << serialN.c_str() << ";2009-06-02 00:00\0";
	//string msg2 = "conf;000001030100397;2009-06-02 00:00\0";
	string msg2 = compose.str();
	len = msg2.size();
	if((bytes_sent = send(sockfd, msg2.c_str(), len, 0)) == -1){
		loger << "error send przedstawienie" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	
	if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1){
		loger << "recive error" << endl;
		perror("recive"); // logowanie do pliku !!
		//exit(1);
	}
	loger << "ok sendign" << endl;
	if(strcmp((char*)pCAPData, "ok") != 0){
		loger << "send ok error" << endl;
		perror("nieudane polaczenie"); // logowanie do pliku !!
		//exit(1);
	}
	loger << "entering for !" << endl;
	for(int i =0; i<6; i++){
		sleep(1);
	    char str[40];
	    int dataLen;
	    sprintf(str, "%s" ,configs[1][i]);
	    char msg3[50];
	    strcpy(msg3, configs[0][i]);
	    len = msg2.size();
	    if((bytes_sent = send(sockfd, str, strlen(str), 0)) == -1 ){
			loger << "sending problem !" << endl;
		    perror("send"); // logowanie do pliku !
		 //   exit(1);
    	}else{
			loger << "Sendet request for data "  << configs[1][i] << endl;
		}
    	memset(pCAPData, 0, sizeof(pCAPData));
    	if((bytes_recv = recv(sockfd, pCAPData, (buffer)-1, 0))== -1){
			loger << "recive eror datalen" << endl;
    		perror("reciv"); // logowanie do pliku !!
    	//	exit(1);
    	}else{
			loger << "reciver dataLen: " << pCAPData << endl;
		}
        dataLen = atoi(pCAPData);
		//cout << dataLen << endl;
    	//cout << "Wysylam potwierdzenie." << endl;
		loger << "wysylam potwierdzenie datalen" << endl;
    	if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))== -1){
			loger << "error sending ok for data len" << endl;
    		perror("send"); // logowanie do pliku !!
    	//	exit(1);
    	}else{
			loger << "Potwierdzeniew yslane poprawnie !!" << endl;
		}

    	memset(pCAPData, 0, sizeof(pCAPData));
		memset(download, 0, sizeof(download));
        bytes_recv = 0;

		while(bytes_recv < dataLen){
			int recive = 0;
				if((recive = recv(sockfd, download, dataLen, 0))== -1){
					loger << "error reciving data for: " << configs[1][i] << endl;
		    		perror("Reciv"); // logowanie do pliku !!
		    //		exit(1);
		    	}else{
					loger << "recived part of this size: " << recive << endl;
				}
				bytes_recv += recive;
				//sleep(0.5);
				strcat(pCAPData, download);
				memset(download, 0, sizeof(download));		
		}
		// przekazuje pobrane dane to stringa 
		loger << "copying pCAPData to dane" << endl;
        string dane(pCAPData);
		loger << "creatong vector for: " << configs[1][i] << endl;
		// tworze vector
        vector<string> tokens;
		// prasuje ztringa i podaje go do vectora
		loger << "tokenize data !" << endl;
        Tokenize(dane, tokens, ";");
		// iteruje vector i wrzucam dane do pliku !!
		loger << "creating iterator" << endl;
        std::vector<string>::iterator j;
		int licz=0;
		file << "[" << configs[1][i] << "]" << endl;
		loger << "writing data to file" << endl;
        for(j=tokens.begin(); j<tokens.end(); ++j){
			if(configs[1][i]== "ok"){
				file << *j;
				file << endl;
			}else{
				file << "No" << licz << " = " << *j << endl;
				licz++;
			}
        }
		file << "[/" << configs[1][i] << "]" << endl;
		file << endl;
		loger << "data writen" << endl;
       // cout << endl;
		memset(pCAPData, 0, sizeof(pCAPData));
	}
	// potwierdzam zakonczenie pobierania danych !!
	sleep(1);
	loger << "sending ok that the data was reciver properly" << endl;
    if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))==-1){
        perror("send");
        //exit(1);
    }
	// koncze polaczenie z serwerem !! 
	sleep(1);
	loger << "sendin 'bye' and ending connection !" << endl;
    if((bytes_sent = send(sockfd, "bye", 3, 0))== -1){
        perror("send");
        //exit(1);
    }
	// zamykam plik konfiguracyjny !
    file.close();
	// zamykam socket !
	close(sockfd);
}

int networkControler::startConf(int type)
{
	int conf;
	cout << "startConf pizda" << endl;
	switch(type){
		case 0:
			gprsInit();
			checkSignalStr();
			gprsConnect();
			gprsCon();
		break;
		case 1:
			cout << "ethCon pizdacz !" << endl;
			conf = ethConf();
		break;
		default:
		return 0;
			//zaloguj bledny typ;
			cout << "bledny typ" << endl;
		break;
	}
}

void networkControler::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos){
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}
}

const char* const networkControler::configs[2][6]=
{
{
"par.podst.", "zab.karty",
"opcje menu", "nagrody",
"sprzedawcy", "obliczenia"
},{
"ok", "lc",
"menu", "prize",
"seller", "comp"
}
};
