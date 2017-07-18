#include "bsp.h"

#define SAMP_COUNT	20		/* 样本个数，表示200ms内的采样数据求平均值 */

uint16_t g_usAdcValue;	/* ADC 采样值的平均值 */
/*********************************************************************************************************
*	函 数 名: ADC_Configuration
*	功能说明: 配置ADC, PC4作为ADC通道输入
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

    /* 使能 ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);//使能ADC1时钟

	/* 配置PA4为模拟输入(ADC Channel0) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置ADC1, 不用DMA, 用软件触发 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;//规定进行规则转换的ADC通道的数目。这个数目的取值范围是1到16
	ADC_Init(ADC1, &ADC_InitStructure);

	/* 配置ADC1 规则通道14 channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

	/* 使能ADC1 DMA功能 */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 使能ADC1 复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/* 检查ADC1的复位寄存器 */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 启动ADC1校准 */
	ADC_StartCalibration(ADC1);
	/* 检查校准是否结束 */
	while(ADC_GetCalibrationStatus(ADC1));

	/* 软件启动ADC转换 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**********************************************************************************************************
*	函 数 名: AdcPro
*	功能说明: ADC采样处理，插入1ms systick 中断进行调用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
void AdcPro(void)
{
	static uint16_t buf[SAMP_COUNT];
	static uint8_t write;
	uint32_t sum;
	uint8_t i;

	buf[write] = ADC_GetConversionValue(ADC1);
	if (++write >= SAMP_COUNT)
	{
		write = 0;
	}

	/* 下面这段代码采用求平均值的方法进行滤波
		也可以改善下，选择去掉最大和最下2个值，使数据更加精确
	*/
	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += buf[i];
	}
	g_usAdcValue = sum / SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* 软件启动下次ADC转换 */
}
/*
*********************************************************************************************************
*	函 数 名: GetADC
*	功能说明: 读取ADC采样的平均值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* 因为	g_AdcValue 变量在systick中断中改写，为了避免主程序读变量时被中断程序打乱导致数据错误，因此需要
	关闭中断进行保护 */

	/* 进行临界区保护，关闭中断 */
	__set_PRIMASK(1);  /* 关中断 */

	ret = g_usAdcValue;

	__set_PRIMASK(0);  /* 开中断 */

	return ret;
}



