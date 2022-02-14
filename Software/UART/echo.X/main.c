/*
 * Main file of echo.X
 * Author: mosee
 *
 * Projet servant à tester une connexion UART.
 * RB6 sert de patte Rx du dsPIC et RB7 sert de patte Tx.
 * L'UART du dsPIC est configuré en 8N1 à 57.6 kb/s
 * Le dsPIC implémente un simple écho : il écoute son Rx et renvoie tout ce 
 * qu'il reçoit sur son Tx.
 */

#include "xc.h"
#include "adc.h"



int main(void) {
    unsigned char c;
    
    /* Configuration du Peripheral Pin Select (PPS) pour connecter le signal
     * Rx de l'UART1 à RB6/RP6 et le signal Tx à RB7/RP7 */
	_U1RXR = 6;    // U1RX -> RP6
	_RP7R = 3;     // RP7 -> U1Tx

    // Configuration de l'UART1 avec un format de trame 8N1, à 57600 bits/s
    U1MODEbits.PDSEL = 0;       // 8 bits, pas de parité
    U1MODEbits.STSEL = 0;       // 1 stop bit
    /* L'UART peut fonctionner à 2 vitesses différentes. Le mode "high speed" 
     * est plus sensible au bruit et ne doit donc être utilisé que pour
     * des débits trop rapides pour le mode standard 
     * En mode standard, le débit est donné par :
     * baud rate = FCY / (16*(U1BRG+1) 
     * => U1BRG = (3.685MHz / (16*57.6kHz)) - 1  =  2.998*/
    U1MODEbits.BRGH = 0;
    U1BRG = 3;
    
    U1MODEbits.UARTEN = 1;      // on active l'UART
    U1STAbits.UTXEN = 1;        // on active l'émission
    
    // Boucle principale
	while(1) {
        if (U1STAbits.URXDA) {      // si UART1 a reçu un octet
            c = U1RXREG;                // on le lit
            while (U1STAbits.UTXBF) {}  // on attend que le buffer d'émission soit libre
            U1TXREG = c;                // et on le renvoie
        }
        
	}
}
