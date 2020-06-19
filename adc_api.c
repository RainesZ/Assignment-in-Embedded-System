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
		LPC_ADC->CR |= (1 << 24); /* ����ת�� */
		while((LPC_ADC->DR[1] & 0x80000000) == 0); /* ��ȡAD0DR0 ��Done */
		ulADCBuf = LPC_ADC->DR[1]; /* ��ȡ����Ĵ��� */
		ulADCBuf = (ulADCBuf >> 6) & 0x3ff;
		ulADCData += ulADCBuf;
	}
	ulADCData = ulADCData / 10; /* ����10 �ν����ǲ����� */
	temperature = (ulADCData * 5000) / 1023.0f/10.0f; /* �����¶�ֵ */
	sendADCData();	//������Ϣ
	
	if((ulADCData > 36)&&(ulADCData < 37.3))	//�¶� ����37.3��&&����36�� ������
	{
		tempOK = TRUE;	//�¶Ȳ���ͨ��
		myDelay(500);	//0.5s��������
		cleanStop();
		myDelay(1500);	//����1.5s
		cleanStop();
		cleanedOK = TRUE;
	}
	else
	{
		tempOK = FALSE;
		icOK   = FALSE;
	}
}

void sendADCData(void)	//�༭�û�����
{
	uint8_t tmpLength = 0;
	if(temperature < 10)
		tmpLength = 5;
	else if(temperature < 100)
		tmpLength = 6;
	else
		tmpLength = 7;
    sprintf(&msgToSend[0], " %.2f" , temperature);	//������ �¶� ת�ַ���
	memcpy(&msgToSend[tmpLength],&user001.ID,7);
	tmpLength+=7;
	msgSend((uint8_t *)msgToSend[0],tmpLength);	//����ģ�鷢������
}

