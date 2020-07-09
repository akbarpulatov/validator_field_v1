
#include <stdlib.h>		
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <ctype.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>  
#include <pthread.h>

#include <time.h>  
#include "oslib.h"
#include "toolslib.h"

#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

static int getConsoleKey(int millisec);
extern void LC812_test(void);
const unsigned char aucLogoCL[] =
{
64,40,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x20,0x20,
0x10,0x10,0x10,0x10,0x08,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x40,0x40,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x40,0x10,0x08,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0xC0,0x80,0x00,0x00,0x70,0xF0,0xC0,0x00,0x0C,0x3E,0xFC,
0xE0,0x80,0x00,0x00,0x00,0xC0,0x20,0x00,0x20,0x20,0x20,0x21,0x22,0x42,0xC4,0x08,
0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x78,0x87,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x87,0xFF,0xFC,0x00,0x00,0xC7,0xFF,0xFC,0x00,0x00,0x83,0xFF,0xFE,0x00,0x00,0x03,
0xFF,0xFF,0x00,0x00,0x08,0x17,0x10,0x26,0x2B,0xB1,0xE1,0x62,0x42,0x86,0x07,0x09,
0x19,0x11,0x21,0x01,0x02,0x06,0x08,0x30,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x04,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x01,0x00,0x00,0x06,0x0F,0x03,0x00,0x00,0x38,0x3F,0x0F,0x01,0xC0,0xF0,0x7F,
0x1F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1F,0x12,0x94,0x61,0x2A,
0x24,0x38,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x01,0x02,0x0C,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x04,0x04,0x08,0x08,0x18,
0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x10,0x10,0x10,0x10,0x18,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


/**
 * @brief  显示GPS图标以及卫星数
 * @param[in] a 搜索到的卫星数量 4-29，其它数值清除图标。
 * @retval 0 成功
 * @retval other 失败
*********************************************************************************************************/
uint8_t LCD_GPS_Icon(uint8_t a);


/**
 * @brief 显示信号图标
 * @param[in] a 信号强度 取值范围 0 - 32 . 大于32-清除图标
 * @retval 0 成功
 * @retval 否则失败
 */
uint8_t LCD_Signal_Icon(uint8_t a);

/**
 * @brief 显示蓝牙图标
 * @param[in] a 图标类型
 * \li 0 wrong 空闲中
 * \li 1 right 正常工作
 * \li other 清除图标
 * 
 */
uint8_t LCD_BT_Icon(uint8_t a);
/**
 * @brief  显示电量图标
 * @param[in] a 电量 取值范围 0-100; 大于100-清除图标
 * @return 0 成功
 * @return other 否则失败
 */
uint8_t LCD_Electricity_Icon(uint8_t a);

#define WIDTH  128//160
#define HEIGHT 64//80

#define N (WIDTH * HEIGHT / 8)

/*//idtwo test
#define IDTWODEV_NAME "/dev/idtwo"
void  IDTWO_Init(void)
{
	int idtwofd;
	idtwofd = OSUART_Init("/dev/ttymxc6","115200,n,8,1");
	if(idtwofd == 0)
	{
		printf("open com2 err!\n");
	}
	idtwofd = open(IDTWODEV_NAME,O_RDWR);
	if(idtwofd < 0) {
			printf("open file[%s] error!\n",IDTWODEV_NAME);
	}
}
int IDTWO_UartTx(int handle,uint8_t *pucSend,uint32_t uiSendLen)
{
	return OSUART_TxStr(handle,pucSend,uiSendLen);
}
int IDTWO_UartRx(int handle,uint8_t *pucRev,uint32_t timeout_ms)
{
	 return OSUART_RxStr(handle,pucRev,1,timeout_ms);
}
void  IDTWO_Close(int handle)
{
	
	OSUART_Flush(handle);
	OSUART_Close(handle);
	close(handle); 
}

*/
#define IDTWODEV_NAME "/dev/idtwo"
#define SEND_SAM_DATA  0
#define GET_SAM_DATA   1

typedef struct _idtwo_struct
{
	char idtwoset;
	char idtwoget;
}idtwo_struct,*iidtwo_struct;

//"\xaa\xaa\xaa\x96\x69\x00\x03\x20\x01\x22"  寻卡
//"\xaa\xaa\xaa\x96\x69\x00\x03\x20\x02\x21" 选卡
//"\xaa\xaa\xaa\x96\x69\x00\x03\x30\x01\x32"读信息

int test_idtwo(void){
	#define IDTWO_DELAY 0x800000
	int idtwofd;
	int com7fd;
	char rx_buffer[200] = {0};
	char rxlen;
	int ret ;
	int i;

	idtwo_struct idpara;
	com7fd = OSUART_Init("/dev/ttymxc6","115200,n,8,1");
	if(com7fd == 0)
	{
		printf("open com7 err!\n");
	} 
	idtwofd = open(IDTWODEV_NAME,O_RDWR);
	if(idtwofd < 0) {
			printf("open file[%s] error!\n",IDTWODEV_NAME);
	}
//	rx_buffer[0] = 2;
//	ioctl(idtwofd, SEND_SAM_DATA, rx_buffer[0]);
//	rx_buffer[0] = 3;
//	ioctl(idtwofd, SEND_SAM_DATA, rx_buffer[0]);
	rx_buffer[0] = 0;
	ioctl(idtwofd, GET_SAM_DATA, &idpara);
	rxlen = idpara.idtwoget;
	printf("rxlen = %d\n",rxlen);
	//发寻卡指令
	OSUART_TxStr(com7fd,"\xaa\xaa\xaa\x96\x69\x00\x03\x20\x01\x22", 10);
	for(i=0;i<IDTWO_DELAY;i++){
		lseek(idtwofd,0, SEEK_SET);
		ret = read(idtwofd,rx_buffer,rxlen);
		if(ret>0)
		{
			break;
		}
	}
	
	if(i == IDTWO_DELAY)
		printf("IDTWO_DELAY error!\n");
	for(i=0;i<ret;i++)
		printf("rx_buffer = %2x\n",rx_buffer[i]);
	//OSUART_TxStr(com7fd,"\xaa\xaa\xaa\x96\x69\x00\x03\x20\x01\x22", 10);
	ret = OSUART_RxStr(com7fd,rx_buffer,sizeof(rx_buffer),100);
	if(ret){
		for(i=0;i<ret;i++)
			printf("rx_buffer = %2x\n",rx_buffer[i]);
	}
	OSUART_Flush(com7fd);
	close(idtwofd);
	return i;
	
}
int test_movie(int argc, char* argv[])
{
	int fd1;
	unsigned char buf[N] = {0};
	size_t nbyte = 0;
	int n = 1;

	if(argc < 2)
	{
		printf("Please input Movie name,Like this: %s <movie.bin>\n",argv[0]);
		return -1;
	}
	if((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		perror("Fail to open Movie file1!\n");
		return -1;
	}
	printf("\nBegin to play movie!\n");
	while((nbyte = read(fd1, buf, N)) > 0)
	{
                LCD_DisplayLogo(0,0,WIDTH,HEIGHT,buf,1);
		printf("\rFrame %d", n++);
		fflush(stdout);
		usleep(39000);
	}
	printf("\nPlay movie Over!\n");
	close(fd1);

	printf("Exit!\n");  
	return 0x00;
}
void test_drawrect(void)
{
    uint16_t x0 = 0,y0 = 0; 
    do
    {
       //LCD_ClearScreen(0);
       LCD_DrawRect(x0,y0,LCD_GetWidth() - x0 * 2,LCD_GetHeight()-y0 * 2,(void*)1);
       LCD_FillRect(x0,y0,LCD_GetWidth() - x0 * 2,LCD_GetHeight()-y0 * 2,(unsigned char )1);
       LCD_DrawEllipse(LCD_GetWidth()/2,LCD_GetHeight()/2,LCD_GetWidth()/2 - x0 - 2,LCD_GetHeight()/2-y0 - 2,1);
       LCD_FillEllipse(LCD_GetWidth()/2,LCD_GetHeight()/2,LCD_GetWidth()/2 - x0 - 2,LCD_GetHeight()/2-y0 - 2,0);
        x0 ++; y0 ++;
        if(x0 >= LCD_GetWidth() / 2)
            break;
        //if(y0 >= LCD_GetHeight() / 2)
        //    break;
        usleep(500);
    }while(1);
}

#define MAX_BALANCE 9999

void dump_data(char *str,unsigned char *text,int len)
{
	int i;
    printf("%s(%d):",str,len);
    for(i = 0;i < len;i++)
    {
        printf("%02X ",(unsigned char)text[i]);
    }
    printf("\n");
}

void test_sim(void){
    unsigned char atr[64],rpdu[300];
    unsigned int atr_len,rpdu_len;
    int ret;
    unsigned char apdu[] = "\x00\x84\x00\x00\x08";
    unsigned char slot = ICC_SIM1;
    
    ret = ICC_Init(slot);
    ret = ICC_GetATR(slot,atr,&atr_len);
    if(ret)
        printf("Atr error [%d]\n",ret);
    else
        dump_data("atr",atr,atr_len);
    
    ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
    if(ret)
        printf("exchange error [%d]\n",ret);
    else
        dump_data("rpdu<--",rpdu,rpdu_len);
    ICC_Close(slot);
}

int rsa_test_1984_65537(int loop_time)
{
    int i;
    unsigned char pucPublicKey[] ={
        0xCB,0xF2,0xE4,0x0F,0x08,0x36,0xC9,0xA5,0xE3,0x90,0xA3,0x7B,0xE3,0xB8,0x09,0xBD,
0xF5,0xD7,0x40,0xCB,0x1D,0xA3,0x8C,0xFC,0x05,0xD5,0xF8,0xD6,0xB7,0x74,0x5B,0x5E,
0x9A,0x3F,0xA6,0x96,0x1E,0x55,0xFF,0x20,0x41,0x21,0x08,0x52,0x5E,0x66,0xB9,0x70,
0xF9,0x02,0xF7,0xFF,0x43,0x05,0xDD,0x83,0x2C,0xD0,0x76,0x3E,0x3A,0xA8,0xB8,0x17,
0x3F,0x84,0x77,0x71,0x00,0xB1,0x04,0x7B,0xD1,0xD7,0x44,0x50,0x93,0x12,0xA0,0x93,
0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0x32,0xED,0x25,0xFE,0xD5,
0x2A,0x95,0x94,0x30,0x76,0x8C,0xC3,0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,
0x68,0xCC,0xD9,0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,
0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x82,
0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,
0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x52,0xA9,
0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x32,0xFD,0x8C,0x8A,
0xD9,0x12,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0x95,0x87,
0x69,0x75,0x89,0x97,0x89,0x78,0x94,0x30,0x76,0x8C,0xCD,0x90,0x2F,0xD3,0xF3,0x4B,
0x92,0xE7,0x92,0x4D,0x72,0x9C,0xB6,0x47,0x35,0x33,0xAE,0x2B,0x2B,0x55,0xBF,0x0E,
0x44,0x96,0x4F,0xDE,0xA8,0x44,0x01,0x17
    };
    unsigned int uiPublicKeyLen = 248;
    unsigned char pucPublicKeyExp[] = {0x01, 0x00, 0x01};
    unsigned int uiPublicKeyExpLen = 3;
    unsigned char pucIn[] ={0x00,0x01,0xE4,0x0F,0x08,0x36,0xC9,0xA5,0xE3,0x90,0xA3,0x7B,0xE3,0xB8,0x09,0xBD,
0xF5,0xD7,0x40,0xCB,0x1D,0xA3,0x8C,0xFC,0x05,0xD5,0xF8,0xD6,0xB7,0x74,0x5B,0x5E,
0x9A,0x3F,0xA6,0x96,0x1E,0x55,0xFF,0x20,0x41,0x21,0x08,0x52,0x5E,0x66,0xB9,0x70,
0xF9,0x02,0xF7,0xFF,0x43,0x05,0xDD,0x83,0x2C,0xD0,0x76,0x3E,0x3A,0xA8,0xB8,0x17,
0x3F,0x84,0x77,0x71,0x00,0xB1,0x04,0x7B,0xD1,0xD7,0x44,0x50,0x93,0x12,0xA0,0x93,
0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0x32,0xED,0x25,0xFE,0xD5,
0x2A,0x95,0x94,0x30,0x76,0x8C,0xC3,0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,
0x68,0xCC,0xD9,0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,
0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x82,
0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,
0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x52,0xA9,
0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x32,0xFD,0x8C,0x8A,
0xD9,0x12,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0x95,0x87,
0x69,0x75,0x89,0x97,0x89,0x78,0x94,0x30,0x76,0x8C,0xCD,0x90,0x2F,0xD3,0xF3,0x4B,
0x92,0xE7,0x92,0x4D,0x72,0x9C,0xB6,0x47,0x35,0x33,0xAE,0x2B,0x2B,0x55,0xBF,0x0E,
0x44,0x96,0x4F,0xDE,0xA8,0x44,0x01,0x17
    };
    unsigned int uiInLen = 248;
    unsigned char pucOut[248];
#if 1
    unsigned char pucOut2[] = {0x62,0xEA,0x15,0x1B,0x26,0x34,0x9F,0x59,0xE7,0x41,0x07,0xE7,0x05,0xEF,0x58,0xD0,
0xCC,0x3E,0xB2,0x8A,0x80,0xDC,0x39,0xF6,0x0D,0x82,0xC6,0xC3,0x10,0x90,0xAF,0xBE,
0xD9,0x84,0x71,0x4B,0xD8,0x06,0x88,0x43,0x78,0x67,0x16,0x11,0x46,0xF8,0x7B,0x4A,
0x34,0x7A,0xAC,0x98,0x6E,0x1B,0xA7,0xE9,0xCB,0x22,0x0A,0x8E,0x91,0x28,0x26,0x7B,
0xE0,0x25,0x59,0xED,0xD4,0x3F,0x67,0xA0,0x5F,0x11,0xEA,0x72,0xB9,0x61,0x6F,0x2E,
0xA3,0xDF,0x1A,0x3A,0x56,0x59,0x33,0x3D,0x59,0x91,0x8C,0x5E,0xBA,0xBC,0xBC,0xC6,
0x88,0x68,0x5C,0xD6,0x46,0xCB,0x88,0xEF,0x7A,0x8C,0x6D,0xAD,0x5A,0xEA,0xDF,0xDD,
0x86,0xC4,0xB3,0x10,0x8A,0x3A,0x64,0xC4,0x1F,0xEC,0xDF,0x03,0xAD,0xB9,0x32,0x42,
0xBA,0x24,0xE4,0x2E,0xB9,0x8C,0x4B,0x54,0x47,0x02,0xC2,0x60,0xD8,0xA7,0x22,0xA0,
0x0F,0x0A,0x35,0x71,0x0F,0x8C,0xA2,0xB4,0x3C,0x2A,0xA5,0x46,0x06,0x3B,0xAA,0xA8,
0xDE,0xF2,0x78,0x0C,0x06,0xD6,0x59,0xDC,0x6E,0x2A,0x09,0xA7,0x88,0xE1,0x35,0xA6,
0x80,0x68,0x51,0x32,0xFE,0x9E,0x8E,0xD6,0x2B,0x28,0x24,0x65,0x6B,0xE2,0x78,0x3F,
0x24,0x46,0x1F,0xAA,0x21,0xE3,0xE6,0x6A,0x99,0x60,0xDC,0x60,0xA5,0x19,0xD9,0x75,
0xA1,0xE4,0x5B,0xE5,0x37,0x1E,0xB8,0x64,0xF9,0x6D,0x4A,0x0C,0xE7,0xEB,0x1C,0xD0,
0xF6,0x1F,0x25,0x27,0x2D,0x5F,0x84,0x6C,0x93,0x03,0x2F,0x36,0xC6,0xAE,0xD9,0x04,
0x16,0xA0,0x30,0x64,0xB1,0xB9,0xDE,0xA1
    };
#endif
    for(i = 0;i < loop_time;i++)
    {
        RSA_PublicDecrypt(pucPublicKey, uiPublicKeyLen,
                        pucPublicKeyExp, uiPublicKeyExpLen,
                        pucIn, uiInLen,
                        pucOut);
        if(memcmp(pucOut,pucOut2,uiPublicKeyLen))
        {
            printf("rsa test error\n");
            return 0x01;
        }
        //dump_data("pucOut ",pucOut,uiPublicKeyLen);
    }
    return 0x00;
}
static void dump_register(void)
{
    uint8_t i;
    uint32_t data[0x39];
    uint32_t ret;
    
    
    extern uint16_t phhalHw_ReadRegister(
    uint8_t bAddress,
    uint32_t * pValue);
    
    printf("reading register value!\n");
    
    for(i = 0;i < 0x39;i++)
    {

        ret = phhalHw_ReadRegister(i, &data[i]);
        if(ret)
        {
            printf("read reg addr[%02X]err[0x%02x]\r\n",i,ret);
            break;
        }
        printf("addr[%02X]_[0x%08X]\r\n",i,data[i]);
    }

}

void test_rsa(void){
    
    unsigned long tick;
    unsigned int loop_time = 200;
    tick = OSTIMER_GetTickCount();
    rsa_test_1984_65537(loop_time);
    printf("rsa_test_1984_65537 Time Cost %ldms/%d  \n",OSTIMER_GetTickCount() - tick,loop_time);
}


void test_picc(void){
    unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret;
    unsigned int rpdu_len;
    int slot;
    int slot_num;

    if(Sys_TermType() == 2)
    	slot_num = 3;
    else
    	slot_num = 1;

    PICC_Open(0);

    //dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"ISO14443 TypeA Test");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Swipping card");

	if(Sys_TermType() == 2)
		LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"Please input three cards");

	for(slot = 0;slot < slot_num;slot++)
	{
		do{
			ret = PICC_PollCard(slot,0);
			if(!ret)
				Sys_Beep();
		}while(ret);
	}

	for(slot = 0;slot < slot_num;slot++)
	{
		dump_data("apdu--->",apdu,sizeof(apdu));
		ret = PICC_Exchange(slot, apdu,sizeof(apdu),rpdu,&rpdu_len);

		if(!ret /*&& !memcmp(rpdu[rpdu_len-2],"\x90\x00",2)*/)
		{
			dump_data("rpdu<---",rpdu,rpdu_len);
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TypeA Test Success");
			if(Sys_TermType() == 2)
				LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"slot[%d]",slot);
			OSTIMER_DelayMs(500);
		}
		else
		{
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TypeA Test error");
			if(Sys_TermType() == 2)
				LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"slot[%d]",slot);
			OSTIMER_DelayMs(500);
		 }
		PICC_Close(slot);
	}
}
void  print_start(char* str){
	printf("************  %s  ************",str);
	printf("\n");
	printf("\n");
}
void  print_greenok(char* str){
	printf("============================================\n");
	printf("!!!!!!!!!!  \033[42;37m%s\033[0m  !!!!!!!!!!",str);
	printf("\n");
	printf("============================================\n");
}
void  print_rederr(char* str){
	printf("########################################\n");
	printf("!!!!!!!!!!  \033[41;37m%s\033[0m  !!!!!!!!!!",str);
	printf("\n");
	printf("########################################\n");
}
void  print_end(char* str){
	printf("============================================\n");
	printf("!!!!!!!!!!  \033[42;37m%s\033[0m  !!!!!!!!!!",str);
	printf("\n");
	printf("============================================\n");
}
#define font_red   "31m"
#define font_new   "32m"
#define font_white "37m"
#define back_blue  "\033[44;"
#define back_red   "\033[41;"
#define back_green "\033[42;"
#define back_yellow "\033[43;"
#define end         "\033[0m"

