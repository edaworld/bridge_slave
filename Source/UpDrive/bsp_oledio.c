#include "bsp.h"
#include "OLed_Table.h"

void OLED_WB ( uint8_t data )
{
	/* Loop while DR register in not emplty */
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET );
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData ( SPI1, data );
	bsp_DelayUS ( 2 );
}
/*******************一个字节数据写入***********************/
void OLED_WrDat ( uint8_t data )
{
	OLED_DC = 1;
	OLED_WB ( data );
}

/********************一条指令写入**********************/
void OLED_WrCmd ( uint8_t cmd )
{
	OLED_DC = 0;
	OLED_WB ( cmd );
}

/**********************设置显示位置**********************/
void OLED_Set_Pos ( uint8_t x, uint8_t y )
{
	/* Page addressing mode */
	OLED_WrCmd ( 0xb0 + ( y & 0x07 ) ); /* set page start address */
	OLED_WrCmd ( x & 0x0f ); /* set lower nibble of the column address */
	OLED_WrCmd ( ( ( x & 0xf0 ) >> 4 ) | 0x10 ); /* set higher nibble of the column address */
}

/**********************写满屏数据**********************/
void OLED_Fill ( unsigned char bmp_dat )
{
	unsigned char y, x;
	OLED_WrCmd ( 0x20 ); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd ( 0x00 ); //
	OLED_WrCmd ( 0x21 ); //-Set Column Address
	OLED_WrCmd ( 0x00 );
	OLED_WrCmd ( 0x7f );
	OLED_WrCmd ( 0x22 ); //-Set Page Address
	OLED_WrCmd ( 0x00 );
	OLED_WrCmd ( 0x07 );
	LCD_DLY_ms ( 1 ); /* 等待内部稳定   */
	for ( y = 0; y < Page; y++ )
	{
		for ( x = 0; x < X_WIDTH; x++ )
		{
			OLED_WrDat ( bmp_dat );
		}
	}
//    LCD_WrCmd(0xaf);//--turn off oled panel
}

/*********************清屏函数***********************/
void OLED_CLS ( void )
{
	unsigned char y, x;
	for ( y = 0; y < 8; y++ )
	{
		OLED_WrCmd ( 0xb0 + y );
		OLED_WrCmd ( 0x01 );
		OLED_WrCmd ( 0x10 );
		for ( x = 0; x < X_WIDTH; x++ )
		{ OLED_WrDat ( 0 ); }
		LCD_DLY_ms ( 200 );
	}
}

/*********************延时函数***********************/
void LCD_DLY_ms ( unsigned int ms )
{
	unsigned int a;
	while ( ms )
	{
		a = 1335;
		while ( a-- );
		ms--;
	}
	return;
}

/*****************************************************************************
    函 数 名  : OLEDIO_Init
    功能描述  : OLED的IO初始化
    输入参数  : void
    输出参数  : NONE
    返 回 值  : NONE
*****************************************************************************/
void OLEDIO_Init ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
}


