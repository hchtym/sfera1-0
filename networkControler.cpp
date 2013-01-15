#include "networkControler.h"

#define buffer (2048)


using namespace std;

networkControler::networkControler(string &ipr, string &portr, string &apnr, string &userr, string &passwordr, string &serialNr){
	cout << "pizda2" << endl;
	cout << "konfig z obiektu networkControler przed przekazaniem !!" << endl;
	ip = ipr;
	port = portr;
	apn = apnr;
	user = userr;
	password = passwordr;
	serialN = serialNr;
	
}

networkControler::~networkControler(){
	
	
}

int networkControler::connectAllQuiet(){
	ofstream loger("logs.txt", ios_base::app);
	loger << "start ethCon" << endl;
	// konfiguracja socketa !! 
	char pCAPData[buffer*10];
	char download[buffer*10];
	memset(pCAPData, 0, sizeof(pCAPData));
	memset(download, 0, sizeof(download));
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
	compose << "nr;" << serialN << endl; 
	string msg = compose.str();
	len = msg.size();
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
		loger << "send serial error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);
	loger << "otworzylem polaczenie sieciowe !!" << endl;
	loger.close();
}

int networkControler::disconnectAllQuiet(){
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
	
	
}

int networkControler::sendTransaction(){
	cout << "jestem w send trx" << endl;
	connectAllQuiet();
	cout << "wchodze do sendtrx" << endl;
	sendTrx();
	cout << "rozlaczam sie" << endl;
	disconnectAllQuiet();
}

int networkControler::fileSize(){
	int size;
	FILE *pFile = NULL;
	pFile = fopen("trx.txt", "rb" );
	fseek( pFile, 0, SEEK_END );
	size = ftell( pFile );
	fclose( pFile );
	
	return size;
}

int networkControler::sendTrx(){
	cout << "jestem w send trx" << endl;
	ofstream loger("logs.txt", ios_base::app);
	char pCAPData[buffer*10];
	char temp[730];
	memset(temp, 0, sizeof(temp));
	int ulLen =0;
	int ulHandle =0;
	memset(pCAPData, 0, sizeof(pCAPData));
	stringstream compose; 
	string msg;
	string info;
	int x;
	compose.str("");
	
	compose << "filetx;"; //<< endl;
	msg = compose.str();
	int len,bytes_sent,bytes_recv;
	int resend = 0;
	len = msg.size();
	bytes_sent = 0;
	bytes_recv = 0;
	/*if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
		loger << "send filetx; error" << endl;
		perror("send"); // logowanie do pliku !
		//exit(1);
	}
	sleep(1);*/
		cout << "sprawdzam rozmiar" << endl;
	int size = fileSize();
	cout << size << " oto rozmiar" << endl;
	if(size > 0)
	{
		//compose.str("");
		compose << size << ";" << sizeof(struct Transaction) << endl;
		msg.clear();
		msg = compose.str();
		len = msg.size();
		// send file to the serwer
		cout << "wysylam zapytanie" << endl;
		if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
			loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
			//exit(1);
		}
	sleep(1);
		// przechodze do przesylania pliku 
		cout << "otwieram plik" << endl;
		FILE *pFile = NULL;
		pFile = fopen("tranzakcje.txt", "r" );
		compose.str("");
		for(int i = 0; i < size; i+=720)
		{
			//fseek(pFile, ulHandle, SEEK_CUR);
			cout << i << " iteracja nr" << endl;
			ulLen = size -1;
			if(ulLen > 720) ulLen = 720;
			int j =0;
			while(x != EOF)
			{
				if(j > ulLen) break;
				cout << j << " znaczek numer" << endl;
				x = fgetc(pFile);
				if(x == EOF) break;
				temp[j]= x;
				j++;
			}
			/*for(int g =0; temp[i] != EOF; i++){
				msg += temp[i];
			}*/
			//msg.clear();
			//msg = temp;
			//cout << msg << endl;
			//cout << temp << endl;
			//len = msg.size();
			//int sended = 0;
				len = strlen(temp);
			//while(sended != len){
				if((bytes_sent = send(sockfd, temp, len, 0)) == -1){
					loger << "send filetx; error" << endl;
					perror("send"); // logowanie do pliku !
					//exit(1);
				}
			//	sended += bytes_sent;
			//}

		}
		memset(pCAPData, 0, sizeof(pCAPData));
		if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1){
			loger << "recive error" << endl;
			perror("recive"); // logowanie do pliku !!
			//exit(1);
		}
		compose.str("");
		compose << pCAPData;
		info = compose.str();
		if((info.compare("ok")) == 0)
		{
			loger << "serwer recived msg properly" << endl;
		}
		else
		{
			loger << "serwer didn't recive message" << endl;
		}	
		
		
		fclose( pFile );
	}else{
		//send 0;0;
		compose.str();
		compose << "0;0;" << endl;
		msg.clear();
		msg = compose.str();
		len = msg.size();
repete:		
		if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
			loger << "send filetx; error" << endl;
			perror("send"); // logowanie do pliku !
			//exit(1);
		}
		// czekamy na ok
		if((bytes_recv = recv(sockfd, pCAPData,(buffer) -1, 0)) == -1){
			loger << "recive error" << endl;
			perror("recive"); // logowanie do pliku !!
			//exit(1);
		}
		compose.str("");
		compose << pCAPData;
		info = compose.str();
		if((info.compare("ok")) == 0)
		{
			loger << "serwer recived msg properly" << endl;
		}
		else
		{
			loger << "serwer didn't recive message" << endl;
			if(resend > 3)
			{
				loger.close(); 
				return 0;
			}
			resend++;
			goto repete;
		}
		
	}
	
	loger.close();
	return 0;
}

int networkControler::checkSignalStr(){
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

void networkControler::gprsInit(){
	Lcd_Cls();
	Wls_InputUidPwd((BYTE *)user.c_str(), (BYTE *)password.c_str() );
	int ret;

	//ret = Wls_Init();
	//DelayMs(500); 
	//if(ret != ERR_OK){
	//	Lcd_Printxy(0,8,0, "Blad modolu gprs");
	//}

//	DelayMs(3000);
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

int networkControler::gprsConnect(){
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

int networkControler::gprsCon(){
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

int networkControler::ethConf(){
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
	string msg2 = "conf;000001030100397;2009-06-02 00:00\0";
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

int networkControler::startConf(int type){
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
			//cout << "ethCon pizdacz !" << endl;
			conf = ethConf();
		break;
		default:
		return 0;
			//zaloguj bledny typ;
		break;
	}
	
/*	if(type == 1){
		cout << "pizda z ifa" << endl;
		int t= ethConf();
	}*/

}

void networkControler::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " "){
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos){
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}

}

const char* const networkControler::configs[2][6]={
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
