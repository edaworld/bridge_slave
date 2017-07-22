#include "bsp.h"

#define NULL 0
//extern uint8_t g_uart1_timeout; //检测串口1接收数据超时的全局变量，在bsp_slavemsg.c文件中声明
//extern uint8_t g_uart2_timeout; //检测串口2接收数据超时的全局变量
//extern RECVDATA_T g_tUart1; //初始化从串口1，BLE接收数据结构体，在bsp_slavemsg.c文件中声明
//extern RECVDATA_T g_tUart2; //初始化从串口2，Lora接收数据结构体，在bsp_slavemsg.c文件中声明

extern uint8_t TPCTaskNum; //任务数量，在bsp_task.c中被初始化，bsp_tpc.c中使用
extern uint8_t IsEnterIRQ;  //在main.c中监测lora的IRQ引脚，有状态变化即为接收到lora数据，置位
uint8_t IsReceiveHostData = FALSE; //从主机广播包解析正确，设置的标志位
SLAVEDATA s_SlaveData; //从机发送到主机的结构体
READVALUE s_ReadData; //发送实部和虚部的结构体
/************************任务结构体说明*************************************/
/**
    typedef struct _TPC_TASK
    {
	uint8_t   attrb;  //静态任务：0，动态任务：1
	uint8_t   Run;  // 程序运行标记，0：不运行，1：运行xternt
	uint16_t  Timer;  // 计时器
	uint16_t  ItvTime;  // 任务运行间隔时间
	void      (*Task)(void); // 要运行的任务函数
    } TPC_TASK; // 任务定义
**/
/************************任务结构体说明*************************************/
TPC_TASK TaskComps[4] =
{
	//添加新任务时，请注意单个任务中改变任务属性的代码
	{ 0, 0, 10, 1000, Task_LEDDisplay }, // 静态任务，LED闪烁任务，时间片到达即可执行
	{ 0, 0, 1, 1, Task_RecvfromHost }, // 静态任务，处理从SPI接口的SX127 8接收的数据任务，时间片到达即可执行
	{ 1, 0, 1, 1, Task_SendToHost }, // 动态任务，收到广播信号，发送从机数据到主机
	{ 1, 0, 1, 50, Task_ReadAD5933 }, // 读取AD5933任务
//    { 0, 0, 1, 10, Task_KeyScan }, // 按键扫描任务
//    { 0, 0, 2, 8, Task_PowerCtl }, // 按键扫描任务
//    { 0, 0, 3, 10, Task_ADCProcess} //采集电池电量任务
};

/*********************************************************************************************************
    函 数 名: TaskInit
    功能说明: 任务初始化
    形    参: 无
    返 回 值: 无
*********************************************************************************************************/
void TaskInit ( void )
{
	TPCTaskNum = ( sizeof ( TaskComps ) / sizeof ( TaskComps[0] ) ); // 获取任务数
}
/*********************************************************************************************************
    函 数 名: Task_LEDDisplay
    功能说明: LED闪烁代码
*********************************************************************************************************/
static char printbuffer1[32];
static uint64_t ledtoggletimes;
void Task_LEDDisplay ( void )
{
	LED1_TOGGLE(); //蓝色D3
	ledtoggletimes ++;
	sprintf ( printbuffer1, "toggle times %d", ledtoggletimes );
	LCD_P6x8Str ( 0, 0, printbuffer1 );
}
/*********************************************************************************************************
    函 数 名: Task_ReadAD5933
    功能说明: 读取AD5933电阻抗数据任务
*********************************************************************************************************/
static unsigned char temp;
static unsigned int real, img;
static char printbuffer2[32];
static uint32_t readAD5933count;
void Task_ReadAD5933 ( void )
{
	DISABLE_INT();
	temp = AD5933_Get_DFT_ST();
	ENABLE_INT();
	if ( ( temp & 0x04 ) != 0x04 )
	{
		if ( temp & 0x02 ) //实部和虚部有效
		{
            s_ReadData.head = '&';
            s_ReadData.tail = '%'; 
            s_ReadData.msg[1] = 0x07;
			s_ReadData.msg[2] = 0x01; 
            
			real = AD5933_Get_Real();
 			img  = AD5933_Get_Img();           
            s_ReadData.real = real;
            s_ReadData.img = img;
            
            RFSendData(s_ReadData.msg, 12);
			AD5933_Set_Mode_Freq_UP();
			readAD5933count ++;
			sprintf ( printbuffer2, "read5933 times %d", readAD5933count );
			LCD_P6x8Str ( 0, 1, printbuffer2 );
			sprintf ( printbuffer2, "real is :%X", real );
			LCD_P6x8Str ( 0, 5, printbuffer2 );
			sprintf ( printbuffer2, "img is :%X", img );
			LCD_P6x8Str ( 0, 6, printbuffer2 );
		}
	}
	else if ( ( temp & 0x04 ) == 0x04 )
	{
		TaskComps[3].attrb = 1;
	}
}

