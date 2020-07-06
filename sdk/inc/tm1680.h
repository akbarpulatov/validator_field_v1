#ifndef __TM1680_H
#define __TM1680_H

#include <stdint.h>
	
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

typedef enum{
LED_LIGHT = 0, 		/*!< led灯显示  \sa  LIGHT_COLOR  */ 
LED_SIGNAL,	   		/*!< 移动信号 */
LED_WIFI,	   		/*!< WIFI信号     */
LED_BLUETOOTH, 		/*!< 蓝牙图标*/
LED_GPS,			/*!< GPS图标*/
LED_UP,				/*!< 流量向上图标 */
LED_DOWN,			/*!< 流量向下图标 */
LED_PERCERNT,		/*!<  百分号*/
LED_TICKET_PRICE,	/*!<  票价*/
LED_STATION,		/*!<  站号*/
LED_TOTAL_PRICE,	/*!<  总额*/
LED_TOTAL_SUM,		/*!<  总次图标*/
LED_BALANCE,		/*!<  余额图标*/
LED_TIME,			/*!<  时间图标*/
LED_COLON1,			/*!<  左冒号*/
LED_COLON2,			/*!<  右冒号*/
}TM_LED;

typedef enum{
LIGHT_BLACK  = 0, 
LIGHT_RED	 = 1,
LIGHT_GREEN  = 2,
LIGHT_YELLOW = 3,
LIGHT_BLUE	 = 4,
LIGHT_PURPLE = 5,
}LIGHT_COLOR;

typedef enum{
	CLEAR_LEVEL_DEFAULT = 0,
	CLEAR_LEVEL_0 = 0,
	CLEAR_LEVEL_1 = 1,
	CLEAR_LEVEL_2 = 2,
	CLEAR_LEVEL_3 = 3,	
}CLEAR_LEVEL;


/**
 * @brief 点亮多色led灯
 * @param [in]color 颜色. \sa LIGHT_COLOR
 *	LIGHT_BLACK 	= 0  黑色（熄灭）
 *	LIGHT_RED		= 1  红色
 *	LIGHT_GREEN 	= 2  绿色
 *	LIGHT_YELLOW	= 3  黄色
 *	LIGHT_BLUE		= 4  蓝色
 *	LIGHT_PURPLE	= 5  紫色
 * @return 0 true,other  false.
 */
int TM1680_DisplayLight(LIGHT_COLOR color);
/**
 * @brief 点亮多色GPS灯
 * @param [in]color 颜色.
 *	LIGHT_BLACK 	= 0  黑色（熄灭）
 *	LIGHT_RED		= 1  红色
 *	LIGHT_GREEN 	= 2  绿色
 *	LIGHT_ORANGE	= 3  黄色
 * @param color \sa LIGHT_COLOR
 */
int TM1680_DisplayGPS(LIGHT_COLOR color);

/**
 * @brief 图标控制
 * @param [in]led	 参考TM_LED
 * @param [in]level    
 *    LED_LIGHT    	0-5 led灯颜色不同
 *    LED_SIGNAL   	0-4 信号强度越强
 *    LED_WIFI 		0-4 信号强度越强
 *    other       	0:熄灭 1：点亮 
 * @return 0 true,other false.
 */
int TM1680_Control(TM_LED led,int level);
/**
 * @brief 数码管显示
 * @param [in]index  数码管指引（顺序从左到右，从上到下，共四组）
 * @param [in]str	 支持显示0-9、 A-Z和‘-’ 注 str 为null，灭灯包括冒号
 *   example："LOAD"
 *            "123.12"
 * @return 0 true,other false.
 * @note 字母只支持大写字母A-Z，小写按大写显示
 * @note str 默认右对齐显示
 */
int TM1680_DisplaySeg(int index ,char *str);
/**
 * @brief 时间显示
 * @param [in]str_time 时间字符串 
 * 		example "22:33"
 *		   		"11:22:33"
 * @return 0 true,other false.
 */
int TM1680_DisplayTime(char *str_time);

/**
 * @brief 显示百分比图标 00-99
 * @param[in] percent < 0 清除百分比图标
 *					 > 100 显示最大值99%
 * @return 0 true,other false.
 */
int TM1680_DisplayPercent(int percent);


/**
 * @brief 清屏函数
 * @param[in] level  0 全屏清除
 *					 1 不清图标行6个图标
 *					 2 不清图标行6个图标加百分号
 *  				 3 不清图标行6个图标加百分号加汉字灯（只清除两行数码管）
 * @return 0 true,other false.
 */
int TM1680_DisplayClear(int level, int reverse);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _TM1680_H_ */

