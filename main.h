/** main.h **/
#ifndef __MAIN_H
#define __MAIN_H
#include "LPC11XX.h"

#define UART_BPS 9600

#define ioICUartRxOn 	LPC_GPIO2->DATA|=1<<0
#define ioNetUartRxOn	LPC_GPIO2->DATA|=1<<1
#define ioGateOn		LPC_GPIO2->DATA|=1<<2
#define ioCleaner1On	LPC_GPIO2->DATA|=1<<3
#define ioCleaner2On	LPC_GPIO2->DATA|=1<<4
#define ioCleaner3On	LPC_GPIO2->DATA|=1<<5
#define ioCleaner4On	LPC_GPIO2->DATA|=1<<6

#define ioICUartRxOff 	LPC_GPIO2->DATA&=~(1<<0)
#define ioNetUartRxOff	LPC_GPIO2->DATA&=~(1<<1)
#define ioGateOff		LPC_GPIO2->DATA&=~(1<<2)
#define ioCleaner1Off	LPC_GPIO2->DATA&=~(1<<3)
#define ioCleaner2Off	LPC_GPIO2->DATA&=~(1<<4)
#define ioCleaner3Off	LPC_GPIO2->DATA&=~(1<<5)
#define ioCleaner4Off	LPC_GPIO2->DATA&=~(1<<6)

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

extern bool SOFTWARE_DEBUG;
extern bool ICWORKING;
extern bool uartDataReady;
extern bool adcStart;

extern bool icOK;
extern bool tempOK;
extern bool cleanedOK;

extern uint8_t uartDataLength;
extern uint8_t	uartRcvBuf[20];
void GPIO_Init(void);
void UART_Init(void);
void ADC_Init(void);
void TIMER_Init(void);
void myDelay(uint16_t ms);
#endif
