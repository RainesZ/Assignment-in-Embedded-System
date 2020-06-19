/** uart_api.h **/
#ifndef __UART_API_H
#define __UART_API_H
#include "main.h"

typedef struct
{
	uint8_t ID;
	uint8_t Length;
	uint8_t Data[5];
}ICDATA;
extern ICDATA user001;
#define ISIDVALID(ID) 	(((ID) == EM4100)||\
						 ((ID) == MIFARE1K)||\
						 ((ID) == MIFARE4K)||\
						 ((ID) == HID)||\
						 ((ID) == T5567)||\
						 ((ID) == G2NDCARD)||\
						 ((ID) == ISO14443B)||\
						 ((ID) == FELICA)||\
						 ((ID) == LABEL15693)||\
						 ((ID) == CPUCADR)||\
						 ((ID) == SECTORDATA)||\
						 ((ID) == KEYDATA))
//卡片类型定义
#define EM4100 		0x02
#define MIFARE1K 	0x01
#define MIFARE4K 	0x03
#define HID			0x10
#define T5567		0x11
#define G2NDCARD	0x20
#define ISO14443B	0x21
#define FELICA		0x22
#define LABEL15693	0x30
#define CPUCADR		0x50
#define SECTORDATA	0x51
#define KEYDATA		0xff

uint8_t uartGetByte (void);
void uartSendByte (uint8_t ucDat);
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum);
void icDataDeal(void);
void icFun(void);
void msgSend(uint8_t *msgPtr,uint32_t length);
void msgDataDeal(void);
#endif