static char *result_name[4] = {
		"did not test",
		"success",
		"error",
		"unknow"
};

//utf8字符长度1-6，可以根据每个字符第一个字节判断整个字符长度

//0xxxxxxx
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//
//定义查找表，长度256，表中的数值表示以此为起始字节的utf8字符长度
static const unsigned char utf8_look_for_table[] =
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define UTF8_LEN(x)  utf8_look_for_table[(x)]

//计算str字符数目
static int get_utf_to_gbk_len(char *str)
{
	int clen = strlen(str);
	int utf8_len = 0;
	char *ptr;
	int len = 0;

	for( ptr = str;*ptr && (len < clen);){
		len = UTF8_LEN((unsigned char)*ptr) ;
		if(len == 1)
			utf8_len += 1;
		else
			utf8_len += 2;
		ptr += len;
	}
	return utf8_len;
}

static void print_menu_str(char *head,char *menu,int result){
	int clen = strlen(menu);
	int gbk_str_len = get_utf_to_gbk_len(menu);
#define DISP_ALIGN_LEN 20
	if(gbk_str_len < DISP_ALIGN_LEN)
		clen += DISP_ALIGN_LEN - gbk_str_len;

	printf("(%s) %s%s %-*s %s",head,back_blue,font_white,clen,menu,end);

	result %= 4;
	if(result == 0)
		printf("-----%s%s [%s] %s \n",back_yellow,font_white,result_name[result],end);
	else if(result == 1)
		printf("-----%s%s [%s] %s \n",back_green,font_white,result_name[result],end);
	else if(result == 2)
		printf("-----%s%s [%s] %s \n",back_red,font_white,result_name[result],end);
}
static void  print_ok(char* str ,char space){
	printf("%s",str);
	if(space != NULL)
		printf("\n");
}
static  void print_error(char *str,char space){
	printf("\033[41;37m%s\033[0m",str);
	if(space != NULL)
		printf("\n");
}
static void felica_read_write(void){
	uint16_t    status;
	uint8_t       bLength;
	uint8_t       bMoreCardsAvailable;
	uint8_t       bRxNumBlocks;
	uint8_t       aNdefSystemCode[2] = {0x12, 0xFC};
	//uint8_t       aNdefSystemCode[2] = {0x01, 0x18};
	uint8_t       aBlockList[2] = {0x80, 0x00};
	uint8_t       aReadServiceList[2] = {0x0B, 0x00};
	uint8_t       aData[16] = {0};
	uint16_t      CheckSum = 0;
	uint8_t       bIndex = 0;

	aNdefSystemCode[0] = 0xFF;
	aNdefSystemCode[1] = 0xFF;
    /* Send ReqC with system code 0x12FC */

    status = phalFelica_ActivateCard(
        aNdefSystemCode,
        0x00,
        aData,
        &bLength,
        &bMoreCardsAvailable);
	printf("[%3d]phalFelica_ActivateCard status = %X,bMoreCardsAvailable=%d\n",__LINE__,status,bMoreCardsAvailable);
	sDumpData("aData",aData,sizeof(aData));

	{
		int i;
	    srand((int)time(0));
		for(i = 0; i < 0x0F;i++){
			aBlockList[0] = 0x80;

			aBlockList[1] = i;
			//aBlockList[1] = 0x01;
			/* Read attribute information data */
			status = phalFelica_Read(
				0x01,
				aReadServiceList,
				0x01,
				aBlockList,
				0x02,
				&bRxNumBlocks,
				aData);
			printf("[%3d] block[%d]phalFelica_Read status = %X\n",__LINE__,i, status);
			printf("read  aData block [%02X]",i);
			sDumpData("",aData,sizeof(aData));
			memcpy(aData, "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF", 16);

		    srand((int)time(0));
		    aData[1] = (uint8_t)rand();

			status = phalFelica_Write(
			                            1,      /**< [In] Number of Services. */
			                            "\x09\x00",     /**< [In] List of Services. */
			                            1,        /**< [In] Number of Blocks to send. */
										aBlockList,       /**< [In] List of Blocks to write. */
			                            2,  /**< [In] Number of Blocks to write. */
										aData        /**< [In] Block data to write. */
			                            );

			printf("[%3d]phalFelica_Write status = %X\n",__LINE__,status);
			sDumpData("write aData",aData,sizeof(aData));


		}
		for(i = 0x80; i < 0x86;i++){
			aBlockList[0] = 0x80;

			aBlockList[1] = i;
			//aBlockList[1] = 0x01;
			/* Read attribute information data */
			status = phalFelica_Read(
				0x01,
				aReadServiceList,
				0x01,
				aBlockList,
				0x02,
				&bRxNumBlocks,
				aData);
			printf("[%3d] block[%d]phalFelica_Read status = %X\n",__LINE__,i, status);
			printf("read  aData block [%02X]",i);
			sDumpData("",aData,sizeof(aData));

		}
	}
    srand((int)time(0));
    aData[1] = (uint8_t)rand();

	status = phalFelica_Write(
	                            1,      /**< [In] Number of Services. */
	                            "\x09\x00",     /**< [In] List of Services. */
	                            1,        /**< [In] Number of Blocks to send. */
								aBlockList,       /**< [In] List of Blocks to write. */
	                            2,  /**< [In] Number of Blocks to write. */
								aData        /**< [In] Block data to write. */
	                            );

	printf("[%3d]phalFelica_Write status = %X\n",__LINE__,status);
	sDumpData("write aData",aData,sizeof(aData));

    /* Send ReqC with system code 0x12FC */
    status = phalFelica_ActivateCard(
        aNdefSystemCode,
        0x00,
        aData,
        &bLength,
        &bMoreCardsAvailable);
	printf("[%3d]phalFelica_ActivateCard status = %X\n",__LINE__,status);

	aReadServiceList[0] = 0x09;
	aReadServiceList[1] = 0x00;
	/* Read attribute information data */
	status = phalFelica_Read(
		0x01,
		aReadServiceList,
		0x01,
		aBlockList,
		0x02,
		&bRxNumBlocks,
		aData);
	printf("[%3d]phalFelica_Read status = %X\n",__LINE__,status);
	printf("[%3d]bRxNumBlocks = %d\n ",__LINE__,bRxNumBlocks);
	sDumpData("aData",aData,sizeof(aData));
}

void test_felica(void){
	int ret ;
	int slot;
	int slot_num;
	uint8_t card_type;

	if(Sys_TermType() == 2)
		slot_num = 3;
	else
		slot_num = 1;

	PICC_Open(0);

	//dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"ISO14443 TypeA Test");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"swipping card");

	do{
		for(slot = 0; slot <= slot_num;slot ++){
			uint8_t uid_card[10],uid_size;
			unsigned long tick;
			tick = OSTIMER_GetTickCount();
			ret =  PICC_PowerOn(slot, 'C', &card_type,uid_card,&uid_size);
			//ret = PICC_PollCard(slot,0);
			if(!ret){
				printf("寻卡时间:%ld ms,card_type = %d\n",OSTIMER_GetTickCount() - tick,card_type);
				Sys_Beep();
				break;
			}
		}
	}while(ret);

	if(!ret && (card_type == TYPE_C_TCL)){
		felica_read_write();
		return;
	}

	PICC_Close(slot);
}


void test_felica_exchange(void){
	uint8_t cmd1[] = "\x06\x00\xFF\xFF\x00\x03";
	uint8_t cmd2[] = "\x06\x00\xFF\xFF\x00\x00";
	uint8_t cmd3[] = "\x10\x06\x01\x2E\x3D\x24\x69\x12\x70\x5A\x01\x0B\x00\x01\x80\x00";
//1D 07 01 2E 3D 24 69 12 70 5A 00 00 01 D0 D2 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF
	uint8_t cmd4[] = "\x20\x08\x01\x2E\x3D\x24\x69\x12\x70\x5A\x01\x09\x00\x01\x80\x00\xD0\x92\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF";
	uint8_t response[256];
	uint32_t resp_len;
	uint16_t status;
	uint8_t aIDmPMm[8 + 8];

	PICC_Open(0);
	PICC_TypeCInit(0);
	do{
	status = PICC_TypeCExchange(0,cmd1,sizeof(cmd1) - 1,response,&resp_len);
	if(!status)
		sDumpData("Response",response,resp_len);
	else
		break;
	status = PICC_TypeCExchange(0,cmd2,sizeof(cmd2) - 1,response,&resp_len);
	if(!status)
		sDumpData("Response",response,resp_len);
	else
		break;
	memcpy(aIDmPMm,&response[2],16);
	status = PICC_TypeCExchange(0,cmd3,sizeof(cmd3) - 1,response,&resp_len);
	if(!status)
		sDumpData("Response",response,resp_len);
	else
		break;
	memcpy(&cmd4[2], aIDmPMm , 8);
	status = PICC_TypeCExchange(0,cmd4,sizeof(cmd4) - 1,response,&resp_len);
	if(!status)
		sDumpData("Response",response,resp_len);
	else
		break;
	}while(0);
//0C 09 01 2E 3D 24 69 12 70 5A 00 00
}

