
#ifndef LCD_H
#define LCD_H

#include <delays.h>
#include <string.h>

//LCD will use port B


//port defines
#define DB7_Pin LATBbits.LATB7 
#define DB6_Pin LATBbits.LATB6 
#define DB5_Pin LATBbits.LATB5
#define DB4_Pin LATBbits.LATB4 
#define RS_Pin LATBbits.LATB0
#define EN_Pin LATBbits.LATB1 

//LCF command Defines

   //commands
#define    LCD_CLEARDISPLAY          0x01
#define    LCD_RETURNHOME            0x02
#define    LCD_ENTRYMODESET          0x04
#define    LCD_DISPLAYCONTROL        0x08
#define    LCD_CURSORSHIFT           0x10
#define    LCD_FUNCTIONSET           0x20
#define    LCD_SETCGRAMADDR          0x40
#define    LCD_SETDDRAMADDR          0x80

    //flags for display entry mode
#define    LCD_ENTRYRIGHT            0x00
#define    LCD_ENTRYLEFT             0x02
#define    LCD_ENTRYSHIFTINCREMENT   0x01
#define    LCD_ENTRYSHIFTDECREMENT   0x00

    //flags for display on/off control
#define    LCD_DISPLAYON             0x04
#define    LCD_DISPLAYOFF            0x00
#define    LCD_CURSORON              0x02
#define    LCD_CURSOROFF             0x00
#define    LCD_BLINKON               0x01
#define    LCD_BLINKOFF              0x00

    //flags for display/cursor shift
#define    LCD_DISPLAYMOVE           0x08
#define    LCD_CURSORMOVE            0x00

    //flags for display/cursor shift
#define    LCD_DISPLAYMOVE           0x08
#define    LCD_CURSORMOVE            0x00
#define    LCD_MOVERIGHT             0x04
#define    LCD_MOVELEFT              0x00

    //flags for function set
#define    LCD_8BITMODE              0x10
#define    LCD_4BITMODE              0x00
#define    LCD_2LINE                 0x08
#define    LCD_1LINE                 0x00
#define    LCD_5x10DOTS              0x04
#define    LCD_5x8DOTS               0x00

#define 	LCD_WAIT				3

char upper_bits;
char lower_bits;

void delayx(int t)
	{
	int i;
	for(i=1;i < t;i++)
	;
	}

void LCD_setPortBOutput(void)
	{
	ADCON1=0x0F;
	TRISB=0x00;
	LATB = 0x00;
	//INTCON2=0x00;
	}


void LCD_enable(void)
	{
	EN_Pin = 0;
	//delay10TCYx(LCD_WAIT);
	delayx(LCD_WAIT);
	EN_Pin = 1;
	//Delay10TCYx(LCD_WAIT);
	delayx(LCD_WAIT);
	EN_Pin = 0;
	//Delay10TCYx(LCD_WAIT);
	delayx(LCD_WAIT);
	}

void LCD_write(char data, int char_mode)
	{

	
	//Delay100TCYx(250);
	delayx(LCD_WAIT*2);
	
	upper_bits =  (data & 0xF0);
	LATB = (LATB & 0x0F) | upper_bits;
	RS_Pin = char_mode;
	LCD_enable();
	
	lower_bits = ((data & 0x0F) << 4);
	LATB = (LATB & 0x0F) | lower_bits;
	RS_Pin = char_mode;
	LCD_enable();
	}
	
void LCD_clear(void)
	{
	
	LCD_write(LCD_CLEARDISPLAY,0);
	//Delay100TCYx(250);
	delayx(LCD_WAIT * 5);
	}	

void LCD_initialize(void)
	{
	// Initialize the display.
    LCD_setPortBOutput();
    LCD_write(0x33,0);
    LCD_write(0x32,0);
    LCD_write(0x28,0);
    LCD_write(0x0C,0);
    LCD_write(0x06,0);

    //Initialize display control, function, and mode registers.
    // Write registers.
    
    
    //LCD_write(LCD_DISPLAYCONTROL | ( LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF ),0 );
    //LCD_write(LCD_FUNCTIONSET | ( LCD_4BITMODE | LCD_1LINE | LCD_2LINE | LCD_5x8DOTS ),0 );
    //LCD_write(LCD_ENTRYMODESET | ( LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT ),0 );
    
    
    LCD_write(LCD_ENTRYMODESET | (LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT),0 );
    
    LCD_clear();
 }

void LCD_input( unsigned char string[])
	{
	int i;
	unsigned char x;
	
	for(i=0; i < strlen(string); i++)      
		{
		x = string[i];
		if(x == '\n')
			{
			LCD_write(0xC0,0);
			}	
		else
			{
			LCD_write(x,1);
			}
		
		}       
	}
	
#endif