/*********************12864初始化***********************/
void OLED_Init ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
	OLED_RST = 0;
	LCD_DLY_ms ( 50 );
	OLED_RST = 1;
	//从上电到下面开始初始化要有足够的时间，即等待RC复位完毕
	OLED_WrCmd ( 0xae ); //--turn off oled panel
	OLED_WrCmd ( 0xa8 ); //--set multiplex ratio(1 to 64)
	OLED_WrCmd ( 0x3f ); //--1/64 duty
	OLED_WrCmd ( 0xd3 ); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd ( 0x00 ); //-not offset
	OLED_WrCmd ( 0x40 ); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd ( 0xa1 ); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd ( 0xc8 ); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd ( 0xda ); //--set com pins hardware configuration
	OLED_WrCmd ( 0x12 );
	OLED_WrCmd ( 0x81 ); //--set contrast control register
	OLED_WrCmd ( 0xcf ); // Set SEG Output Current Brightness
	OLED_WrCmd ( 0xa4 ); // Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd ( 0xa6 ); // Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd ( 0xd5 ); //--set display clock divide ratio/oscillator frequency
	OLED_WrCmd ( 0x80 ); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd ( 0x8d ); //--set Charge Pump enable/disable
	OLED_WrCmd ( 0x14 ); //--set(0x10) disable
	OLED_WrCmd ( 0xaf ); //--turn on oled panel
	OLED_WrCmd ( 0xd9 ); //--set pre-charge period
	OLED_WrCmd ( 0xf8 ); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd ( 0xdb ); //--set vcomh
	OLED_WrCmd ( 0x40 ); //Set VCOM Deselect Level
	OLED_Fill ( 0x00 ); //初始清屏
}
/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
//12864像素点，能显示21列，8行
void LCD_P6x8Str ( unsigned char x, unsigned char y, unsigned char ch[] )
{
	unsigned char c = 0, i = 0, j = 0;
	while ( ch[j] != '\0' )
	{
		c = ch[j] - 32;
		if ( x > 126 ) {x = 0; y++;}
		OLED_Set_Pos ( x, y );
		for ( i = 0; i < 6; i++ )
		{ OLED_WrDat ( F6x8[c][i] ); }
		x += 6;
		j++;
	}
}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y）****************/
//12864像素点，y的取值为0，2，4，6才能正常显示
void LCD_P8x16Str ( unsigned char x, unsigned char y, unsigned char ch[] )
{
	unsigned char c = 0, i = 0, j = 0;
	while ( ch[j] != '\0' )
	{
		c = ch[j] - 32;
		if ( x > 120 ) {x = 0; y++;}
		OLED_Set_Pos ( x, y );
		for ( i = 0; i < 8; i++ )
		{ OLED_WrDat ( F8X16[c * 16 + i] ); }
		OLED_Set_Pos ( x, y + 1 );
		for ( i = 0; i < 8; i++ )
		{ OLED_WrDat ( F8X16[c * 16 + i + 8] ); }
		x += 8;
		j++;
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void LCD_P16x16Ch ( unsigned char x, unsigned char y, unsigned char N )
{
	unsigned char wm = 0;
	unsigned int adder = 32 * N; //每个字模的起始地址
	OLED_Set_Pos ( x , y );
	for ( wm = 0; wm < 16; wm++ ) //
	{
		OLED_WrDat ( F16x16[adder] );
		adder += 1;
	}      									 //上半屏
	OLED_Set_Pos ( x, y + 1 );
	for ( wm = 0; wm < 16; wm++ ) //
	{
		OLED_WrDat ( F16x16[adder] );
		adder += 1;
	} 	  									 //下半屏
}
/*****************功能描述：显示32*32点阵  显示的坐标（x,y），y为页范围0～7****************************/
void LCD_P32x32Ch ( unsigned char x, unsigned char y, unsigned char N )
{
	unsigned char wm = 0;
	unsigned int adder = 128 * N; //
	OLED_Set_Pos ( x , y );
	for ( wm = 0; wm < 32; wm++ ) //
	{
		OLED_WrDat ( F32x32[adder] );
		adder += 1;
	}
	OLED_Set_Pos ( x, y + 1 );
	for ( wm = 0; wm < 32; wm++ ) //
	{
		OLED_WrDat ( F32x32[adder] );
		adder += 1;
	}
	OLED_Set_Pos ( x, y + 2 );
	for ( wm = 0; wm < 32; wm++ ) //
	{
		OLED_WrDat ( F32x32[adder] );
		adder += 1;
	}
	OLED_Set_Pos ( x, y + 3 );
	for ( wm = 0; wm < 32; wm++ ) //
	{
		OLED_WrDat ( F32x32[adder] );
		adder += 1;
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP ( unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char const BMP[] )
{
	unsigned int j = 0;
	unsigned char x, y;
	if ( y1 % 8 == 0 ) { y = y1 / 8; }
	else { y = y1 / 8 + 1; }
	for ( y = y0; y < y1; y++ )
	{
		OLED_Set_Pos ( x0, y );
		for ( x = x0; x < x1; x++ )
		{
			OLED_WrDat ( BMP[j++] );
		}
	}
}







