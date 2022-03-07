/*
 * File:   main.c
 * Author: M. Osée
 *
 * V1.0		09/10/2013
 */


#include "xc.h"
#include "init.h"
#include "FskDetector.h"

#define LED  LATBbits.LATB2
#define NB   9      // nombre de trames de test


int main(void) {
    int message;
	int trame[NB][13] = { {0, 0,0, 1,0,1,0,1,0,1,0, 0, 0},    // 0 00 10101010 0 0 : trame correcte, parité 0
                          {0, 0,0, 0,1,0,1,0,1,0,1, 1, 0},    // 0 00 01010101 1 0 : trame avec erreur de parité (parité 0)
                          {0, 0,0, 1,0,1,0,1,0,0,0, 1, 0},    // 0 00 10101000 1 0 : trame correcte, parité 1
                          {0, 0,1, 0,0,0,0,1,1,1,1, 0, 0},    // 0 01 00001111 0 0 : trame avec erreur de parité (parité 1)
                          {0, 0,0, 1,0,1,0,1,0,0,0, 1, 0},    // 0 00 10101000 1 0 : trame correcte, parité 1
                          {1, 0,1, 1,1,1,1,0,0,0,0, 1, 0},    // 1 01 11110000 1 0 : trame avec erreur de start bit
                          {0, 0,0, 1,0,1,0,1,0,0,0, 1, 0},    // 0 00 10101000 1 0 : trame correcte, parité 1
                          {0, 0,1, 0,0,0,0,1,1,1,1, 0, 0},    // 0 10 00110100 0 1 : trame avec erreur de stop bit
                          {0, 0,0, 1,0,1,0,1,0,0,0, 1, 0} };  // 0 00 10101000 1 0 : trame correcte, parité 1

    int result[NB] = { 0b0010101010,    // 0 00 10101010 0 0 : trame correcte, parité 0
                       0,               // 0 00 01010101 1 0 : trame avec erreur de parité (parité 0)
                       0b0010101000,    // 0 00 10101000 1 0 : trame correcte, parité 1
                       0,               // 0 01 00001111 0 0 : trame avec erreur de parité (parité 1)
                       0b0010101000,    // 0 00 10101000 1 0 : trame correcte, parité 1
                       0,                // 1 01 11110000 1 0 : trame avec erreur de start bit
                       0b0010101000,    // 0 00 10101000 1 0 : trame correcte, parité 1
                       0,               // 0 10 00110100 0 1 : trame avec erreur de stop bit
                       0b0010101000};   // 0 00 10101000 1 0 : trame correcte, parité 1
    
    int trameNb, bitNb, sampNb;		// index de boucle
	int detLow, detHigh;	// sorties des détecteurs

    init();
    LED = 0;
    AD1PCFGLbits.PCFG2 = 1;     // configure la patte de la LED en I/O logique
    TRISBbits.TRISB2 = 0;       // configure la patte de la LED en sortie

    // on envoie les 13 bits de la trame de poids faible en commencant par le MSB
    for (trameNb = 0; trameNb < NB; trameNb++) {
        for (bitNb=0; bitNb<13; bitNb++) {
            if (trame[trameNb][bitNb] == 1) { // si le bit vaut 1, on aura detLow=0 et detHigh=1;
                detLow = 0;
                detHigh = 1;
            } else {
                detLow = 1;
                detHigh = 0;
            }
            for (sampNb = 0; sampNb < OSR; sampNb++) {
                message = fskDetector(detLow, detHigh);
            }
        }
        if (message != result[trameNb]) {
            while (1); //
        }
        for (sampNb = 0; sampNb < OSR; sampNb++) {  // après une trame, on simule un bit de silence pour réinitialiser la machine d'état
            fskDetector(0,0);
        }
    }
    LED = 1;
    while(1);
}
