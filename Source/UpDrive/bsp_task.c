#include "bsp.h"

#define NULL 0
//extern uint8_t g_uart1_timeout; //��⴮��1�������ݳ�ʱ��ȫ�ֱ�������bsp_slavemsg.c�ļ�������
//extern uint8_t g_uart2_timeout; //��⴮��2�������ݳ�ʱ��ȫ�ֱ���
//extern RECVDATA_T g_tUart1; //��ʼ���Ӵ���1��BLE�������ݽṹ�壬��bsp_slavemsg.c�ļ�������
//extern RECVDATA_T g_tUart2; //��ʼ���Ӵ���2��Lora�������ݽṹ�壬��bsp_slavemsg.c�ļ�������

extern uint8_t TPCTaskNum; //������������bsp_task.c�б���ʼ����bsp_tpc.c��ʹ��
extern uint8_t IsEnterIRQ;  //��main.c�м��lora��IRQ���ţ���״̬�仯��Ϊ���յ�lora���ݣ���λ
uint8_t IsReceiveHostData = FALSE; //�������㲥��������ȷ�����õı�־λ
SLAVEDATA s_SlaveData; //�ӻ����͵������Ľṹ��

//uint8_t BlEisReady = FALSE; //���봮�ڣ�ͬʱ���յ�����ȷ�����ݣ���־λ
//SLVMSG_T s_tSlaMsg; //STM32���ʹӻ����ݵĽṹ��
//uint8_t KeyScan(void); //����״̬���İ���ɨ�躯��

/************************����ṹ��˵��*************************************/
/**
    typedef struct _TPC_TASK
    {
	uint8_t   attrb;  //��̬����0����̬����1
	uint8_t   Run;  // �������б�ǣ�0�������У�1������xternt
	uint16_t  Timer;  // ��ʱ��
	uint16_t  ItvTime;  // �������м��ʱ��
	void      (*Task)(void); // Ҫ���е�������
    } TPC_TASK; // ������
**/
/************************����ṹ��˵��*************************************/
TPC_TASK TaskComps[4] =
{
	//����������ʱ����ע�ⵥ�������иı��������ԵĴ���
	{ 0, 0, 10, 1000, Task_LEDDisplay }, // ��̬����LED��˸����ʱ��Ƭ���Ｔ��ִ��
	{ 0, 0, 1, 5, Task_RecvfromHost }, // ��̬���񣬴�����SPI�ӿڵ�SX127 8���յ���������ʱ��Ƭ���Ｔ��ִ��
	{ 1, 0, 1, 5, Task_SendToHost }, // ��̬�����յ��㲥�źţ����ʹӻ����ݵ�����


    { 0, 0, 1, 1000, Task_ReadAD5933 }, // ��ȡAD5933����
//    { 0, 0, 1, 10, Task_KeyScan }, // ����ɨ������
//    { 0, 0, 2, 8, Task_PowerCtl }, // ����ɨ������
//    { 0, 0, 3, 10, Task_ADCProcess} //�ɼ���ص�������
};

/*********************************************************************************************************
    �� �� ��: TaskInit
    ����˵��: �����ʼ��
    ��    ��: ��
    �� �� ֵ: ��
*********************************************************************************************************/
void TaskInit ( void )
{
	TPCTaskNum = ( sizeof ( TaskComps ) / sizeof ( TaskComps[0] ) ); // ��ȡ������
}
/*********************************************************************************************************
    �� �� ��: Task_LEDDisplay
    ����˵��: LED��˸����
*********************************************************************************************************/
void Task_LEDDisplay ( void )
{
    LED1_TOGGLE(); //��ɫD3
	LCD_P6x8Str ( 0, 1, "LED1_TOGGLE" );
}
/*********************************************************************************************************
    �� �� ��: Task_ReadAD5933
    ����˵��: ��ȡAD5933���迹��������
*********************************************************************************************************/
static unsigned char temp;
static unsigned int real, img;
static uint8_t printbuffer3[32];
static uint8_t readAD5933count;
void Task_ReadAD5933 ( void )
{

	AD5933_Set_Mode_Freq_Start();
  
	if ( ( AD5933_Get_DFT_ST() & 0x04 ) != 0x04 )
	{

		while ( 1 ) //wait for DFT finish
		{
			temp = AD5933_Get_DFT_ST();
			if ( temp & 0x02 ) //ʵ�����鲿��Ч
			{ break; }
		}      
		real = AD5933_Get_Real();
		img  = AD5933_Get_Img();
		AD5933_Set_Mode_Freq_UP();
        readAD5933count ++;
        sprintf ( printbuffer3, "count is :%d", readAD5933count );
        LCD_P6x8Str ( 0, 4, printbuffer3 );
        sprintf ( printbuffer3, "real is :%d", real );
        LCD_P6x8Str ( 0, 5, printbuffer3 );        
        sprintf ( printbuffer3, "img is :%d", img );
        LCD_P6x8Str ( 0, 6, printbuffer3 );          
	}
}

