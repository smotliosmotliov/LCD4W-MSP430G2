
#include "lcdfuncset.h"
#include <msp430g2553.h>
/*
 * main.c
 */
unsigned char hello[] = {"DENI+MSP430G2553"};
char Bukva = 'D';



int main(void) {
	initMSP430();
	initLCD();
	int count;
	while(1){
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
		//Try anime
		clearDisplay();
		sendData(0xC3, RESETRS);
		sendData(0x5F, SETRS);
		delays(5000000);
		sendData(0xC3, RESETRS);
		sendData(0x1D, SETRS);
		delays(5000000);
		sendData(0xC3, RESETRS);
		sendData(0x2D, SETRS);
		delays(5000000);
		sendData(0xC3, RESETRS);
		sendData(0xFF, SETRS);
		delays(5000000);
		sendData(0x83, RESETRS);
		sendData(0x5F, SETRS);
		delays(5000000);
		sendData(0x83, RESETRS);
		sendData(0x1D, SETRS);
		delays(5000000);
		sendData(0x83, RESETRS);
		sendData(0x2D, SETRS);
		delays(5000000);
		sendData(0x83, RESETRS);
		sendData(0xFF, SETRS);
		delays(5000000);
		count=10;
		for(;count>0;count--)	{
			delays(16000000);
		}
	}
}

