/*********************************************************************************************************
    ģ������ : RFģ��(For STM32F1XX)
    �ļ����� : bsp_rf.c
    ˵    �� :
*********************************************************************************************************/
#include "bsp.h"
const char *rfName = "SX1278";
u16	iSend, iRev;    //���߷��ͺͽ��ռ���
u8	sendBuf[64];    //���ͻ�����
u8	revBuf[128];    //���ջ�����
//��ʼ��SX1278���ĸ�IO��
void RFGPIOInit ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure PA8 RF_CE as Output push-pull -------------------------------*/
	//��������ԭ��ͼ����NREST����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	/* Configure PB12 RF_Csn as Output push-pull -------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
	/* Configure PB10 RF_IRQ as input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	/* Configure PB11 RF_SDN as Output push-pull -------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//��Ƶģ���ʼ��
void RFInit ( void )
{
	SPI2_Init();
	RFM96_LoRaEntryRx(); //�������ģʽ
}

//��Ƶģ��������ģʽ
void RFRxMode ( void )
{
	RFM96_LoRaEntryRx(); //�������ģʽ
}

//��Ƶģ���������
u8 RFRevData ( u8 *buf )
{
	u8 length = 0;
	if ( GPIO_ReadInputDataBit ( GPIOA, RF_IRQ_PIN ) ) //�յ����ݸߵ�ƽ�ж�
	{
		length = RFM96_LoRaRxPacket ( revBuf );
		RFRxMode();
	}
	if ( length > 0 )
	{
		iRev++;
	} //�������ݸ���
	return ( length );
}
//��Ƶģ�鷢������
u8 RFSendData ( u8 *buf, u8 size )
{
	int ret = 0;
	ret = RFM96_LoRaEntryTx ( size ); //���ط����ֽ���
	ret = RFM96_LoRaTxPacket ( buf, size ); //���ط����ֽ���
	bsp_DelayMS ( 5 );
	RFRxMode(); //�������ģʽ
	if ( ret > 0 )
	{
		iSend++;
	}    //�������ݸ���
	return ( ret ); //�ɹ������0��ֵ
}


