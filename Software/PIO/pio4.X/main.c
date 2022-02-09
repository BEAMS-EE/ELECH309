/*
 * Main file of gpio4.X
 * Author: mosee
 *
 * A LED is connected to RB15 and a push-button is connected to RB14.
 * When the button is pressed, the LED is toogled (its state changes).
 */


#define FCY 3685000     // µcycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"         // Contains register variable definitions


int main(void) {
    // Setup
    TRISBbits.TRISB15 = 0;  // Configure RB15 as a digital output pin
    TRISBbits.TRISB14 = 1;  // Configure RB14 as a digital input pin

    // Main (infinite) loop
    while(1) {
        /* When  the button is pressed, the LED is toggled. Then we wait for
         * the release of the button.  This avoid multiple toggling for one
         * button pressing. The delays "filters" button bouncing. */
        if (PORTBbits.RB14) {
            LATBbits.LATB15 = !LATBbits.LATB15;
            __delay_ms(100);
            while(PORTBbits.RB14) {
            }
            __delay_ms(100);
        }
    }
    
    return 0;
}

