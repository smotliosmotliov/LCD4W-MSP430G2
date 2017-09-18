
#include <msp430g2553.h>
#include "lcdfuncset.h"
#include "adc10support.h"
#include "i2csupport.h"
#include "uart_driver.h"

/*
 * main.c
 */
unsigned char hello[] = {"DENI+MSP430G2553"};
char Bukva = 'D';


int main(void) {
	initMSP430();
	initLCD();
	initializeADC10();
	i2cInitialization();
	int count;
	while(1){
		buttonCheckADC10();
		i2cCommunication();
		count=10;
		clearDisplay();
		unsigned char i, addr=0;
		for(i=0; i<(sizeof(hello)/sizeof(hello[0])); i++)	{
			if(addr==7)	{
				addr=0x40;
			}
			else	{
				sendData((addr++)|0x80, RESETRS);
			}

			sendData(hello[i], SETRS);
			delays(5000000);
		}
		clearDisplay();
		iWrite("Writed ON CCS6.1", 0,1);
		for(;count>0;count--)	{
			delays(16000000);
		}
		P1OUT ^=0x40;
		//Draw with character CGROM data
		clearDisplay();
		addr=0;
		for(i=0; i<16; i++)	{
			if(addr==7)	{
				addr=0x40;
			}
			else	{
				sendData((addr++)|0x80, RESETRS);
			}

			sendData(0xF6, SETRS);
			delays(5000000);
		}
		clearDisplay();
		iWrite("ADC10 and I2C ST", 0,1);
		delays(5000000);
		iWrite("ADC10 and I2C ST", 0,1);
		delays(5000000);
	}
}

