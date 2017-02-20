/*
 * This file is writen from Deni Smotliov and its used
 * for initialize function and variable from LCD_4Wire driver.
 */
#include <msp430g2553.h>
#include "lcdfuncset.h"
#include <string.h>

unsigned int checTime;
uint8_t division;

//Initialize MSP430G2
void initMSP430()	{
	WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	P1DIR |=0x41;
	CCTL0 = CCIE;                             			// CCR0 interrupt enabled
	P3DIR |=0xFF;											//Set direction on PORT2
	P3OUT &=~0xFF;
	P2DIR |=0x04;
	P2OUT |=0x04;
	_BIS_SR(GIE);											//Interrupt enable

}
//end

/*
 * Bus scan function is writen separetly for purpose.
 * This function clock correct in time pulses into LCD
 * whit it LCD do write and read operation in write time.
 */
void busScan(uint8_t instData)	{
	if(instData==SETRS)	{					//Define data write cicle
		P3OUT &=~RS;
		P3OUT |=RW;
		delays(32000);
		P3OUT |= RS;
		P3OUT &=~(RW|EN);
		delays(16000);
		P3OUT |=EN;
		delays(32000);
		P3OUT &=~EN;
		delays(16000);
		P3OUT &=~RS;
		P3OUT |=RW;
		delays(16000);
	}
	else if(instData==RESETRS)	{			//Define instruction write cicle
		P3OUT |=RS;
		P3OUT |=RW;
		P3OUT &=~EN;
		delays(32000);
		P3OUT &=~RS;
		P3OUT &=~RW;
		delays(16000);
		P3OUT |=EN;
		delays(32000);
		P3OUT &=~EN;
		delays(16000);
		P3OUT |=RS;
		P3OUT |=RW;
		delays(32000);
	}
	P3OUT &=~(RS|RW|EN);
}
/*
 * This is core function. About it MSP430 write data or instruction on LCD.
 * Because LCD is in 4-wire mode, data are send with nibble structure.
 * First is send MSB nibble, start bus sync, secon send LSB nibble and
 * start bus sync.
 */
void sendData(char sendBits, uint8_t bitForRS)	{					//Write Data to LCD RAM
	char HighNibble = (sendBits & HHMASK);							//Extract low nibble bits
	P3OUT &=~0xF0;													//Clear data bits
	P3OUT |= HighNibble;												//Send MSB Octet into Data BUS
	busScan(bitForRS);												//Extract LSB Nibble
	//Set Enable bit
	P3OUT &=~0xF0;
	P3OUT |= ((sendBits & LHMASK)<<4);
	busScan(bitForRS);


}

/*
 * This function is used for define address on LCD.
 * Beacause MSB bit on data line is need to be on hight state,
 * we need add 0x80.
 */
void iWrite(const char* data, uint8_t possitionX, uint8_t lineY)	{					//Sets DD RAM address in address counter
	volatile uint8_t addresSet = 0x80;											//This address is 7 bits and 1 for setting
	size_t lenght=strlen(data);
	int xp;
	for(xp=possitionX;xp<(lenght); xp++)	{
		if(lineY==1 && possitionX<=8)	{
			sendData(addresSet|(possitionX++), RESETRS);
			sendData(*(data++), SETRS);
			if(possitionX==8)	{
				addresSet = 0xC0;
				lineY=2;
				possitionX=0;
			}
			//do something to write on line 0

		}
		else if (lineY==2 && possitionX<=8)	{
			sendData(addresSet|(possitionX++), RESETRS);
			sendData(*(data++), SETRS);
			if(possitionX==8)	{
				addresSet = 0x80;
				lineY=1;
				possitionX=0;
			}
			//do something to write on line 1
		}
		else	{
			char bad[]="BAD_ADD";
			int i;
			for(i=0;i<(sizeof(bad)/sizeof(bad[0])); i++){
				sendData((i|0x80), RESETRS);
				sendData(bad[i], SETRS);
			}

		}
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


/*
 * This function is initialize LCD display with him delay parameters
 */
void initLCD()	{
	delays(480000);					//Wheith for 30ms for power stable
	functionSet();
	delays(16000);
	dispControl();
	delays(16000);
	clearDisplay();
	delays(320000);
	modeSet();
}


/*
 * This delay function use a timerA0 on MSP430. When this function is used
 * timer is activated and CPU on MSP430 is going down to LOW POWER MOD/LPM0.
 * TimerA0 is count to 0 after that going to interrupt service routine and
 * load new value or clear and start CPU again.
 */
void delays(unsigned long time)	{
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

	else if(time>132000 && time<=196000)	{
		division = 4;
		checTime = (time/division);
	}
	else if(time>65500 && time <= 132000)	{
			division = 2;
			checTime = (time/division);

	}
	else	{
		division =0;
		checTime = 0;
	}
	CCTL0 = CCIE;                             	// CCR0 interrupt enabled
	CCR0 = time;
	TACTL = TASSEL_2 + MC_2;                  	// SMCLK, contmode
	_BIS_SR(LPM0_bits);							//Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
/*
 * This routine is serve for delays() function
 */
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A0 (void)
 {
   P1OUT ^= 0x01;                            		// Toggle P1.0/ indicator for correctnes


   if(division>0)	{
	   CCR0 += checTime;                            // Add Offset to CCR0
	   division--;

   }
   else	{
	   CCTL0 &=~CCIE;
	   division=0;
	   __bic_SR_register_on_exit(CPUOFF);			//Activate CPU with clear LPM bits
   }
 }

