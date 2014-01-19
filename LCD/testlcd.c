/***************************************************************************************************************
    This file is part of Library forMSP430.

    Library for MSP430 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Library for MSP430 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Library for MSP430.  If not, see <http://www.gnu.org/licenses/>.
/**************************************************************************************************************/


/**************************************************************************************************************
   File   : testlcd.c
   Author : Sriharsha
   Email  : helpzuna@gmail.com
/**************************************************************************************************************/

#include "lcd.h"

#define LCD_REFRESH_DELAY   500         // LCD Refresh rate in milliseconds

#if _LCD_TYPE_ == 1
#define String1  "    Hello World    "
#define String2  "    Hai MSP430     "
#define String3  "    This is        "
#define String4  "    LCD TEST       "

#elif _LCD_TYPE_ == 2
#define String1  "    Hello World    "
#define String2  "    Hai MSP430     "
#endif



/*** Function    : stopWatchDog
**   Parameters  : None
**   Return      : None
**   Description : It will stop Watch Dog timer of MSP430
**/
void stopWatchDog(void)
{
	WDTCTL = WDTPW + WDTHOLD;         // Stop Watch Dog timer
}

/*** Function    : main
**   Parameters  : None
**   Return      : int
**   Description : It is entry point of program
**/
int main(void)
{
	stopWatchDog();
	lcdBegin();                       // Initiate the LCD
	while(1)
	{
    #if _LCD_TYPE_ == 1
	lcdSetCursor(1,0);
    lcdPrint(String1);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdSetCursor(2,0);
    lcdPrint(String2);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdSetCursor(3,0);
    lcdPrint(String3);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdSetCursor(4,0);
    lcdPrint(String4);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdClear();
    __delay_ms__(LCD_REFRESH_DELAY);
    #elif _LCD_TYPE_ == 2
	lcdSetCursor(1,0);
    lcdPrint(String1);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdSetCursor(2,0);
    lcdPrint(String2);
    __delay_ms__(LCD_REFRESH_DELAY);
    lcdClear();
    __delay_ms__(LCD_REFRESH_DELAY);
    #endif
	}
}
