/*
 * Main file of gpio3.X
 * Author: mosee
 *
 * A LED is connected to RB15 and a push-button is connected to RB14.
 * When the button is pressed, the LED is turned on for 5s.
 * The button has no effect during this 5s.
 * 
 * Created on 27 January 2021, 11:32
 */


#define FCY 3685000     // µcycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"         // Contains register variable definitions

int main(void) {
    /* Setup
     * Configure RB14 as an input is not mandatory, as it is its default state.
     * There are two main reasons to do it:
     *  - All the pins used appears in the setup, so that reading the
     *    setup code gives an idea of the circuit.
     *  - It may happens that initialisation functions are called in the setup.
     *    These functions (if poorly written), may change the state of SFR */
    TRISBbits.TRISB15 = 0;  // Configure RB15 as a digital output pin
    TRISBbits.TRISB14 = 1;  // Configure RB14 as a digital input pin

    // Main (infinite) loop
    while(1) {
        if (PORTBbits.RB14) {
            LATBbits.LATB15 = 1;
            __delay_ms(5000);
            LATBbits.LATB15 = 0;
        }
    }
    
    return 0;
}

