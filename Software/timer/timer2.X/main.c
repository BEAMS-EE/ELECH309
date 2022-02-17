/*
 * Main file of timer2.X
 * Author: mosee
 *
 * This project shows the polling on hardware timer solution.
 * The RB15 timing is still not perfect, but the mean value of its period
 * is correct.
 * 
 * We want to create a 50kHz square wave on RB15.  Our code also need to
 * execute aLongTask().  This function will mess with the timing of our signal.
 * RB13 is toggled before calling aLongTask() to see its execution time.
 * As the execution time of the T1IF polling is very small in comparison,
 * it's more readable on the ocilloscope.
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
    _TRISB15 = 0;
    _TRISB13 = 0;
    
    /* Timer1 clock is FCY. Timer1 will count FCY/1000 clock periods in 1ms.
     * As explained in the reference manual, the timer counts from 0 to PR1.
     * Hence its overflow period is PR1+1 periods of FCY 
     * => PR1 = FCY/1000 -1 */
    PR1 = 36849;
    // starts timer1
    T1CONbits.TON = 1;

    // Main (infinite) loop
    while(1) {
        // RB15 signal creation.  Better than timer1.X
        if (IFS0bits.T1IF) {
            IFS0bits.T1IF = 0;  // _T1IF shortcut exists
            _LATB15 = !_LATB15;
        }
        
        // we change the way we use RB13 to be more readable on an oscilloscope.
        _LATB13 = !_LATB13;
        //_LATB13 = 1;
        aLongTask();
        //_LATB13 = 0;
    }
    
    return 0;
}
