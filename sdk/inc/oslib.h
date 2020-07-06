#ifndef _OSLIB_H_ 
#define _OSLIB_H_ 

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// > svn557 

typedef unsigned short int ushort;

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/**
* \ingroup DISPLAY
*/
/*@{*/

//状态表
#define LCD_SUCESS              0x00   //成功
#define LCD_BUSY                0x01   //忙
#define LCD_ERR_PARA            0x02   //参数错误
#define LCD_ERR_FONT            0x03   //字体错误


/* oslib.h */
//#define LCDICONTYPE_MAXNB		6
#define LCDDATE_LEN				13
typedef enum{
ICON_MINNUM		= 0x00,
ICON_SIGNAL		= 0x00,
ICON_TELEPHONE	= 0x01,
ICON_MESSAGE	= 0x02,
ICON_PRINTER	= 0x03,
ICON_MAGCARD	= 0x04,
ICON_ICCCARD	= 0x05,
ICON_BATTERY	= 0x06,
ICON_DATETIME	= 0x07,
ICON_UPICON		= 0x08,
ICON_DOWNICON	= 0x09,
ICON_LOCK		= 0x0A,
ICON_USB		= 0x0B,
ICON_BT			= 0x0C,
ICON_GPS        = 0x0D,
ICON_NETWORK    = 0x0E,
ICON_WIFI		= 0X0F,
                
ICON_MAXNUM		= 0x0F,
}LCD_ICON;

typedef enum{
DISP_BIGFONT        = 0x0000,	/*!< 大字体 \li 英文8x16 \li 中文16X16 \li 缺省 */
DISP_LEFT           = 0x0000,	/*!< 居左对齐,缺省 */
DISP_FONT6X8        = 0x0001,	/*!< 小字体6X8,英文 */
DISP_FONT12         = 0x0002,	/*!< 中字体 \li 英文6x12 \li 中文12X12*/
DISP_CENTER         = 0x0020,	/*!< 居中对齐 */
DISP_RIGHT		    = 0x0040,	/*!< 居右对齐 */
DISP_ALIGN_MASK     = 0x0060,
DISP_REVERT			= 0x0080,	/*!< 反显*/
DISP_UNDERLINE      = 0x0100, /*!< 带下划线正显 */
DISP_CLRLINE        = 0x0200, /* 显示之前清行 */
DISP_FONT24         = 0x0400, /* 24x24 */
DISP_FONT32         = 0x0800, /* 32x32 */
DISP_GBK            = 0x1000, /* 显示字符集为GBK编码*/
}LCDMODE;

#define RTGUI_ARGB(a, r, g, b)  \
        ((uint32_t)(((uint8_t)(b)|\
        (((unsigned long)(uint8_t)(g))<<8))|\
        (((unsigned long)(uint8_t)(r))<<16)|\
        (((unsigned long)(uint8_t)(a))<<24)))

#define RTGUI_RGB(r, g, b)  RTGUI_ARGB(0, (r), (g), (b))
        
#define  CLOSE       0    /*!< 关闭图标[针对所有图标] */
#define  OPEN        1    /*!< 显示图标[针对打印机、IC卡、锁、电池、向上、向下] */

#define _RESERVER_STATUS_BAR 16 /* 保留区域,用于icon显示*/


typedef enum{
    ROW0 = 0,
    ROW1 = 2,
    ROW2 = 4,
    ROW3 = 6,
    ROW4 = 8,
    ROW5 = 10,
    ROW6 = 12,
    ROW7 = 14,
    ROW8 = 16,
}LCD_ROW;

/*@}*/

/**
* \ingroup ICC
*/
/*@{*/

#define OK              0
#define ICCERR_SUCCESS						0x00 /*!< IC卡通信成功 */
#define ICCERR_NO_CARD						0x01 /*!< IC卡不存在 */
#define ICCERR_ATR_ERROR					0x02 /*!< IC卡复位失败 */
#define ICCERR_INVALID_TYPE					0x03 /*!< IC卡槽错误 */
#define ICCERR_DO_WARM_RESET				0x04 /*!< IC卡热复位失败 */
#define ICCERR_RECEIVE_TIMEOUT				0x05 /*!< */
#define ICCERR_EXCHANGE							0x06 /*!< IC卡指令错误 */
#define ICCERR_NOPOWER						0x07 /*!< IC卡未上电 */
#define ICCERR_OPEN 0x08  /*!< IC卡打开失败 */

#ifdef _WIN32
#define ICC_USER				0x00
#define ICC_CL					0x01
#define ICC_MAG					0x80
#else
typedef enum
{
	ICC_USER = 1,	/*!< 大卡1 */
	ICC_CL,
	ICC_SIM1,
	ICC_SIM2,
	ICC_SIM3,
	ICC_SIM4,
	ICC_SIM5,
    ICC_ESAM,  /*!< ESAM 加密模块 */
	ICC_USER2, /*!< 大卡2 */
	ICC_INVALID = ICC_USER2,
    ICC_MAX_READER=ICC_USER2,
	ICC_MAG = 0x80,/*< 磁条卡 */
}ICC_TYPE;
#endif
typedef enum{
BAUD_9600  	= 0,
BAUD_19200  = 1,
BAUD_38400  = 2,
BAUD_57600  = 3,
BAUD_115200 = 4,
BAUD_55800  = 5,
BAUD_111600 = 6,
BAUD_223200 = 7,
BAUD_446400 = 8,

}ICC_BAUD;

