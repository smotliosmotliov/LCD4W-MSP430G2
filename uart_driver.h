/*
 * uart_driver.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Deni
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>
#include <msp430g2553.h>

//Defines for CTLR0
#define UART_CTRL_REG0	UCA0CTL0
#define UART_CTRL_REG1	UCA0CTL1
#define UART_SW_RESET	UCSWRST
#define UART_MODE		UCMODE0
#define	UART_ASYNK_MODE	UCSYNC
#define UART_INP_SRC_CLK	UCSSEL_2
#define UART_BOUD_RADE_CTRL0	UCA0BR0
#define UART_BOUD_RADE_CTRL1	UCA0BR1
#define UART_MODULATION_CTRL	UCA0MCTL
#define UART_OVERSAMPLING	UCOS16
#define UART_FIRST_MOD_STAGE	UCBRF_11

#define R_BUFF_SIZE	4U


//Config UART baud rate
typedef struct {
	uint32_t baud;
}UART_Conf_t;

typedef struct {
	unsigned char UART_CTRL_REG0;
	unsigned char UART_CTRL_REG1;
	unsigned char UART_BOUD_RADE_CTRL0;
	unsigned char UART_BOUD_RADE_CTRL1;
	unsigned char UART_MODULATION_CTRL;
} Uart_Init_t;

int uartInitF(UART_Conf_t* baudRate);

typedef struct ringBuff_t{
	int head;
	int tail;
	uint8_t *buff;
} ringBuff[R_BUFF_SIZE];

#endif /* UART_DRIVER_H_ */
