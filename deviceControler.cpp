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
				hexStr(str1, buf + 1, buf[0]);
				break;
			}
		}

		if(ERR_OK == RF_WaitCard_Timeout(RF_TYPE_A, buf, 150)){
			cout << "typeA" << endl;
			if(buf[0] > 0){
				sprintf(str, "%x\n",(long)buf+1);
				hexStr(str1, buf + 1, buf[0]);
				break;
			}
		}


		
	}
	RF_Close();
	
	int len = strlen(str1);
	Lcd_Cls();
	if(len <= 16){
		Lcd_Printxy(0,0,0, str1);
                //CBasicDialog::Show("nr karty", 0, 0 , str2);
    }else{
		if(len <= 32){
			Lcd_Printxy(0,0,0 str1);
                //CBasicDialog::Show("nr karty", 0, str2, str2+16);
		}else{
	 		if(len <= 48){
				Lcd_Printxy(,0,0, str1);
                //CBasicDialog::Show("nr karty", str2, str2+16, str2+32);
			}
 		}
	}

}

int deviceControler::rfidMemWrite(){
	
	
}

void deviceControler:hexToString(string &str, BYTE* buf, int len){
	stringstream compose;
	void hexStr(char* buf, unsigned char* str, int len){
	        int j = 0;
	        for(int i = 0; i < len; i++){
	                buf[j] = (str[i]/16);
	                if(buf[j] > 9)
	                        buf[j] += 'A' - 10;
	                else
	                        buf[j] += '0';
	                buf[++j] = (str[i]%16);
	                if(buf[j] > 9)
	                        buf[j] += 'A' - 10;
	                else
	                        buf[j] += '0';
	                j++;
	        }
	        putchar('\n');
	        buf[j] = 0;
	}
	
	
	
	
}