typedef enum{

 VOLT_5V  =  0x00,   /*!< 5V */
 VOLT_3V3 = 0x01,    /*!< 3V */
 VOLT_1V8 = 0x02,    /*!< 1.8V */
 PPS_SUPPORT = 0x80, /*!< BIT8  1:支持PPS,0:不支持（缺省) */
}ICC_VOLT;

#define     SAK_TypeA_TCL				0x20  
#define     SAK_MiFare1K				0x08 
#define     SAK_MiFare4K				0x18 
#define     SAK_MiFareUltralight		0x00
#define     SAK_MiFareMINI				0x09
#define     SAK_XIZHOU_CITIZEN_CARD		0x53

typedef enum{
TYPE_NO_PICC                = 0x00, /*!< 无卡 */
TYPE_A_TCL                  = 0x11, /*!< A卡 */
TYPE_A_MIFARE               = 0x12, /*!< Mifare卡 */
TYPE_A_MIFARE_1K            = 0x13, /*!< 1K Mifare 卡 */
TYPE_A_MIFARE_4K            = 0x14, /*!< 4K Mifare 卡 */
TYPE_A_MIFARE_Ultralight    = 0x15, /*!< Ultralight 卡 */
TYPE_A_MiFareMINI			= 0x16, /*!< Mifare mini卡 */
TYPE_A_Ntag_21x             = 0x18, /*!< Ntag 卡 */
TYPE_A_OTHERS_MEM			= 0x1F, /*!< 其他memory卡 */
TYPE_A_MASK                 = 0x10, /*!< */

TYPE_B_TCL                  = 0x21, /*!< type B卡 */
TYPE_B_OTHER                = 0x22, /*!< B卡，如身份证卡 */
TYPE_B_MASK                 = 0x20,

TYPE_C_TCL                  = 0x41, /*!< C卡 */

TYPE_UNKNOWN                = 0xFF,
}PICC_CARD_TYPE;
/*@}*/

/**
* \ingroup LED
*/
/*@{*/

typedef enum
{
	LED_BLUE      = 0x01,
	LED_YELLOW    = 0x02,
	LED_GREEN     = 0x04,
	LED_RED       = 0x08,
    LED_BACKRED   = 0x10,
    LED_BACKGREEN = 0x20,
	LED_ALL = 0xFF,
}LED_TYPE;
/*@}*/



/**
* \ingroup System
*/
/*@{*/

typedef struct __gprmc__  
{  
   uint32_t time;/* gps定位时间 */
   char pos_state;/*gps状态位*/
   float latitude;/*获取纬度 */
   float longitude;/*获取经度 */
   float speed; /* 速度 */
   float direction;/*航向 */
   uint32_t date;  /*日期  */
   float declination; /* 磁偏角 */
   char dd;  
   char mode;/* GPS模式位 */
   char hemisphere[2+1];  /*半球信息 */
   int sate_num;  /* 卫星数*/
   char quality; /*gps信号质量*/
   
   char date_time[14+1]; /* 转换的当地时间 YYYYMMDDHHMMSS */
   double latitude_wgs84;/*转换后wgs84纬度坐标*/
   double longitude_wgs84;/*转换后wgs84经度坐标*/
   char strgps[1024];
}GNRMC;

typedef struct __gpgsa__  
{  
   int mode1;/* 模式1: M=手动，A=自动 */
   int mode2;/*模式2:1=未定位，2=2D 定位，3=3D 定位*/
   int satelliteID1;/*使用卫星ID1 */
   int satelliteID2;/*使用卫星ID2 */
   int satelliteID3;/*使用卫星ID3 */
   int satelliteID4;/*使用卫星ID4 */
   int satelliteID5;/*使用卫星ID5*/
   int satelliteID6;/*使用卫星ID6 */
   int satelliteID7;/*使用卫星ID7 */
   int satelliteID8;/*使用卫星ID8 */
   int satelliteID9;/*使用卫星ID9 */
   int satelliteID10;/*使用卫星ID10 */
   int satelliteID11;/*使用卫星ID11 */
   int satelliteID12;/*使用卫星ID12 */
   float pdop;/*位置精度稀释值 PDOP: 0.5 至 99.9*/
   float hdop; /* 水平精度稀释值 HDOP:0.5 至 99.9 */
   float vdop;/*垂直精度稀释值 VDOP:0.5 至 99.9 */
   uint32_t Checksum;  /*总和检查码  */
}GPGSA;

typedef struct __gpgsv__  
{  
   int numbermsg;/*讯息总数 */
   int sequencenum;/*讯息号码*/
   int satellitesnum;/*天空中卫星总数 */
   int satelliteID1;/*卫星1编号 */
   int elevation1; /* 卫星仰角 */
   int azimuth1;/*卫星方位角 */
   int SNR1;  /*信号噪声比1:00 至 99 dB；无表未接收到讯号  */
   int satelliteID2;/*卫星2编号 */
   int elevation2; /* 卫星仰角 */
   int azimuth2;/*卫星方位角 */
   int SNR2;  /*信号噪声比2   :00 至 99 dB；无表未接收到讯号*/
   int satelliteID3;/*卫星3编号 */
   int elevation3; /* 卫星仰角 */
   int azimuth3;/*卫星方位角 */
   int SNR3;  /*信号噪声比  3:00 至 99 dB；无表未接收到讯号*/
   uint32_t Checksum;  /*总和检查码  */
}GPGSV;




