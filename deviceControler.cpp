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

char deviceControler::magCardScan(bool kbd){
	BYTE key = NOKEY;
	char track1[100];
	char track2[100];
	char track3[100];
	char *btrck;
	char trck[10];
	string str2;
	str2.clear();
	stringstream compo;
	string temp;
	int ret;
	bool presed = false;
	memset(track1, 0, sizeof(track1));
	memset(track1, 0, sizeof(track2));
	memset(track1, 0, sizeof(track3));
//	Lcd_Cls();
//	Lcd_Printxy(0,0,0,"Test czytnik magnet");
	
	if(Mcr_Open()<0)
	{
		// zapis do pliku nie moge zainicjalizowac urzadzenia !!
	}
	Kb_Flush();
while(1){
	key = NOKEY;
	Lcd_Cls();
	Lcd_Printxy(0,0,0, "Przeciagnij karte");
	if(kbd){
		Lcd_Printxy(0,32,0, const_cast<char *>(str2.c_str()) );
					if(!Kb_Hit()){
						key = Kb_GetKey();
						if(key != NOKEY){
							presed = true;
						}else{
							presed = false;
							key = NOKEY;
						}
					}
				//cout << "jestem przed switchem klawiszy" << endl;
				int k =0;
				if(presed){
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
						cout << "nacisnalem enterem" << endl;
						str2.clear();
						str2 = compo.str();
						if(str2.size() >= 6 && str2.size()<30){
							cout << "wyciepuje wartosc z klawci" << endl;
						return *str2.c_str();
						}else{
							Lcd_Cls();
							Lcd_Printxy(0,0,0,"Informacja");
							Lcd_Printxy(0,32,0, "Podales zakrotki badz zadlugi numer.");
							DelayMs(5000);
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
				}

		
	}
	DelayMs(50);	
	ret = Mcr_Read((BYTE *)track1, (BYTE *)track2, (BYTE *)track3);
	if (ret&0x80)
	{//detected swiping
		if(ret & 1){
			cout << "track 1" << endl;
			track1[10] = 0;
			for(int i = 0; i < 10; i++)
			{
				trck[i]=track1[i];
			}
			Mcr_Close();
			cout << "dane z track1 w trck: " << trck << endl;
			return *trck;
			break;
		}
		if(ret & 2){
			cout << "track 2: " << endl;
			track2[10] = 0;
			for(int i = 0; i < 10; i++)
			{
				trck[i]=track2[i];	
			}
			Mcr_Close();
			cout << "dane z track2 w trck: " << trck;
			return *trck;
			break;
		}
		if(ret & 4){
			cout << "track 3" << endl;
			track1[10] = 0;
			for(int i = 0; i < 10; i++)
			{
				trck[i]=track3[i];
			}

			Mcr_Close();
			cout << "dane z track3 w trck: " << trck << endl;
			return *trck;
			break;
		}
	}
	
}	
cout << "jestem za while przed mcrclose" << endl;
//	Mcr_Close();
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
