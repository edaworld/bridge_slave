#ifndef  __TASK__H__
#define  __TASK__H__
#include "bsp_tpc.h"
#pragma anon_unions //ʹ�������ṹ��

/********************************************************************************************************
* �궨��
********************************************************************************************************/
#define KEY_KEY 	PAin(4) //PA4,KEYɨ�谴��
#define KEY_PWR		PCout(13) //PC13,PWR���ư���

typedef union
{
	uint8_t msg[8];
	struct
	{
		uint8_t head; //��ͷ
		uint8_t devID; //�豸id
		uint8_t Heartdata; //����ֵ
		uint8_t HrtPowerdata; //���ʴ�����
		uint8_t BatPowerdata; //��ص���
		uint8_t tail; //��β
	};
} SLVMSG_T; //�ӻ���Ϣ�ṹ��

typedef union
{
	uint8_t msg[12];
	struct
	{
		uint8_t head; //��ͷ
        uint8_t flag; //����־λ
        uint8_t reserve1; //����־λ
        uint8_t reserve2; //����־λ
		int32_t data; //�豸id
		uint8_t tail; //��β
        uint8_t reserve3; //����־λ
        uint8_t reserve4; //����־λ
        uint8_t reserve5; //����־λ        
	};
} SLAVEDATA; //�ӻ���Ϣ�ṹ��

/********************************************************************************************************
* ��������
********************************************************************************************************/
extern TPC_TASK TaskComps[];

/********************************************************************************************************
* �ڲ�����
********************************************************************************************************/
static void Task_RecvfromUart(void);  // �㲥����
static void Task_LEDDisplay(void);  //LED��˸����
static void Task_SendToHost(void); //����������PC��
static void Task_RecvfromHost(void); //��Lora��SX1278�������ӵĴ���2��ȡ��������
static void Task_KeyScan(void); //ɨ��һ��������������
static void Task_PowerCtl(void); //���ƹػ�����
static void Task_ADCProcess(void); //ADC�ɼ�������ѹ�����õ�������
static void Task_ReadAD5933(void); //��AD5933��ȡ���迹��������
/********************************************************************************************************
* ȫ�ֺ���
********************************************************************************************************/
extern void TaskInit(void); // ��ʼ��
extern void bsp_KeyScan(void);
/*******************************************************************************************************/
#endif