typedef enum{
    // 注意linux下不能有名称污染，和 linux/input.h冲突
_KEY_0			= 0x30, /*!< 数字键0 */
_KEY_1			= 0x31, /*!< 数字键1 */
_KEY_2			= 0x32,	/*!< 数字键2 */
_KEY_3			= 0x33,	/*!< 数字键3 */
_KEY_4			= 0x34,	/*!< 数字键4 */
_KEY_5			= 0x35,	/*!< 数字键5 */
_KEY_6			= 0x36,	/*!< 数字键6 */
_KEY_7			= 0x37,	/*!< 数字键7 */
_KEY_8			= 0x38,	/*!< 数字键8 */
_KEY_9			= 0x39,	/*!< 数字键9 */

_KEY_ENTER       = 0x3A,	/*!< 回车键 */
_KEY_CANCEL		= 0x3B,	/*!< 取消键 */
_KEY_CLEAR       = 0x3D,	/*!< 清除/退格键 */
_KEY_00_PT       = 0x3E,	/*!< 进纸键 */
_KEY_BCKSP       = 0x3D,	/*!< 退格键 */
_KEY_F1          = 0x40,	/*!< F1键 */
_KEY_F2          = 0x41,
_KEY_F3          = 0x42,
_KEY_F4          = 0x43,
_KEY_F5          = 0x44,
_KEY_F6          = 0x45,
_KEY_F7          = 0x46,
_KEY_F8          = 0x47,
_KEY_F9          = 0x48,
_KEY_F           = 0x45,	/*!< =KEY_F6*/
_KEY_R           = 0x46,	/*!<=KEY_F7*/

_KEY_SYS         = 0x3F,	/*!<  */
_KEY_PAPER_FEED  = 0x40,	/*!< 走纸键 */
_KEY_MENU		= 0x50,	/*!< 菜单键 */
_KEY_UP			= 0x60,	/*!< 向上键 */
_KEY_DOWN		= 0x61,	/*!< 向下键 */
_KEY_LEFT 		= 0x62,	/*!< 向左键 */
_KEY_RIGHT		= 0x63,	/*!< 向右键 */

_KEY_ALPHA		= 0xF2,	/*!< *键 */
_KEY_JIN			= 0xF3,	/*!< '#'键 */
_KEY_ABORT		= 0xFF,	/*!< 虚拟退出键,实际应用自定义 */

}KEYVALUE;

typedef enum{
	T80_A2 = 0,
	P18_L2,
	T80_L2,
	P18_L2C,
	P18_L2L = 9,
	TERM_TYPE_INVALID = 0x0A
}TERM_TYPE;

/*@}*/

/* error define  */
#define MIFARE_SUCCESS                 0 
#define MIFARE_ERROR                   1
#define MIFARE_PARA_ERROR              2
#define MIFARE_WALLETFORMAT_ERROR      3

#define SYS_OPENDEVICE -0001
#define SYS_TIMEOUT -2001

/**
* \defgroup DISPLAY 显示接口
*/
/*@{*/

/**
 * @brief 显示屏初始化
 */
int LCD_Init(void);
/**
 * @brief 获取显示屏点阵高度
 */
uint32_t LCD_GetHeight(void);
/**
 * @brief 获取显示屏点阵宽度
 */
uint32_t LCD_GetWidth(void);
/**
 * @brief 清除全屏
 * @param[in] color 清除使用的颜色
 *            对于黑白屏
 *            \li 0 清除
 *            \li 非0 反显颜色
 *            对于彩屏
 *            ARGB 32 bit 颜色
 */
uint8_t LCD_ClearScreen(uint32_t color);

/**
 * @brief 清除可显区域，不包括状态栏
 */
uint8_t LCD_ClearAll(void);
/**
 * @brief 清除状态栏(图标栏)
 * @note 12行点阵
 */
void LCD_ClearIconLine(void);
/**
 * @brief 屏幕显示
 * @param[in] row  y方向坐标 [0~LCD_HEIGHT)
 * @param[in] mode 显示模式参数 \sa LCDMODE
 * @param[in] fmt  待显示字符串，默认UTF-8编码，
 * @note 如果要显示GBK字符串，mode 需要使用DISP_GBK
 */
uint8_t LCD_Printf(uint16_t row,uint32_t mode,void *fmt,...);

/**
 * @brief 清多行
 * @param[in] pixel_y y方向坐标 [0~LCD_HEIGHT)
 * @param[in] height 点阵高度
 */
uint8_t LCD_Clear_rows(uint16_t pixel_y, uint16_t height);

/**
 * @brief 单行显示GB18030字符串
 * @param[in] str     待显示字符串
 * @param[in] pixel_y y方向坐标 [0~LCD_HEIGHT)
 * @param[in] pixel_x x方向坐标 [0~LCD_WIDTH)
 * @param[in] mode 显示模式参数 \sa LCDMODE
 */
uint8_t LCD_Display_Row(char *str,uint16_t pixel_y,uint16_t pixel_x,unsigned int mode);

/**
 * @brief 显示icon
 * @param[in] ucIcon 待显示icon
 * @param[in] bOnOff 亮/灭
 */
void LCD_DisplayIcon(LCD_ICON ucIcon,uint8_t bOnOff);

void LCD_FillEllipse(int xpos, int ypos, int a, int b,unsigned char mode);

void LCD_DrawEllipse(unsigned char xpos,unsigned char ypos,unsigned char a,unsigned char b,unsigned char mode);

