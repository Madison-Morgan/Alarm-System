/*------------------------------------------------
File: siren.c
Description: Module for Siren. Uses the channel 5
Author:			Luay Grira
-------------------------------------------------*/

#include "mc9s12dg256.h"

/* Constants */
#define HIGH_MS	600   // 600 * 4/3 micro-sec = 0.800 ms
#define LOW_MS 	150   // 150 * 4/3 micro-sec = 0.200 ms

#define BIT5 	0b00100000
#define HIGH 	1
#define LOW 	0

/* Global variables */
volatile int levelTC5;	/* Current audio level (high or low) */

/*---------------------------------------------
Function: initSiren
Description:
-----------------------------------------------*/
void initSiren()
{
	TIOS_IOS5 = 1;  /* Output-compare TC5 */
}

/*---------------------------------------------
Function: turnOnSiren
Description:
-----------------------------------------------*/

void turnOnSiren()
{
	levelTC5 = HIGH;

	/* Enable channel 5 interrupt */
	TIE |= BIT5;
   
	/* Enable toggle as output-compare action for timer channel 5 (will 
	   toggle PT5, which is connected to the on-board speaker. */
	TCTL1 |= 0b00001100;
	CFORC = 0b00100000; /* Force PT5 to high immediately */
	TCTL1 &= 0b11110111;
	TC5 = TCNT + HIGH_MS;
}

/*---------------------------------------------
Function: turnOffSiren
-----------------------------------------------*/
void turnOffSiren()
{
	/* Disable channel 5 interrupt */
	TIE &= 0b11011111;
	
	/* Set output-compare action to reset */
	TCTL1 |= 0b00001000;
	TCTL1 &= 0b11111011;
   	
	/* And force the action immediately */
	CFORC = 0b00100000;
}

/*---------------------------------------------
Interrupt: timer channel 5
-----------------------------------------------*/
void interrupt VectorNumber_Vtimch5 sirenISR()
{
	/* Setup the next interrupt, based on current output level. This is
	   basically an 80% duty cycle, software-based PWM. */
	if (levelTC5 == HIGH)
	{
		levelTC5 = LOW;
		TC5 += LOW_MS;
	}
	else
	{
		levelTC5 = HIGH;
		TC5 += HIGH_MS;
	}
}



