/*
 * Main file of voltmetre1.X
 * Author: mosee
 *
 * Impl�mentation du voltm�tre en mode manuel.  La fr�quence d'�chantillonnage
 * est d�finie par un d�lai (ce n'est pas un probl�me car le code ne fait rien
 * d'autre).
 * Ce n'est pas pr�cis, mais ce n'est pas non plus un probl�me pour cette
 * application : il suffit que le bargraph soit actualis� suffisament vite,
 * pas besoin d'�tre pr�cis.
 */

#define FCY 3685000     // �cycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"
#include "adc.h"

#define MAX 2.0



int main(void) {
    float voltage;
    
    // Configuration de L'ADC pour un d�clenchement par adcStart())
    adcInit(ADC_MANUAL_SAMPLING);
    TRISB = 0x07FF;
    
	while(1) {
        // On d�marre une conversion
        adcStart();
        
        // on attend qu'elle soit finie
        while (!adcConversionDone()) {}
        
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
        
        // on attend 10ms, pour avoir une fr�quence d'�chantillonnage de +-100Hz.
        __delay_ms(10);
	}
}
