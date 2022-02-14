/*
 * Main file of voltmetre2.X
 * Author: mosee
 *
 * Impl�mentation du voltm�tre en utilisant le timer3. La fr�quence
 * d'�chantillonnage est la plus pr�cise possible pour le dsPIC, car
 * les conversions sont enti�rement g�r�es par le hardware.
 */

#include "xc.h"
#include "adc.h"

#define MAX 2.0



int main(void) {
    float voltage;
    
    // Configuration de L'ADC pour utilisation en polling sur AN0
    adcInit(ADC_TIMER3_SAMPLING);
    TRISB = 0x07FF;
    
    PR3 = 36849;                 // T=10ms=(PR1+1)/3.685MHz => PR1=36849
	T3CONbits.TON = 1;
    
	while(1) {
        // On v�rifie si un nouvel �chantillon est disponible
        if (adcConversionDone()) {
            // On lit le r�sultat
            voltage = ( 3.3*adcRead() ) / 1023;
            
            // On affiche le r�sultat sur le bargraph
            LATB = 0;
            if (voltage > MAX/6) {
                _LATB11 = 1;
            }
            if (voltage > (2*MAX)/6) {
                _LATB12 = 1;
            }
            if (voltage > (3*MAX)/6) {
                _LATB13 = 1;
            }
            if (voltage > (4*MAX)/6) {
                _LATB14 = 1;
            }
            if (voltage > (5*MAX)/6) {
                _LATB15 = 1;
            }
        }
	}
}
