/** adc_api.c **/
#include "adc_api.h"
#include "uart_api.h"
#include "stdio.h"
#include "string.h"
#include "gpio_api.h"

char msgToSend[20];
float temperature = 0.0f;
void adcDataDeal(void)
{
	uint32_t i;
	uint32_t ulADCData;
	uint32_t ulADCBuf;
	
	ulADCData = 0;
	for (i = 0; i < 10; i++) {
		LPC_ADC->CR |= (1 << 24); /* 立即转换 */
		while((LPC_ADC->DR[1] & 0x80000000) == 0); /* 读取AD0DR0 的Done */
		ulADCBuf = LPC_ADC->DR[1]; /* 读取结果寄存器 */
		ulADCBuf = (ulADCBuf >> 6) & 0x3ff;
		ulADCData += ulADCBuf;
	}
	ulADCData = ulADCData / 10; /* 采样10 次进行虑波处理 */
	temperature = (ulADCData * 5000) / 1023.0f/10.0f; /* 计算温度值 */
	sendADCData();	//发送信息
	
	if((ulADCData > 36)&&(ulADCData < 37.3))	//温度 超过37.3℃&&低于36℃ 不正常
	{
		tempOK = TRUE;	//温度测试通过
		myDelay(500);	//0.5s后开启消毒
		cleanStop();
		myDelay(1500);	//消毒1.5s
		cleanStop();
		cleanedOK = TRUE;
	}
	else
	{
		tempOK = FALSE;
		icOK   = FALSE;
	}
}

void sendADCData(void)	//编辑用户函数
{
	uint8_t tmpLength = 0;
	if(temperature < 10)
		tmpLength = 5;
	else if(temperature < 100)
		tmpLength = 6;
	else
		tmpLength = 7;
    sprintf(&msgToSend[0], " %.2f" , temperature);	//浮点型 温度 转字符串
	memcpy(&msgToSend[tmpLength],&user001.ID,7);
	tmpLength+=7;
	msgSend((uint8_t *)msgToSend[0],tmpLength);	//网络模块发送数据
}

