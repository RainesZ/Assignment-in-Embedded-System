/** uart_api.c **/
#include "uart_api.h"
#include "gpio_api.h"

//网络模块固定传输的字符串
const uint8_t AT_sendmsg[11] = "AT+SMS=0,3,";
const uint8_t PhoneNumber[11] = "188xxxxxxxx";



uint8_t uartGetByte (void)
{
	uint8_t ucRcvData;
	while ((LPC_UART->LSR & 0x01) == 0); /* 等待接收标志置位 */
	ucRcvData = LPC_UART->RBR; /* 读取数据 */
	return (ucRcvData);
}

void uartSendByte (uint8_t ucDat)
{
	LPC_UART->THR = ucDat; /* 写入数据 */
	while ((LPC_UART->LSR & 0x40) == 0); /* 等待数据发送完毕 */
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
	uint8_t BBCdata = 0;//开始全是0 
	icOK   = FALSE;	//初始化 防止出错
	for(uint8_t i=0;i<uartDataLength;i++)	   //循环次数和bufbuf长度一致
	{
		usData = uartRcvBuf[i];
		switch(RX_Status)	//根据状态判断数据解析进度
		{
			case RX_HEAD:
				if(usData == 0x02)	//接收到数据头
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
				if(ISIDVALID(usData))	//判断是否ID有效
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
				if(i<icDataLength-2)	//总长度去除最后两位非数据
				{
					BBCdata ^= usData;
					user001.Data[i-3] = usData;	//去除前三位非数据
				}
				else	//数据提取完毕
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
				if(usData == 0x03)	//检查数据尾
				{
					icFun();		//数据检查通过
				}
				RX_Status = RX_HEAD;
				break;
			default:			
				RX_Status = RX_HEAD;				
				break;
		}
	}
}

void icFun(void)	//编辑用户函数
{
	icOK = TRUE;		//ic检测通过
	adcStart = TRUE;	//开始检测温度
}


void msgSend(uint8_t *msgPtr,uint32_t length)	//向用户发送数据
{
	uartSendStr(&AT_sendmsg[0],11);
	uartSendStr(&PhoneNumber[0],11);
	uartSendStr(msgPtr,length);
}

void msgDataDeal(void)	//处理网络模块返回数据 作业未要求
{
	
}

