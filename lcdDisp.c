/*-------------------------------------
File: 			lcdDisp.c
Description: 	Provides display functions for the
            	LCD. It makes use of the assembly LCD module.
-------------------------------------*/

#include <mc9s12dg256.h>
#include "lcd_asm.h"

/* Constants */
#define NUM_LINES 	2
#define LINE_OFFSET 40
#define LINE_SIZE 	16

/* Local prototypes */
static void padLCDString(char *, char *, byte);

/*--------------------------
Function: initLCD
Parameters: None.
Returns: nothing
Description: Initialised the LCD hardware by
             calling the assembler subroutine.
---------------------------*/
void initLCD(void)
{
	lcd_init();
}

/*--------------------------
Function: printStr
Parameters: str - pointer to string to be printed 
                  (only 16 chars are printed)
            lineno - 0 first line
                     1 second line
Returns: nothing
Description: Prints a string on the display on one of the
             two lines.  String is padded with spaces to
             erase any existing characters.
---------------------------*/
void printLCDStr(char *str, byte lineno)
{
    char buf[LINE_SIZE + 1];
    
    /* Check for invalid line numbers */
    if (lineno < 0 || lineno > 1)
    	return;
    
    set_lcd_addr(lineno * LINE_OFFSET);
    padLCDString(str, buf, LINE_SIZE);
    type_lcd(buf);
}

/*--------------------------
Function: padLCDString
Parameters: str - string to pad
            buf - new string, padded
            size - size of new string.
Returns: nothing
Description: Copies the string referenced by str
             to buffer referenced by newstr and
             pad with spaces to fill the buffer.
---------------------------*/
static void padLCDString(char *str, char *buf, byte size)
{
    int i = 0;
    
    /* Truncate */
    while (i < size && *str != '\0')
    {
    	*buf++ = *str++;
      	i++;
    }
    
    /* And pad with spaces */
    while (i < size)
    {
       *buf++ = ' ';
       i++;
    }
    
    /* Terminate the string */
    *buf = '\0';
}

/*--------------------------
Function: putLCDChar
Parameters: ch - character to be printed
            lineno - 0 first line
                     1 second line
            chpos - 0 to 15            
Description:  Prints the character at position 
              on the line.
---------------------------*/
void putLCDChar(char ch, byte lineno, byte chpos)
{
    byte addr;
    
    if (lineno < NUM_LINES && chpos < LINE_SIZE)
    {
       addr = lineno * LINE_OFFSET + chpos;
       set_lcd_addr(addr);
       data8(ch);
    }
}