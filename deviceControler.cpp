#include "deviceControler.h"

using namespace std;

deviceControler::deviceControler(){

};

int deviceControler::rfidScan(){;
	BYTE buf[40] = {0};
	char str[20]= {0};
	char str2[20]= {0};
	char str3[20]= {0};
	RF_Init();
	DelayMs(1000);
	Lcd_Cls();
	Lcd_Printxy(0,0,0, "Test RFID");
	
	
	while(1){

		if(ERR_OK == RF_WaitCard_Timeout(RF_M1, buf, 150)){
			cout << "M1" << endl;
			if(buf[0] > 0){
				sprintf(str, "%x\n",(long)buf+1);
				hexToString(str2, buf + 1, buf[0]);
				break;
			}
		}

		if(ERR_OK == RF_WaitCard_Timeout(RF_TYPE_A, buf, 150)){
			cout << "typeA" << endl;
			if(buf[0] > 0){
				sprintf(str, "%x\n",(long)buf+1);
				hexToString(str2, buf + 1, buf[0]);
				break;
			}
		}


		
	}
	RF_Close();
	
	int len = strlen(str2);
	Lcd_Cls();
	if(len <= 16){
		Lcd_Printxy(0,0,0, str2);
                //CBasicDialog::Show("nr karty", 0, 0 , str2);
    }else{
		if(len <= 32){
			Lcd_Printxy(0,0,0, str2);
                //CBasicDialog::Show("nr karty", 0, str2, str2+16);
		}else{
	 		if(len <= 48){
				Lcd_Printxy(0,0,0, str2);
                //CBasicDialog::Show("nr karty", str2, str2+16, str2+32);
			}
 		}
	}

}

/*int deviceControler::rfidMemWrite(){
	
	
}*/

int deviceControler::atc24read(){
	stringstream compose;
	string name;
	while(1){
	int cardtype = At24c_Detect();
	if(ERR_OK != At24c_Open(cardtype))
	{
		Lcd_Cls();
		Lcd_Printxy(0,0,0, "No Card to open");
	}else{
		compose << cardtype << endl;
		name = compose.str();
	Lcd_Cls();
	Lcd_Printxy(0,0,0, const_cast<char *>(name.c_str()) );
	}
}
		
}

void deviceControler::hexToString(char *str, BYTE* buf, int len){
//	stringstream compose;
	        int j = 0;
	        for(int i = 0; i < len; i++){
	                str[j] = (buf[i]/16);
	                if(str[j] > 9)
	                        str[j] += 'A' - 10;
	                else
	                        str[j] += '0';
	                str[++j] = (buf[i]%16);
	                if(str[j] > 9)
	                        str[j] += 'A' - 10;
	                else
	                        str[j] += '0';
	                j++;
	        }
	        putchar('\n');
	        str[j] = 0;
	}
