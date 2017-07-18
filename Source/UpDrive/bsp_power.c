#include "bsp.h"


//KEY连接到PA4
//BAT_C连接到PB6
//BAT_M连接到PB5
//PWR连接到PC13
/********************************************************************************************************
*   函 数 名: bsp_POWER_Init
*   功能说明: 初始化一键开关机电路的IO口
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************/
void bsp_POWER_Init(void)
{
//	初始化BAT_C 和BAT_M为输出
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
//	初始化KEY_PA4为输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
       
//	初始化PWR_PC13为输出
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//开C口时钟，复用时钟。
    GPIOC->CRH &= 0X000FFFFF;	//PC13,14,15设置成输出 2MHz 推挽输出   
    GPIOC->CRH |= 0X22200000; 
    PWR->CR    |= 1<<8;	//取消备份区写保护 
    RCC->BDCR  &= 0xFFFFFFFE;	//外部低俗振荡器关闭 C14，PC15成为普通IO	 	
    BKP->CR    &= 0xFFFFFFFE; 	//侵入检测TAMPER引脚作为通用IO口使用 
    PWR->CR    &= 0xFFFFFEFF;	//备份区写保护  

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