/*********************************************************************************************************
    函 数 名: Task_RecvfromLora
    功能说明: 处理从SPI接口的Lora接收的数据任务
*********************************************************************************************************/
static uint8_t recvdatbuffer[32];   //接收数据缓冲区
static char printbuffer3[32]; //接收数据打印缓冲区
static uint8_t ucrevcount;
void Task_RecvfromHost ( void )
{
	uint8_t length;
	if ( IsEnterIRQ  == TRUE ) //主函数中检测到中断引脚为高，表示接收到数据后，置位该标志位
	{
		length = RFM96_LoRaRxPacket ( recvdatbuffer );
		RFRxMode();
		if ( length > 0 )
		{
			ucrevcount++;
		}
		if ( ( recvdatbuffer[0] == '%' ) && ( recvdatbuffer[8] == '&' ) )
		{
			IsReceiveHostData = TRUE;   //设置接收到的主机包标志位
			TaskComps[2].attrb = 0; //将节点发送任务设置为静态任务
			sprintf ( printbuffer3, "rcvhost times %d", ucrevcount );            
			LCD_P6x8Str ( 0, 2, printbuffer3 );
		}
		IsEnterIRQ = FALSE;
	}
}
static char printbuffer4[32]; //接收数据打印缓冲区
/*********************************************************************************************************
    函 数 名: Task_SendToMaster
    功能说明: 发送数据包至主机任务
*********************************************************************************************************/
void Task_SendToHost ( void )
{
	if ( IsReceiveHostData == TRUE ) //接收到主机发送过来的信号
	{
		if ( recvdatbuffer[1] == 0xEF ) //握手指令
		{
			s_SlaveData.msg[1] = 0xEF;
			s_SlaveData.msg[2] = 0x01;
		}
		if ( recvdatbuffer[1] == 0x7F ) //设置起始频率、截止频率、测量点数、PZT和阻抗量程
		{
			memcpy ( s_SlaveData.msg, recvdatbuffer, 12 );
			AD5933_Set_Freq_Start ( s_SlaveData.msg[2] );
			AD5933_Set_Freq_Num ( s_SlaveData.data );
//			sprintf ( printbuffer4, "data is :%d", s_SlaveData.data );            
//			LCD_P6x8Str ( 0, 3, printbuffer4 );            
			AD5933_Set_Freq_Add ( ( s_SlaveData.msg[3] - s_SlaveData.msg[2] ) * 1000 / s_SlaveData.data );
			pztMuxSwitch ( s_SlaveData.msg[9] );
			rfbMuxSwitch ( s_SlaveData.msg[10] );
			s_SlaveData.msg[1] = 0x7F;
			s_SlaveData.msg[2] = 0x01;
		}
		if ( recvdatbuffer[1] == 0x3F ) //设置单频测量指令
		{
			AD5933_Set_Mode_Freq_Repeat();
			s_SlaveData.msg[1] = 0x3F;
			s_SlaveData.msg[2] = 0x01;
		}
		if ( recvdatbuffer[1] == 0x1F ) //设置扫频测量指令
		{
			AD5933_Set_Mode_Freq_UP();
			s_SlaveData.msg[1] = 0x1F;
			s_SlaveData.msg[2] = 0x01;
		}
		if ( recvdatbuffer[1] == 0x0F )//启动监测指令
		{
			s_SlaveData.msg[1] = 0x0F;
			s_SlaveData.msg[2] = 0x01;
			Init_AD5933();
			AD5933_Set_Mode_Freq_Start();
//			OLED_CLS();
			TaskComps[3].attrb = 0;
		}
		s_SlaveData.head = '&';
		s_SlaveData.tail = '%';
		RFSendData ( s_SlaveData.msg, 12 ); //发送该节点数据
		mem_set ( s_SlaveData.msg, 0, 12 ); //发送完毕后将结构体数据清零
		TaskComps[2].attrb = 1; //将发送节点数据任务设置为动态任务，等待再次接收到广播信号
		IsReceiveHostData = FALSE;
	}
	LED1_TOGGLE(); //蓝色D3
}

