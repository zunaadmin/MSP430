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
**   File       : uart.c
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This is the uart driver for MSP430 family MCU's
***/

#include "uart.h"

// Globals

// Oscillator Frequency (Default 1MHz)
unsigned long OscillatorFrequency = 1000000;


#ifdef SERIAL_RX_INTERRUPT_ENABLE

// Uart New Line Flag (Set when new line received)
 volatile unsigned char uartNewLineFlag = 0;
// UART Read Buffer to store Received Data
 volatile unsigned char uartReadBuffer[UART_RX_BUFFER_SIZE];
// Uart Byte Count
 volatile unsigned int  uartReadCount    = 0;
// Uart New Line Count
 volatile unsigned char uartNewLineCount = 0;
// It holds every new line index
 volatile unsigned char uartNewLineIndexes[NEW_LINE_INDEX_BUFFER_SIZE];

#endif



/*** Function    : SetOsc
**   Parameters  : unsigned long (Oscillator Frequency in Hz)
**   Return      : None
**   Description : It will Set the Oscillator Frequency for Baud Rate Calculations
**/
void SetOsc(unsigned long OscFreq)
{
OscillatorFrequency = OscFreq;
BCSCTL1 = CALBC1_16MHZ; // Set DCO to 16MHz
DCOCTL  = CALDCO_16MHZ; // Set DCO to 16MHz
__delay_cycles(16000000/100);
}

/*** Function    : inituartPort
**   Parameters  : None
**   Return      : None
**   Description : It will initiate the uart ports
**/
void inituartPort(void)
{
	P1DIR &= ~(BIT1 | BIT2); // Revert to default to GPIO input
	P1SEL = BIT1 | BIT2; // P1.1=RXD, P1.2=TXD
	P1SEL2 = BIT1 | BIT2; // P1.1=RXD, P1.2=TXD
}

/*** Function    : Serialbegin
**   Parameters  : unsigned long (Standard BaudRate)
**   Return      : None
**   Description : It will Set the baud rate for serial communication
**/
void Serialbegin(unsigned long baudRate)
{
	 unsigned long autoReloadval = (OscillatorFrequency + (baudRate >> 1)) / baudRate; // Bit rate divisor
	 inituartPort();
	 UCA0CTL1 = UCSWRST; // Hold USCI in reset to allow configuration
	 UCA0CTL0 = 0; // No parity, LSB first, 8 bits, one stop bit, UART (async)
	 UCA0BR1 = (autoReloadval >> 12) & 0xFF; // High byte of whole divisor
	 UCA0BR0 = (autoReloadval >> 4) & 0xFF; // Low byte of whole divisor
	 UCA0MCTL = ((autoReloadval << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
	 UCA0CTL1 = UCSSEL_2; // Use SMCLK for bit rate generator, then release reset
}

/*** Function    : Serialavailable
**   Parameters  : None
**   Return      : __bit (If byte is available in receive buffer returns 1, else returns 0)
**   Description : It will give the whether Receiver is available or not
**/
unsigned char Serialavailable(void)
{
return 1;
}


/*** Function    : Serialwrite
**   Parameters  : unsigned char (Single character that will send to UART)
**   Return      : None
**   Description : It will write single character to UART
**/
void Serialwrite(unsigned char Byte)
{
	 while(!(IFG2 & UCA0TXIFG)); // wait for TX buffer to be empty
	UCA0TXBUF = Byte;
}

/*** Function    : Serialread
**   Parameters  : None
**   Return      : unsigned char
**   Description : It will read single byte from uart
**/
unsigned char Serialread(void)
{
	while(!(IFG2 & UCA0RXIFG));
	IFG2 &= ~UCA0RXIFG;
	return UCA0RXBUF;
}

/*** Function    : Serialprint
**   Parameters  : unsigned char *
**   Return      : None
**   Description : It will send the string to UART
**/
void Serialprint(unsigned char *sPtr)
{
for(;*sPtr!='\0';Serialwrite(*(sPtr++)));
}


/*** Function    : SerialIntWrite
**   Parameters  : unsigned char *
**   Return      : None
**   Description : It will send the string to UART
**/
void SerialIntWrite(signed int num)
{
char *tempBuffer;
sprintf(tempBuffer,"%d",num);
Serialprint((unsigned char*)tempBuffer);
}


#ifdef SERIAL_RX_INTERRUPT_ENABLE

/*** Function    : setSerialinterrupt
**   Parameters  : None
**   Return      : None
**   Description : It sets the Serial Interrupt
**/
void setSerialinterrupt(void)
{
	 IE2      |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	 __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}

/*** Function    : Serialflush
**   Parameters  : None
**   Return      : None
**   Description : It clears the UART buffer,Index Buffer and Flags
**/
void Serialflush(void)
{
unsigned char i;
uartReadCount    = 0; // Clear Uart Byte Count
uartNewLineFlag  = 0; // Clear New Line Flag
uartNewLineCount = 0; // Clear New Line Count

// Flush New Line Index Buffer
for(i=0;i<=NEW_LINE_INDEX_BUFFER_SIZE;i++)
uartNewLineIndexes[i] = CHAR_NULL;

// Flush Uart Read Buffer
for(i=0;i<=UART_RX_BUFFER_SIZE;i++)
uartReadBuffer[i] = CHAR_NULL;
}

/*** Function    : uartISR
**   Parameters  : None
**   Return      : None
**   Description : It is ISR for UART Receive (It will trigger if any byte is received)
**/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void uartISR(void)
{
uartReadBuffer[uartReadCount++] = UCA0RXBUF;
if(UCA0RXBUF == LF)
{
uartNewLineIndexes[uartNewLineCount] = uartReadCount;
uartNewLineCount++;
uartNewLineFlag = 1;
}
}

#endif



