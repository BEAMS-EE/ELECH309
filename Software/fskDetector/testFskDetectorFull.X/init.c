/*
 * File:   configuration.c
 * Author: M. Osée
 *
 * Created on 31 janvier 2013, 13:25
 */


#include "init.h"


// disables the JTAG to prevent conflicts with TRISA
_FICD(JTAGEN_OFF);
// disables the watchdog
_FWDT(FWDTEN_OFF);
// ???
_FGS(GSS_OFF & GCP_OFF & GWRP_OFF);

// The dsPIC will be clocked by the primary oscillator with a 10MHz crystal.
// We want to use the PLL to obtain Fosc = 80MHz ( <=> 40MIPS ).
// Problem : with a 10MHz crystal, PLL constraints are not met with the
// default parameter.
// Solution :
//	- boot using the internal FRC oscillator as clock source,
//	- set the right PLL parameters to obtain Fosc = 80MHz, without violating
//	  the PLL constraints,
//	- switch the clock source to the PLL
//	- wait for the end of the clock switch
//
// Select internal FRC oscillator as clock source
_FOSCSEL(FNOSC_FRC);
// enables clock switching and configure the primary oscillator for a 10MHz crystal
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);



void init(void) {
	// Configures PLL prescaler, PLL postscaler, PLL divisor
	// to obtain Fosc = 80MHz with the 10MHz xt oscillator
	// Fosc = Fin*M/(N1+N2), where :
	// 		N1 = PLLPRE + 2
	// 		N2 = 2 x (PLLPOST + 1)
	//		M = PLLDIV + 2
	PLLFBD = 30;						// M = 32
	CLKDIVbits.PLLPRE = 0;				// N1 = 2
	CLKDIVbits.PLLPOST = 0;				// N2 = 2
	// Initiate Clock Switch to Primary Oscillator with PLL
	__builtin_write_OSCCONH( 3 );
	__builtin_write_OSCCONL( 1 );
	// Wait for Clock switch to occur
	while (OSCCONbits.COSC != 0b011);
}
