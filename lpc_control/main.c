#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer32.h"
#include "i2c.h"
#include "w25x10.h"
#include "adc.h"
// #include "timer16.h"
#include "lpc_control.h"
#include "stepmotor.h"

extern volatile uint32_t	UARTCount;
extern volatile uint8_t         UARTBuffer[BUFSIZE];
int main (void) 
{
	SystemInit();
	init_timer32(0,0);
	GPIOInit();
	InitStep();
	UARTInit(9600);
	I2CInit( (uint32_t)I2CMASTER );
	W25X10_Init();
	ADCInit(ADC_CLK);
	GPIOSetDir(PORT2,8,1);
	GPIOSetDir(PORT2,9,1);
	GPIOSetDir(PORT2,10,1);
	GPIOSetDir(PORT2,11,1);
	GPIOSetDir(PORT1,5,1);
	GPIOSetDir(PORT1,8,1);
	GPIOSetDir(PORT1,9,1);
	GPIOSetDir(PORT1,11,1);
	GPIOSetValue(PORT2,8,1); 
	GPIOSetValue(PORT2,9,1); 
	GPIOSetValue(PORT2,10,1);
	GPIOSetValue(PORT2,11,1);
	GPIOSetValue(PORT1,5,1);
	GPIOSetValue(PORT1,8,1);
	GPIOSetValue(PORT1,9,1);
	GPIOSetValue(PORT1,11,1);
	while (1)
	{
		delay32Ms(0,50); // !!!!!
		wm_op();
		if ( UARTCount != 0 )
		{
			LPC_UART->IER = IER_THRE | IER_RLS;          
			UARTSend( (uint8_t *)UARTBuffer, UARTCount );
			memset(UARTBuffer,0,UARTCount);
			UARTCount = 0;
			LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;
		}
	}
}
