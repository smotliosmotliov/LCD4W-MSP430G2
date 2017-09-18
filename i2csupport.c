/*
 * i2csupport.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Deni
 */

//This add support for IIC support for TCN75A temperature sensor.

#include "i2csupport.h"
#include <msp430g2553.h>


void i2cInitialization(void)		{
	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_0 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 80;                             // fSCL = SMCLK/40 = ~400kHz
	UCB0BR1 = 0;
	UCB0I2CSA = 0x91;                         // Set slave address
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
	IE2 |= UCB0RXIE;                          // Enable RX interrupt
	TA1CTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
	return;
}

void i2cCommunication(void)	{
	char count=5;
	while((count--)>0)	{
		 //Replace with timer support
		RxByteCtr = 2;                          // Load RX byte counter
		UCB0CTL1 |= UCTXSTT;                    // I2C start condition
		__bis_SR_register(GIE);        			//enable interrupts
											// Remain in LPM0 until all data
											// is RX'd

		if (RxWord < 0x1d00)                    // >28C?
			P1OUT &= ~0x01;                       // No, P1.0 = 0
		else
			P1OUT |= 0x01;                        // Yes, P1.0 = 1

		__disable_interrupt();
		TA1CCTL0 |= CCIE;                        // TACCR0 interrupt enabled
		__bis_SR_register(GIE);        			//enable interrupts
											// Remain in LPM0 until TACCR0
											// interrupt occurs
		TA1CCTL0 &= ~CCIE;
	}

	return;
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void TA1_ISR(void)	{
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
}


// The USCIAB0TX_ISR is structured such that it can be used to receive any
// 2+ number of bytes by pre-loading RxByteCtr with the byte count.

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)	{
  RxByteCtr--;                              // Decrement RX byte counter

  if (RxByteCtr)
  {
    RxWord = (unsigned int)UCB0RXBUF << 8;  // Get received byte
    if (RxByteCtr == 1)                     // Only one byte left?
      UCB0CTL1 |= UCTXSTP;                  // Generate I2C stop condition
  }
  else
  {
    RxWord |= UCB0RXBUF;                    // Get final received byte,
                                            // Combine MSB and LSB
   // __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
}