/*********************************************************************************************************
    �� �� ��: Task_RecvfromLora
    ����˵��: ������SPI�ӿڵ�Lora���յ���������
*********************************************************************************************************/

static uint8_t recvdatbuffer[32];   //�������ݻ�����
static uint8_t printbuffer1[32]; //�������ݴ�ӡ������
static uint8_t ucrevcount;
void Task_RecvfromHost ( void )
{
	uint8_t length;
	if ( IsEnterIRQ  == TRUE ) //�������м�⵽�ж�����Ϊ�ߣ���ʾ���յ����ݺ���λ�ñ�־λ
	{
//		OLEDPrint(0, 2, "RF Received");
//		sprintf(sendBuf, "��������:%d", bsp_GetRunTime());
//		OLEDPrint(0, 2, sendBuf);
//		printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��
//		��������������ʱ8ms
		length = RFM96_LoRaRxPacket ( recvdatbuffer );
//        RFM96_LoRaRxPacket(recvdatbuffer);
		RFRxMode();
//		��������������ʱ8ms
//		printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��
		if ( length > 0 )
		{
			ucrevcount++;
			sprintf ( printbuffer1, "rcvcnt is :%d", ucrevcount );
		}
//		OLEDPrint(0, 2, recvprintbuffer);
//		printf("receive data length is %d\n", length);
//		COMx_SendBuf(COM1, recvdatbuffer, 6);
		if ( ( recvdatbuffer[0] == '%' ) && ( recvdatbuffer[8] == '&' ) )
		{
			IsReceiveHostData = TRUE;   //���ý��յ�����������־λ
			TaskComps[2].attrb = 0; //���ڵ㷢����������Ϊ��̬����
			LCD_P6x8Str ( 0, 2, printbuffer1 );
		}
		IsEnterIRQ = FALSE;
	}
}
static uint8_t printbuffer2[32]; //�������ݴ�ӡ������
/*********************************************************************************************************
    �� �� ��: Task_SendToMaster
    ����˵��: �������ݰ�����������
*********************************************************************************************************/
void Task_SendToHost ( void )
{
	if ( IsReceiveHostData == TRUE ) //���յ��������͹������ź�
	{
		
		if ( recvdatbuffer[1] == 0xEF ) //����ָ��
		{
			s_SlaveData.msg[1] = 0xEF;
			s_SlaveData.msg[2] = 0x01;
		}
        if( recvdatbuffer[1] == 0x7F) //������ʼƵ�ʡ���ֹƵ�ʡ�����������PZT���迹����
        {
            memcpy(s_SlaveData.msg,recvdatbuffer,12);
            
            AD5933_Set_Freq_Start(s_SlaveData.msg[2]);
            AD5933_Set_Freq_Num(s_SlaveData.data);
            AD5933_Set_Freq_Add((s_SlaveData.msg[3]-s_SlaveData.msg[2])*1000/s_SlaveData.data);
            pztMuxSwitch(s_SlaveData.msg[9]);
            rfbMuxSwitch(s_SlaveData.msg[10]);
                       
			s_SlaveData.msg[1] = 0x7F;
			s_SlaveData.msg[2] = 0x01;             
        }
        if(recvdatbuffer[1] == 0x3F)//���õ�Ƶ����ָ��
        {
            AD5933_Set_Mode_Freq_Repeat(); 
			s_SlaveData.msg[1] = 0x3F;
			s_SlaveData.msg[2] = 0x01;            
        }
        if(recvdatbuffer[1] == 0x1F)//����ɨƵ����ָ��
        {
            AD5933_Set_Mode_Freq_UP();
			s_SlaveData.msg[1] = 0x1F;
			s_SlaveData.msg[2] = 0x01;            
        }
        if(recvdatbuffer[1] == 0x0F)
        {
			s_SlaveData.msg[1] = 0x0F;
			s_SlaveData.msg[2] = 0x01;
            TaskComps[3].attrb = 0;
        }
        
		s_SlaveData.head = '&';
		s_SlaveData.tail = '%';                
		RFSendData ( s_SlaveData.msg, 12 ); //���͸ýڵ�����
		mem_set ( s_SlaveData.msg, 0, 12 ); //������Ϻ󽫽ṹ����������
		TaskComps[2].attrb = 1; //�����ͽڵ�������������Ϊ��̬���񣬵ȴ��ٴν��յ��㲥�ź�
		IsReceiveHostData = FALSE;
	}
	LED1_TOGGLE(); //��ɫD3
}

/*********************************************************************************************************
    �� �� ��: Task_ADCProcess
    ����˵��: �ɼ���ص�������,10ms����һ��
*********************************************************************************************************/
//void Task_ADCProcess(void)
//{
//	AdcPro(); //ͨ��adc�ɼ���ص���
//}

