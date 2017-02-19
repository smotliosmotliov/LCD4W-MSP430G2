/*
 * This header file is written from Deni Smotliov
 * and its used for define function prototypes and macros definition
 * for LCD_4Wire display driver.
 */



#ifndef LCDFUNCSET_H_
#define LCDFUNCSET_H_

#include <inttypes.h>
//Deffine
#define MASK	0xFF
#define LHMASK	0x0F
#define HHMASK	0xF0
#define RS		0x10			//Data/Instruction select
#define RW		0x20			//Read/Write Select
#define EN		0x40			//Enable Signal
#define D0		0x01			//Data signal 0
#define D1		0x02			//Data signal 1
#define D2		0x04			//Data signal 2
#define D3		0x08			//Data signal 3
#define MVDATA	(D3|D2|D1|D0)	//Nibble structure
#define SETRS	(int)1
#define	RESETRS	(int)0
#define DL8		1
#define DL4		0
#define	NDL2	1
#define FONT_11	1
#define FONT8	0
#define SHIFTCUR_ON	1
#define SHIFTCUR_OFF	0
#define	DISPSHIFT_ON	1
#define	DISPSHIFT_OFF	0

//Function Prototypes
void delays(unsigned long time);
void initMSP430();
void sendData(char sendBits, uint8_t bitForRS);
void clearDisplay();
void functionSet();
void shiftDispOrCur(uint8_t cur, uint8_t disp);
void iWrite(const char* data, uint8_t possitionX, uint8_t lineY);
void clearDisplay();
void dispControl();
void modeSet();
void returnHome();
void initLCD();



#endif
