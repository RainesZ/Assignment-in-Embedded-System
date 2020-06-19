/** gpio.c **/
#include "gpio_api.h"

//CD4066оƬ���ƶ� �ߵ�ƽ��ͨ �͵�ƽ�ض�
void icUartStart(void)
{
	ICWORKING = TRUE;
	ioICUartRxOn;
	ioNetUartRxOff;
}

void netUartStart(void)
{
	ICWORKING = FALSE;
	ioICUartRxOff;
	ioNetUartRxOn;
}

void cleanStop(void)
{
	ioCleaner1On;
	ioCleaner2On;
	ioCleaner3On;
	ioCleaner4On;
}

void cleanEnd(void)
{
	ioCleaner1Off;
	ioCleaner2Off;
	ioCleaner3Off;
	ioCleaner4Off;
}

//����բ�ſ��� ����͵�ƽ�ر� �ߵ�ƽ����
void gateOpen(void)
{
	ioGateOn;
}

void gateClose(void)
{
	ioGateOff;
}



