/*
 * Main file of voltmetre1.X
 * Author: mosee
 *
 * Implémentation du voltmètre en mode manuel.  La fréquence d'échantillonnage
 * est définie par un délai (ce n'est pas un problème car le code ne fait rien
 * d'autre).
 * Ce n'est pas précis, mais ce n'est pas non plus un problème pour cette
 * application : il suffit que le bargraph soit actualisé suffisament vite,
 * pas besoin d'être précis.
 */

#define FCY 3685000     // µcycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"
#include "adc.h"

#define MAX 2.0



int main(void) {
    float voltage;
    
    // Configuration de L'ADC pour un déclenchement par adcStart())
    adcInit(ADC_MANUAL_SAMPLING);
    TRISB = 0x07FF;
    
	while(1) {
        // On démarre une conversion
        adcStart();
        
        // on attend qu'elle soit finie
        while (!adcConversionDone()) {}
        
        // On lit le résultat
        voltage = ( 3.3*adcRead() ) / 1023;
        
        // On affiche le résultat sur le bargraph
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
        
        // on attend 10ms, pour avoir une fréquence d'échantillonnage de +-100Hz.
        __delay_ms(10);
	}
}