void test_picc_single(void){
    unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret ;
    unsigned int rpdu_len;
    int slot;
    int slot_num;
    uint8_t card_type;

    if(Sys_TermType() == 2)
    	slot_num = 3;
    else
    	slot_num = 1;

    PICC_Open(0);

    //dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"ISO14443 TypeA Test");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"swipping card");

 	do{
 		for(slot = 0; slot <= slot_num;slot ++){
 		    uint8_t uid_card[10],uid_size;
 		    unsigned long tick;
 		    tick = OSTIMER_GetTickCount();
 		    ret =  PICC_PowerOn(slot, 0, &card_type,uid_card,&uid_size);
 			//ret = PICC_PollCard(slot,0);
 			if(!ret){
 				printf("Time of card detected:%ld ms\n",OSTIMER_GetTickCount() - tick);
 				Sys_Beep();
 				break;
 			}
 		}
	}while(ret);
	dump_data("apdu--->",apdu,sizeof(apdu));
	ret = PICC_Exchange(slot, apdu,sizeof(apdu),rpdu,&rpdu_len);

	if(!ret /*&& !memcmp(rpdu[rpdu_len-2],"\x90\x00",2)*/)
	{
		dump_data("rpdu<---",rpdu,rpdu_len);
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TTypeA test success");
		if(Sys_TermType() == 2)
			LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"slot[%d]",slot);
		OSTIMER_DelayMs(500);
	}
	else
	{
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TypeA test error");
		if(Sys_TermType() == 2)
			LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"slot[%d]",slot);
		OSTIMER_DelayMs(500);
	 }
	PICC_Close(slot);
}   

void test_mifare(void){
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16],data_len = 16;
    uint8_t snr[16],snr_len;
    int i;
    
    PICC_Open(0);

    ret = Mifare_PowerOn(0,snr,&snr_len);

    for(i = 0 ;i < 16 && !ret;i++){
        printf("\n==========Block[%2d]==========\n",i);
        if(!ret) ret = Mifare_AuthenBlock(MIFARE_KEY_A ,i * 4,key);
        
        if(!ret) ret = Mifare_ReadBlock(0 + i * 4,data);
        if(!ret) dump_data("Mifare Read0",data,data_len);
        
        if(!ret) ret = Mifare_ReadBlock(1 + i * 4,data);
        if(!ret) dump_data("Mifare Read1",data,data_len);
        
        
        if(!ret) ret = Mifare_ReadBlock(2 + i * 4,data);
        if(!ret) dump_data("Mifare Read2",data,data_len);

        if(!ret) ret = Mifare_ReadBlock(3 + i * 4,data);
        if(!ret) dump_data("Mifare Read3",data,data_len);
            
    }
    
}
void test_led(void){
    LCD_ClearScreen(0);
    //LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"led 测试");
    
    printf("--------led Tes --------\n");
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"all the led light up");
    LED_Control(LED_ALL,1);
    OSTIMER_DelayMs(1000);
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"all the led turn off");
    LED_Control(LED_ALL,0);
    OSTIMER_DelayMs(1000);
    //LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24,"led 同时闪烁");
    LED_Flicker(LED_ALL,100,100,10);
    OSTIMER_DelayMs(2500);
    //LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT32,"led 全闪");
    
    LED_Flicker(LED_BLUE,100,100,10);
    LED_Flicker(LED_YELLOW,500,500,10);
    LED_Flicker(LED_GREEN,1000,1000,10);
    LED_Flicker(LED_RED,2000,2000,10);
}
void test_LedSeg(void){
    int i;
	char key;
    for(i = 0 ;i < MAX_BALANCE + 1;i++){
        LedSeg_Balance(i);
    }
    LedSeg_Display(0,"01234567898.8.8.8.");
	printf("Please check the led segment adn press button>>\n");
	while(1)
	{
		key = Sys_WaitKey(100);
		if(key == 'R')
		{
			printf("Right Key !\n");
			key = 0;
		}
		if(key == 'L')
		{
			printf("Left Key !\n");
			key = 0;
		}
	}
}
void test_lcd(void){
	while(1){
		LCD_ClearScreen(1);
		OSTIMER_DelayMs(1000);
		LCD_ClearScreen(0);
		OSTIMER_DelayMs(1000);
    	LCD_Display_Row("LCD Test", 8,0,DISP_CENTER);
		LCD_Display_Row("LCD Test",24,0,DISP_CENTER);
    	LCD_Display_Row("LCD Test",40,0,DISP_CENTER|DISP_CLRLINE);
		LCD_Display_Row("LCD Test",56,0,DISP_CENTER);
    	LCD_Display_Row("LCD Test",72,0,DISP_CENTER);
		OSTIMER_DelayMs(2000);
		LCD_ClearScreen(1);      
		LCD_ClearScreen(0);
    	LCD_Signal_Icon(29);
		LCD_GPS_Icon(29);
		LCD_Display_Row("amount",        8, 88,DISP_FONT32);
		LCD_Display_Row("No 30",       24, 24,DISP_FONT24);
		LCD_Display_Row("$1.5",      40,80,DISP_FONT32);
		LCD_Display_Row("20:20",      72, 0,DISP_FONT6X8);
    	OSTIMER_DelayMs(2000);
	}
}

// 按键暂停（500ms超时）， 再按键继续
static int wait_key_suspend(uint32_t timeout){
    int key ;
    key = Sys_WaitKey(timeout);
    if(key > 0)
        key = Sys_WaitKey(0);
    return key;
}
void test_lcd_ng(void)
{
	int i;

	do
	{
        LCD_ClearScreen(1);      
		wait_key_suspend(1000);
		LCD_ClearScreen(0);
        wait_key_suspend(1000);
        LCD_Signal_Icon(29);
		LCD_GPS_Icon(29);
		LCD_Display_Row("amount",        8, 88,DISP_FONT32);
		LCD_Display_Row("No 30",       24, 24,DISP_FONT24);
		LCD_Display_Row("$1.5",      40,80,DISP_FONT32);
		LCD_Display_Row("20:20",      72, 0,DISP_FONT6X8);
        
		wait_key_suspend(1000);
        LCD_ClearScreen(0);
        LCD_Display_Row("LCD Test", 8,0,DISP_CENTER);
		LCD_Display_Row("LCD Test",24,0,DISP_CENTER);
        LCD_Display_Row("LCD Test",40,0,DISP_CENTER|DISP_CLRLINE);
		LCD_Display_Row("LCD Test",56,0,DISP_CENTER);
        LCD_Display_Row("LCD Test",72,0,DISP_CENTER);

		wait_key_suspend(1000);
		LCD_ClearScreen(0);
		LCD_Display_Row("card reading distance test:",8,0,DISP_CENTER);
		LCD_Display_Row("swipping card>>",24,0,DISP_CENTER);
		for(i =0; i<3; i++)
		{
			LCD_Clear_rows(56, 16);
			wait_key_suspend(500);
			LCD_Display_Row("card detected success",56,0,DISP_CENTER);
            
			Sys_BeepMs(100);
			wait_key_suspend(500);
		}
        if(wait_key_suspend(500) == 'R')
            break;
	}while(1);
    {
    uint8_t revert = DISP_REVERT;
    if(revert) revert = DISP_REVERT;
    LCD_ClearScreen(0);
    lcd_clearblock(0,0,160,16,!!revert);
    LCD_Signal_Icon(29|revert);
	LCD_BT_Icon(1|revert);
	LCD_GPS_Icon(25|revert);
	LCD_Electricity_Icon(50|revert);
	LCD_Display_Row("12:34",        4,110,DISP_FONT6X8|revert);
    }    
	LCD_Display_Row("$ 1.00",      16, 32,DISP_FONT32);
	LCD_Display_Row("swipping card or scan QR code",  48, 0,DISP_CENTER|DISP_FONT12);
	LCD_Display_Row("2017/12/29",  72, 6,DISP_FONT6X8);
	LCD_Display_Row("66666/888888",72,80,DISP_FONT6X8);
}
void test_lcd_speed(void){
    int i;
    unsigned long start;
#define _LCD_LOOP_TIME 10
    start = OSTIMER_GetTickCount();
    for(i = 0;i < _LCD_LOOP_TIME;i++){
        //LCD_ClearAll();
        //LCD_ClearIconLine();
        LCD_ClearScreen(0);
        LCD_DisplayIcon(ICON_BATTERY,88);
        LCD_DisplayIcon(ICON_SIGNAL,31);

        LCD_Printf(16,DISP_REVERT,"      LCD Test      ");
        
        //LCD_Display_Row("Welcome to iMX6UL World",0,0,0);
        //lcd_show_line(0,"融四岁，能让梨。弟于长，宜先知。",0,0,0);
        //lcd_show_line(1,"首孝悌，次见闻。知某数，识某文。",16,0,0);
        //lcd_show_line(2,"一而十，十而百。百而千，千而万。",40,0,0);
        LCD_Printf(32,DISP_UNDERLINE,"welcom to Decard");
        LCD_Printf(64,0,"<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        LCD_DrawRect(0,0,LCD_GetWidth(),LCD_GetHeight(),(void*)1);
    }
    printf("all %ld ms/%d \n",OSTIMER_GetTickCount() - start,_LCD_LOOP_TIME);
}
void test_lcd_logo(void){
    LCD_ClearScreen(0);
    LCD_DisplayLogo(16,16,aucLogoCL[0],aucLogoCL[1],(unsigned char *)&aucLogoCL[2],0);
}
void test_buzzer(void){
    Sys_BeepBeep(3,100,200);
}
void test_speaker(void){
#if 1
	system("aplay /opt/sound/yycs.wav 2> /dev/null &");
	OSTIMER_DelayMs(1000);
	system("aplay /opt/sound/7499.wav 2> /dev/null &");
#endif

}

void test_rtc(void){
    char curr_time[15];
    int ret;
	
    Sys_GetRealTime((char*)curr_time);
    printf("GetRealTime:%s\n",curr_time);
    printf("current time:%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            &curr_time[0],&curr_time[4],&curr_time[6],
            &curr_time[8],&curr_time[10],&curr_time[12]);
    strcpy(curr_time,"20181226144444");
    ret = Sys_SetRealTime(curr_time);
    printf("try to reset time to %s[%d]\n",curr_time,ret);
    
    strcpy(curr_time,"20381426144461");
    ret = Sys_SetRealTime(curr_time);
    printf("try to reset time to %s[%d]\n",curr_time,ret);   
}

int system_init(void){
    
    //char *font_file = "/opt/font/SIMSUN.TTC";
    char *font_file = "/opt/font/RTWSYueRoudGoG0v1-Regular.ttf";
    char *bg_file = "/opt/app/png/bg1.png";
    int fd;

    fb_init(font_file,bg_file);
    
	LCD_Init();
	Sys_Gps_Init("/dev/ttymxc4","9600,8,n,1");
	LedSeg_Init();
	Beep_Init();

	fd = QRCode_Open(0);
    //QRCode_Default(fd);
    close(fd);

	return 0x00;
}

char test_QRcode(void)
{
	int qrfd1,qrfd2,ret;
	unsigned char TmpBuff[1024];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
	unsigned long tick_last = 0,tick_end;
	unsigned int succ_num = 0;
	unsigned int tick_start = 0;
	
	printf("\n******** Start to test QR code ********>>\n");
    printf("scan the QR code>>\r\n");
	
    
    while(1)
	{
		ret = QRCode_RxStr(qrfd1, TmpBuff, 1024, 100);
		if(ret <= 0)
			ret = QRCode_RxStr(qrfd2, TmpBuff, 1024, 100);

		if(ret > 0)
		{
			tick_end = OSTIMER_GetTickCount() ;
			if(tick_start == 0)
				tick_start = tick_end;
			printf("\nscan the QR code:[length %2d] : [%s]\r\n",ret,TmpBuff);
			succ_num ++;
			
			if(tick_last)
				printf("[time interval %ld ms] time interval%d ms(%d)\n",tick_end - tick_last,
				(tick_end - tick_start)/succ_num,succ_num);
			else
				printf("Start to test the scan time interval\n");
			tick_last = tick_end;
			Sys_BeepMs(100);
		}
	}

    QRCode_Close(qrfd1);
    QRCode_Close(qrfd2);

}

char test_GPS(void)
{
	int i;
	int err=0;
    int time=0;
    GNRMC *mes_gps;
	//LCD_ClearScreen(0);
	//LCD_ClearAll();
	//LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"gps测试:");
	printf("\n********* Start to test GPS **********>>\n");
	Sys_Gps_Debug(1);
	for(i=0; i<10000; i++)
	{
		mes_gps = Sys_Gps_QueryLaLO("Asia/Shanghai");
		if(mes_gps == NULL)
		{
			printf("invalid data\n");
            err++;
		}
		else
		{
            if(mes_gps->sate_num == 0)
                err++;
			printf("\nvalid data:\n");
			printf("latitude:%08f\n",mes_gps->latitude_wgs84);
			printf("longitude:%08f\n",mes_gps->longitude_wgs84);
			printf("time:%s\n",mes_gps->date_time);
			printf("satenumber:%d\n",mes_gps->sate_num);
			printf("*******************************\n");
			printf("*****    Test GPS   OK   *****\n");
			printf("*******************************\n");
		}
        time++;
        printf("err/time:%d/%d\n",err,time);
        OSTIMER_DelayUs(813456);
	}
	if(i == 100)
	{
		printf("###############################\n");
		printf("#####    GPS Test error   #####\n");
		printf("###############################\n");
		err = (err*2) | 1 ;
		OSTIMER_DelayMs(3000);
	}
	return err;
}
static char *get_icc_name(uint8_t slot){

	switch(slot)
	{
		case ICC_SIM1:
			return "ICC_SIM1";
		case ICC_SIM2:
			return "ICC_SIM2";
		case ICC_SIM3:
			return "ICC_SIM3";
		case ICC_SIM4:
			return "ICC_SIM4";
        case ICC_ESAM:
        	return "ICC_ESAM";
		case ICC_SIM5:
			return "ICC_SIM5";
	}
	return "unknown";
}
static char *get_icc_bps(uint8_t slot){

	switch(slot)
	{
	case BAUD_9600:    return "9600";
	case BAUD_19200:   return "19200";
	case BAUD_38400:   return "38400";
	case BAUD_57600:   return "57600";
	case BAUD_115200: return "115200";
	case BAUD_55800:   return "55800";
	case BAUD_111600: return "111600";
	case BAUD_223200: return "223200";
	case BAUD_446400: return "446400";
	}
	return "unknown";
}
uint8_t get_icc_slot(uint8_t slot)
{
	switch(slot)
	{
		case 1:
			slot = ICC_SIM1;
			break;
		case 2:
			slot = ICC_SIM2;
			break;
		case 3:
			slot = ICC_SIM3;
			break;
		case 4:
			slot = ICC_SIM4;
			break;
		case 5:
			slot = ICC_ESAM;
			break;
		default:
			slot = ICC_SIM5;
			break;
	}
	return slot;
}
void test_Sam(uint8_t slot,uint8_t sambps)
{
	unsigned char atr[64],rpdu[300];
    unsigned int atr_len,rpdu_len;
    int ret;
    unsigned char apdu[] = "\x00\x84\x00\x00\x08";

	slot = get_icc_slot(slot);
	printf("slot = [%d] %s\n",slot,get_icc_name(slot));
	printf("sambps = [%d](%s)\n",sambps,get_icc_bps(sambps));

	ret = ICC_Init(slot);
	ret = ICC_PowerUp(slot,sambps,VOLT_5V,atr,&atr_len);
	if(ret)
		printf(" ICC_SIM Atr error [%d]\n",ret);
	else
		dump_data("ICC_SIM atr",atr,atr_len);
	if(ret == 0){
		ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
		if(ret)
			printf("ICC_SIM exchange error [%d]\n",ret);
		else
			dump_data("ICC_SIM rpdu<--",rpdu,rpdu_len);
	}
	ICC_Remove(slot);
}

void test_esam(uint8_t slot,uint8_t sambps)
{
	unsigned char atr[64],rpdu[300];
    unsigned int atr_len,rpdu_len;
    int ret;
    struct
	{
    	int len;
    	unsigned char apdu[300];
	}icc_command[] = {
    {7,"\x00\xA4\x00\x00\x02\xDF\x10\x00"},
    {50,"\x80\xEC\x01\x03\x2D\xEF\x03\xEF\x11\x10\x31\x32\x33\x34\x35\x36\x37\x38\x31\x32\x33\x34\x35\x36\x37\x38\x44\xA0\x00\x01\x00\x00\x00\x33\x3A\xCB\xE9\xD5\xDC\x69\x85\x55\x51\xA5\x0C\x43\x1B\x86\x9A\x5D\x00"},
    {0}
    };

	slot = get_icc_slot(slot);
	slot = ICC_ESAM;
	printf("slot = [%d] %s\n",slot,get_icc_name(slot));
	printf("sambps = [%d](%s)\n",sambps,get_icc_bps(sambps));

	ret = ICC_Init(slot);
	ret = ICC_PowerUp(slot,sambps,VOLT_5V,atr,&atr_len);
	if(ret)
		printf(" ICC_SIM Atr error [%d]\n",ret);
	else
		dump_data("ICC_SIM atr",atr,atr_len);
	if(ret == 0){
		int i;
		for(i = 0;i < 3;i++){
			if(icc_command[i].len == 0)
				break;
			dump_data("ICC_SIM apdu-->",icc_command[i].apdu,icc_command[i].len);
			ret = ICC_Exchange(slot,icc_command[i].apdu,icc_command[i].len,rpdu,&rpdu_len);
			if(ret)
				printf("ICC_SIM exchange error [%d]\n",ret);
			else
				dump_data("ICC_SIM rpdu<--",rpdu,rpdu_len);
		}
	}
	ICC_Remove(slot);

	ICC_Close(slot);//关闭句柄
}

int test_eeprom(void)
{
#define DATA_LEN 8192
#define EEPROM_NUM 2
    int i;
    uint8_t *buf_write = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    uint8_t *buf_read  = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    int ret = 0;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"eeprom Test:");
    
    if((buf_write == 0)||(buf_read == 0)){
            printf("malloc buffer error\n");
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Test error");
            goto _error;
    }
    
    for(i = 0;i < DATA_LEN;i++)
    {
        buf_write[i] = (uint8_t)i + '0';
    }
    for(i = 0;i < EEPROM_NUM;i++){
        
        dc_swr_eeprom(i,0,DATA_LEN,buf_write);
        memset(buf_read,0,DATA_LEN);
        dc_srd_eeprom(i,0,DATA_LEN,buf_read);	
        
        //dump_data("buf_read",buf_read,DATA_LEN);
        
        if(!memcmp(buf_read,buf_write,DATA_LEN)){
            printf("eeprom [%d] readwrite success\n",i + 1);
            LCD_Printf(32 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"[%d]Test success",i + 1);
        }
        else{
            printf("eeprom [%d] readwrite error\n",i + 1);
            LCD_Printf(32 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"[%d]Test error",i + 1);
            ret |= (1 << i);
        }
    }
_error:
    if(buf_read) free(buf_read);
    if(buf_write)free(buf_write);
	OSTIMER_DelayMs(500);

    return ret;
}

