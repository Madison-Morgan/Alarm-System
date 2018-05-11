/*--------------------------------------------
File: 			SegDisp.c
Description:  	Segment Display Module
              	Use Timer Channer 1
---------------------------------------------*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "SegDisp.h"

/* Constants */
#define BIT1 		0b00000010
#define delaydisp 	3750	/* For 5 ms delay (1 1/3 microSec/tick) */
#define NUMDISPS 	4 		/* Number of 7-segment displays */

/* Global variables */
volatile byte dNum = 0;
static char codedisplay[NUMDISPS];	/* Display table, holds the current display values */

static char table[17][2] =			/* Table for ASCII conversion to 7-segment code */
{
	{'0', 0b00111111},
	{'1', 0b00000110},
	{'2', 0b01011011},
	{'3', 0b01001111},
	{'4', 0b01100110},
	{'5', 0b01101101},
	{'6', 0b01111101},
	{'7', 0b00000111},
	{'8', 0b01111111},
	{'9', 0b01101111},

	{'a', 0b01110111},
	{'b', 0b01111100},
	{'c', 0b00111001},	
	{'d', 0b01011110},
	
	{' ', 0b00000000},
	{'*', 0b01000110},
	{'#', 0b01110000}
};

static byte enableCodes[NUMDISPS] =
{
   	0b00001110,
    	0b00001101,
    	0b00001011,
    	0b00000111
};	

/*---------------------------------------------
Function: initDisp
Description: initializes hardware for the
             7-segment displays.
-----------------------------------------------*/
void initDisp(void)
{
	DDRB	= 0xFF;    	/* Set PORTB to all outputs */
	PORTB	= 0xAA;    	/* Initialize the PORTB value */
	DDRP	= 0xFF;    	/* Set PORTP to all outputs also */
	PTP		= 0xAA;    	/* Initialize PORTP */
	
	TIOS |= BIT1;		/* Set output compare on channel 1 */
	TIE |= BIT1; 		/* Enable channel 1 interrupt */
	TC1 = TCNT + delaydisp;
}

/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void)
{
	/* Reset the display table */
	codedisplay[0] = 0b00000000;
	codedisplay[1] = 0b00000000;
	codedisplay[2] = 0b00000000;
	codedisplay[3] = 0b00000000;
}

/*---------------------------------------------
Function: setCharDisplay
Description: Receives an ASCII character (ch)
             and translates
             it to the corresponding code to
             display on 7-segment display.  Code
             is stored in appropriate element of
             codes for identified display (dispNum).
-----------------------------------------------*/
void setCharDisplay(char ch, byte dispNum)
{
	int i;

	for (i = 0; i < sizeof(table); i++)
	{
		if (ch == table[i][0])
		{
	 		codedisplay[dispNum] = table[i][1];
	    	break;
	    }
	}
}

/*-------------------------------------------------
Interrupt: tco1_isr
---------------------------------------------------*/
void interrupt VectorNumber_Vtimch1 disp_isr(void)
{
	byte upperPtp;

	/* Put the 7-segment code on PORTB */
  	PORTB = codedisplay[dNum];
  	
  	/* Enable the current segment's cathode, while preserving the upper 4 bits
  	   of port P */
  	upperPtp = PTP & 0xF0;
  	PTP = upperPtp | enableCodes[dNum];

	/* Setup next interrupt */
  	dNum = (dNum + 1) % 4;
	TC1 = TC1 + delaydisp;
}


/*---------------------------------------------
Function: turnOnDP
Description: Turns on decimal point of display
 id by dNum.
-----------------------------------------------*/
void turnOnDP(int dNum)
{
	byte upperPtp;
	
	PORTB=PORTB || 10000000; //(activate PORTB(7) decimal point)
	
	//enable current segments, perserve upper 4 bits of Port P
	upperPtp=PTP & 0xF0;
	PTP=upperPtp | enableCodes[dNum];
	
}

/*---------------------------------------------
Function: turnOffDP
Description: Turns off decimal point of display
 id by dNum.
-----------------------------------------------*/
void turnOffDP(int dNum)
{
	byte upperPtp;
	
	PORTB=PORTB & 01111111; //deactive PORTB(7), decimal point
	
	//enable current segments,preserve upper 4 bits of Port P
	upperPtp=PTP & 0xF0;
	PTP=upperPtp | enableCodes[dNum];
}



