#ifndef  __BSP_MUX_H
#define  __BSP_MUX_H
#include "stm32f10x.h" // Device header



#define PZTMUX_GPIO_PORT    	GPIOC			              
#define PZTMUX_GPIO_CLK 	    RCC_APB2Periph_GPIOC		
#define PZTMUX_GPIO_EN1		    GPIO_Pin_6			        
#define PZTMUX_GPIO_RA1		    GPIO_Pin_3	
#define PZTMUX_GPIO_RA2		    GPIO_Pin_4	
#define PZTMUX_GPIO_RA3		    GPIO_Pin_5	




#define RFB_GPIO_PORT    	    GPIOC			             
#define RFB_GPIO_CLK 	        RCC_APB2Periph_GPIOC
#define RFB_GPIO_EN2		    GPIO_Pin_7			        
#define RFB_GPIO_RB1		    GPIO_Pin_0	
#define RFB_GPIO_RB2		    GPIO_Pin_1	
#define RFB_GPIO_RB3		    GPIO_Pin_2	




//以下定义反馈电阻对应序号
#define RFB_100_1K          0
#define RFB_1K_10K          1
#define RFB_10K_50K         2
#define RFB_50K_100K        3
#define RFB_100K_500K       4
#define RFB_500K_1M         5
#define RFB_1M_10M          6
#define RFB_10M_50M         7
//以下定义接入端口号
#define PZT1        0
#define PZT2        1
#define PZT3        2
#define PZT4        3
#define PZT5        4
#define PZT6        5
#define PZT7        6
#define PZT8        7

void    bsp_pztMux_Init(void); //所对应的外部接入口初始化函数
void    bsp_rfbMux_Init(void); //所对应的反馈电阻接入口初始化函数
void    pztMuxSwitch(uint8_t portnum);
void    rfbMuxSwitch(uint8_t portnum);


#endif