void test_gprscmd(void)
{
	int signum,act;
	char operator[100]={0};
	signum = ppp_get_signal();
	printf("signal number = %d\n", signum);
	ppp_network_type(operator, &act);
	printf("operator name: %s, act:%d.\n", operator, act);	
}

void test_power_down(void)
{	
	while(1)
	{
		printf("power if down: %02x\n",Sys_WaitPowDown(1000));
	}
}

void disp_realtime(void)
{
	unsigned char dispbuff[25] = {0};
	char realtimec[15]={0};
	Sys_GetRealTime((char*)realtimec);
	sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
      		&realtimec[0],&realtimec[4],&realtimec[6],
            &realtimec[8],&realtimec[10],&realtimec[12]);
	LCD_Printf(70, DISP_FONT6X8|DISP_RIGHT,dispbuff);
}
static void curr_info_thread(void *p){
	while(1){
		
		unsigned char dispbuff[25] = {0};
		char realtimec[15]={0};
		char snbuf[64];
		Sys_GetRealTime((char*)realtimec);
		Sys_GetSN((char*)snbuf);
			
		sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
				&realtimec[0],&realtimec[4],&realtimec[6],
				&realtimec[8],&realtimec[10],&realtimec[12]);
		//LCD_Printf(10, DISP_FONT6X8|DISP_RIGHT,dispbuff);

	    fb_disp_start();
	    lcd_erase_foreground(0,0,LCD_GetWidth(),16);
		LCD_Printf(1, DISP_FONT6X8|DISP_RIGHT,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s",
				&realtimec[0],&realtimec[4],&realtimec[6],
				&realtimec[8],&realtimec[10],&realtimec[12]);
		LCD_Printf(72, DISP_FONT6X8|DISP_CENTER,"S/N:%s",snbuf);
		fb_disp_end();
		OSTIMER_DelayMs(1000);
	}
}

static void test_curr_info(void){
	int rtc_thread;

	// 线程自销毁
	pthread_attr_t attr; //线程属性
	pthread_attr_init(&attr);  //初始化线程属性
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);      //设置线程属性

	pthread_create( &rtc_thread, &attr, curr_info_thread, (void*)NULL); 
}
void print_help(void)
{

	printf("------------------------help-------------------------\n");
	printf("command  P1   P2            P3  \n");
	printf("- - - -   1    1(or2)	        ISO 14443 typeA Test\n");
	printf("- - - -   2    1-5	  0         PSAM Test P2=slot, P3=baudrate:9600=0,19200=1,38400=2,57600=3,115200=4,55800=5,111600=6,223200=7,446400=8\n");
	printf("- - - -   3    1000(or2)        Buzzer Test\n");
	printf("- - - -   4    1(or2)	        LCD Test\n");
	printf("- - - -   5    1(or2)	        GPS Test\n");
	printf("- - - -   6    1(or2)	        QR code Test\n");
	printf("- - - -   7    1	        LED segment & Button Test\n");
	printf("- - - -   8    1-2	        FRAM-RTC Test 1:RTC Read & Write 2:RTC Read only\n");
	printf("- - - -   9    0	        LED Test\n");
	printf("- - - -   0    0		    Sound Test\n");
	printf("- - - -   a    0	        Internet Test\n");
	printf("- - - -   i    0	        GPRS module status check\n");
	printf("- - - -   j    0	        Bluetooth Test\n");
	printf("- - - -   k    100000	0-1     CAN Test  P2 baudrate，P3 mode\n");
	printf("- - - -   l    0	        COM5-485 Test (ttymxc4)\n");
	printf("- - - -   n    0	        COM2-232 Test (ttymxc1)\n");
	printf("- - - -   p    0	        Data Protection Test\n");
	printf("- - - -   t    0	        TCP/IP Test\n");
	printf("- - - -   q    0	        eeprom Test\n");
	printf("- - - -   r    0	        Auto Test\n");
	printf("- - - -   s    0	        P18-L2 LCD test\n");
	printf("- - - -   z    0            Scan QR code to test\n");
    printf("- - - -   A    0            lc812 Advertising screen test\n");
    printf("- - - -   B    0            T80-L2 contactless test\n");
    printf("- - - -   C    0-3            T80-L2 CL3000led Test\n");
}



int select_test(void)
{
	int fd,ret,i;
	int select;

	unsigned char TmpBuff[2048],snbuf[200]={"\0"};;
#define QR_SCAN_LOOP 100 // x100ms    
	print_start("scan code to enter test item");
	print_start("enter to the test item after 10s");
	memset(TmpBuff, 0, sizeof(TmpBuff));
	//LCD_Clrscr();
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT, "scan code to enter test item:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"enter to the auto test after 10s");
    
	fd = QRCode_Open(0);
    //QRCode_Default(fd);
    
	printf("Scan QR code>>\r\n");
	for(i = 0; i < QR_SCAN_LOOP; i++)
	{
		ret = QRCode_RxStr(fd,TmpBuff,1024,100);
		if(ret > 0)
		{	
			if(ret == 11)
			{	
				Sys_BeepMs(100);	
				ret = Sys_SetSN((char*)TmpBuff);
				ret = Sys_GetSN((char*)snbuf);
				if(!memcmp(snbuf,TmpBuff,strlen((char*)TmpBuff)))
				{
					LCD_ClearAll();
					LCD_Clear_rows(32,32);
					LCD_Printf(17, DISP_CENTER,"write sn code success>>");
					LCD_Printf(35, DISP_CENTER,snbuf);
					print_greenok("write sn code success");
					print_greenok(snbuf);
					system("sync");
					continue;
				}
				else
				{
					LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"write sn code error");
					print_rederr("write sn code error");
					OSTIMER_DelayMs(3000);
				}
			}
			Sys_Beep();
			printf("QR recive : [%s]\n",TmpBuff);
			OSTIMER_DelayMs(1);
			select = TmpBuff[ret -1];
		}
	}

	if(i == QR_SCAN_LOOP)
	{
		print_start("Enter to the auto test");
		OSTIMER_DelayMs(1000);
		select = 'f';
	}

    QRCode_Close(fd);

	return select;
}

int select_test_timeout(int millisecond)
{
	int fd,ret,i;
	int select = 0;

	unsigned char TmpBuff[2048],snbuf[200]={"\0"};;
#define QR_SCAN_LOOP 100 // x100ms
#define TEST_CASE_HEAD "p18_l1_test_"
	//print_start("请扫描选择测试项");
	//print_start("10秒后进入自动测试");
	memset(TmpBuff, 0, sizeof(TmpBuff));
	//LCD_Clrscr();
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT, "scan code to select the test item:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"enter to the auto test after 10s");

	fd = QRCode_Open(0);

	//printf("请扫二维码>>\r\n");
	for(i = 0; i < millisecond/100; i++)
	{
		ret = QRCode_RxStr(fd,TmpBuff,1024,150);

		if(ret > 0)
		{
			if(ret == 11)
			{
				Sys_BeepMs(100);
				ret = Sys_SetSN((char*)TmpBuff);
				ret = Sys_GetSN((char*)snbuf);
				if(!memcmp(snbuf,TmpBuff,strlen((char*)TmpBuff)))
				{
					LCD_ClearAll();
					LCD_Clear_rows(32,32);
					LCD_Printf(17, DISP_CENTER,"write sn code success>>");
					LCD_Printf(35, DISP_CENTER,snbuf);
					print_greenok("write sn code success");
					print_greenok(snbuf);
					system("sync");
					continue;
				}
				else
				{
					LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"write sn code error");
					print_rederr("write sn code error");
					OSTIMER_DelayMs(3000);
				}
			}
			Sys_Beep();
			//p18_l1_test_j
			printf("QR recive : [%s]\n",TmpBuff);
			if((ret == strlen(TEST_CASE_HEAD) + 1) && !memcmp(TmpBuff,TEST_CASE_HEAD,strlen(TEST_CASE_HEAD))){
				OSTIMER_DelayMs(1);
				select = TmpBuff[ret -1];
				break;
			}
		}
	}

    QRCode_Close(fd);

	return select;
}
char select_aging(void)
{
	int fd,ret,i;
	unsigned char TmpBuff[2048];
	LCD_ClearScreen(0);
	LCD_ClearAll();
    fd = QRCode_Open(0);
	print_start("scan other code to exit the aging test");
	print_start("enter to the aging test after 10s");
	LCD_Printf(10, DISP_LEFT|DISP_FONT12,"scan other code to exit the aging test:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"enter to the aging test after 10s");
	for(i = 0; i<100; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1024, 100);
		if(ret > 0)
		{	
			Sys_BeepMs(100);
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"exit the aging test");
			print_greenok("exit the aging test");
			OSTIMER_DelayMs(1000);
		    QRCode_Close(fd);
			return 0;
		}
	}

    QRCode_Close(fd);
	if(i == 100)
	{
		return 'f';
	}
	return 0x00;
}