void LCD_FillRect(uint16_t xstart,uint16_t ystart,uint16_t xlen, uint16_t ylen ,unsigned char mode);

uint8_t LCD_DrawRect(uint16_t xstart,uint16_t ystart,uint16_t xlen,uint16_t ylen,void *color);

/**
 * @brief 显示单色图标
 * @param[in] x0  图标显示ｘ坐标 [0-LCD_WIDTH-1]
 * @param[in] y0  图标显示y坐标 [0 * 8 - LCD_HEIGHT]
 * @param[in] width  图标显示宽度
 * @param[in] height  图标显示高度
 * @param[in] logo_data 待显示图标数据
 * @param[in] ucMode 保留
 */
void LCD_DisplayLogo(uint32_t x0,unsigned int y0,                       
        unsigned int width,
        unsigned int height,
        unsigned char *logo_data,unsigned char ucMode);

/**
 * @brief 清除行
 * @param[in] x0  起始ｘ坐标
 * @param[in] y0  起始y坐标
 * @param[in] w 清除宽度
 * @param[in] h 清除高度
 * @param[in] color 颜色选择
 */
void lcd_clearblock(int x0, int y0,int w,int h,uint8_t color); 

/**
 * @brief 设置字体和背景图片
 * @param[in] file_ttf  字体
 * @param[in] bg_file  背景图片
 */
int fb_init(char * file_ttf,char *bg_file);

/**
 * @brief 在屏幕指定坐标画图
 * @param[in] x  起始ｘ坐标
 * @param[in] y  起始y坐标
 * @param[in] file_name 图片
 */
int picture_write(int x, int y, char *file_name);

/**
 * @brief 显示信号图标
 * @param[in] net_type  信号模式2-4G:2-4
 * @param[in] signal  信号质量
 */
int fb_show_signal(int net_type,int signal);

/**
 * @brief 显示WIFI图标
 * @param[in] level  信号强度0-4 4为最大
 */
int fb_show_wifi(int level);

/**
 * @brief 显示GPS图标
 * @param[in] satilite_num  连接卫星数量，以GPS模块返回为准，0-29
 */
int fb_show_GPS(int satilite_num);

/**
 * @brief 显示蓝牙状态图标
 * @param[in] value 蓝牙状态
 *          \li 0 清除
 *          \li 1 关闭
 *          \li 2 已打开
 */
int fb_show_bluetooth(int value);


/**
 * @brief 指定坐标显示字
 * @param[in] char_size  字大小
 * @param[in] xoffset  起始ｘ坐标
 * @param[in] yoffset  起始y坐标(L2C可显示坐标为80-475)
 * @param[in] text_gb2312 字符串
 * @param[in] fc 显示模式
 */
int fb_gbk_display(int char_size,
                   int xoffset,int yoffset,
                   char *text_gb2312,
                   uint32_t fc);
/**
 * @brief 指定坐标显示字
 * @param[in] char_size  字大小
 * @param[in] x1  起始x坐标
 * @param[in] x2  结束x坐标
 * @param[in] yoffset  起始y坐标(L2C可显示坐标为80-475)
 * @param[in] text_utf8 字符串
 * @param[in] fc 显示前景色
 * @param[in] mode 显示模式 @sa #LCDMODE
 */
int fb_utf8_display_align(int char_size,
                     int x1,int x2,int yoffset,
                     char *text_utf8,
                     uint32_t fc,
                     uint32_t mode
                  );

/**
 * @brief 清除前景色块
 * @param[in] x      起始x坐标
 * @param[in] y      起始y坐标
 * @param[in] width  待清除色块宽度
 * @param[in] height 待清除色块高度
 */
int lcd_erase_foreground(int x,int y,int width,int height);
/**
 * @brief 开启屏幕操作锁
 */
void fb_disp_start(void);

/**
 * @brief 关闭屏幕操作锁
 */
void fb_disp_end(void);

/**
 * @brief 设置fb接口使用的字库，立即有效
 * @param[in] file_ttf 字库绝对路径
 */
int fb_set_ttf(char *file_ttf);
/*@}*/


/**
* \defgroup UART 串口通信
*/
/*@{*/

/**
 * @brief 串口初始化
 * @param[in] dev_name 串口名称，如/dev/ttymxc1
 * @param[in] attr 串口参数 "115200,n,8,1"
 *               \li "115200"使用缺省'n,8,1'
 * @return >0 串口句柄
 * @return <0 打开失败
 */
int OSUART_Init(char *dev_name,char *attr);
/**
 * @brief 串口发送
 * @param[in] handle 串口句柄
 * @param[in] pucSend 待发送数据
 * @param[in] uiSendLen 数据长度
 */
int OSUART_TxStr(int handle,uint8_t *pucSend,uint32_t uiSendLen);


/**
 * @brief 串口接收
 * @param[in] handle 串口句柄
 * @param[in] pucRev 待接收数据数组
 * @param[in] uiLen  接收最大长度，一般为数组大小
 * @param[in] uiTimeout 最大接收超时时间，单位毫秒
 * @note 接收字符间隔超时为10ms，
 *       每次接收一包数据，对于多包发送的协议，需要多次接收，以确定完整性
 */
int  OSUART_RxStr(int handle,unsigned char *pucRev,uint32_t uiLen,uint32_t uiTimeout);
/**
 * @brief 串口单字节接收
 * @param[in] handle 串口句柄
 * @param[in] pucRev 待接收数据数组
 * @param[in] uiTimeout 最大接收超时时间，单位毫秒
 */
