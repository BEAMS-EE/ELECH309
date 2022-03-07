/*
 * File:   main.c
 * Author: M. Os�e
 *
 * V1.0		09/10/2013
 */


#include "xc.h"
#include "init.h"
#include "FskDetector.h"
#include "uart1.h"
#include <stdio.h>


int parity(int data) {
    int cnt = 0;
    int i;

    for (i=0; i<10; i++) {
        cnt += (data >> i) & 1;
    }
    return (cnt%2);
}


int main(void) {
    int messageRecu;    // message d�cod� par FskDetector()
    int messageEmis;    // message initial
	int trame;          // trame � fournir � FskDetector()



    int bitNb, sampNb;		// index de boucle
	int detLow, detHigh;	// sorties des d�tecteurs


    init();
    RPOR2bits.RP5R = 3;     // connecte U1TX � RP5
    RPINR18bits.U1RXR = 4;  // connecte U1RX � RP4
    uart1Config(115200);    // configure l'uart avec un baud rate de 115200
    uart1GetChar();         // on attend de recevoir un caract�re pour d�marrer
    printf("D�but du test\n");
    for (messageEmis = 0; messageEmis < 0x3FF; messageEmis++) {
        trame = messageEmis << 2;   // on d�cale le message de 2 bits vers la gauche pour laisser la place au bits de parit� et au stop bit
        if (parity(messageEmis) == 1) {   // si le message est de parit� impaire, on met le bit de parit� � 1
            trame |= 2;
        }
        // on envoie les 13 bits de la trame de poids faible en commencant par le MSB
        for (bitNb=12; bitNb>=0; bitNb--) {
            if (trame & (1<<bitNb)) {       // si le bit vaut 1, on aura detLow=0 et detHigh=1;
                detLow = 0;
                detHigh = 1;
            } else {
                detLow = 1;
                detHigh = 0;
            }
            for(sampNb=0; sampNb<OSR; sampNb++){
                messageRecu = fskDetector(detLow, detHigh);
            }
            if ( (messageRecu != 0) && (messageRecu != messageEmis) ) {
                printf("message �mis : %X\n", messageEmis);
                printf("Message re�u : %X\n", messageRecu);
            }
        }
    }
    printf("Test termin�\n");
    while(1);
}
