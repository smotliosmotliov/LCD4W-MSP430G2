
//#include <msp430g2553.h>
#include "adc10support.h"
/*
 * main.c
 */
unsigned char hello[] = {"DENI+MSP430G2553"};
char Bukva = 'D';


int main(void) {
	initMSP430();
	initLCD();
	initializeADC10();
	int count;
	while(1){
		buttonCheckADC10();
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

			sendData(0xFF, SETRS);
			delays(5000000);
		}
		clearDisplay();
		iWrite("Writed ON CCS6.1", 0,1);
	}
}

