/*--------------------------------------------
File: 			keyPad.c
Description:  	KeyPad Module with interrupt
              	We will be using channel 4
---------------------------------------------*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "SegDisp.h"

/* Contants */
#define NOKEY		0
#define BIT4 		0b00010000
#define TimeMSEC	7500 /* 10 ms = 7500 x 1 1/3 micro-second or 3000 */

#define ROW1 0b11101111
#define ROW2 0b11011111
#define ROW3 0b10111111
#define ROW4 0b01111111

/* Local structures */
typedef enum
{
	WAITING_FOR_KEY = 0,
	DEB_KEYPRESS = 1,
	WAITING_FOR_REL = 2,
	DEB_REL = 3
} state_t;

/* Global variables */
volatile byte keyCode;
volatile state_t currentState;

static byte kCodes[] =
{
	0b11101110, 0b11101101, 0b11101011, 0b11100111,
    0b11011110, 0b11011101, 0b11011011, 0b11010111,
    0b10111110, 0b10111101, 0b10111011, 0b10110111,
    0b01111110, 0b01111101, 0b01111011, 0b01110111
};

static char aCodes[] =
{
	'1', '2', '3', 'a',
	'4', '5', '6', 'b',
	'7', '8', '9', 'c',
	'*', '0', '#', 'd'
};

/* Local prototypes */
static char getAscii(byte);
static byte getKCode(void);

/*---------------------------------------------
Function: initKeyPad
Description: initializes hardware for the
             keypad.
-----------------------------------------------*/
void initKeyPad(void)
{
	DDRA   = 0xF0;	/* PORTA all outputs */
	PORTA  = 0x00;  /* Initialize PORTA values */
	PUCR |= 0x01;	/* Enable pull-ups */

	TIOS |= BIT4;	/* Set ouput compare for TC4 */
	TIE |= BIT4; 	/* Enable interrupt on TC4 */
	TC4 = TCNT + TimeMSEC;
	
	keyCode = NOKEY;
}

/*---------------------------------------------
Function: pollReadKey
Description: convert value into ASCII
-----------------------------------------------*/
char pollReadKey()
{
	char ch = 0;
	
    if (keyCode != NOKEY)
    {
        ch = getAscii(keyCode);
        keyCode = NOKEY;
    }
    
    return ch;
}

/*---------------------------------------------
Function: readKey
Description: Verify if the value of the Port A isn't NOKEY,
			if the value changes, we call pollreadkey()
-----------------------------------------------*/
char readKey(void)
{
	char ch;
	   
	/* Wait for key press */
	while (keyCode == NOKEY)
	   	;
	   	
	ch = getAscii(keyCode);
	keyCode = NOKEY;
	
	return ch;
}

/*---------------------------------------------
Interrupt
Description: Interrupt service that checks the keypad every 10ms
We are going to use a state machine to implement the interrupt service
-----------------------------------------------*/
void interrupt VectorNumber_Vtimch4 key_isr(void)
{
	static byte code;

	switch(currentState)
	{
		case WAITING_FOR_KEY:
      		code = PORTA;
      		
      		/* A key has been pressed: go to debouncing state */
      		if (code != 0x0F)
      			currentState = DEB_KEYPRESS;
      			
      		break;
    	case DEB_KEYPRESS:
    		/* Code has changed; consider it a glitch */
      		if (PORTA != code) 
      			currentState = WAITING_FOR_KEY;
      		/* Otherwise we consider a key has really been pressed, and wait
      		   for it to be released */
      		else
      		{
         		code = getKCode();
         		currentState = WAITING_FOR_REL;
      		}
      		
     		break;
    	case WAITING_FOR_REL:
    		/* Wait for the key to be released, and go to debouncing when it is */
      		if (PORTA == 0x0F) 
      			currentState = DEB_REL;
      			
      		break;
    	case DEB_REL:
      		if (PORTA != 0x0F)
      			currentState = WAITING_FOR_REL;
      		else
      		{
      			/* Save the code, go back to waiting for another key */
          		keyCode = code;
          		currentState = WAITING_FOR_KEY;
      		}
      		
			break;
	}
	
	/* Setup next interrupt */
	TC4 = TC4 + TimeMSEC;
}

/*-------------------------------------------------
Function: getKCode()
Description: Gets a code from PORTA that corresponds
             to a keypress, through a scanning process.
             Same code as the given one for the lab 2
---------------------------------------------------*/
static byte getKCode()
{
	byte code;

  	PORTA = ROW1;
  	
  	if (PORTA == ROW1)
  	{
    	PORTA = ROW2;
    	
     	if (PORTA == ROW2)
     	{
        	PORTA = ROW3;
        	
        	if (PORTA == ROW3)
           		PORTA = ROW4;
     	}
  	}
  	
  	code = PORTA;
  	PORTA = 0x00; /* Reset PORTA */
  	
  	return code;
}

/*-------------------------------------------------
Function: getAscii()
Description: Translates PORTA values into corresponding ASCII code.
             We use a table that has the key code and a second table that has
             the corresponding ascii value.
---------------------------------------------------*/
static char getAscii(byte cd)
{
    int i;
    char ch = NOKEY;
    
    /* Loop through the translation tables */
    for (i = 0; i < sizeof(kCodes); i++)
    {
		if (kCodes[i] == cd)
       	{
        	ch = aCodes[i];
          	break;
       	}
    }
    
    return ch;
}