/*********************************************************************************************************
    �� �� ��: Task_RecvfromUart
    ����˵��: ������uart1�ӿڽ��յ���CC2541���͹�������������
*********************************************************************************************************/
//void Task_RecvfromUart(void)
//{

////����3.5���ַ�ʱ���ִ��Uart1_RxTimeOut������ȫ�ֱ��� g_uart1_timeout = 1; ֪ͨ������ʼ����
//	if (g_uart1_timeout == 0)
//	{
//		return; // û�г�ʱ���������ա���Ҫ���� g_tUart1.RxCount
//	}
//	if (g_tUart1.RxCount < 5)    // ���յ�������С��3���ֽھ���Ϊ����
//	{
//		return;
//	}
////    printf("%d",g_tUart1.RxCount); //���Խ������ݸ����Ƿ���ȷ
//	g_uart1_timeout = 0; // ��ʱ���־
////    printf("\t%d\n",bsp_GetRunTime());//���Գ�ʱʱ��
//	if ((g_tUart1.RxBuf[0] != '$') && (g_tUart1.RxBuf[4] != '#')) //������ݰ�ͷ�Ƿ���ȷ
//	{
//		printf("error in head!");
//	}
//    else if (g_tUart1.RxBuf[1] == 'P' || g_tUart1.RxBuf[1] == 'H') //������ݰ��Ƿ���ȷ
//	{
////���ݰ�������ȷ
//		s_tSlaMsg.Heartdata = g_tUart1.RxBuf[2];//������ֵ
//		s_tSlaMsg.HrtPowerdata = g_tUart1.RxBuf[3];//���ʴ���ص���
//        BlEisReady = TRUE;
//	}
//    else
//	{
////		s_tSlaMsg.Heartdata = 0;
////		s_tSlaMsg.HrtPowerdata = 0;
//	}
//	g_tUart1.RxCount = 0; // ��������������������´�֡ͬ��
//}

/*********************************************************************************************************
    �� �� ��: Task_KeyScan
    ����˵��: ����ɨ������
*********************************************************************************************************/
//static uint32_t KeyStatus = 0;//�洢����״̬����
//void Task_KeyScan(void)
//{
//    KeyStatus = KeyScan();
//}
///*********************************************************************************************************
//*   �� �� ��: Task_PowerCtl
//*   ����˵��: PWR���Ƶ�Դ���ţ����Ƶ�Դ�رյ�����
//*********************************************************************************************************/
//void Task_PowerCtl(void)
//{
//    if(KeyStatus == 2)
//    {
//        KEY_PWR =  0;//�رյ�Դ
//    }
//}
/*********************************************************************************************************
    �� �� ��: KeyScan
    ����˵��: ����ɨ����룬�����ӵ�PA4���ţ������°�������3���ӵ�ʱ��
*********************************************************************************************************/
//uint8_t KeyScan(void)
//{
//	static uint8_t ucKeyStatus = 0; //��ʾ����״̬��0��ʾδ���£�1��ʾ���£�2��ʾ������3��ʾ����
//	static uint32_t uiKeyInput = 0; //�����ȡ�İ�����ֵ����������ʱΪ�ߵ�ƽ
//	static uint32_t uiCount = 0; //�������������������뿪����Ӧ����100��
//	uint8_t key_return = 0; //��������ֵ
//	uiKeyInput = KEY_KEY;

//	switch (ucKeyStatus)
//	{
//	case 0: //����ǳ�ʼ̬�����޶���
//		if (uiKeyInput == 0)
//		{
//			ucKeyStatus = 1;
//		} else
//		{
//			ucKeyStatus = 0;
//		}
//		break;
//	case 1: //�����ǰ�Ǳ�����
//		if (uiKeyInput == 0) //������ڻ�������
//		{
//			ucKeyStatus = 2; //�л�����ʱ̬
//			uiCount = 0;
//		} else
//		{
//			ucKeyStatus = 0;
//		}
//		break;
//	case 2: //����Ѿ��л���״̬2
//		if (uiKeyInput == 0) //������ڻ�������
//		{
//			ucKeyStatus = 2;
//			uiCount++;
//			key_return = 1; //����1,һ����������ͨ�������������������飬˵���Ѿ���2������10ms���жϣ������Ѿ�����
//							//��ô��ʱ��⵽�������ͷţ�˵����һ����ͨ�̰�
//			if (uiCount > 300) //����ʱ�����3s
//			{
//				uiCount = 0;
//				key_return = 2; //�������
//			}
//		} else //����Ѿ�����
//		{
//			ucKeyStatus = 3;
//		}
//		break;
//	case 3: //����Ѿ��л���״̬3
//		if (uiKeyInput == 1)//����Ѿ�����
//		{
//			ucKeyStatus = 0;
//			uiCount = 0;
//		}
//		break;
//	default:
//		ucKeyStatus = 0;
//		uiCount = 0;
//	}
//	return key_return;
//}