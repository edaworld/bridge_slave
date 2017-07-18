#include "bsp.h"

void bsp_pztMux_Init ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;/*定义一个GPIO_InitTypeDef类型的结构体*/
	RCC_APB2PeriphClockCmd ( PZTMUX_GPIO_CLK , ENABLE );/*开启相关的GPIO外设时钟*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*设置引脚速率为50MHz */
	
	GPIO_InitStructure.GPIO_Pin = PZTMUX_GPIO_EN1;/*选择要控制的GPIO引脚*/
	GPIO_Init ( PZTMUX_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/  
    
	GPIO_InitStructure.GPIO_Pin = PZTMUX_GPIO_RA1;/*选择要控制的GPIO引脚*/
	GPIO_Init ( PZTMUX_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/
    	
	GPIO_InitStructure.GPIO_Pin = PZTMUX_GPIO_RA2;/*选择要控制的GPIO引脚*/
	GPIO_Init ( PZTMUX_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/
    
	GPIO_InitStructure.GPIO_Pin = PZTMUX_GPIO_RA3;/*选择要控制的GPIO引脚*/
	GPIO_Init ( PZTMUX_GPIO_PORT, &GPIO_InitStructure ); /*调用库函数，初始化GPIO*/  
    
	GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_EN1 );
//	GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
//	GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
//	GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );    
}

void bsp_rfbMux_Init ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;/*定义一个GPIO_InitTypeDef类型的结构体*/
	RCC_APB2PeriphClockCmd ( RFB_GPIO_CLK , ENABLE );/*开启相关的GPIO外设时钟*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*设置引脚速率为50MHz */
	
	GPIO_InitStructure.GPIO_Pin = RFB_GPIO_EN2;/*选择要控制的GPIO引脚*/
	GPIO_Init ( RFB_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/  
    
	GPIO_InitStructure.GPIO_Pin = RFB_GPIO_RB1;/*选择要控制的GPIO引脚*/
	GPIO_Init ( RFB_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/
    
	GPIO_InitStructure.GPIO_Pin = RFB_GPIO_RB2;/*选择要控制的GPIO引脚*/
	GPIO_Init ( RFB_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/
    
	GPIO_InitStructure.GPIO_Pin = RFB_GPIO_RB3;/*选择要控制的GPIO引脚*/
	GPIO_Init ( RFB_GPIO_PORT, &GPIO_InitStructure );/*调用库函数，初始化GPIO*/    
    
	GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_EN2 );
//	GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
//	GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
//	GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );   
}
void    pztMuxSwitch(uint8_t portnum)
{
    switch(portnum)
    {
        case PZT1:
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT2:
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT3:
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT4:
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT5:
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT6:
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT7:
            GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;
        case PZT8:
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA1 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA2 );
            GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_RA3 );
            break;        
    }    
}

void    rfbMuxSwitch(uint8_t portnum)
{
    switch(portnum)
    {
        case RFB_100_1K:
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_1K_10K:
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_10K_50K:
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_50K_100K:
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_100K_500K:
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_500K_1M:
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_1M_10M:
            GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;
        case RFB_10M_50M:
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB1 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB2 );
            GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_RB3 );
            break;           
    }
}
