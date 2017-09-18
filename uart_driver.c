/*
 * uart_driver.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Deni
 */

#include "uart_driver.h"

Uart_Init_t uartCtl;

int uartInitF(UART_Conf_t* baudRate)	{
	int status=-1;
	if(uartCtl.UCA0CTL1&UART_SW_RESET)	{
		uartCtl.UCA0CTL1 |= UART_INP_SRC_CLK;							//Set input clock source to SMCLK
		uartCtl.UCA0CTL0 |=UART_MODE|UART_ASYNK_MODE;
		uartCtl.UCA0MCTL |= UART_FIRST_MOD_STAGE|UART_OVERSAMPLING;		//Over-sampling enable
		UART_BOUD_RADE_CTRL0 |=8;
		UART_BOUD_RADE_CTRL1 = 0;
		//TO DO the baudRate implementation
	}

	uartCtl.UCA0CTL1 &=~UART_SW_RESET;
	status = 0;


	return status;
}


//TO DO the ring buffer
