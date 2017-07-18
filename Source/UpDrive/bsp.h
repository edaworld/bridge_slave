/**********************************************************************************************************
*
*   模块名称 : BSP模块
*   文件名称 : bsp.h
*   说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*             不需要#include 每个模块的 h 文件
*
*
*********************************************************************************************************/

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V4
/* 检查是否定义了开发板型号 */
#if !defined (STM32_V4) && !defined (STM32_X2)
#error "Please define the board model : STM32_X2 or STM32_V4"
#endif

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION       "1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()        bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()    __set_PRIMASK(0)    /* 使能全局中断 */
#define DISABLE_INT()   __set_PRIMASK(1)    /* 禁止全局中断 */

#define BSP_Printf printf


#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_uartfifo.h"
#include "bsp_led.h"
#include "bsp_systimer.h"
#include "bsp_spi1.h"
#include "bsp_spi2.h"
#include "bsp_oledio.h"
#include "bsp_userlib.h"
#include "bsp_tpc.h"
#include "bsp_task.h"
#include "bsp_uartpro.h"
#include "bsp_timer.h"
#include "bsp_sx1276-LoRa.h"
#include "bsp_rf.h"
#include "bsp_power.h"
#include "bsp_adc.h"
#include "bsp_i2c_ee.h"
#include "bsp_ad5933.h"
#include "bsp_mux.h"

//位带操作,实现51类似的GPIO控制功能,IO口操作宏定义
#define BITBAND(addr, bitnum)   ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)          *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void bsp_RunPer1ms(void);
void bsp_RunPer10ms(void);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