int  OSUART_RxChar(int handle,uint8_t *pucRev,uint32_t uiTimeout);/**
 * @brief 串口关闭
 * @param[in] handle 串口句柄
 * @note 对于一般通信串口，可不做关闭操作
 */
void OSUART_Close(int handle);

/**
 * @brief 清空串口缓存
 * @param[in] handle 串口句柄
 */
void OSUART_Flush(int handle);
/**
 * @brief libpos调试信息配置
 * @param[in] on_off
 */
void Uart_SetDebug(uint8_t on_off);

/**
 * @打印调试信息
 * @param pcFmt 输入格式
 */
void Uart_Printf(char *pcFmt,...);

/**
 * @brief dump 16进制数组
 * @param[in] str  数组名称
 * @param[in] pBuf 数组缓存
 * @param[in] len  数组长度
 */
void DumpData(const char * str, uint8_t * pBuf, uint32_t len);

/**
 * @brief 单行dump16进制数组
 * @param[in] str  数组名称
 * @param[in] pBuf 数组缓存
 * @param[in] len  数组长度
 */
void sDumpData(const char * str, uint8_t * pBuf, uint32_t len);

/*@}*/

/*timer 和rtc */
/**
 * @brief 获取系统运行滴答，单位毫秒
 */
unsigned long OSTIMER_GetTickCount(void);

/**
 * @brief 微秒延时
 */
void OSTIMER_DelayUs(int num);

/**
 * @brief 毫秒延时
 */
void OSTIMER_DelayMs(int ms);
/**
 * @param[in] DateTime
 * @brief 设置系统时间，格式YYYYMMDDHHMMSS\0
 * @note 设置系统时间和rtc时钟
 */
int Sys_SetRealTime(char *DateTime);
/**
 * @brief 获取系统时间，格式YYYYMMDDHHMMSS\0
 * @param[out] DateTime
 * @note 获取系统时钟
 */
int Sys_GetRealTime(char *DateTime);


/**
 * @brief 蜂鸣初始化
 */
void Beep_Init(void);
/**
 * @brief 蜂鸣，非阻塞
 */
void Sys_Beep(void);

/**
 * @brief 蜂鸣，阻塞
 * @param[in] ms 毫秒
 */
void Sys_BeepMs(uint32_t ms);
/**
 * @brief 多次蜂鸣，非阻塞
 * @param cycles 共蜂鸣次数
 * @param beep_time 单次蜂鸣时间
 * @param silent_time 单次蜂鸣后间隔时间
 */
void Sys_BeepBeep(uint32_t cycles,uint32_t beep_time,uint32_t silent_time);

/**
 * @brief 警告蜂鸣，阻塞
 * @param[in] num 蜂鸣次数
 */
void Sys_ErrorBeep(int num);


/**
* \defgroup LED LED灯和数码管接口
*/
/*@{*/

/**
 * @brief led灯闪烁控制
 * @param[in] led     led灯索引
 * @param[in] uiOnTime 单次循环，灯亮时间，单位毫秒
 * @param[in] uiOffTime 单次循环，灯灭时间，单位毫秒
 * @param[in] uiCycle 亮灭周期
 */
uint8_t LED_Flicker(LED_TYPE led, uint32_t uiOnTime, uint32_t uiOffTime, uint32_t uiCycle);

/**
 * @brief led灯控制
 * @param[in] led     led灯索引
 * @param[in] ucOnOff 打开/关闭
 */
uint8_t LED_Control(LED_TYPE led,uint8_t ucOnOff);

/**
 * @brief 清空数码管显示
 */
void LedSeg_Clear(void);
/**
 * @brief 数码管显示余额
 * @param[in] uiBalance 待显示余额
 * @note 最大显示金额由当前数码管数量决定
 * @note 自动适配当前终端类型
 */
void LedSeg_Balance(uint32_t uiBalance);
/**
 * @brief 数码管显示接口
 * @param[in] index 索引号[0-15]
 * @param[in] str 待显示字符串,支持’0‘-’9‘ ’A'-'Z',‘.'指示当前数码管是否显示小数点
 *        \li  " 1.23"
 *        \li  "  234.11"
 *        \li  "A2.3B"
 */
void  LedSeg_Display(int index,char *str);

/**
 * @brief 数码管初始化
 */
int LedSeg_Init(void);
/*@}*/

/**
* \defgroup ICC IC卡接口
*/
/*@{*/
/**
 * @brief ic卡或sam模块初始化
 * @param[in] slot 卡槽号
 */
int ICC_Init(uint8_t slot);
/**
 * @brief ic卡或sam模块上电
 * @param[in] slot 卡槽号
 * @param[in] atr 复位值
 * @param[in] len atr长度
 */

int ICC_GetATR(uint8_t slot,uint8_t* atr,uint32_t* len);

/**
 * @brief ic卡或sam模块上电
 * @param[in] slot 卡槽号
 * @param[in] baud 波特率，默认9600
 *            \li BAUD_9600  9600
 *            \li BAUD_19200 19200
 *            \li BAUD_38400 38400
 *            \li BAUD_57600  57600
 *            \li BAUD_115200 115200
 * @param[in] volt 操作电压，默认5.0V
 *              BIT1-BIT2
 *                  \li 0x00：5V
 *                  \li 0x01：3V
 *                  \li 0x02：1.8V
 *              \li BIT8  1:支持PPS,0:不支持（缺省)
 * @param[out] atr 卡片复位应答信息
 * @param[out] len atr长度
 */
