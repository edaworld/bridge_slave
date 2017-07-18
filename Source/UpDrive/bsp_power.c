#include "bsp.h"


//KEY���ӵ�PA4
//BAT_C���ӵ�PB6
//BAT_M���ӵ�PB5
//PWR���ӵ�PC13
/********************************************************************************************************
*   �� �� ��: bsp_POWER_Init
*   ����˵��: ��ʼ��һ�����ػ���·��IO��
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************/
void bsp_POWER_Init(void)
{
//	��ʼ��BAT_C ��BAT_MΪ���
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
//	��ʼ��KEY_PA4Ϊ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
       
//	��ʼ��PWR_PC13Ϊ���
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//��C��ʱ�ӣ�����ʱ�ӡ�
    GPIOC->CRH &= 0X000FFFFF;	//PC13,14,15���ó���� 2MHz �������   
    GPIOC->CRH |= 0X22200000; 
    PWR->CR    |= 1<<8;	//ȡ��������д���� 
    RCC->BDCR  &= 0xFFFFFFFE;	//�ⲿ���������ر� C14��PC15��Ϊ��ͨIO	 	
    BKP->CR    &= 0xFFFFFFFE; 	//������TAMPER������Ϊͨ��IO��ʹ�� 
    PWR->CR    &= 0xFFFFFEFF;	//������д����  

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

