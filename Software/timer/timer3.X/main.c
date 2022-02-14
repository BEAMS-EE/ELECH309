/*
 * Main file of timer3.X
 * Author: mosee
 *
 * This project shows the use of the timer1 interruption solution.
 * The RB15 timing is now as expected and independant of the execution
 * of alongTask()
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
    _TRISB15 = 0;  // Configure RB15 as a digital output pin
    _TRISB13 = 0;
    
    /* Timer1 clock is FCY. Timer1 will count FCY/1000 clock periods in 1ms.
     * As explained in the reference manual, the timer counts from 0 to PR1.
     * Hence its overflow period is PR1+1 periods of FCY 
     * => PR1 = FCY/1000 -1 */
    PR1 = 36849;
    // starts timer1
    T1CONbits.TON = 1;
    // Enable timer1 interrupt, so that its ISR will be called on overflow
    _T1IE = 1;  // _T>1IE is a shortcut for IEC0bits.T1IE

    // Main (infinite) loop
    while(1) {
        // main loop now only contains the dummy task
        _LATB13 = !_LATB13;
        aLongTask();
    }
    
    return 0;
}

/* Timer1 ISR
 * Called automatically when T1IF is set.
 * NEVER CALL AN ISR ANYWHERE IN THE CODE
 */
void __attribute__((interrupt, no_auto_psv))_T1Interrupt(void) {
    // ISR code does the same things that the main loop did in polling
    _T1IF = 0;
    _LATB15 = !_LATB15;
}

