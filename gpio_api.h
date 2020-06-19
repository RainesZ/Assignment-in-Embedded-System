/** gpio.h **/
#ifndef __GPIO_API_H
#define __GPIO_API_H
#include "main.h"

void icUartStart(void);
void netUartStart(void);
void cleanStop(void);
void cleanEnd(void);
void gateOpen(void);
void gateClose(void);
#endif