int ICC_PowerUp(uint8_t slot,ICC_BAUD baud,ICC_VOLT volt,uint8_t* atr,uint32_t* len);
/**
 * @brief 卡在位检测
 * @param[in] slot 卡槽号
 * @return 0 卡在位检测
 * @return 1 卡片不在位
 */
int ICC_Detect(uint8_t slot);
/**
 * @brief 卡片APDU交互
 * @param[in] slot 卡槽号
 * @param[in] apdu  apdu数据
 * @param[in] apdu_len   apdu长度
 * @param[out] rpdu 返回数据
 * @param[out] rpdu_len 返回长度
 */
int	ICC_Exchange(uint8_t slot,const uint8_t* apdu,const uint32_t apdu_len, uint8_t* rpdu, uint32_t* rpdu_len);

/**
 * @brief 卡片下电
 * @param[in] slot 卡槽号
 * @note 仅对支持下电的卡槽有效
 */
int ICC_Remove(uint8_t slot);
/**
 * @brief SAM卡关闭
 * @param[in] slot 卡槽号
 */
int ICC_Close(uint8_t slot);

/**
 * @brief ISO14443寻卡，A&B
 * @param[in] slot 卡槽号
 * @param[out] uid 卡片UID
 */
int PICC_PollCard(uint8_t slot,uint8_t uid[10]);
/**
 * @brief 非接关场
 * @param[in] slot 卡槽号
 */
int PICC_Close(uint8_t slot);

/**
 * @brief 非接模块初始化
 * @param[in] slot 对于多个非接模块，使用0,1,2区分
 */
int PICC_Open(uint8_t slot);
/**
 * @brief 场复位
 * @param[in] slot 卡槽号
 */
int PICC_Reset(uint8_t slot);
/**
 * @brief 非接上电
 * @param[in] slot 卡槽号
 * @param[in] mode 寻卡模式
 *                \li 'A' 只寻A卡
 *                \li 'B' 只寻B卡
 *                \li 'C' 只寻Felica卡
 *                \li 0   寻AB卡
 * @param[out] cardType 卡类型 \sa #PICC_CARD_TYPE
 * @param[out] uid 卡片UID
 * @param[out] uid_size 卡片UID长度
 * @note 对于Felica,uid 返回 Manufacture ID + PMm, uid_size 为16
 */
int PICC_PowerOn(uint8_t slot,uint8_t mode,uint8_t *cardType,uint8_t *uid,uint8_t *uid_size);
/**
 * @brief PICC apdu通信
 * @param[in] slot 卡槽号
 * @param[in] apdu_buff  apdu数据
 * @param[in] apdu_len   apdu长度
 * @param[out] resp_buffer 返回数据
 * @param[out] resp_len 返回长度
 */
int PICC_Exchange(uint8_t slot,uint8_t * apdu_buff, uint32_t apdu_len, uint8_t *resp_buffer, uint32_t * resp_len);

/**
 * @brief init type c reader mode
 * @param[in] bitrate
 *             \li 0 212kb/s
 *             \li 1 424kb/s
 * @note to use with #PICC_TypeCExchange
 */
int PICC_TypeCInit(uint8_t bitrate);
/**
 * @brief Type C low-level communication
 * @param[in] slot 卡槽号(Reserved)
 * @param[in] apdu_buff  apdu数据
 * @param[in] apdu_len   apdu长度
 * @param[out] resp_buffer 返回数据
 * @param[out] resp_len 返回长度
 */
int PICC_TypeCExchange(uint8_t slot,uint8_t *apdu_buff, uint16_t apdu_len, uint8_t *resp_buffer, uint32_t *resp_len);
/**
 * @brief 非接场开启/关闭
 * @param[in] slot 卡槽号
 * @param[in] carrier_on_off 开启/关闭
 */
int PICC_Carrier(uint8_t slot,uint8_t carrier_on_off);
/**
 * @brief 设置非接场强大小，默认最大
 * @param[in] level 0-8 0最大 8最小
 */
void PICC_FieldLevel(uint8_t level);
/**
 * @brief 非接模块关闭
 * @param[in] slot 卡槽号
 * @note 暂时只做关场操作
 */
int PICC_Close(uint8_t slot);
/*@}*/



/**
* \defgroup Mifare Mifare卡接口
*/
/*@{*/

typedef enum _ENUM_MIFARE_KEY_TYPE_
{
    MIFARE_KEY_A = 0x60,
    MIFARE_KEY_B = 0x61
}ENUM_MIFARE_KEY_TYPE;

/**
 * @brief Mifare扇区密钥认证
 * @param[in] type 密钥类型
 * @param[in] addr 扇区地址
 * @param[in] key Mifare扇区密钥，6字节
 */
uint8_t Mifare_AuthenBlock(ENUM_MIFARE_KEY_TYPE type,uint8_t addr,uint8_t *key);
/**
 * @brief 读取Mifare卡扇区数据
 * @param[in] addr 扇区地址[0~63]
 * @param[out] out_data 读出的扇区数据
 */
uint8_t Mifare_ReadBlock(uint8_t addr,uint8_t *out_data);
/**
 * @brief 写入Mifare卡扇区数据
 * @param[in] addr 扇区地址[0~63]
 * @param[in] in_data 写入的扇区数据
 */
