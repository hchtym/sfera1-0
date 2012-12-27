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

int deviceControler::atc24Read(){
	stringstream compose;
	string name;
	BYTE buf[1024];
	while(1){
		memset(buf, 0, sizeof(buf));
	int cardtype = At24c_Detect();
		if(ERR_OK != At24c_Open(cardtype))
		{
			Lcd_Cls();
			Lcd_Printxy(0,0,0, "No atc24 Card to open");
		}else{
			At24c_Read(1,100, buf);
			DelayMs(300);
			//compose << buf;
			//name = compose.str();
			char str[1040];
			sprintf(str, "%s", buf);
			Lcd_Cls();
			Lcd_Printxy(0,0,0, str);
			//Lcd_Printxy(0,0,0, const_cast<char *>(name.c_str()) );
			At24c_Close();
			compose.str("");
			DelayMs(8000);
		}
	}		
}

int deviceControler::sleRead(){
	BYTE buf[50] = {0};
	stringstream compose;
	string name;
	int ret;
	while(1){
	ret = SLE_Read(0,5,buf);
		if(ret != ERR_OK){
			Lcd_Cls();
			Lcd_Printxy(0,0,0, "No sle Card to open");
		}else{
			compose << buf;
			name = compose.str();
			Lcd_Cls();
			Lcd_Printxy(0,0,0, const_cast<char *>(name.c_str()) );
		}

		DelayMs(2000);
	}
}

int deviceControler::chipCardScan(){
	
	
	
}

int deviceControler::magCardScan(stringstream &stream){
	char track1[11];
	char track2[11];
	char track3[11];
	int ret;
	memset(track1, 0, sizeof(track1));
	memset(track1, 0, sizeof(track2));
	memset(track1, 0, sizeof(track3));
//	Lcd_Cls();
//	Lcd_Printxy(0,0,0,"Test czytnik magnet");
	
	if(Mcr_Open()<0)
	{
		// zapis do pliku nie moge zainicjalizowac urzadzenia !!
	}
while(1){
	DelayMs(50);	
	ret = Mcr_Read((BYTE *)track1, (BYTE *)track2, (BYTE *)track3);
	if (ret&0x80)
	{//detected swiping
	if(ret & 1){
		Lcd_Cls();
		Lcd_Printxy(0,0,0,"Track 1 is OK");
		cout << track1 << endl;
		Lcd_Printxy(0,8,0, track1);
			track1[10] = 0;
			char strg[10];
			sprintf(strg, "%s", track1);
			stream << strg;
			cout << strg << endl;
			cout << track1 << endl;
			cout << stream.str() << endl;
			//return 0;
		break;
	        //Track 1 is OK
	}else {
	        //Track 1 is error
	}
	if(ret & 2){
		Lcd_Cls();
		Lcd_Printxy(0,0,0,"Track 2 is OK");
		cout << track2 << endl;
		Lcd_Printxy(0,8,0, track2);
			track2[10] = 0;
			stream << (char *)track2;
			//return 0;
	        //Track 2 is OK
	    }else{
	        //Track 2 is e
	}
	if(ret & 4){
		Lcd_Cls();
		Lcd_Printxy(0,0,0,"Track 3 is OK");
		cout << track3 << endl;
		Lcd_Printxy(0,8,0, track3);
			track3[10] = 0;
			stream << (char *)track3;
			//return 0;
	        //Track 3 is OK
	}else{
	        //Track 3 is error
	    }
	}
}	
	Mcr_Close();
	return 0;
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
