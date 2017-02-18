/*
 * adc10support.c
 *
 *  Created on: Feb 18, 2017
 *      Author: Deni
 */
#include "adc10support.h"

//Define function for ADC10 initialization details for MSP430G2553

void initializeADC10()	{
	ADC10CTL0 |= SREF_1|ADC10SHT_1|ADC10SR|~REF2_5V|REFON|ADC10ON|ADC10IE;
	ADC10CTL1 |= INCH_3|SHS0|CONSEQ_0|ADC10SSEL_0;

	return;
}

char buttonPress=0;
//Using ADC10
void buttonCheckADC10()	{
	int ADC10GetData=0;
	if((P1IN&0x08)==0)	{
		do	{
			ADC10CTL0 |=ENC|ADC10SC;
			while(!ADC10BUSY);
			ADC10GetData=ADC10MEM;
			ADC10GetData = (1023*(ADC10GetData/1.5));

			if(ADC10GetData<=256)	{

			}
			else if (ADC10GetData >=256)	{

			}
			else if(ADC10GetData >=512)	{

			}
			else if(ADC10GetData <=1024)	{

			}
			else	{

			}
		}while(buttonPress);

	}


	return;
}
