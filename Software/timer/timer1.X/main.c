/*
 * Main file of timer1.X
 * Author: mosee
 *
 * This project shows the limitation of using delays to define accurate
 * time duration.
 * 
 * We want to create a 50kHz square wave on RB15.  Our code also need to
 * execute aLongTask().  This function will mess with the timing of our signal.
 * RB13 is set before calling aLongTask() and reset just after.
 * This allows us the see the execution time of aLongTask().
 */


#define FCY 3685000     // µcycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"         // Contains register variable definitions


/* This function does nothing, but it takes 100us to execute.
 * except each 20th call which takes 1ms to execute.
 * The idea is to simulate a "long" task that doesn't always takes the 
 * same time to execute. */
void aLongTask(void) {
    /* Variable to count the number of function call.
     * A static variable continues to exist when the function ends and its
     * value is kept between function calls. */    
    static int callCount = 0;
    
    if (callCount == 20) {
        callCount = 0;
        __delay_ms(1);
    }
    else {
        callCount++;
        __delay_us(100);
    }
}


int main(void) {
    // Configure RB15 and RB13 as a digital outputs
    // _TRISBx is a define that can be used as a shortcut for TRISBbits.TRISBx
    // (_LAT, _RB also exists)
    _TRISB15 = 0;
    _TRISB13 = 0;
    
    // Main (infinite) loop
    while(1) {
        // should create a 50kHz square wave on RB15 (if aLongTask is not called)
        _LATB15 = !_LATB15;
        __delay_ms(10);
        
        // This simulates a second task of the main loop
        // RB13 is used to see the execution timing of aLongTask
        _LATB13 = 1;
        aLongTask();
        _LATB13 = 0;
    }
    
    return 0;
}

