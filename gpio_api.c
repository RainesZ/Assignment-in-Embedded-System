/** gpio.c **/
#include "gpio_api.h"

//CD4066芯片控制端 高电平导通 低电平关断
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

//控制闸门开关 假设低电平关闭 高电平开启
void gateOpen(void)
{
	ioGateOn;
}

void gateClose(void)
{
	ioGateOff;
}



