#include "bsp.h"  // Device header

uint8_t IsEnterIRQ = FALSE;//�յ����ݱ�־λ��

int main(void)
{
	bsp_Init();  //��ʼ��Ӳ���豸   
    bsp_DelayMS(10);
    TaskInit(); //��ʼ������,���ǻ�ȡ�������������������bsp_idle�е���task_processʵ��    

	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOA, RF_IRQ_PIN)) //SPI��SX1278�����յ�����ʱ��IRQ����Ϊ��
		{
			IsEnterIRQ = TRUE;
		}
		bsp_Idle();       
	}
}
