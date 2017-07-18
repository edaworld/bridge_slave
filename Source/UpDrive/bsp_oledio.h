#ifndef __OLED_H__
#define __OLED_H__

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/

#include "stm32f10x.h"
/***
 *SPI1硬件驱动SSD1306 OLED 
 *模块是4线SPI接口 片选已经硬件置低
 *D/C拉高代表输入数据 拉低代表输入命令
 *SDA:数据输入 SCL:时钟输入
 ****/
#define     RCC_APB2Periph_OLED_PORT        RCC_APB2Periph_GPIOA
#define     OLED_DC      PBout(1)
#define     OLED_RST     PBout(0)
#define     OLED_PORT    GPIOA

#define     OLED_SCL_PIN					GPIO_Pin_5
#define	    OLED_SDA_PIN					GPIO_Pin_7 

#define     XLevelL		    0x00
#define     XLevelH		    0x10
#define     XLevel		    ((XLevelH&0x0F)*16+XLevelL)
#define     Max_Column	    128
#define     Max_Row		    64
#define	    Brightness	    0xCF
#define     X_WIDTH         128
#define     Y_WIDTH         64
#define		  Page			8

void OLED_WB(uint8_t data);
/*********************LCD写数据************************************/ 
void OLED_WrDat(uint8_t data);
/*********************LCD写命令************************************/
void OLED_WrCmd(uint8_t cmd);
/*********************设定坐标************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y);
/*********************LCD全屏操作************************************/
void OLED_Fill(unsigned char bmp_dat);
/*********************LCD复位************************************/
void OLED_CLS(void);
/*********************LCD对应IO控制口初始化************************************/
void OLEDIO_Init(void);
/*********************LCD初始化************************************/
void OLED_Init(void);
/*********************延时函数***********************/
void LCD_DLY_ms(unsigned int ms);
/*****************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
/*****************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void LCD_P16x16Ch(unsigned char x,unsigned char y,unsigned char N);
/*****************功能描述：显示32*32点阵  显示的坐标（x,y），y为页范围0～7****************************/
void LCD_P32x32Ch(unsigned char x,unsigned char y,unsigned char N);
/*****************功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,unsigned char const BMP[]);

#endif