uint8_t Mifare_WriteBlock(uint8_t addr,uint8_t *in_data);
/**
 * @brief 电子钱包初始化
 * @param[in] addr 钱包块地址
 * @param[in] balance 初始值
 */
uint8_t Mifare_InitWallet(uint8_t addr,uint32_t balance);
/**
 * @brief 读钱包值
 * @param[in] addr 钱包块地址
 * @param[out] balance 钱包值
 * @return \li 0：正确
 *         \li 1：失败
 *         \li 2:其他错误
 */                   
uint8_t Mifare_ReadWallet(uint8_t addr,uint32_t *balance);
/**
 * @brief 钱包增值
 * @param[in] addr 钱包块地址
 * @param[in] add_value 钱包增值
 * @return \li 0：正确
 *         \li 1：失败
 *         \li 2:其他错误
 */                   
uint8_t Mifare_AddWallet(uint8_t addr,uint32_t add_value);

/**
 * @brief 钱包扣值
 * @param[in] addr 钱包块地址
 * @param[in] dec_value 钱包扣值
 * @return \li 0 正确
 *         \li 1 失败
 *         \li 2 其他错误
 */                   
uint8_t Mifare_DecWallet(uint8_t addr,uint32_t dec_value);

/**
 * @brief 钱包备份
 * @param[in] addr_from 待备份区地址
 * @param[in] addr_to   钱包备份地址
 * @return \li 0 正确
 *         \li 1 失败
 *         \li 2 其他错误
 */      
uint8_t Mifare_BackupWallet(uint8_t addr_from,uint8_t addr_to);

/**
 * @brief Mifare寻卡
 * @param[in] slot 卡槽号
 * @param[out] snr 卡片数据
 *             atqa(2) + sak(1) + uid(4-10)
 * @param[out] snr_len snr长度
 */
int Mifare_PowerOn(uint8_t slot,uint8_t *snr,uint8_t *snr_len);

/*@}*/


/**
* \defgroup System 系统接口
*/
/*@{*/
/**
 * @brief GPS模块硬件初始化
 * @param[in] mxc 串口名称  例:"/dev/ttymxc3"
 * @param[in] para 串口配置参数 例:"9600,8,n,1"
 */
int Sys_Gps_Init(char *mxc,char *para);



/**
 * @brief GPS模块关闭
 */
void Sys_Gps_Close(void);


/**
 * @brief GPS调试信息开关
 * @param[in] mode =0关闭; =1打开;
 */
void Sys_Gps_Debug(unsigned char mode);

/**
 * @brief GPS获取解析后信息
 * @param[in] timezone 当前所在时区
 * @return 解析后信息结构体指针
 
  typedef struct __gprmc__  
  {  
    uint32_t time; //gps定位时间
    char pos_state; //gps状态位
    float latitude; //获取纬度
    float longitude;//获取经度
    float speed; // 速度
    float direction;//航向
    uint32_t date;  //日期
    float declination; // 磁偏角
    char dd;  
    char mode;  //GPS模式位
    char hemisphere[2+1];  //半球信息
    int sate_num;  // 卫星数
    char quality;  //gps信号质量
   
    char date_time[14+1]; //转换的当地时间 YYYYMMDDHHMMSS
    double latitude_wgs84;//转换后wgs84纬度坐标
    double longitude_wgs84;//转换后wgs84经度坐标
    char strgps[1024];	//获取到的未解析数据
   }GNRMC;  
 */
GNRMC* Sys_Gps_QueryLaLO(char *timezone);


/**
 *@brief 查询终端类型
 *@return 终端类型 @sa TERM_TYPE
 */
int Sys_TermType(void);

/**
 *@brief 查询终端名称
 *@param[out] term_name 终端名称
 */
int Sys_TermName(char *term_name);

/**
 *@brief 查询按键
 *@param[in] TimeOut 超时时间，毫秒
 */
int Sys_WaitKey(int TimeOut);
/**
 *@brief 清空按键缓存
 */
int Sys_FlushKey(void);
/**
 * @brief 设置终端序列号
 * @param[in] sn
 */
int Sys_SetSN(char *sn);
/**
 * @brief 读取终端序列号
 * @param[in] sn
 */
int Sys_GetSN(char *sn);

/**
 * @brief 读取终端分辨率
 * @param[out] x 宽度
 * @param[out] y 高度
 */
int Sys_GetResolution(int *x,int *y);
/**
 * @brief 获取函数库版本号
 * @return 版本信息
 */
char *Get_HardwareVer(void);



/**
 * @brief 获取libpos api 版本
 * @param [out] version 版本，可传NULL
 * @return API版本 < 63字节
 */
char *Sys_ApiVer(char *version);

/**
 * @brief 获取rootfsi 版本
 * @param [out] ver 版本
 * @return 0: true  other: false
 */
char  Sys_RootfsVer(char *ver);


/*@}*/

/**
* \defgroup QRCode QRCode驱动接口
*/
/*@{*/

//int QRCode_Init(char *mxc,long baudrate);
/**
 * @brief 二维码模块打开
 * @return 模块句柄
 */
int QRCode_Open(int index);
/**
 * @brief 二维码模块接收
 * @param[in] handle 模块句柄
 */
int QRCode_Close(int handle);
/**
 * @brief 二维码模块接收
 * @param[in] handle 串口句柄
 * @param[in] pucRev 待接收数据
 * @param[in] uiLen  待接收数据最大长度
 * @param[in] uiTimeout 超时时间，毫秒
 */
