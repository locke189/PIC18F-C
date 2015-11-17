#include <p18f4550.h>
#include "i2c.h"
#include "sw_i2c.h"
#include "delays.h"
#include "timers.h"

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

//define  Slave Address
#define SLAVE_PIC 0x14

//Define Function Prototypes

void interruptVectorHigh( void );
void interruptHandlerHigh( void );
void delayms( void );

//variables
unsigned char IntF = 0; //this appears to be a flag
unsigned char mivar1, mivar2; // mivar1 =??? ; mivar2 =???;


void main()
{
	// Main. PIC register configuration
	OSCCON = 0x7F; //
	ADCON1 = 0xFF; //no analogic inputs
	
	//LATA = 0;
	//LATB = 0;
	//LATC = 0;
	
	TRISA = 0; // PortA needed for output
	

	//Main. PIC SSP Module configuration
	TRISB = 0xFF; //port b Input
	
	SSPCON1 = 0x36; // 7 bit Slave no interrupt
	SSPCON2 = 0x00; // no masking, no clock stretch
	SSPADD = SLAVE_PIC; // Address asigment for the PIC
	
	SSPSTATbits.SMP = 0; // slew rate
	SSPSTATbits.CKE = 1; // slew rate
	
	PIR1bits.SPPIF = 0; // interrupt bits cleared for SSP
	
	//OpenI2C(SLAVE_7, SLEW_ON); // Libtest
	

	//Main. PIC Interrupt Configuration
	PIE1bits.SSPIE = 1; //enable SSP Interrupt
	INTCONbits.PEIE = 1; //enables peripheral interrupt
	INTCONbits.GIE = 1; //global interrupt enable
	
	//Main. program loop
	
	LATAbits.LATA1 = 1;
	LATAbits.LATA0 = 0;	

	

	while(1){
		
	if( IntF ){

			
			if ( mivar2 == 0xFF ){
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
			}

			if ( mivar2 == 0x66 ){
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
				LATAbits.LATA0 = !PORTAbits.RA0;
				Delay1KTCYx(50);
				LATAbits.LATA0 = !PORTAbits.RA0;
			}
			
			
			mivar2 = 0x00;
			IntF = 0;
		}
	}
}


// High Priority interrupt
#pragma code interruptVectorHigh = 0x08
void interruptVectorHigh(){
	_asm goto interruptHandlerHigh _endasm
}

// High Priority Interrupt Handler
#pragma code
#pragma interrupt interruptHandlerHigh
void interruptHandlerHigh(){
	//this seems to be a timer interrupt
	if (PIR1bits.TMR2IF){
		PIR1bits.TMR2IF = 0;
	}
	//SSP interrupt handler
	if ( PIR1bits.SSPIF ){
		//LATAbits.LATA1 = !PORTAbits.RA1;
		

		if ( !SSPSTATbits.R_W & !SSPSTATbits.D_A & SSPSTATbits.BF ){
			mivar1 = SSPBUF;
			PIR1bits.SSPIF = 0;
			
			
//			if ( mivar1 == 0x00 ){
//				LATAbits.LATA0 = !PORTAbits.RA0;
//			}			

		}

		if ( !SSPSTATbits.R_W & SSPSTATbits.D_A & SSPSTATbits.BF ){
			mivar2 = SSPBUF;
			LATAbits.LATA1 = !PORTAbits.RA1;
			PIR1bits.SSPIF = 0;	
		}
		
		if ( SSPSTATbits.R_W ){
			SSPBUF = 0x14;
			SSPCON1bits.CKP = 1;

			SSPBUF = 0x14;
			SSPCON1bits.CKP = 1;

			LATAbits.LATA1 = !PORTAbits.RA1;
			PIR1bits.SSPIF = 0;
//			if ( mivar2 == 0x66 ){
//				LATAbits.LATA0 = !PORTAbits.RA0;
//			}			
		}

//		if ( DataRdyI2C() ){
//
//  			mivar1 = ReadI2C();
//			LATAbits.LATA1 = !PORTAbits.RA1;
//			PIR1bits.SSPIF = 0;
//		}
		SSPCON1bits.WCOL = 0; // slew rate
		SSPCON1bits.SSPOV = 0; // slew rate
		SSPCON1bits.CKP = 1;
		PIR1bits.SSPIF = 0;
		IntF = 1;
	}	
}
