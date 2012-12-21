#include "deviceControler.h"

using namespace std;

deviceControler::deviceControler(){

};

int deviceControler::rfidScan(){;
	BYTE buf[40] = {0};
	string str;
	string str2;
	string str3;
	RF_Init();
	
	while(1){

	if(ERR_OK == RF_WaitCard_Timeout(RF_M1, buf, 150)){
		cout << "M1" << endl;
		if(buf[0] > 0){
			sprintf(str1, "%x\n",(long)buf+1);
			hexStr(str2, buf + 1, buf[0]);
			break;
		}
	}

	if(ERR_OK == RF_WaitCard_Timeout(RF_TYPE_A, buf, 150)){
		cout << "typeA" << endl;
		if(buf[0] > 0){
			sprintf(str1, "%x\n",(long)buf+1);
			hexStr(str2, buf + 1, buf[0]);
			break;
		}
	}


		
}

}

int deviceControler::rfidMemWrite(){
	
	
}
