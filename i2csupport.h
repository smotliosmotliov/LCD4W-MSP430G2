/*
 * i2csupport.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Deni
 */


#ifndef I2CSUPPORT_H_
#define I2CSUPPORT_H_
unsigned int RxByteCtr;
unsigned int RxWord;
void i2cInitialization(void);
void i2cCommunication(void);

__interrupt void USCIAB0TX_ISR(void);
__interrupt void TA1_ISR(void);
__interrupt void USCIAB0RX_ISR(void);


#endif /* I2CSUPPORT_H_ */
