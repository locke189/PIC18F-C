/*
PIC18F4550 - Slave_main.c
The purpose of this program is to control a digital output (Port C) and one
ADC channel through a RaspberryPi sending commands via I2C.
*/
//#include <p18f4550.h>
#include <p18f2550.h>


//#include "i2c.h" //I2C libraries... not useful
//#include "sw_i2c.h"
//#include "delays.h"
//#include "timers.h"

//#pragma config FOSC = INTOSCIO_EC //Internal oscillator, port function on RA6, EC used by USB 
//#pragma config WDT = OFF //Disable watchdog timer
//#pragma config SOSCSEL = DIG

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


//Definition of commands
#define DIGITAL_ALL_ON 0x10
#define DIGITAL_ALL_OFF 0x11



//Define Function Prototypes

void interruptVectorHigh( void );
void interruptHandlerHigh( void );
void delayms( void );

//variables
unsigned char IntF = 0; //this appears to be a 
unsigned char commandI2C;
unsigned char outputI2C;
unsigned char mivar1, mivar2; // mivar1 =??? ; mivar2 =???;



void main()
{
	// Main. PIC register configuration
	OSCCON = 0x7F; //
	ADCON1 = 0x0F; //no analog inputs
//	T1CONbits.T1OSCEN = 0;
//	T1CONbits.TMR1CS = 0;
//  T1CONbits.TMR1ON = 0;
//	T3CONbits.TMR3ON = 0;
//	UCONbits.USBEN = 0;
//	UCFGbits.UTRDIS = 1;
	
	//LATA = 0;
	//LATB = 0;
	//PORTC = 0;
	//LATC = 0;
	
	TRISA = 0; // PortA needed for output
	//Main. PIC SSP Module configuration
	TRISB = 0xFF; //port b Input
	
	PORTC = 0x00;
	TRISC = 0x00; //Port as outputs
	
	SSPCON1 = 0x36; // 7 bit Slave no interrupt
	SSPCON2 = 0x00; // no masking, no clock stretch
	SSPADD = SLAVE_PIC; // Address asigment for the PIC
	
	SSPSTATbits.SMP = 0; // slew rate
	SSPSTATbits.CKE = 1; // slew rate
	
	
	//PIR1bits.SPPIF = 0; // interrupt bits cleared for SSP
	  
	//OpenI2C(SLAVE_7, SLEW_ON); // Libtest
	

	//Main. PIC Interrupt Configuration
	PIE1bits.SSPIE = 1; //enable SSP Interrupt
	INTCONbits.PEIE = 1; //enables peripheral interrupt
	INTCONbits.GIE = 1; //global interrupt enable
	
	//Main. program loop
	
	//Port Initialization
	LATAbits.LATA1 = 1;
	LATAbits.LATA0 = 0;	
	
	LATC = 0xF5;	
	commandI2C = 0x00;
	
	LATCbits.LATC0 = 1;
	PIR1bits.SSPIF = 0;
	while(1){
		
	if( IntF ){

			
			if ( commandI2C == DIGITAL_ALL_ON ){
				LATC = 0x07;
				LATAbits.LATA1 = !PORTAbits.RA1;
			}

			if ( commandI2C == DIGITAL_ALL_OFF ){
				LATC = 0x05;
				LATAbits.LATA0 = !PORTAbits.RA0;
			}
			
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
		
		//Detect Address not implemented
		if ( !SSPSTATbits.R_W & !SSPSTATbits.D_A & SSPSTATbits.BF ){
			mivar1 = SSPBUF;
			PIR1bits.SSPIF = 0;

		}

		//DATA READ FROM I2C
		if ( !SSPSTATbits.R_W & SSPSTATbits.D_A & SSPSTATbits.BF ){
			commandI2C = SSPBUF;
			PIR1bits.SSPIF = 0;	
		}

		/*
		//Write commands
		if ( SSPSTATbits.R_W ){

			SSPBUF = 0x14;			//data to write
			SSPCON1bits.CKP = 1;

			LATAbits.LATA1 = !PORTAbits.RA1;
			PIR1bits.SSPIF = 0;

		}
		*/

		SSPCON1bits.WCOL = 0; // slew rate
		SSPCON1bits.SSPOV = 0; // slew rate
		SSPCON1bits.CKP = 1;
		PIR1bits.SSPIF = 0;
		IntF = 1;
	}	
}
