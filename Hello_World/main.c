#include <p18f4550.h>
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#pragma config FOSC = INTOSCIO_EC //Internal oscillator, port function on RA6, EC used by USB 
//#pragma config WDT = OFF //Disable watchdog timer


#pragma config PLLDIV = 5 , CPUDIV = OSC1_PLL2 , USBDIV = 2    // You can write this way
// OR
#pragma config FOSC = INTOSCIO_EC
#pragma config FCMEN = OFF                                 // OR this way
#pragma config BORV = 3
#pragma config WDT = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config LVP = OFF

int count;
int flag;

void main()
{
	unsigned char text[]="Crota must\ndie!";
	unsigned char text2[32];
	Delay10KTCYx(25);
	LCD_initialize();
	flag = 0;
	count = 0;
	while(flag == 0)
	{
		//LCD_write('f',1);
		itoa(count,text2);
		LCD_input(text2);
		//LCD_write('f',1);
		LATBbits.LATB2 = ~LATBbits.LATB2;
		Delay10KTCYx(25);
		LCD_clear();
		count++;
	}

}