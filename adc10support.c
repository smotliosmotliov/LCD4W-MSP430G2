/*
 * adc10support.c
 *
 *  Created on: Feb 18, 2017
 *      Author: Deni
 */
#include "adc10support.h"

//Define function for ADC10 initialization details for MSP430G2553

void initializeADC10(void)	{
	ADC10CTL0 &=~ENC;
	ADC10CTL0 = SREF_1|ADC10SHT_2|ADC10SR|REFON|ADC10ON|ADC10IE;
	ADC10CTL1 |= INCH_3|SHS_0|CONSEQ_2|ADC10SSEL_0;

	return;
}

char buttonPress;
//Using ADC10
void buttonCheckADC10()	{	//Need some extra work!!!
	P1DIR &=~0x08;
	P1IE |= 0x08;
	P1IES |=0x01;			//Interrupt Edge Select high-to-low transition set IFG
	P1IFG &=~0x08;
	__enable_interrupt();		//Enable interrupts
	int increment = 0, decrement=0;
	if((P1IN&0x08)==0)	{
		buttonPress=1;
		do	{
			ADC10CTL0 |=ENC|ADC10SC;
			while(!ADC10BUSY);
			//ADC10GetData=ADC10MEM;
			ADC10GetData = (1.5*(ADC10GetData/1023));

			if(ADC10GetData<=256)	{
				increment++;
				clearDisplay();
				iWrite((char*)increment, 0,1);
				delays(16000000);

			}
			else if (ADC10GetData >=256)	{
				decrement--;
				clearDisplay();
				iWrite((char*)decrement, 1,1);
				delays(16000000);
			}
			else if(ADC10GetData >=512)	{
				increment = increment+10;
				clearDisplay();
				iWrite((char*)increment, 0,1);
				delays(16000000);
			}
			else if(ADC10GetData <=1024)	{
				decrement=increment--;
				clearDisplay();
				iWrite((char*)decrement, 1,1);
				delays(16000000);
			}
			else	{
				statusCtrl=ERROR;
				clearDisplay();
				iWrite(statusCtrl, 0,1);
				delays(16000000);
			}
		}while(buttonPress);

	}else	{
		ADC10CTL0 &=~(ENC|ADC10SC);
		__disable_interrupt();
	}
	return;
}


#pragma vector = PORT1_VECTOR
__interrupt void buttonPress_ISR(void){
	buttonPress=0;
	P1IFG &=~0x08;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
	ADC10GetData=ADC10MEM;
	//ADC10CTL0 &=~ADC10IFG;
}