/*********************************************************************************************************
    函 数 名: Task_ADCProcess
    功能说明: 采集电池电量任务,10ms调用一次
*********************************************************************************************************/
//void Task_ADCProcess(void)
//{
//	AdcPro(); //通过adc采集电池电量
//}

/*********************************************************************************************************
    函 数 名: Task_RecvfromUart
    功能说明: 处理从uart1接口接收到的CC2541发送过来的数据任务
*********************************************************************************************************/
//void Task_RecvfromUart(void)
//{

////超过3.5个字符时间后执行Uart1_RxTimeOut函数。全局变量 g_uart1_timeout = 1; 通知主程序开始解码
//	if (g_uart1_timeout == 0)
//	{
//		return; // 没有超时，继续接收。不要清零 g_tUart1.RxCount
//	}
//	if (g_tUart1.RxCount < 5)    // 接收到的数据小于3个字节就认为错误
//	{
//		return;
//	}
////    printf("%d",g_tUart1.RxCount); //测试接收数据个数是否正确
//	g_uart1_timeout = 0; // 超时清标志
////    printf("\t%d\n",bsp_GetRunTime());//测试超时时间
//	if ((g_tUart1.RxBuf[0] != '$') && (g_tUart1.RxBuf[4] != '#')) //检测数据包头是否正确
//	{
//		printf("error in head!");
//	}
//    else if (g_tUart1.RxBuf[1] == 'P' || g_tUart1.RxBuf[1] == 'H') //检测数据包是否都正确
//	{
////数据包接收正确
//		s_tSlaMsg.Heartdata = g_tUart1.RxBuf[2];//心率数值
//		s_tSlaMsg.HrtPowerdata = g_tUart1.RxBuf[3];//心率带电池电量
//        BlEisReady = TRUE;
//	}
//    else
//	{
////		s_tSlaMsg.Heartdata = 0;
////		s_tSlaMsg.HrtPowerdata = 0;
//	}
//	g_tUart1.RxCount = 0; // 必须清零计数器，方便下次帧同步
//}

/*********************************************************************************************************
    函 数 名: Task_KeyScan
    功能说明: 按键扫描任务
*********************************************************************************************************/
//static uint32_t KeyStatus = 0;//存储按键状态变量
//void Task_KeyScan(void)
//{
//    KeyStatus = KeyScan();
//}
///*********************************************************************************************************
//*   函 数 名: Task_PowerCtl
//*   功能说明: PWR控制电源引脚，控制电源关闭的任务
//*********************************************************************************************************/
//void Task_PowerCtl(void)
//{
//    if(KeyStatus == 2)
//    {
//        KEY_PWR =  0;//关闭电源
//    }
//}
/*********************************************************************************************************
    函 数 名: KeyScan
    功能说明: 按键扫描代码，按键接到PA4引脚，当按下按键超过3秒钟的时候
*********************************************************************************************************/
//uint8_t KeyScan(void)
//{
//	static uint8_t ucKeyStatus = 0; //表示按键状态，0表示未按下，1表示按下，2表示长按，3表示弹起
//	static uint32_t uiKeyInput = 0; //保存读取的按键键值，按键按下时为高电平
//	static uint32_t uiCount = 0; //长按计数器，长按三秒开机，应计数100次
//	uint8_t key_return = 0; //函数返回值
//	uiKeyInput = KEY_KEY;

//	switch (ucKeyStatus)
//	{
//	case 0: //如果是初始态，即无动作
//		if (uiKeyInput == 0)
//		{
//			ucKeyStatus = 1;
//		} else
//		{
//			ucKeyStatus = 0;
//		}
//		break;
//	case 1: //如果先前是被按着
//		if (uiKeyInput == 0) //如果现在还被按着
//		{
//			ucKeyStatus = 2; //切换到计时态
//			uiCount = 0;
//		} else
//		{
//			ucKeyStatus = 0;
//		}
//		break;
//	case 2: //如果已经切换到状态2
//		if (uiKeyInput == 0) //如果现在还被按着
//		{
//			ucKeyStatus = 2;
//			uiCount++;
//			key_return = 1; //返回1,一次完整的普通按键，程序进入这个语句块，说明已经有2次以上10ms的中断，等于已经消抖
//							//那么此时检测到按键被释放，说明是一次普通短按
//			if (uiCount > 300) //按键时间大于3s
//			{
//				uiCount = 0;
//				key_return = 2; //长按情况
//			}
//		} else //如果已经弹起
//		{
//			ucKeyStatus = 3;
//		}
//		break;
//	case 3: //如果已经切换到状态3
//		if (uiKeyInput == 1)//如果已经弹起
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