int  aging_test(void)
{
	int ret;
	unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
	char dispbuff[100] = {0};
	unsigned int rpdu_len;
	int i,numbok = 0,numbfi = 0;
	char err;
	int slot = 0;
	int fd;
	unsigned char buff[2048];
//////////////////////////////////老化测试//////////////////////////////////////////////>
	// 太吵了
	system("amixer set Headphone 60%");
	system("amixer set PCM 60%");
	LED_Control(LED_ALL,1);
	LedSeg_Display(0,"8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.");
	print_start("start the contactless card aging test");
	print_start("swipping card");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT, "start the contactless card aging test:");
	LCD_Printf(25, DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE, "swipping card");
	fd = QRCode_Open(0);

	while(1){	
		if(getConsoleKey(50) == 0x1B)//ESCAPE
			break;

		ret = QRCode_RxStr(fd, buff, sizeof(buff) - 1, 50);
		if(ret > 0)
		{
			Sys_BeepMs(100);
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"exit the aging test");
			print_greenok("exit the aging test");
			OSTIMER_DelayMs(1000);
		    QRCode_Close(fd);
			return 0;
		}
		PICC_Open(0);
		ret = PICC_PollCard(slot,0);
		//无卡
		if(ret){
			PICC_Close(0);
			continue;
		}
		for(i=0; i<30; i++){
			ret = PICC_Exchange(slot,apdu,sizeof(apdu),rpdu,&rpdu_len);
		    if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
				err = 0;
			}
		    else{
				err = 1;
				break;
			}
		}
		if(err == 0){
			LCD_Clear_rows(25,16);
			LCD_Printf(25,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test contactless card success");
			print_greenok("test contactless card success");
			system("aplay /opt/sound/7499.wav > /dev/null 2&>1 &");
			numbok++;
		}
		else{
			LCD_Clear_rows(25,16);
			LCD_Printf(25,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test contactless card error");
			print_rederr("test contactless card error");
			system("aplay /opt/sound/sksb.wav > /dev/null 2&>1 &");
			numbfi++;	
		}
		memset(dispbuff, 0, sizeof(dispbuff));
		sprintf(dispbuff,"OK:%d ERR:%d",numbok,numbfi);
		LCD_Clear_rows(50,16);
		LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,dispbuff);
		PICC_Close(0);
		OSTIMER_DelayMs(200);
	}
	return (numbok > 0)? 0:1;
}
int test_uart_thread_snd(int uart_handle)
{
	unsigned int uiLen;
	unsigned char buff[1024];
	FILE*  fp;
	int ret;
	int ret_flag=0;

	printf("thread func:%s\n",__func__);

	fp=fopen("/home/root/tmp/sourcefile.bin","r");
	if(fp==NULL){
		printf("open /home/root/tmp/sourcefile.bin fail\n");
		return -1;
	}
	printf("writing /home/root/tmp/sourcefile.bin to uart\n");
	do
	{
		ret=fread(buff,sizeof(unsigned char),1024,fp);
		if(ret){
			ret_flag=1;
			//sDumpData("uartsnd",buff,ret);
			OSUART_TxStr(uart_handle,buff,ret);
		}else{
			if(ret_flag) printf("sndfile complete...\n");
			else printf("wrong file or null file\n");
			break;
		}
	}while(1);
	fclose(fp);
}
int test_uart_thread_rev(int uart_handle)
{
	unsigned int uiLen;
	unsigned char buff[1024];
	FILE*  fp;
	int ret;
	int ret_flag=0;

	printf("thread func:%s\n",__func__);

	fp=fopen("/tmp/revfile.bin","w");
	if(fp==NULL){
		printf("open /home/root/tmp/revfile.bin fail\n");
		return -1;
	}
	printf("waiting uart datastream,write to /tmp/revfile.bin\n");
	do
	{
		ret=OSUART_RxStr( uart_handle,buff,1024,1000);
		if(ret){
			sDumpData("uartrec",buff,ret);
			sleep(1); //有意为之，发送端不能delay
			ret=fwrite(buff,sizeof(unsigned char),ret,fp);
			printf("fwirte ret=%d\n",ret);
			ret_flag=1;
		}else{
			if(ret_flag==1) break;
			else printf("waiting uart data...\n");
		}
	}while(1);
	fclose(fp);
}
int test_uart(char * dev_name,char *attr,unsigned int test_mode)
{

	//uchar ucRec[]
	//pthread_mutex_lock(&mutex_lock);
	int handle=-1;
	unsigned int mode=test_mode;

	if(mode==0)//大数据自收发
	{

		pthread_t tidsnd=0;
		pthread_t tidrev=0;
		int err;
#if 1
		handle=OSUART_Init(dev_name,attr);
		if(handle <0){
			printf("uart init[%s][%s] fail\n",dev_name,attr);
			return -1;
		}
		err=pthread_create(&tidsnd,NULL,test_uart_thread_snd,handle);
		if(err!=0){
			printf("pthread_create snd fail\n");
		}else{
			printf("pthread_create sndid=%d\n",tidsnd);
		}
		err=pthread_create(&tidrev,NULL,test_uart_thread_rev,handle);
		if(err!=0){
			printf("pthread_create rev fail\n");
		}else{
			printf("pthread_create revid=%d\n",tidrev);
		}
		pthread_join(tidsnd,NULL);
		pthread_join(tidrev,NULL);
		OSUART_Close(handle);
#endif
		err=system("diff /tmp/revfile.bin /home/root/tmp/sourcefile.bin");
		if(!err) printf("diff file ok\n");
		else     printf("diff file fail\n");
	}
	else if(mode==1) //大数据接收
	{
		unsigned int uiLen;
		unsigned char buff[1024];
		FILE*  fp;
		int ret;
		int ret_flag=0;

		printf("%s test bigdata receive\n",dev_name);
		handle=OSUART_Init(dev_name,attr);
		if(handle <0){
			printf("uart init[%s][%s] fail\n",dev_name,attr);
			return -1;
		}
		fp=fopen("/tmp/revfile.bin","w");
		if(fp==NULL){
			printf("open /tmp/revfile.bin fail\n");
			OSUART_Close(handle);
			return -1;
		}
		printf("waiting uart datastream,write to /tmp/revfile.bin\n");
		do
		{
			ret=OSUART_RxStr( handle,buff,1024,1000);
			if(ret){
				sDumpData("uartrec",buff,ret);
				sleep(1); //有意为之，发送端不能delay
				ret=fwrite(buff,sizeof(unsigned char),ret,fp);
				printf("fwirte ret=%d\n",ret);
				ret_flag=1;
			}else{
				if(ret_flag==1) break;
				else printf("waiting...\n");
			}
		}while(1);
		fclose(fp);
		OSUART_Close(handle);
	}
	else //if(mode==1) //大数据发送
	{
		unsigned int uiLen;
		unsigned char buff[1024];
		FILE*  fp;
		int ret;
		int ret_flag=0;

		printf("%s test bigdata tranmit\n",dev_name);
		handle=OSUART_Init(dev_name,attr);
		if(handle <0){
			printf("uart init[%s][%s] fail\n",dev_name,attr);
			return -1;
		}
		fp=fopen("/home/root/tmp/sourcefile.bin","r");
		if(fp==NULL){
			printf("open /home/root/tmp/sourcefile.bin fail\n");
			return -1;
		}
		printf("writing /home/root/tmp/sourcefile.bin to uart\n");
		do
		{
			ret=fread(buff,sizeof(unsigned char),1024,fp);
			if(ret){
				ret_flag++;
				//sDumpData("uartsnd",buff,ret);
				printf("uartsnd[%d]package[%d]\n",ret,ret_flag);
				OSUART_TxStr(handle,buff,ret);
			}else{
				if(ret_flag) printf("sndfile complete...\n");
				else printf("wrong file or null file\n");
				break;
			}
		}while(1);
		fclose(fp);
		OSUART_Close(handle);
	}
	return 0;
}



char auto_Speaker(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT, "Start the sound test:");
	print_start("Start the sound test");
	system("amixer set Headphone 100%");
	system("amixer set PCM  90%");
	system("aplay /opt/sound/yycs.wav 2> /dev/null &");
	OSTIMER_DelayMs(1500);
	system("aplay /opt/sound/7499.wav 2> /dev/null &");
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"finish the sound tesg");
	print_end("finish the sound test");
	//OSTIMER_DelayMs(2000);
	return 0;
}
char auto_Sam(void)
{
	unsigned char atr[64],rpdu[300];
	unsigned int atr_len,rpdu_len;
	int ret;
	unsigned char apdu[] = "\x00\x84\x00\x00\x08";
	unsigned char slot;
	unsigned char simnum;
	unsigned char err = 0;
	unsigned char printfbuff[100];
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT, "Start the SAM test");
	//print_start("sam卡开始测试:");
	for(simnum = 1;simnum <= 5;simnum++){
		switch(simnum)
		{
			case 1:
				slot = ICC_SIM1;
				break;
			case 2:
				slot = ICC_SIM2;
				break;
			case 3:
				slot = ICC_SIM3;
				break;
			case 4:
				if(Sys_TermType() == 0)
					continue;
				else
					slot = ICC_SIM4;
				break;
            case 5:
                slot = ICC_ESAM;
                break;
			default:
				slot = ICC_SIM1;
				break;
		}
		if(slot==ICC_ESAM)
			print_start("Start the ESAM test:");
		else{
			printf("************  Start to test SAM %d:       ************",simnum);
			printf("\n");
		}
		ret = ICC_Init(slot);
    	ret = ICC_GetATR(slot,atr,&atr_len);
		ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
		if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
			dump_data("ICC_SIM rpdu<--",rpdu,rpdu_len);
			if(slot == ICC_ESAM){
				LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test ESAM success");
				print_greenok("test ESAM success");
			}
			else{
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test SAM %d success",simnum);
				printf("============================================\n");
				printf("!!!!!!!!!!  \033[42;37msam %d test success\033[0m       !!!!!!!!!!",simnum);
				printf("\n");
				printf("============================================\n");
			}
			OSTIMER_DelayMs(10);
		}
		else{
			if(slot == ICC_ESAM){
				LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test ESAM error");
				print_rederr("test ESAM error");
			}
			else{
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test SAM %d error",simnum);
				printf("########################################\n");
				printf("!!!!!!!!!!  \033[41;37msam %d test error\033[0m  !!!!!!!!!!",simnum);
				printf("\n");
				printf("########################################\n");
				//print_rederr("sam卡测试失败");
			}
			err += (1<<(simnum-1));
			OSTIMER_DelayMs(10);
			//err =
		}
		ICC_Remove(slot);
	}
	if((err & 0x1F) == 0){
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test SAM success");
	}
#if 0
	if((err & 0x10) == 0){
		LCD_Printf(48,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test ESAM success");
	}
	else{
		LCD_Printf(48,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"Test ESAM error");
	}
#endif
	//OSTIMER_DelayMs(2000);
	return err;
}

char auto_eeprom(void)
{
#define EEPROM_DATA_LEN 10
#define EEPROM_NUM 2
    int i;
    uint8_t *buf_write = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    uint8_t *buf_read  = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    int ret = 0;
	char lcdzkbuff[4][100] = {"Test main board EEPROM success","Test MCU EEPROM success"
						      ,"Test main board EEPROM error","Test MCU EEPROM error"};
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10,DISP_LEFT,"Start the EEPROM test:");
    print_start("Start the EEPROM test");
    if((buf_write == 0)||(buf_read == 0)){
            //printf("malloc buffer error\n");
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"error");
			print_rederr("error");
            goto _error;
    }
    
    for(i = 0;i < EEPROM_DATA_LEN;i++)
    {
        buf_write[i] = (uint8_t)i + '0';
    }
    for(i = 0;i < EEPROM_NUM;i++){
        
        dc_swr_eeprom(i,0,EEPROM_DATA_LEN,buf_write);
        memset(buf_read,0,EEPROM_DATA_LEN);
        dc_srd_eeprom(i,0,EEPROM_DATA_LEN,buf_read);	
        
        //dump_data("buf_read",buf_read,EEPROM_DATA_LEN);
        
        if(!memcmp(buf_read,buf_write,EEPROM_DATA_LEN)){
            LCD_Printf(25 +24* i,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,lcdzkbuff[i]);
			print_greenok(lcdzkbuff[i]);
        }
        else{
            LCD_Printf(25 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,lcdzkbuff[i+2]);
			print_rederr(lcdzkbuff[i+2]);
            ret |= (1 << i);
        }
    }
_error:
    if(buf_read) free(buf_read);
    if(buf_write)free(buf_write);
	//OSTIMER_DelayMs(3000);

    return ret;
}

char auto_Beep(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the buzzer test:");
	print_start("Start the buzzer test");
	Sys_BeepMs(2000);
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"finish the buzzer test");
	print_end("finish the buzzer test");
	//OSTIMER_DelayMs(2000);
	return 0;
}
char auto_QRcode(void)
{
	int err=0;
	int i,fd,ret;
	unsigned char TmpBuff[1024];
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"start the QR code test:");
	print_start("Start the QR code test");
	fd = QRCode_Open(0);
	for(i = 0; i<1000; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1024, 100);
		if(ret > 0)
		{	
			Sys_BeepMs(100);	
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"success");
			print_greenok("test QR code success");
			err = 0;
			//OSTIMER_DelayMs(2000);
			break;
		}
	}
	if(i == 1000)
	{
		err = 1;
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"error");
		print_rederr("test QR code error");
		//OSTIMER_DelayMs(3000);
	}
    QRCode_Close(fd);
	return err;
}
char auto_Lcd(void)
{
	char i;
	print_start("Please check the LCD");
	for(i=0; i<2; i++)
	{
        LCD_ClearScreen(1);      
		LCD_ClearScreen(0);
        LCD_Display_Row("Welcome to Decard", 8,0,DISP_CENTER);
		LCD_Display_Row("Welcome to Decard",24,0,DISP_CENTER);
        LCD_Display_Row("Welcome to Decard",40,0,DISP_CENTER|DISP_CLRLINE);
		LCD_Display_Row("Welcome to Decard",56,0,DISP_CENTER);
        LCD_Display_Row("Welcome to Decard",72,0,DISP_CENTER);
		OSTIMER_DelayMs(2000);
		LCD_ClearScreen(1);      
		LCD_ClearScreen(0);
        LCD_Signal_Icon(29);
		LCD_GPS_Icon(29);
		LCD_Display_Row("amount",        8, 88,DISP_FONT32);
		LCD_Display_Row("No 30",       24, 24,DISP_FONT24);
		LCD_Display_Row("$ 1.5",      40,80,DISP_FONT32);
		LCD_Display_Row("20:20",      72, 0,DISP_FONT6X8);
        OSTIMER_DelayMs(2000);
	}
	print_end("finish the LCD test");
	//OSTIMER_DelayMs(2000);
	return 0;
}

