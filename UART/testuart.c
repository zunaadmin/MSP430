/***************************************************************************************************************
    This file is part of Library for MSP430.

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

/***
**   File       : testuart.c
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This is the test code for MSP430 uart driver
***/

#include "uart.h"

const unsigned long OSC_FREQ            = 16000000;
#define BAUD_RATE           9600

#define NEW_LINE_THRESHOLD  3

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
**   Description : It is the entry point of program
**/
int main(void)
{
stopWatchDog();
SetOsc(OSC_FREQ);             // Set Oscillator Freq
Serialbegin(BAUD_RATE);       // Set Baud Rate
Serialflush();                // Clear the buffers
Serialprint("uart test\n\r"); // Print a string
setSerialinterrupt();         // Enable Serial Interrupt
while(1)
{
 while(uartNewLineCount<NEW_LINE_THRESHOLD); // Wait until new line count reaches threshold
 Serialprint(uartReadBuffer);                // Print the uart read buffer
 Serialflush();                              // Flush the buffer
}
}
