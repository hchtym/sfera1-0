#include "networkControler.h"

#define buffer 1024

using namespace std;

networkControler::networkControler(string *ip, port, string *apn, string *user, string *password){
	
	
}
networkControler::~networkControler(){
	
	
}

int networkControler::gprsCon(){
	
	
}


void networkControler::gprsInit(){
	
	
	
}

int networkControler::ethCon(){
	// tworze plik konfiguracyjny !
	ofstream file("config.txt", ios_base::app);
	
	// konfiguracja socketa !! 
	int sockfd;
	char pCAPData[buffer*buffer];
	char download[buffer* buffer];
	struct sockaddr_in dest_addr;
	
	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket"); // ogowanie do pliku !! 
		exit(1);
	}
	int ports;
	atoi(port.c_str());
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(ports); // wstawic port 
	dest_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // wstawic ip
	memset(&(dest_addr.sin_zero), '\0', 8);

	if(connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)) == -1){
		perror("connect"); // zamienic na logowanie do pliku !
		exit(1);
	}
	int len,bytes_sent,bytes_recv;
	string msg = "nr;000001030100397";
	len = msg.size();
	if((bytes_sent = send(sockfd, msg.c_str(), len, 0)) == -1){
		perror("send"); // logowanie do pliku !
		exit(1);
	}
	sleep(1);
	string msg2 = "conf;000001030100397;2009-06-02 00:00\0";
	len = msg2.size();
	if((bytes_sent = send(sockfd, msg2.c_str(), len, 0)) == -1){
		perror("send"); // logowanie do pliku !
		exit(1);
	}
	
	if((bytes_recv = recv(sockfd, pCAPData,(buffer* buffer) -1, 0)) == -1){
		perror("recive"); // logowanie do pliku !!
		exit(1);
	}

	if(strcmp((char*)pCAPData, "ok") != 0){
		perror("nieudane polaczenie"); // logowanie do pliku !!
		exit(1);
	}

	for(int i =0; i<6; i++){
		sleep(1);
	    char str[40];
	    int dataLen;
	    sprintf(str, "%s" ,configs[1][i]);
	    char msg3[50];
	    strcpy(msg3, configs[0][i]);
	    len = msg2.size();
	    if((bytes_sent = send(sockfd, str, strlen(str), 0)) == -1 ){
		    perror("send"); // logowanie do pliku !
		    exit(1);
    	}
    	memset(pCAPData, 0, sizeof(pCAPData));
    	if((bytes_recv = recv(sockfd, pCAPData, (buffer*buffer)-1, 0))== -1){
    		perror("reciv"); // logowanie do pliku !!
    		exit(1);
    	}
        dataLen = atoi(pCAPData);
		cout << dataLen << endl;
    	cout << "Wysylam potwierdzenie." << endl;
    	if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))== -1){
    		perror("send"); // logowanie do pliku !!
    		exit(1);
    	}
    	memset(pCAPData, 0, sizeof(pCAPData));
		memset(download, 0, sizeof(download));
        bytes_recv = 0;

		while(bytes_recv < dataLen){
			int recive = 0;
				if((recive = recv(sockfd, download, dataLen, 0))== -1){
		    		perror("Reciv"); // logowanie do pliku !!
		    		exit(1);
		    	}
				bytes_recv += recive;
				sleep(0.5);
				strcat(pCAPData, download);
				file2 << download;
				memset(download, 0, sizeof(download));
//				cout << recive << "-" << bytes_recv << endl; 		
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
		file << "[" << config[1][i] << "]" << endl;
        for(j=tokens.begin(); j<tokens.end(); ++j){
			if(config[1][i]== "ok"){
				file << *j;
				file << endl;
			}else{
				file << "No" << licz << " = " << *j << endl;
				licz++;
			}
        }
		file << endl;
        cout << endl;
		memset(pCAPData, 0, sizeof(pCAPData));
	}
	// potwierdzam zakonczenie pobierania danych !!
	sleep(1);
    if((bytes_sent = send(sockfd, "ok", strlen("ok"), 0))==-1){
        perror("send");
        exit(1);
    }
	// koncze polaczenie z serwerem !! 
	sleep(1);
    if((bytes_sent = send(sockfd, "bye", 3, 0))== -1){
        perror("send");
        exit(1);
    }
	// zamykam plik konfiguracyjny !
    file.close();
	// zamykam socket !
	close(sockfd);
	
	
	
}

int networkControler::startConf(int type){
	switch(type){
		case 0:
			gprsInit();
			gprsCon();
		break;
		case 1:
			ethCon();
		break;
		default:
			//zaloguj bledny typ;
		break;
	}
	
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

const char* const networkControler::configs[][]={
{
"par.podst.", "zab.karty",
"opcje menu", "nagrody",
"sprzedawcy", "obliczenia"
},{
"ok", "lc",
"menu", "prize",
"seller", "comp"
}
}