int auto_TypeAB(void)
{
	unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret;
    unsigned int rpdu_len;
	unsigned char testnum = 0; 
	int slot = 0;
	
    PICC_Open(0);
    
    dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the contactless card test:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"swipping card");
	print_start("Start the contactless card test");
	print_start("swipping card");
	PICC_Close(slot);
	while(1){
		PICC_Open(0);
		ret = PICC_PollCard(slot,0);
		if(!ret)	
			break;
		PICC_Close(slot);
		testnum++;
		if(testnum>10)
			break;
		OSTIMER_DelayMs(500);
	}
    
    dump_data("apdu--->",apdu,sizeof(apdu));
    ret = PICC_Exchange(slot,apdu,sizeof(apdu),rpdu,&rpdu_len);
	
    if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
		dump_data("rpdu<---",rpdu,rpdu_len);
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test contactless card success");
		print_greenok("test contactless card success");
		Sys_BeepMs(100);
		//OSTIMER_DelayMs(2000);
		PICC_Close(slot);
		return 0;
	}
    else{
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test contactless card error");
		print_rederr("test contactless card error");
		//OSTIMER_DelayMs(2000);
		PICC_Close(slot);
		return 1;
	 }
}
char auto_GPS(void)
{
	int i;
    int time=0;
    GNRMC *mes_gps;

	if(Sys_TermType() == 0)//T80-A2无gps
	{
		return 0;
	}
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the GPS test:");
	print_start("Start the GPS test");
	Sys_Gps_Debug(1);
	for(i=0; i<60; i++)
	{
		mes_gps = Sys_Gps_QueryLaLO("Asia/Shanghai");
		if(mes_gps == NULL)
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test GPS error");
			print_rederr("test GPS error");
		}
		else
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test GPS success");
			print_greenok("test GPS success");
			Sys_BeepMs(100);
			//OSTIMER_DelayMs(2000);
			break;
		}
        time++;
        OSTIMER_DelayUs(813456);
	}
	if(i == 60)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test GPS error");
		print_rederr("test GPS error");
		//OSTIMER_DelayMs(3000);
		return 1;
	}
	return 0;
}
char auto_SmKey(void)
{
	int i;
	int err = 0;
    char buffer[32];
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the LED segment test:");
	print_start("Start the LED segment test");
    //LedSeg_Display(0,"012345678901.8.8.8.8.");
	for(i = 0;i < 10;i++){
		sprintf(buffer,"%d%d%d%d%d%d",i,i,i,i,i,i);
		printf("to display %s\n",buffer);
		LedSeg_Display(0,buffer);
		OSTIMER_DelayMs(200);
	}

	for(i = 0;i < 10;i++){
		sprintf(buffer,"%d.%d.%d.%d.%d.%d",i,i,i,i,i,i);
		printf("to display %s\n",buffer);
		LedSeg_Display(0,buffer);
		OSTIMER_DelayMs(200);
	}
    LedSeg_Display(0,"8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.8.");
	if(Sys_TermType() == 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"please check the LED segment");
		print_start("Please check the LED segment");
		print_greenok("finish the LED segment test");
		//OSTIMER_DelayMs(3000);
		return 0;
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Please check the LED segment");
	print_start("Please check the LED segment");
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Press the left one button");
	print_start("Press the right one button");
	for(i = 0; i < 60; i++)
	{
		if(Sys_WaitKey(500) == 'R')
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test the right one button success");
			print_greenok("test the right one button success");
			Sys_BeepMs(100);
			err = (err*2);
			break;
		}
		OSTIMER_DelayMs(100);
	}
	if(i == 60)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test the right one button error");
		print_rederr("test the right one button error");
		err = (err*2) | 1 ;
		OSTIMER_DelayMs(300);
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Press the left one button");
	print_start("Press the left one button");
	for(i = 0; i < 40; i++)
	{
		if(Sys_WaitKey(500) == 'L')
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test the left one button success");
			print_greenok("test the left one button success");
			err = (err*2);
			break;
		}
		OSTIMER_DelayMs(100);
	}
	if(i == 40)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test the left one button error");
		print_rederr("test the left one button error");
		err = (err*2) | 1 ;
		//OSTIMER_DelayMs(3000);
	}
	if((err & 0x03) == 0)
	{
		Sys_BeepMs(100);	
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test LED segment and button success");
		print_greenok("test LED  segment and button success");
		//OSTIMER_DelayMs(2000);
	}
	else
	{
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test LED segment and button error");
		print_rederr("test LED segment and button error");
		//OSTIMER_DelayMs(3000);
	}
	return err;
}
char auto_RTC(char *readtime1)
{
	char readtime1_default[15] = {"20180414170835"} ;//字符串结尾有'\0'
	char readtime2[15] = {0};
	int ret;

	if(readtime1 == NULL)
		readtime1 = readtime1_default;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"RTC Test:");
	print_start("RTC Test");
	ret = Sys_SetRealTime(readtime1); //设置rtc和系统时间

	OSTIMER_DelayMs(2000);
	Sys_GetRtcTime(readtime2);
	//printf("readtime1:%s\n",readtime1);
	//printf("readtime2:%s\n",readtime2);
	system("hwclock -s");
	if(!ret && memcmp(readtime2,readtime1,14) > 0)// > 0 说明RTC时间在走
	{
		Sys_BeepMs(100);	
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Test RTC success");
		print_greenok("Test RTC success");
		//OSTIMER_DelayMs(2000);
		return 0;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Test RTC error");
		print_rederr("Test RTC error");
		//OSTIMER_DelayMs(2000);
		return 1;
	}
}
char auto_Led(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the LED test:");
	print_start("Start the LED test");
	print_start("all the led light up");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"all the led light up");
    LED_Control(LED_ALL,1);
    OSTIMER_DelayMs(1000);
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"all the led turn off");
	print_start("all the led turn off");
    LED_Control(LED_ALL,0);
    OSTIMER_DelayMs(1000);
	LED_Control(LED_ALL,1);
	return 0;
}

int  auto_RTC_Read(void)
{
	char realtimem[32] = {0} ;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the RTC power control test:");
	print_start("Start the RTC power control test");
	Sys_GetRealTime(realtimem);
	if((realtimem[6] != 0x00) )
	{
		Sys_BeepMs(100);	
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"RTC power off success");
		print_greenok("RTC power off success");
		//OSTIMER_DelayMs(2000);
		return 0;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"RTC power off error");
		print_rederr("RTC power off error");
		//OSTIMER_DelayMs(3000);
		return 1;
	}	

}

char *get_picc_name(uint8_t card_type){
	//printf("card_type = 0x%X\n",card_type);
	switch(card_type)
	{
	case TYPE_NO_PICC: return "no card";
	case TYPE_A_TCL: return "Type A";
	case TYPE_A_MIFARE: return "Mifare";
	case TYPE_A_MIFARE_1K: return "1K Mifare";
	case TYPE_A_MIFARE_4K: return "4K Mifare";
	case TYPE_A_MIFARE_Ultralight: return "Ultralight";
	case TYPE_A_MiFareMINI: return "Mifare mini";
	case TYPE_A_Ntag_21x: return "Ntag";
	case TYPE_A_OTHERS_MEM: return "other mifare";
	case TYPE_B_TCL: return "type B";
	case TYPE_B_OTHER: return "Type B ID card";
	case TYPE_C_TCL: return "Type C";
	default:
		return "unknow card";
	}
}

char auto_High()
{
	int i;
	int ret;
	char pollfirstok = 0;
	int slot = 0;
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"blind area test:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"swippping card");
	print_start("blind area test");
	print_start("swipping card");
	for(i=0; i < 250;i++){
		
		PICC_Open(0);
		ret = PICC_PollCard(slot,0);
		if(ret)
		{
			if(pollfirstok)
				Sys_BeepMs(50);
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"no card");
			print_rederr("no card");
			OSTIMER_DelayMs(50);
		}else{
			pollfirstok = 1;
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"card exist");
			print_greenok("card exist");
			printf("%s\n",get_picc_name(PICC_GetPiccType()));
			OSTIMER_DelayMs(50);
		}
		PICC_Close(slot);
	}
	return 0;
}
char auto_USB(void)
{
	int usbfd;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the U-disk test:");
	print_start("Start the U-disk test");
	usbfd = open("/var/run/media/sda1/test.txt",O_RDWR|O_CREAT);
	if(usbfd < 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test U-disk error");
		print_rederr("test U-disk error");
		//OSTIMER_DelayMs(3000);
		return 1;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test U-disk success");
		print_greenok("test U-disk success");
		Sys_BeepMs(100);
		//OSTIMER_DelayMs(2000);
	}
	return 0;
}
char auto_TF(void)
{
	int tffd;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the TF card test:");
	print_start("Start the TF card test");
	tffd = open("/run/media/mmcblk0p1/test.txt",O_RDWR);
	if(tffd < 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test TF card error");
		print_rederr("Test TF card error");
		//OSTIMER_DelayMs(3000);
		return 1;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Test TF card success");
		print_greenok("Test TF card success");
		Sys_BeepMs(100);
		//OSTIMER_DelayMs(2000);
		return 0;
	}
}

int auto_4G(void)
{
	int state;
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the 4G test:");
	print_start("Start the 4G test");
	state = system("/opt/script/test_tcpip.sh ppp0");
	if(state == 0)
	{
		print_greenok("test 4G success");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test 4G success");
		Sys_BeepMs(100);	
	}
	else
	{
		print_rederr("test 4G error");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test 4G error");
		
	}
	//OSTIMER_DelayMs(2000);
	return state;
}

int auto_Wifi(void)
{
	int state;
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the WIFI test:");
	print_start(" Start the WIFI test");
	state = system("/opt/script/test_tcpip.sh wlan0");
	if(state == 0)
	{
		print_greenok("test WIFI success");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test WIFI success");
		Sys_BeepMs(100);	
		//OSTIMER_DelayMs(2000);
	}
	else
	{
		system("killall udhcpc");
		print_rederr("test WIFI error");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test WIFI error");
		//OSTIMER_DelayMs(3000);
		//return 1;
	}
	system("ifconfig wlan0 down");
	//OSTIMER_DelayMs(2000);
	return state;
}
char auto_TCP(void)
{
	int state;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the Ethernet test:");
	print_start("Start the Ethernet test");
	state = system("/opt/script/test_tcpip.sh eth0");
	if(state == 0)
	{
		print_greenok("test ethernet success");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test Ethernet success");
		Sys_BeepMs(100);	
		//OSTIMER_DelayMs(2000);
	}
	else
	{
		print_greenok("test the Enternet error");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test Ethernet error");
		//OSTIMER_DelayMs(3000);
		return 1;
	}
	return 0;
}
char sn_down(void)
{
	struct timeval ;
	int ret,i;
    int fd;
	unsigned char TmpBuff[1028]={"\0"},snbuf[200]={"\0"};
	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	ret = Sys_GetSN((char*)snbuf);
	if(ret > 0)
	{
		LCD_Printf(10, DISP_LEFT,"current SN code:");
		LCD_Printf(50, DISP_CENTER,snbuf);
		Sys_BeepMs(2000);
	}
	LCD_Printf(10, DISP_LEFT,"Start to download SN code:");
	print_start("Start to download SN code");
	print_start("scan the SN code");
	fd = QRCode_Open(0);
	LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Scan the SN code>>");
	for(i = 0; i<5000; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1023, 100);
		//sn码不限制格式
		if((ret > 0))
		{	
			Sys_BeepMs(100);	
			ret = Sys_SetSN((char*)TmpBuff);
			ret = Sys_GetSN((char*)snbuf);
		    QRCode_Close(fd);
			if(!memcmp(snbuf,TmpBuff,strlen((char*)TmpBuff)))
			{
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"write SN code success>>");
				LCD_Printf(50, DISP_CENTER,snbuf);
				print_greenok("write SN code success");
				print_greenok(snbuf);
				//OSTIMER_DelayMs(3000);
				system("sync");
				return 0;
			}
			else
			{
				LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"write SN code error");
				print_rederr("write SN code error");
				//OSTIMER_DelayMs(3000);
				return 1;
			}
		}
	}
    QRCode_Close(fd);
	if(i == 5000)
	{
		LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Scan the code error");
		print_rederr("Scan the code error");
		//OSTIMER_DelayMs(3000);
		//return 'f';
		return 1;
	}
	return 0x00;
}
unsigned char auto_can(int mode,unsigned int bitrate)
{
	int can_fd,i,nbytes,state,com5fd,com2fd,ret,j,sendnumber=0,canerr=0,timeflg=0;
    unsigned int rcv_len;
	unsigned char wtbuff[10], rdbuff[10];
	unsigned char pBuffer[1024]={0};
	unsigned char dispbuff[25] = {0};
	GNRMC *mes_gps;
	char realtimec[15]={0};
	//memset(mes_gps, 0, sizeof(mes_gps));
	
	can_fd = Can_Init("can0",bitrate);
	
	com5fd = OSUART_Init("/dev/ttymxc2","115200,n,8,1");
	com2fd = OSUART_Init("/dev/ttymxc1","115200,n,8,1");
	if(mode)
	{
		print_start("CAN 485 host mode");
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(10, DISP_LEFT,"host mode test:");
			ret = system("echo 74 > /sys/class/gpio/export");
		if(ret < 0)
		{
			printf("export err\n");
		}
		ret = system("echo out > /sys/class/gpio/gpio74/direction");
		ret = system("echo 0 > /sys/class/gpio/gpio74/value");
		while(1)
    	{
    		nbytes = Can_Read(can_fd, 0x11, &rcv_len, rdbuff, 1000); //接收报文
    		//printf("read [%d]",nbytes);
    		//显示报文
    		if(nbytes > 0)
    		{
    			sendnumber++;
				LCD_Clear_rows(32,32);
    			LCD_Printf(32, DISP_CENTER,"CAN receive data package<<");
				sprintf((char*)dispbuff,"%d",sendnumber);
				LCD_Printf(48, DISP_CENTER,dispbuff);
    			nbytes = Can_Write(can_fd, 0x11, rcv_len, rdbuff);
    		}
			memset(pBuffer, 0, sizeof(pBuffer));
			ret = OSUART_RxStr(com5fd,pBuffer,sizeof(pBuffer),100);
			if(ret>0)
			{
				ret = system("echo 1 > /sys/class/gpio/gpio74/value");
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"485-232 receive data<<");
				OSUART_TxStr(com5fd, pBuffer,sizeof(pBuffer));
				LCD_Printf(48, DISP_CENTER,"485-232 send data>>");
				ret = system("echo 0 > /sys/class/gpio/gpio74/value");
				OSTIMER_DelayMs(1000);
			}
			/*ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),100);
			if(ret>0)
			{
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"COM2接收到数据<<");
				OSUART_TxStr(com2fd, pBuffer,sizeof(pBuffer));
				LCD_Printf(48, DISP_CENTER,"COM2数据已发送>>");
				Sys_GetRealTime((char*)realtimec);
				printf("com2 write time:%s\n",realtimec);
				OSUART_TxStr(com2fd, realtimec,sizeof(realtimec));
				sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            			&realtimec[0],&realtimec[4],&realtimec[6],
            			&realtimec[8],&realtimec[10],&realtimec[12]);
				LCD_Printf(1, DISP_FONT6X8|DISP_RIGHT,dispbuff);
				OSTIMER_DelayMs(1000);
			}
			if(timeflg == 0)
			{
				mes_gps = Sys_Gps_QueryLaLO("Asia/Shanghai");
				//printf("GPS time:%s\n",mes_gps->date_time);
				if(mes_gps != NULL)
				{
					if(mes_gps->date_time[0] == '2')
					{
						printf("GPS time:%s\n",mes_gps->date_time);
						ret = Sys_SetRealTime(mes_gps->date_time);
						if(ret < 0)
							printf("set time err!!!\n");
						sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            			&mes_gps->date_time[0],&mes_gps->date_time[4],&mes_gps->date_time[6],
            			&mes_gps->date_time[8],&mes_gps->date_time[10],&mes_gps->date_time[12]);
						LCD_Printf(1, DISP_FONT6X8|DISP_RIGHT,dispbuff);
						timeflg = 1;
					}
				}
			}*/
    	}
	}
	else
	{
		//if(Sys_TermType() != 0 ){
		//	printf("设备不支持CAN\n");
		//	return 0x00;
		//}
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(10, DISP_LEFT,"Start the CAN test:");
		print_start("Start the CAN test");
		memset(wtbuff, 0x23, 8);
		memset(rdbuff, 0x00, 8);
		for(j=0; j<50; j++)
		{
			Can_Write(can_fd,0x11,8,wtbuff);
			for(i=0;i<100000;i++)
    		{
    			nbytes = Can_Read(can_fd, 0x11, &rcv_len, rdbuff, 1000); //接收报文
    			if(nbytes > 0)
    			{
					state = memcmp(rdbuff, wtbuff, 8);
					if(state == 0)
					{
						canerr = 0|canerr;
						break;
					}
					else
					{
						canerr++;
						break;
					}
    			}
    		}
			if(i == 100000)
			{
				canerr++;
			}
		}
		if(canerr)
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test CAN error");
			print_rederr("test CAN error");
			//OSTIMER_DelayMs(3000);
			Can_Close(can_fd);
			return 1;
		}
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test CAN success");
		print_greenok("test CAN success");
		Sys_BeepMs(100);
		//OSTIMER_DelayMs(2000);
		return 0;
	}
}

