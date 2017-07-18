#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "bsp.h"

void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void *_pCallBack); //使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器，互不干扰。

void bsp_HardTimerInit(void);//配置TIMx，用于us级别硬件定时。TIMx将自由运行，永不停止.

#endif
