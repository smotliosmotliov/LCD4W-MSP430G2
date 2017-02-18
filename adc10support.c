/*
 * adc10support.c
 *
 *  Created on: Feb 18, 2017
 *      Author: Deni
 */
#include "adc10support.h"

//Define function for ADC10 initialization details for MSP430G2553

void initializeADC10()	{
	ADC10CTL0 =	SREF_1;

	return;
}

//Using ADC10
void buttonCheckADC10()	{

	return;
}