unsigned char auto_485(void)
{
	int com3fd;
	int ret;
	int i;
	unsigned char pBuffer[1024]={0};
	unsigned char wbuffer[1024]={0};
	
	print_start("Start the 485 test");
	com3fd = OSUART_Init("/dev/ttymxc2","115200,n,8,1");
	if(com3fd == 0)
	{
		printf("open com3 err!\n");
	}
	ret = system("echo 74 > /sys/class/gpio/export");
	if(ret < 0)
	{
		printf("export err\n");
	}
	ret = system("echo out > /sys/class/gpio/gpio74/direction");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the COM3_485 test:");
	memset(wbuffer, 'k', 100);
	ret = system("echo 1 > /sys/class/gpio/gpio74/value");
	OSUART_TxStr(com3fd, wbuffer, 100);
	fsync(com3fd);
	ret = system("echo 0 > /sys/class/gpio/gpio74/value");
	memset(pBuffer, 0, sizeof(pBuffer));
	for(i = 0; i < 20; i++)
	{
		ret = OSUART_RxStr(com3fd,pBuffer,sizeof(pBuffer),100);
		if(ret>0)
    	{
			ret = memcmp(pBuffer, wbuffer, 100);
			if(ret == 0)
			{
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test 485 success");
				print_greenok("test 485 success");
				Sys_BeepMs(100);	
				//OSTIMER_DelayMs(2000);
				OSUART_Flush(com3fd);
				return 0;
			}
		}
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test 485 error");
	print_rederr("test 485 error");
	//OSTIMER_DelayMs(3000);
	OSUART_Flush(com3fd);
	return 1;
}

unsigned char auto_232(void)
{
	int com2fd;
	int ret;
	int i;
	unsigned char pBuffer[1024]={0};
	unsigned char wbuffer[1024]={0};
	char dispbuff[25]={0};
	
	com2fd = OSUART_Init("/dev/ttymxc1","9600,n,8,1");
	if(com2fd == 0)
	{
		printf("open com2 err!\n");
	}
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the com2 test:");
	print_start("Start the com2 test");
	memset(wbuffer, 'k', 1023);
	
	OSUART_TxStr(com2fd, wbuffer, 1023);
	memset(pBuffer, 0, sizeof(pBuffer));
	for(i = 0; i < 20; i++)
	{
		ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),100);
		if(ret>0)
    	{
			ret = memcmp(pBuffer, wbuffer, 1023);
			if(ret == 0)
			{
				memset(pBuffer, 0, sizeof(pBuffer));
				ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),500);
				if(ret > 0)
				{
					    ret = Sys_SetRealTime(pBuffer);
    					printf("try to reset time to %s[%d]\n",pBuffer,ret);
						sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            			&pBuffer[0],&pBuffer[4],&pBuffer[6],
            			&pBuffer[8],&pBuffer[10],&pBuffer[12]);
						LCD_Printf(70, DISP_FONT6X8|DISP_RIGHT,dispbuff);
				}
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test com2 success");
				print_greenok("test com2 success");
				Sys_BeepMs(100);	
				//OSTIMER_DelayMs(2000);
				OSUART_Flush(com2fd);
				return 0;
			}
		}
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test com2 error");
	print_rederr("test com2 error");
	//OSTIMER_DelayMs(3000);
	OSUART_Flush(com2fd);
	return 1;
	
}



