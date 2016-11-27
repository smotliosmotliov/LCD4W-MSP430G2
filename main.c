
#include "lcdfuncset.h"
#include <msp430g2553.h>
/*
 * main.c
 */
char hello[] = {"DENI+MSP430G2553"};
char Bukva = 'D';



int main(void) {
	initMSP430();
	initLCD();
	while(1){
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
			delays(10000000);
		}

		//delays(16000000);
		P1OUT ^=0x40;

	}
}

