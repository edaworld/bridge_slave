#ifndef  __BSPPOWER_H
#define  __BSPPOWER_H
#include "stm32f10x.h" // Device header

/****************************����ΪHeartbagV2.0************************************/
//KEY���ӵ�PA4
//BAT_C���ӵ�PB6
//BAT_M���ӵ�PB5
//PWR���ӵ�PC13
#define     GPIO_KEY    PAin(4)
#define     GPIO_BAT_C  PBin(6)
#define     GPIO_BAT_M  PBin(5)
#define     GPIO_PWR    PCout(13)

//#define		LedRedOn()      GPIO_ResetBits(LED_PORT1,LEDUp_PIN)	
//#define		LedRedOff()     GPIO_SetBits(LED_PORT1,LEDUp_PIN)	
//#define		LedGreenOn()        GPIO_ResetBits(LED_PORT1,LEDDown_PIN)
//#define		LedGreenOff()       GPIO_SetBits(LED_PORT1,LEDDown_PIN)

void        bsp_POWER_Init(void);

#endif