int  QRCode_RxStr(int handle,unsigned char *pucRev,uint32_t uiLen,uint32_t uiTimeout);

/**
 * @brief 扫描禁止/使能
 * @param[in] handle 串口句柄
 * @param[in] enable 1 使能,0 禁止
 * @return 0 成功
 *          -1 失败
 */
int QRCode_ScanOnOff(int handle,int enable);

/*@}*/


/**
* \defgroup CAN CAN通信接口
*/
/*@{*/

/**
 * @brief CAN总线初始化
 * @param[in] devname CAN设备名称，如can0
 * @param[in] bitrate 速率 100000，100Kb/s
 * @return can句柄
 */
int Can_Init(char *devname,unsigned int bitrate);
/**
 * @brief CAN总线写入
 * @param[in] canfd 句柄
 * @param[in] data_id 数据识别符
 * @param[in] sndlen 发送数据长度
 * @param[in] sndbuff 发送数据缓存
 */
int Can_Write(int canfd ,int data_id, unsigned int sndlen, unsigned char *sndbuff);
/**
 * @brief CAN总线读取
 * @param[in] canfd 句柄
 * @param[in,out] pdata_id 设置接收的数据识别符（忽略其他数据）,
 *                        -1表示接收所有数据，返回值是实际接受的数据id
 *                        注意数据识别符范围（0-0x7FF）11bit
 * @param[in,out] len 期望接收的数据长度
 * @param[in] rcvbuff 接收数据缓存
 * @param[in] timeout_ms 等待接收的超时时间
 * @return 实际接收的数据长度
 *         <=0 表示失败
 */
int Can_Read(int canfd ,int *pdata_id, unsigned int len, unsigned char *rcvbuff, unsigned int timeout_ms);
/**
 * @brief 关闭句柄
 * @param[in] canfd 句柄
 */
int Can_Close(int canfd);
/*@}*/

/**
* \defgroup eeprom eeprom 读写接口
*/
/*@{*/

/**
 * @brief 写EEPROM。
 * @note 写入数据到设备内部EEPROM中，可以用作数据保存等。
 * @param [in] icdev 设备标识符。
 * @param [in] offset 偏移地址。
 * @param [in] length 写入长度。
 * @param [in] send_buffer 传入数据。
 * @return <0 表示失败
 * @return 0表示成功
*/
short dc_swr_eeprom  ( int  icdev,
					short  offset,
					short  length,
					unsigned char *  send_buffer
 );
/**
 * @brief 读EEPROM。
 * @note 读取设备内部EEPROM中的数据，可以用作数据保存等。
 * @param [in] icdev 设备标识符。
 * @param [in] offset 偏移地址。
 * @param [in] length 读取长度。
 * @param [out] rec_buffer 返回的数据。
 * @return <0 表示失败
 * @return 0 表示成功
 */
short dc_srd_eeprom  ( int  icdev,
						short  offset,
						short  length,
						unsigned char *  rec_buffer
 );

/*@}*/

/**
* \defgroup gprs gprs模块接口
*/
/*@{*/


/**
 * @brief 获取4G模块信号强度
 * @retval 0-31 有效值
 * @retval 99 未知状态（可能未插天线）
 * @retval < 0 查询失败
 */
int ppp_get_signal(void);

/**
 * @brief 获取运营商名称和当前网络制式
 */
int ppp_network_type(char *operator_name,int *act);

/**
* @brief at指令查询4G状态
* @param [in] at_command at指令。
* @param [in] ack 正常返回字符串。
* @param [in] nck 非正常返回字符串。
* @param [out] recvbuff 指令返回字符串.
* @return 0 ack,-1 nck.
*/
int ppp_at_command(char *at_command,char *ack, char *nck, char *recvbuff);


/*@}*/

/**
 * @brief 传送上层身份证相关信息
 * @param [in] cmd 身份证信息部分命令。
 * @param [in] send_len 发送数据长度。
 * @param [in] send_para 发送数据。
 * @param [out] rev_len 返回数据长度.
 * @param [out] rev_para 返回数据.
 * @return 0 true,1 false.
 */
unsigned char IDCARD_ReadMess(unsigned char cmd, ushort send_len, 
                      		  unsigned char *send_para, ushort *rev_len, 
                      		  unsigned char *rev_para);

/**
 * @brief 全自动读取身份证数据
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * @return 0 true,1 false.
 */
ushort IDCARD_AutoRead(ushort* len,unsigned char* para);
/**
 * @brief 全自动读取身份证数据+指纹信息
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * @return 0 true,1 false.
 */
ushort IDCARD_AutoRead_Fig(ushort* len,unsigned char* para);

/**
 * @brief 标准协议透传调用
 * @param [in] len 发送长度。
 * @param [in] para 发送数据。
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * @return 0 true,1 false.
 */
ushort IDTWO_ReadMsg(ushort* len, unsigned char* para);
/**
 * @brief 激活身份证
 * @param [out] rev_len 返回数据长度.
 * @param [out] rev_para 返回数据.
 * @return 0 true,1 false.
 */
ushort IDCARD_ActiveIdcard(void);
/**
 * @brief 获取身份证UID
 * @param [out] rev_len 返回数据长度.
 * @param [out] rev_para 返回数据.
 * @return 0 true,1 false.
 */
ushort IDCARD_GetUid(unsigned char *rev_len,unsigned char *rev_para);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _OSLIB_H_ */