void err_log(int derr)
{
	int err;
	err = derr;

	//printf("err = %4x \n",err);
	
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
	print_ok("||  function  | result||  function  | result||  function  | result||",1);
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
//	printf("|  GPS   | OK  ||# wifi #|#ERR#||   USB  | OK  |\n");
////////////////////////////////////one row/////////////////////////////////////////////>
	if((err & 0x01) == 0)
	{
		print_ok("||   GPS  | OK  |",NULL);
	}
	else
	{
		print_error("||#  GPS #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|   485  | OK  |",NULL);
	}
	else
	{
		print_error("|## 485 #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|   CAN  | OK  ||",1);
	}
	else
	{
		print_error("|#  CAN #|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);

////////////////////////////////////one row/////////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("||  COM2  | OK  |",NULL);
	}
	else
	{
		print_error("||##COM2 #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|  WIFI  | OK  |",NULL);
	}
	else
	{
		print_error("|# WIFI #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|   4G   | OK  ||",1);
	}
	else
	{
		print_error("|## 4G ##|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
//////////////////////////////////////////two row////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("||   TF   | OK  |",NULL);
	}
	else
	{
		print_error("||## TF ##|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|   USB  | OK  |",NULL);
	}
	else
	{
		print_error("|#  USB #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("| QR code | OK  ||",1);
	}
	else
	{
		print_error("|#QR code#|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
//////////////////////////////////////three row////////////////////////////////////////>
	print_ok("||  LCD   | OK  |",NULL);
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("| left key | OK  |",NULL);
	}
	else
	{
		print_error("|#left key#|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("| right key | OK  ||",1);
	}
	else
	{
		print_error("|#right key#|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
//////////////////////////////////////four row/////////////////////////////////////////>
	print_ok("||   LED  | OK  ||  BEEP  | OK  ||  Sound  | OK  ||",1);
 	print_ok("++--------+-----++--------+-----++--------+-----++",1);

//////////////////////////////////////five row/////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("||   RTC  | OK  |",NULL);
	}
	else
	{
		print_error("||## RTC #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|  SAM1  | OK  |",NULL);
	}
	else
	{
		print_error("|# SAM1 #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|  SAM2  | OK  ||",1);
	}
	else
	{
		print_error("|# SAM2 #|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
//////////////////////////////////////////six row///////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("||  SAM3  | OK  |",NULL);
	}
	else
	{
		print_error("||# SAM3 #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|  SAM4  | OK  |",NULL);
	}
	else
	{
		print_error("|# SAM4 #|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|  ESAM  | OK  ||",1);
	}
	else
	{
		print_error("|# ESAM #|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
	//////////////////////////////////////////seven row///////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|| MCU FRAM | OK  |",NULL);
	}
	else
	{
		print_error("||#MCU FRAM#|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("| Master FRAM | OK  |",NULL);
	}
	else
	{
		print_error("|#Mstaer FRAM#|#ERR#|",NULL);
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		print_ok("|   TypeA  | OK  ||",1);
	}
	else
	{
		print_error("|#  TypeA #|#ERR#||",1);
	}
	print_ok("++--------+-----++--------+-----++--------+-----++",1);
}

/*0 未测试，1测试成功 2测试失败*/
static int all_test_result[256] = {0};


int dispatch_menu_test(char select,char *pbuff){
	int ret = -1;
	switch(select)
	{
	case '0':
		ret = auto_test();
		break;
	case '1':
		ret = auto_QRcode();
		break;
	case '2':
		ret = auto_Sam();
		break;
	case '3':
		ret = auto_RTC(pbuff);
		break;
	case '4':
		ret = auto_4G();
		break;
	case '5':
		ret = auto_Wifi();
		break;
	case '6':
		ret = auto_GPS();
		break;
	case '7':
		ret = auto_TCP();
		break;
	case '8':
		ret = auto_TypeAB();
		break;
	case '9':
		ret = auto_High();//加无卡时蜂鸣
		break;
	case 'a':
		ret = auto_Beep();
		break;
	case 'b':
		ret = auto_Speaker();
		break;
	case 'c':
		ret = auto_Lcd();
		break;
	case 'd':
		ret = auto_SmKey();
		break;
	case 'e':
		ret = auto_Led();
		break;
	case 'f':
		ret = auto_USB();//读取u盘的文件来判断
		break;
	case 'g':
		ret = auto_TF();//读取tf卡的文件判断
		break;
	case 'h':
		ret = auto_eeprom();
		break;
	case 'i':
		ret = auto_232();	
		break;
	case 'j':
		ret = auto_485();
		break;
	case 'k':
		ret = auto_can(0,100000);	
		break;
	case 'l':
		
		break;
	case 'm':
		
		break;
	case 'n':
		
		break;
	case 'o':
		
		break;
	case 'p':
		
		break;
	case 'q':
		
		break;
	case 'r':
		
		break;
	case 's':
		
		break;
	case 't':
		
		break;
	case 'u':
		
		break;
	case 'v':
		
		break;
	case 'w':
		ret = auto_RTC_Read();
		break;
	case 'x':
		ret = auto_can(1,100000);
		break;
	case 'y':
		ret = sn_down();
		break;
	case 'z':
		ret = aging_test();
		break;
	default:
		//auto_test();
		break;
	}
	return ret;
}

static int getch(int millisec){
    int input1;
    struct termios save, current;
    int fd;
    uint8_t data[1];

    tcgetattr(0, &save);// 得到原来的终端属性
    //printf("current.c_cc[VTIME] = %d\n",current.c_cc[VTIME]);
    current = save;
    current.c_lflag &= ~ICANON;// 设置非正规模式，如果程序每次要从终端读取一个字符的话，这是必须的
    current.c_lflag &= ~ECHO;// 关闭回显
    current.c_cc[VMIN] = 0;// 设置非正规模式下的最小字符数
    current.c_cc[VTIME] = 1;//(millisec + 99)/100;// 设置非正规模式下的读延时

    current.c_iflag &= ~(IXON | INLCR | IGNCR | ICRNL | ISTRIP);
    current.c_oflag &= ~(OPOST);
    current.c_lflag &= ~(ECHO | ECHOE | ISIG | ICANON);

    tcsetattr(0, TCSANOW, &current);// 设置新的终端属性
#if 0
    fd = OSUART_Init("/dev/ttymxc0","115200,n,8,1");
    if(OSUART_RxStr(fd,data,1,500) == 1)
    	input1 = data[0];
    else
    	input1 = 0x00;
    close(fd);
#else
    input1 = getchar();
#endif
	//printf("input1=0x%X\n",input1);
    tcflush(0,TCIFLUSH);
    tcsetattr(0, TCSANOW, &save);// 恢复原来的终端属性，以免干扰shell和之后的程序运行

    return input1;
}

static int getConsoleKey(int millisec){
/*
    int key = getch(millisec);//从控制台读取字符

    if(key > 0)
    	Sys_Beep();
    else
    	key = 0;
    return key;
*/
    return -1;
}

int print_menu(void){

	system("reset > /dev/null");
	system("reset > /dev/null");//清屏，防止控制台乱码
    tcflush(0,TCIFLUSH);

	print_menu_str("0","auto test"      ,all_test_result['0']);
	print_menu_str("1","QR code test"     ,all_test_result['1']);
	print_menu_str("2","PSAM test"     ,all_test_result['2']);
	print_menu_str("3","RTC test"     ,all_test_result['3']);
	print_menu_str("4","4G test",all_test_result['4']);
	print_menu_str("5","WIFI test",all_test_result['5']);
	print_menu_str("6","GPS test",all_test_result['6']);
	print_menu_str("7","Ethernet test",all_test_result['7']);
	print_menu_str("8","contactless test",all_test_result['8']);
	print_menu_str("9","distance test",all_test_result['9']);
	print_menu_str("a","buzzer test",all_test_result['a']);
	print_menu_str("b","sound test",all_test_result['b']);
	print_menu_str("c","LCD test",all_test_result['c']);
	print_menu_str("d","LED segment&key test",all_test_result['d']);
	print_menu_str("e","LED test",all_test_result['e']);
	print_menu_str("f","U-disk test",all_test_result['f']);
	print_menu_str("g","TF card test",all_test_result['g']);
	print_menu_str("h","EEPROM test",all_test_result['h']);
	print_menu_str("i","232 test",all_test_result['i']);
	print_menu_str("j","485 test",all_test_result['j']);
	print_menu_str("k","CAN test",all_test_result['k']);
	print_menu_str("x","host mode",all_test_result['x']);
	print_menu_str("y","write serial number",all_test_result['y']);
	print_menu_str("z","aging test",all_test_result['z']);
	return 0x00;
}
int set_result_flag(char select , int ret){
	//printf("to 设置 %c 结果 [%d]\n",select,ret);
	if(ret < 0)
		return 0x00;
	if(ret == 0)
		ret = 1; //PASS
	else
		ret = 2;//FAIL
	all_test_result[select] = ret;

	return 0x00;
}
#if 1
void auto_test(void)
{
	int err=0;
	int ret;
	int i;
#define autotestsum 20	
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the auto test:");
	printf("\n\n=================== Start the auto test ====================\n");
	for(i=1;i<autotestsum+1;i++){
		if(i<=9){
			ret  = dispatch_menu_test(i+'0',0);
			set_result_flag(i+'0', ret);
		}
		else{
			ret  = dispatch_menu_test((i-10)+'a',0);
			set_result_flag((i-10)+'a', ret);
		}
	}
	printf("\n\n================ finish all the test =================\n");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"finish the auto test");
}

#else
int auto_test(void)
{
	int err=0;
	int ret;
	char retu;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the auto test:");
printf("\n\n=================== Start the auto test ====================\n");
////////////////////////////////A 卡测试///////////////////////////////////////////////>
	ret = auto_TypeAB();
	err = ret;
////////////////////////////////////////eeprom//////////////////////////////////////////>
	ret = auto_eeprom();
	err = (err<<2)|ret;
/////////////////////////////////SAM ////////////////////////////////////////////////////>	
	ret = auto_Sam();
	err = (err<<5)|ret;
/////////////////////////////RTC测试////////////////////////////////////////////////////>
	ret = auto_RTC();
	err = (err<<1)|ret;
///////////////////////////////语音测试/////////////////////////////////////////////////>
	ret = auto_Speaker();
///////////////////////////////蜂鸣器测试///////////////////////////////////////////////>
	ret = auto_Beep();
//////////////////////////////LED 测试//////////////////////////////////////////////////>
	ret = auto_Led();
////////////////////////////////数码管按键 测试//////////////////////////////////////////>
	ret = auto_SmKey();
	err = (err<<2)|ret;
//////////////////////////////////////LCD 测试///////////////////////////////////////////>
	ret = auto_Lcd();
/////////////////////////////////////二维码测试//////////////////////////////////////////>
	ret = 0;// auto_QRcode();
	err = (err<<1)|ret;
////////////////////////////////////////U 盘测试/////////////////////////////////////////>
	ret = auto_USB();
	err = (err<<1)|ret;
///////////////////////////////////////TF卡测试/////////////////////////////////////////>
	ret = auto_TF();
	err = (err<<1)|ret;
/////////////////////////////////网络测试////////////////////////////////////////////////>
	ret = auto_4G();
	err = (err<<1)|ret;
	ret = auto_Wifi();
	err = (err<<1)|ret;
//////////////////////////////////////////BT测试/////////////////////////////////////////>
	ret = auto_232();
	err = (err<<1)|ret;
////////////////////////////////////////CAN测试//////////////////////////////////////////>
	ret = auto_can(0,100000);
	err = (err<<1)|ret;
////////////////////////////////////////485测试//////////////////////////////////////////>
	ret = auto_485();
	err = (err<<1)|ret;
	ret = auto_High();
	////////////////////////////////////////GPS测试//////////////////////////////////////////>
	ret = auto_GPS();
	err = (err<<1)|ret;	

	printf("\n\n================ finish all the test =================\n");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"finish the auto test");
	if(err == 0)
	{
		//system("aplay -q /home/root/sound/7499.wav &");
		printf("*************************************************\n");
		printf("**************    all the function    OK    ***********\n");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test all the functions success");
		err_log(err);
		OSTIMER_DelayMs(3000);
		sn_down();
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test all the functions error");
		printf("**************    all the functions     error  ***********\n");
		err_log(err);
		OSTIMER_DelayMs(3000);
	}
//////////////////////////////////////老化测试///////////////////////////////////////////>
	retu = select_aging();
	if((retu == 'f') || (retu == 'e'))
	{
		aging_test();
	}
	return err;
}
#endif
void menu_test(void)
{
	char select = '0';
	int ret;

	while(1)
	{
		if(select)
			print_menu();// 是否刷屏
			
		select = select_test_timeout(300);
		if(select == 0)
			select = tolower(getConsoleKey(100));
		if(select) printf("select = %c(0x%X)\n", select, select);
		if(select){
			ret  = dispatch_menu_test(select,0);
			set_result_flag(select, ret);
		}
	}
}
extern  __attribute__((weak)) int protoctol_Z9_test(int handle,int qrhandle);

void protocol_Z9_menu_test(void)
{
	char select = '0';
	int ret;
	int com2fd,qrcodefd;
	
	com2fd = OSUART_Init("/dev/ttymxc1","115200,n,8,1");
	qrcodefd = QRCode_Open(0);
	LCD_ClearScreen(0);
    LCD_Display_Row("Welcome", 16,0,DISP_FONT24|DISP_CENTER);
	system("amixer set Headphone 65%");
	system("amixer set PCM  90%");
	system("/usr/sbin/alsactl -f /var/lib/alsa/asound.state store");
	system("aplay /opt/sound/welcom.wav 2> /dev/null &");
	if(com2fd == 0)
	{
		printf("open com2 err!\n");
	}
	while(1)
	{
		if((select) || (protoctol_Z9_test && protoctol_Z9_test(com2fd,qrcodefd)))
		{
			print_menu();// 是否刷屏
			OSTIMER_DelayMs(200);
			select = 0;
		}
	}
}


#include <uchar.h>
extern int fb_unicode_display_align(int char_size,
                     int x1,int x2,int yoffset,
                     uint16_t *text_unicode,
                     uint32_t fc,
                     uint32_t mode
                  );

int test_unicode_str(int font_size){
#ifdef u

	char16_t *unicode_arab = u"Arabic لصور شرطة قوانغتشو تصدر أول بطاقة هوية إلكتر";
	char16_t unicode_tailand[] = u"Thai กขฃคฅฆงจฉชซฌภาษาไทย";
	char16_t unicode_chinese[] = u"Chinese 中文中国人民 ";
	char16_t unicode_greek[] = u"German ΑΒΔΕΖΗΘΙΚΛΜΝΞΟΠ ";
	char16_t unicode_number_forms[] = u"Number ⅣⅤⅥⅠⅡⅢⅦⅧⅨⅩⅪⅫⅰⅱⅲ";
	char16_t unicode_letter_symbols[] = u"Letter ℀℁ℂ℃℄℅℆ℇ℈℉ℊℋℌℍℎℏℐℑℒℓ℔ℕ№℗℘ℙℚℛℜℝ℞℟℠℡™℣␤℥Ω℧ℨ⨩KÅℬℭ℮ℯℰℱℲℳℴℵℶℷℸ";
	char16_t unicode_enclosed_alpha[] = u"Closed alphanumeric ①②③④⑤⑥⑦⑧⑨⑩⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇⒈⒉";
	char16_t unicode_hinagana[] = u"Japanese ぁあぃいぅうぇえぉおかがきくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん゛゜ゝゞ";//Hinagana
	char16_t unicode_russia[] = u"Russian ЁБВГДЕЖЗИЙКЛМНОПРСТУФХЦഇЈЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяѐёђѓєѕіїјљњћќѝўџѠѡѢѣѤѥѦѧѨѩѪѫѬѭѮѯѰѱѲѳѴѵѶѷѸѹѺѻѼѽѾѿҀҁ҂҃҄҅҆҇҈҉ҊҋҌҍҎ";//cyrillic
	char16_t unicode_symbol[] = u"■№☆★○●◎◇※↑←¤℃‰€°";
	char16_t unicode_lao[] = u"Lao ອັກສອນລາວິ໐ ັ ີ ຶ ື ຸ ູ ົ ຼ ່ ້ ໊ ໋ ໌ ໍ ";


	char16_t unicode_Dutch[]= u"Dutch			Autoterminal, mooie vorm";
	char16_t unicode_Portuguese[]= u"Portuguese		Terminal de carro, forma bonita";
	char16_t unicode_Vietnamese[]= u"Vietnamese		Cộng hòa Xã hội Chủ nghĩa Việt Nam";
	char16_t unicode_Spanish[]= u"Spanish		Terminal del coche, hermosa forma";
	char16_t unicode_Russian[]= u"Russian		Автомобильный терминал, красивая форма";
	char16_t unicode_English[]= u"English		Car terminal, beautiful shape";
	char16_t unicode_Hindi[]= u"Hindi		कार टर्मिनल, सुंदर आकार";
	char16_t unicode_Kazakh[]= u"Kazakh		Автокөлік терминалы, әдемі пішін";
	char16_t unicode_Burmese[]= u"Burmese		ချောမွေ့ပြောင်လက်ယာဉ် terminal ကို,";
	char16_t unicode_Uzbek[]= u"Uzbek		Avtomobil terminali, chiroyli shakli";
	char16_t unicode_Arabic[]= u"Arabic		محطة سيارة، شكل جميل";
	char16_t unicode_German[]= u"German Auto-Terminal, schöne Form";

	char16_t *unicode_text[] =
	{
	unicode_arab,
	unicode_tailand,
	unicode_chinese,
	unicode_greek,
	unicode_number_forms,
	unicode_letter_symbols,
	unicode_enclosed_alpha,
	unicode_hinagana,
	unicode_russia,
	unicode_symbol,
	unicode_lao,
	unicode_Dutch,
	unicode_Portuguese,
	unicode_Vietnamese,
	unicode_Spanish,
	unicode_Russian,
	unicode_English,
	unicode_Hindi,
	unicode_Kazakh,
	unicode_Burmese,
	unicode_Uzbek,
	unicode_Arabic,
	unicode_German,
	0
	};
	int i = 0;
	printf("sizeof(char16_t) = %d\n",sizeof(char16_t));
	printf("sizeof(unicode_arab) = %d\n",sizeof(unicode_arab));

    freetype_set_ttf("/opt/font/TIMES.TTF");
    if(font_size <= 0)
    	font_size = 16;
    fb_disp_start();
	for(i = 0;unicode_text[i];i++){
		LCD_ClearAll();
		fb_unicode_display_align(font_size,
						 0,-1,16,
						 unicode_text[i],
						 0,
						 DISP_LEFT
					  );
		wait_key_suspend(1000);
	}
	fb_disp_end();
#endif
	return 0x00;
}
int test_utf8_str(int font_size){
	char *arab[] ={
"رحباً",
"مع سّلامة",
"أتمنى لكم سعادة",
"مساء خير ",
"مرحباً ",
"ما اسمكَ؟",
"اسمي. . .",
"انسة",
"سّيدة",
"سّيد ",
"إسمحْ لي أن أقدم بنفسي",
"أنا سعيد بلقائك",
"أنت من أي دولة ؟",
"أين تَسكن؟",
"أنا أسكن. . .",
"لوسمحت",
	(char*)0,
	};
	int i;
    freetype_set_ttf("/opt/font/TIMES.TTF");
    if(font_size <= 0)
    	font_size = 16;

	for(i = 0;arab[i];i++){
		LCD_ClearAll();
		fb_utf8_display_align(font_size,
						 0,-1,16,
						 arab[i],
						 0,
						 DISP_CENTER
					  );
		wait_key_suspend(1000);
	}
	return 0x00;
}

int main(int argc,char **argv)
{
    argc = (int)argc;
    argv = (char**)argv;

    if(argc < 3)
    {
	    print_help();
	    return -1;
    }
    system_init();
	test_curr_info();
	switch(*argv[1])
	{
		case '0':
			test_speaker();
			break;
		case '1':
		{
			test_picc_single();
			break;
		}
		case '2':
            if(argv[3])
            	test_Sam(atoi(argv[2]),atoi(argv[3]));
			else
				test_Sam(atoi(argv[2]),0);
			break;
		case '3':
			test_buzzer();
			break;
		case '4':
			test_lcd();
			break;
		case '5':
			test_GPS();
			break;
		case '6':
			test_QRcode();
			break;
		case '7':
			test_LedSeg();
			break;
		case '8':
			test_rtc();
			break;
		case '9':
			test_led();
			break;
		case 'a':
			auto_4G();
			auto_Wifi();
			auto_TCP();
			break;
		case 'b':
			//Test_B_card();
			break;
	    case 'c':
			
			test_felica();
			break;
	    case 'd':
	    	test_felica_exchange();
	    	break;
		case 'f':
			PICC_Open(0);
			PICC_TypeCInit(0);
		case 'e':
			{
				
	       		#if 0
				
				felica_read_write();
				#else
				uint8_t aNdefSystemCode[2]="\xFF\xFF";
				uint8_t bMoreCardsAvailable=0;
				uint8_t data[16]={0};
				uint8_t data_len=0;
				uint16_t ret;
				//felica_read_write();
				//aNdefSystemCode[0]=0x00;
				//aNdefSystemCode[1]=0x00;
				ret=phalFelica_ActivateCard( aNdefSystemCode,
											0x00,
        									data,
									        &data_len,
									        &bMoreCardsAvailable);
				if(ret==0){
					sDumpData("active data",data,data_len);
					printf("card num[%d]\n",bMoreCardsAvailable);
				}
				else{
					printf("felica active fail ret=[%d]\n",ret);
				}
				#endif
				Sys_Beep();
				break;
				
				
			}
			
		case 'm':
			test_mifare();
			break;
		case 'g':
			//Rf24g_Test();
			test_idtwo();
			break;
		case 'i':
			test_gprscmd();
			break;
		case 'j':
			//auto_bluetooth();
			break;
		case 'k':
			auto_can(atoi(argv[3]),atoi(argv[2]));
			break;
		case 'l':
			auto_485();
			break;
		case 'n':
			auto_232();
			break;
		case 'o':
			test_lcd_logo();
			break;	
		case 'p':
			test_power_down();
			break;
		case 'q':
			test_eeprom();
			break;
		case 'r':
			auto_test();
			break;
		case 's':
			test_lcd_ng();
			break;
		case 't':
			auto_TCP();
			break;
		case 'z':
			menu_test();
			break;
        case 'A':
            LC812_test();
            break;
		case 'B':
		{
            test_picc();
			break;
		}
        case 'C':
            // CL3000_test(atoi(argv[2]));
            break;
		case 'F':
		{
			test_utf8_str(atoi(argv[2]));
			break;
		}
		case 'G':
		{
			test_unicode_str(atoi(argv[2]));
			break;
		}
		case 'H':
		{
			test_esam(0,0);
			break;
		}
		case 'M':{
			dispatch_menu_test(argv[2][0],0);
			break;
		}
		case 'Y':{
			aging_test();
			break;
		}
		case 'Z':{
			protocol_Z9_menu_test();
			break;
		}
		case 'y':{

			//printf("[%d]\n",atoi(argv[2]));
			test_uart("/dev/ttymxc11","115200,n,8,1",atoi(argv[2]));
			break;
		}

		default:
			print_help();
			break;	 
	}
    return 0;
}



