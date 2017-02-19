/*
 * adc10support.h
 *
 *  Created on: Feb 18, 2017
 *      Author: Deni
 */



#ifndef ADC10SUPPORT_H_
#define ADC10SUPPORT_H_


#define ERROR "Error input"
int ADC10GetData;
char* statusCtrl;
void initializeADC10(void);
void buttonCheckADC10();


__interrupt void buttonPress_ISR(void);
__interrupt void ADC10_ISR(void);

#endif /* ADC10SUPPORT_H_ */
