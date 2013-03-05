#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include "ic_card.h"

#define POS_8200_OLD_BOARD

extern char append_info[2048];

/*注意不要超过定时器库定义的个数，这些宏在库里面也有定义，更新时注意一致*/
#define SYS_TIMER_COMMON	22
#define SYS_TIMER_CURSOR	23
#define SYS_TIMER_MENU	24
#define SYS_TIMER_INPUT	25
#define SYS_COM_RECV	26
#define SYS_TIMER_WLS_1	27
#define SYS_TIMER_WLS_2	28
#define SYS_TIMER_WLS_3	29
#define SYS_COMBINE_KEY	30

#define fl printf("file[%s]--function[%s()]--line[%d]\n", __FILE__, __FUNCTION__, __LINE__);

int ASCIIToHex(char *szInput, int iLenInput, unsigned char *szOutput);
int gt_hex_2_ascii(unsigned char *data, char *buffer, int len);
void dbgh(char *prompt, unsigned char *buf, int len);
unsigned char cal_lrc(unsigned char *p, unsigned int len);
int init_lcd(void);
int detect_24c(void);
void disp_power_off(void);

int test_lcd(void);
int test_keyboard(void);
int test_printer(void);
int test_wireless(void);
int test_mcr(void);
int test_transaction_flow(void);
int test_scanner(void);
int test_udisk(void);
int test_pboc_L1(void);
int disp_apdu(char *prompt_info, APDU_SEND *send_apdu, APDU_RESP *rp, int wait_key_confirm);
int pboc_L1_icc_command(int slot,APDU_SEND *ApduSend, APDU_RESP *ApduRecv);
int test_ped(void);
void set_ic_parameters(void);
int test_wifi(void);
int test_gps(void);
int echo_test(void);
int ping_test(char *if_name);
void log_cur_time(char *buf, char *hint);
void save_test_param(void);
int test_linux_random(void);
int test_serial_port(void);



#define OBJ_SLOT			0		//专用于测试，大卡
//#define	PBOC_L1_APP			1		//当编译PBOC L1测试程序时，必须打开此开关

#define SERIAL_PORT_HDMI	0
#define SERIAL_PORT_RJ11	1

typedef struct
{
	int lcd_contrast;
	int lang;
	int ic_vol;
	int ic_type;
	int ic_debug_flag;
	int pboc_protocol_interval;		//协议测试时的间隔，单位毫秒
	int pboc_protocol_debug;		//是否输出调试信息，默认关闭
	int key_hit_number_for_per_random;	//多少次按键以获取一个随机数

	//串口测试的配置
	int serial_port_number;
	int serial_baudrate;
	int serial_databit;
	int serial_stopbit;
	int serial_parity;
	
}TESTAPP_PARAM;

extern TESTAPP_PARAM g_test_param;

#endif
