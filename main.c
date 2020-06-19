/** main.c **/
#include "main.h" 
#include "uart_api.h"
#include "adc_api.h"
#include "gpio_api.h"

/*
引脚分配
P1_6 	RX
P1_7 	TX
P1_0 	AD1
P0_0 	RESET
P0_10 	SWCLK
P1_13	SWDIO
P2_0	ic串口rx
P2_1	ic串口tx
P2_2	网络模块串口rx
P2_3	网络模块串口tx
P2_4	闸门
P2_5	消毒1
P2_6	消毒2
P2_7	消毒3
P2_8	消毒4
*/




bool SOFTWARE_DEBUG = FALSE;
bool ICWORKING 		= TRUE;	//ic与网络模块切换标志位
bool uartDataReady 	= FALSE;
bool adcStart		= FALSE;

bool icOK			= FALSE;
bool tempOK			= FALSE;
bool cleanedOK		= FALSE;

uint8_t	uartRcvBuf[20] = {0};//防止数据出错 适当扩大长度
uint8_t uartDataLength = 0;
//串口中断
void UART_IRQHandler(void)
{
   if((LPC_UART->IIR & 0x01) == 0)	//是否有中断发生
   {
        switch (LPC_UART->IIR & 0x0E){             //中断标志位
        
            case 0x04:		//RDA
                for (uartDataLength = 0; uartDataLength < 8; uartDataLength++)
				{
					uartRcvBuf[uartDataLength] = LPC_UART->RBR;
                }
                break;
            
            case 0x0C:		//CTI
                while ((LPC_UART->LSR & 0x01) == 0x01) //判断数据是否接收完毕
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
	LPC_TMR16B1->IR      = 1;	//清标志位
}

void SystemInit(void)
{
	
}

uint8_t icTestArray[10] = {0x02,0x0a,0x02,0x2e,0x00,0xb6,0xd7,0xb5,0xf2,0x03};
int main(void)
{
//	底层配置部分
	GPIO_Init();
	ADC_Init();
	UART_Init();
	TIMER_Init();
	while(1)
	{
		//串口接收中断完成 处理接收数据
		if(uartDataReady)
		{
			if(ICWORKING)	//判断当前是哪个串口在工作
				icDataDeal();	//程序中断 第一步
			else
				msgDataDeal();
			uartDataReady = FALSE;
		}
		//软件调试 测试通讯协议
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

void TIMER_Init(void)	//采用定时器精准延迟
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);	//配置定时器时钟

	LPC_TMR16B1->PR      = 799;				//分频系数 8M/0.8k=10k
	LPC_TMR16B1->MCR     = 0x01;			//匹配后发生中断
	LPC_TMR16B1->MR0     = 10;				//10k/10=1kHz	1ms进一次中断	
	LPC_TMR16B1->TCR     = 0x01;			//开始计数
	LPC_TMR16B1->IR      = 1;				//清标志位
	NVIC_EnableIRQ(TIMER_16_1_IRQn); 		//设置中断向量表
	NVIC_SetPriority(TIMER_16_1_IRQn,0x01); //设置优先级
}

void myDelay(uint16_t ms) //8MHz下粗略延时 n ms
{
	timerCnt = ms;
	while(timerCnt);
}
void GPIO_Init(void)
{
	LPC_GPIO2->DIR |= 0x1ff;	//GPIO2 0-6配置GPIO输出
}

void UART_Init(void)
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); /* 打开IOCON 配置时钟 */
	LPC_IOCON->PIO1_6 |= 0x01; /* 将P1.6 1.7 配置为RXD TXD 引脚 */
	LPC_IOCON->PIO1_7 |= 0x01;
	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); /* 打开UART 功能部件时钟 */
	LPC_SYSCON->UARTCLKDIV = 0x01; /* UART 时钟分频 */
	LPC_UART->LCR = 0x83; /* 允许设置波特率 配置DLAB=1*/
	usFdiv = (48000000/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS; /* 设置波特率 */
	LPC_UART->DLM = usFdiv / 256;
	LPC_UART->DLL = usFdiv % 256;
	LPC_UART->LCR = 0x03; /* 锁定波特率 配置DLAB=0*/
	LPC_UART->FCR = 0x87;	//设置8字节触发
	
	NVIC_EnableIRQ(UART_IRQn); // 开串口中断
	NVIC_SetPriority(UART_IRQn,0x01); //设置优先级
	LPC_UART->IER = 0x01;  // 开启RDA中断
}

void ADC_Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);	//IO配置时钟
	LPC_IOCON->R_PIO1_0 = 0x02;				//引脚连接到AD1
	
	LPC_SYSCON->PDRUNCFG &= ~(1<<4);			//配置不下拉
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);	//配置AD时钟
	
	LPC_ADC->CR = (1<<1)|
					(((8000000/1000000) - 1) << 8)|
					(0<<16)|
					(0<<17)|
					(0<<24)|
					(0<<27);
}




