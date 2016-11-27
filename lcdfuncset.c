/*
 * This file is writen from Deni Smotliov and its used
 * for declaring function and variable from LCD_4Wire driver.
 */
#include <msp430g2553.h>
#include "lcdfuncset.h"
//define function
unsigned int checTime;
uint8_t division;

//Initialize MSP430G2
void initMSP430()	{
	WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	P1DIR |=0x41;
	P2SEL &= ~0xC0;										//Use XIN (2.7) and XOUT (2.6) as Output Pins
	CCTL0 = CCIE;                             			// CCR0 interrupt enabled
	P2DIR |=0xFF;											//Set direction on PORT2
	P2OUT &=~0xFF;
	_BIS_SR(GIE);											//Interrupt enable

}
//end
void busScan(uint8_t instData)	{
	if(instData==SETRS)	{
		P2OUT &=~RS;
		P2OUT |=RW;
		delays(32000);
		P2OUT |= RS;
		P2OUT &=~(RW|EN);
		delays(16000);
		P2OUT |=EN;
		delays(32000);
		P2OUT &=~EN;
		delays(16000);
		P2OUT &=~RS;
		P2OUT |=RW;
		delays(16000);
	}
	else if(instData==RESETRS)	{
		P2OUT |=RS;
		P2OUT |=RW;
		P2OUT &=~EN;
		delays(32000);
		P2OUT &=~RS;
		P2OUT &=~RW;
		delays(16000);
		P2OUT |=EN;
		delays(32000);
		P2OUT &=~EN;
		delays(16000);
		P2OUT |=RS;
		P2OUT |=RW;
		delays(32000);
	}
	P2OUT &=~(RS|RW|EN);
}

void sendData(char sendBits, uint8_t bitForRS)	{					//Write Data to LCD RAM
	char lowNibble = (sendBits & LHMASK);							//Extract low nibble bits
	P2OUT &=~0x0F;													//Clear data bits
	//Set Enable bit
	P2OUT |= ((sendBits & HHMASK)>>4);								//Send MSB Octet into Data BUS
	busScan(bitForRS);
	P2OUT &=~0x0F;
	P2OUT |= lowNibble;												//Extract LSB Nibble
	busScan(bitForRS);
}

void setAddress(uint8_t lineX, uint8_t lineY)	{					//Sets DD RAM address in address counter
uint8_t addresSet = 0x80;											//This address is 7 bits and 1 for setting

	if(lineY==1)	{
		lineX |= (lineX & LHMASK);									//Correction for real addressing into lineX
		addresSet |= ((lineY-1)+(lineX-1));
		sendData(addresSet, RESETRS);
	}
	else if (lineY==2){
		addresSet |=(lineY<<4);										//This line make shifting for correct addressing
		lineX = (lineX & LHMASK);									//Correction for real addressing into lineX
		addresSet += (lineX-1);
		sendData(addresSet, RESETRS);
	}

}

void clearDisplay()	{												//Function for Clear Display

	sendData(0x01, RESETRS);
}


void functionSet()	{						//This function SETs interface data length (DL:8-bit/4-bit),
	sendData(0x28,RESETRS);											//number of display lines (N:2 line/1-line) and,											//display font type (F:5x11 dots/ 5x8 dots).
}

void dispControl()	{				//Set display (D), cursor (C), and blink of cursor (B) on/off control bit
	sendData(0x0E,RESETRS);

}

void modeSet()	{
	sendData(0x06, RESETRS);
}

void returnHome()	{			//Set DDRAM addres to 00H from AC and return cursor to its original position if shifted
	sendData(0x02, RESETRS);
}

void shiftDispOrCur(uint8_t cur, uint8_t disp)	{			//Set cursor moving and disp shift control bit, and direction
	uint8_t data = 0x01;
	data = (data<<3)|((cur<<1)|(disp<<1));
	data |= (data<<2);
	//sendData(0x1C, RESETRS);
	sendData(data, RESETRS);
}



void initLCD()	{
	delays(480000);
	functionSet();
	delays(16000);
	dispControl();
	delays(16000);
	clearDisplay();
	delays(320000);
	modeSet();
}


//This timer may be wrong!!!!!!!!!!!!!!
void delays(unsigned long time){
	if(time>1600000 && time <=16000000)	{
		division = 160;
		checTime = (time/division);
	}
	else if(time>500000 && time <=1600000){
		division = 16;
		checTime = (time/division);
	}
	else if(time>200000 && time <=500000){
		division = 8;
		checTime = (time/division);

	}
	else if(time>65500 && time <= 132000)	{
		division = 2;
		checTime = (time/division);

	}
	else if(time>131000 && time<=196000)	{
		division = 3;
		checTime = (time/division);

	}
	else	{
		division =0;
		checTime = 0;

	}
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = time;
	TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
	_BIS_SR(LPM0_bits);			//Enter LPM0 w/ interrupt
}

// Timer A1 interrupt service routine
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A0 (void)
 {
   P1OUT ^= 0x01;                            // Toggle P1.0


   if(division>0)	{
	   CCR0 += checTime;                            // Add Offset to CCR0
	   division--;

   }
   else	{
	   //_BIS_SR(LPM4_bits + GIE);			//Enter LPM4 w/ interrupt
	   CCTL0 &=~CCIE;
	   division=0;
	   __bic_SR_register_on_exit(CPUOFF);
   }
 }

