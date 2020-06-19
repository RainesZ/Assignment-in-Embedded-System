/** main.c **/
#include "main.h" 
#include "uart_api.h"
#include "adc_api.h"
#include "gpio_api.h"

/*
���ŷ���
P1_6 	RX
P1_7 	TX
P1_0 	AD1
P0_0 	RESET
P0_10 	SWCLK
P1_13	SWDIO
P2_0	ic����rx
P2_1	ic����tx
P2_2	����ģ�鴮��rx
P2_3	����ģ�鴮��tx
P2_4	բ��
P2_5	����1
P2_6	����2
P2_7	����3
P2_8	����4
*/




bool SOFTWARE_DEBUG = FALSE;
bool ICWORKING 		= TRUE;	//ic������ģ���л���־λ
bool uartDataReady 	= FALSE;
bool adcStart		= FALSE;

bool icOK			= FALSE;
bool tempOK			= FALSE;
bool cleanedOK		= FALSE;

uint8_t	uartRcvBuf[20] = {0};//��ֹ���ݳ��� �ʵ����󳤶�
uint8_t uartDataLength = 0;
//�����ж�
void UART_IRQHandler(void)
{
   if((LPC_UART->IIR & 0x01) == 0)	//�Ƿ����жϷ���
   {
        switch (LPC_UART->IIR & 0x0E){             //�жϱ�־λ
        
            case 0x04:		//RDA
                for (uartDataLength = 0; uartDataLength < 8; uartDataLength++)
				{
					uartRcvBuf[uartDataLength] = LPC_UART->RBR;
                }
                break;
            
            case 0x0C:		//CTI
                while ((LPC_UART->LSR & 0x01) == 0x01) //�ж������Ƿ�������
				{          
                    uartRcvBuf[uartDataLength] = LPC_UART->RBR;
                    uartDataLength++;
                }
				uartDataReady = TRUE;
                break;
               
            default:
                break;
        }
    }
}
uint16_t timerCnt = 0;
void TIMER16_1_IRQHandler(void)
{
	if(timerCnt)
		timerCnt--;
	LPC_TMR16B1->IR      = 1;	//���־λ
}

void SystemInit(void)
{
	
}

uint8_t icTestArray[10] = {0x02,0x0a,0x02,0x2e,0x00,0xb6,0xd7,0xb5,0xf2,0x03};
int main(void)
{
//	�ײ����ò���
	GPIO_Init();
	ADC_Init();
	UART_Init();
	TIMER_Init();
	while(1)
	{
		//���ڽ����ж���� �����������
		if(uartDataReady)
		{
			if(ICWORKING)	//�жϵ�ǰ���ĸ������ڹ���
				icDataDeal();	//�����ж� ��һ��
			else
				msgDataDeal();
			uartDataReady = FALSE;
		}
		//������� ����ͨѶЭ��
		if(SOFTWARE_DEBUG)
		{
//			sendADCData();
			uartDataLength = 10;
			for(uint8_t i=0;i<10;i++)
			{
				uartRcvBuf[i] = icTestArray[i];
			}
			icDataDeal();
			SOFTWARE_DEBUG = FALSE;
		}
		if(adcStart)
		{
			adcDataDeal();
			adcStart = FALSE;
		}
		if(cleanedOK&&cleanedOK&&tempOK)
		{
			gateOpen();
			myDelay(3000);
			gateClose();
			cleanedOK = FALSE;
			cleanedOK = FALSE;
			tempOK = FALSE;
		}
	}
}

void TIMER_Init(void)	//���ö�ʱ����׼�ӳ�
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);	//���ö�ʱ��ʱ��

	LPC_TMR16B1->PR      = 799;				//��Ƶϵ�� 8M/0.8k=10k
	LPC_TMR16B1->MCR     = 0x01;			//ƥ������ж�
	LPC_TMR16B1->MR0     = 10;				//10k/10=1kHz	1ms��һ���ж�	
	LPC_TMR16B1->TCR     = 0x01;			//��ʼ����
	LPC_TMR16B1->IR      = 1;				//���־λ
	NVIC_EnableIRQ(TIMER_16_1_IRQn); 		//�����ж�������
	NVIC_SetPriority(TIMER_16_1_IRQn,0x01); //�������ȼ�
}

void myDelay(uint16_t ms) //8MHz�´�����ʱ n ms
{
	timerCnt = ms;
	while(timerCnt);
}
void GPIO_Init(void)
{
	LPC_GPIO2->DIR |= 0x1ff;	//GPIO2 0-6����GPIO���
}

void UART_Init(void)
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); /* ��IOCON ����ʱ�� */
	LPC_IOCON->PIO1_6 |= 0x01; /* ��P1.6 1.7 ����ΪRXD TXD ���� */
	LPC_IOCON->PIO1_7 |= 0x01;
	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); /* ��UART ���ܲ���ʱ�� */
	LPC_SYSCON->UARTCLKDIV = 0x01; /* UART ʱ�ӷ�Ƶ */
	LPC_UART->LCR = 0x83; /* �������ò����� ����DLAB=1*/
	usFdiv = (48000000/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS; /* ���ò����� */
	LPC_UART->DLM = usFdiv / 256;
	LPC_UART->DLL = usFdiv % 256;
	LPC_UART->LCR = 0x03; /* ���������� ����DLAB=0*/
	LPC_UART->FCR = 0x87;	//����8�ֽڴ���
	
	NVIC_EnableIRQ(UART_IRQn); // �������ж�
	NVIC_SetPriority(UART_IRQn,0x01); //�������ȼ�
	LPC_UART->IER = 0x01;  // ����RDA�ж�
}

void ADC_Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);	//IO����ʱ��
	LPC_IOCON->R_PIO1_0 = 0x02;				//�������ӵ�AD1
	
	LPC_SYSCON->PDRUNCFG &= ~(1<<4);			//���ò�����
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);	//����ADʱ��
	
	LPC_ADC->CR = (1<<1)|
					(((8000000/1000000) - 1) << 8)|
					(0<<16)|
					(0<<17)|
					(0<<24)|
					(0<<27);
}




