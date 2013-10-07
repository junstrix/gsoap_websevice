#include <stdlib.h>
#include <string.h>
#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer32.h"
#include "i2c.h"
#include "w25x10.h"
#include "ssp.h"
#include "adc.h"
#include "type.h"
//#include "timer16.h"
#include "stepmotor.h"
#include "lpc_control.h"
unsigned int motor_flags = 0;
uint16_t temp_value = 0x00;
uint8_t spidWriteBuf[10]="1001130718"; // define SPI ID
extern volatile uint32_t I2CCount;                     // I2C Êý¾Ý¼ÆÊý
extern volatile uint8_t  I2CMasterBuffer[I2C_BUFSIZE]; // I2C Ö÷»ú×´Ì¬ÏÂµÄÊý¾Ý»º³åÇø
extern volatile uint8_t  I2CSlaveBuffer[I2C_BUFSIZE];  // I2C ´Ó»ú×´Ì¬ÏÂµÄÊý¾Ý»º³åÇø
extern volatile uint32_t I2CMasterState;               // I2C Ö÷»ú×´Ì¬
extern volatile uint32_t I2CReadLength, I2CWriteLength;// I2C ¶ÁÐ´Êý¾Ý³¤¶È±äÁ¿
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];
extern volatile uint32_t ADCValue[ADC_NUM];                                 // ADC²ÉÑùÊý¾Ý
extern volatile uint32_t ADCIntDone;
#define NUM_ADC 1
void wm_op()
{
	switch(UARTBuffer[1])
	{
		case 'A': // read adc value
			read_adc();
			break;	
		case 'T': // read lm75 temperature
			read_temp();
			break;
		case 'S': // read spi id value
			read_spid();
			break;
		case 'L': // control led
			con_led();
			break;
		case 'M': // control motor
			con_motor();
			break;
		default:
			break;
	}
	switch (motor_flags) { // motor control
		case 1 : StepmotorMovePitch(StepMotor_1,MOTOR_CW); break;	
		case 2 : StepmotorMovePitch(StepMotor_1,MOTOR_CCW); break;	
		case 3 : StepmotorMovePitch(StepMotor_1,MOTOR_STOP); break;	
		default : break;
	}
}
void read_adc(void)
{ // TODO not implement yeah.
	uint8_t *ptmp = NULL;
	ptmp = (uint8_t *)UARTBuffer+sizeof(uint8_t);
#if BURST_MODE// ½öµ±ÖÐ¶Ï·½Ê½ÏÂ¿ÉÓÃ
	ADCBurstRead();
	while ( !ADCIntDone );
	ADCIntDone = 0;
#else

#if ADC_INTERRUPT_FLAG // ÖÐ¶Ï·½Ê½²ÉÑù				
	ADCRead( NUM_ADC );
	while ( !ADCIntDone );
	ADCIntDone = 0;
#else
	ADCValue[NUM_ADC] = ADCRead( NUM_ADC ); // ÂÖÑ¯·½Ê½²ÉÑù
#endif
#endif
	sprintf (ptmp, "%d", ADCValue[NUM_ADC]);
	UARTBuffer[8]='#';
	delay32Ms(0,1);
	UARTSend((uint8_t *)UARTBuffer, 8); // ´òÓ¡²ÉÑùÊý¾Ý
	memset(UARTBuffer,0,8);
	UARTCount = 0;
}
void read_temp(void)
{
	uint8_t *ptmp = NULL;
	ptmp = (uint8_t *)UARTBuffer+sizeof(uint8_t);
	/*uint8_t PrintfBuf[10]={0};*/
	for ( int i = 0; i < I2C_BUFSIZE; i++ )
	{
		I2CSlaveBuffer[i] = 0x00;
	}
	I2CReadLength = 2;                                    // I2C Ö÷»ú¶ÁÈ¡Êý¾Ý³¤¶È
	I2CMasterBuffer[0] = 0x91;                            // I2C µØÖ·
	I2CEngine();                                          // Æô¶¯I2C
	temp_value = ((I2CSlaveBuffer[0] <<8) + I2CSlaveBuffer[1]); // ´¦Àí¶ÁÈ¡µ½µÄÎÂ¶ÈÊý¾Ý
	temp_value = temp_value >> 5;
	if(temp_value & 0x400) // - temperature
	{ // TODO no time implement yeah.
		;
		/*temp_value = (temp_value & 0x3ff) >>3;*/
		/*memset((uint8_t *)UARTBuffer,0,UARTCount);*/
		/*UARTCount = sizeof(uint16_t);*/
		/*sprintf((uint8_t *)UARTBuffer, "-%d¡æ\r\n", temp_value);*/
	}
	else // + temperature
	{
		temp_value = (temp_value & 0x3ff) >>3;
		/*memset((uint8_t *)UARTBuffer,0,UARTCount);*/
		/*UARTCount = sizeof(uint16_t);*/
		/*sprintf((uint8_t *)UARTBuffer, "+%d¡æ\r\n", temp_value);*/
		sprintf(ptmp, "%d", temp_value);
	}
}
void read_spid(void)
{ // SS1 read/write
	uint8_t *ptmp = NULL;
	ptmp = (uint8_t *)UARTBuffer+sizeof(uint8_t);
	int spid_op = (char)UARTBuffer[2]-48;
	if (spid_op == 1)
	{
		memset(ptmp,0,UARTCount);
		UARTCount = 10+3;
		W25X10_RData(0, ptmp, 10); // read data
		UARTBuffer[12] = '#';
	}
	else {
		W25X10_Erase();
		delay32Ms(0,30);
		W25X10_WData(0, (uint8_t *)UARTSend, 10); // write data
	}
}
void con_led(void)
{
	int led_id = (char)UARTBuffer[2]-48;
	int led_stat = (char)UARTBuffer[3]-48;
	switch (led_id) {
		case 1 : GPIOSetValue(PORT2,8,led_stat); break;	
		case 2 : GPIOSetValue(PORT2,9,led_stat); break;	
		case 3 : GPIOSetValue(PORT2,10,led_stat); break;	
		case 4 : GPIOSetValue(PORT2,11,led_stat); break;	
		case 5 : GPIOSetValue(PORT1,5,led_stat); break;	
		case 6 : GPIOSetValue(PORT1,8,led_stat); break;	
		case 7 : GPIOSetValue(PORT1,9,led_stat); break;	
		case 8 : GPIOSetValue(PORT1,11,led_stat); break;
		case 9 : GPIOSetValue(PORT2,8,led_stat); 
			 GPIOSetValue(PORT2,9,led_stat); 
			 GPIOSetValue(PORT2,10,led_stat);
			 GPIOSetValue(PORT2,11,led_stat);
			 GPIOSetValue(PORT1,5,led_stat); 
			 GPIOSetValue(PORT1,8,led_stat); 
			 GPIOSetValue(PORT1,9,led_stat); 
			 GPIOSetValue(PORT1,11,led_stat);
			 break;
		default : break;
	}
}
void con_motor(void)
{
	/*int motor_id = (char)UARTBuffer[2]-48;*/ // TODO motor_id
	int motor_op = (char)UARTBuffer[3]-48;
	switch (motor_op) {
		case  1 : // forward
			motor_flags = 1;
			break;	
		case  2 : //reverse
			motor_flags = 2;
			break;	
		case  3 : // stop
			motor_flags = 3;
			break;	
		default : break;
	}
}
