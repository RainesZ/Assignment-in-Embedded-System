/** uart_api.c **/
#include "uart_api.h"
#include "gpio_api.h"

//����ģ��̶�������ַ���
const uint8_t AT_sendmsg[11] = "AT+SMS=0,3,";
const uint8_t PhoneNumber[11] = "188xxxxxxxx";



uint8_t uartGetByte (void)
{
	uint8_t ucRcvData;
	while ((LPC_UART->LSR & 0x01) == 0); /* �ȴ����ձ�־��λ */
	ucRcvData = LPC_UART->RBR; /* ��ȡ���� */
	return (ucRcvData);
}

void uartSendByte (uint8_t ucDat)
{
	LPC_UART->THR = ucDat; /* д������ */
	while ((LPC_UART->LSR & 0x40) == 0); /* �ȴ����ݷ������ */
}
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++) 
	{
        uartSendByte (*pucStr++);
    }
}

#define RX_HEAD           	0
#define RX_LENGTH 		    1
#define RX_ID             	2
#define RX_DATA            	3
#define RX_TAIL             4

ICDATA user001 = {0};
void icDataDeal(void)
{
	static uint8_t RX_Status =	RX_HEAD;
	uint8_t usData;
	uint8_t icDataLength = 0;
	uint8_t BBCdata = 0;//��ʼȫ��0 
	icOK   = FALSE;	//��ʼ�� ��ֹ����
	for(uint8_t i=0;i<uartDataLength;i++)	   //ѭ��������bufbuf����һ��
	{
		usData = uartRcvBuf[i];
		switch(RX_Status)	//����״̬�ж����ݽ�������
		{
			case RX_HEAD:
				if(usData == 0x02)	//���յ�����ͷ
				{
					icDataLength=0;
					BBCdata = 0;
					RX_Status = RX_LENGTH;
				}
				else
				{
					RX_Status = RX_HEAD;
				}
			break;
			case RX_LENGTH:
				user001.Length = icDataLength = usData;
				BBCdata ^= usData;
				RX_Status = RX_ID;
				break;
			case RX_ID:
				if(ISIDVALID(usData))	//�ж��Ƿ�ID��Ч
				{
					RX_Status = RX_DATA;
					BBCdata ^= usData;
					user001.ID = usData;
				}
				else
				{
					RX_Status = RX_HEAD;
				}
				break;
			case RX_DATA:
				if(i<icDataLength-2)	//�ܳ���ȥ�������λ������
				{
					BBCdata ^= usData;
					user001.Data[i-3] = usData;	//ȥ��ǰ��λ������
				}
				else	//������ȡ���
				{
					if(usData == BBCdata)
					{
						RX_Status = RX_TAIL;
					}
					else
					{
						RX_Status = RX_HEAD;
					}
				}
				break;
			case RX_TAIL:
				if(usData == 0x03)	//�������β
				{
					icFun();		//���ݼ��ͨ��
				}
				RX_Status = RX_HEAD;
				break;
			default:			
				RX_Status = RX_HEAD;				
				break;
		}
	}
}

void icFun(void)	//�༭�û�����
{
	icOK = TRUE;		//ic���ͨ��
	adcStart = TRUE;	//��ʼ����¶�
}


void msgSend(uint8_t *msgPtr,uint32_t length)	//���û���������
{
	uartSendStr(&AT_sendmsg[0],11);
	uartSendStr(&PhoneNumber[0],11);
	uartSendStr(msgPtr,length);
}

void msgDataDeal(void)	//��������ģ�鷵������ ��ҵδҪ��
{
	
}

