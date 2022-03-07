#include <xc.h>
#include "init.h"
#include "uart1.h"


#define ABS(x) ((x) < 0 ? - (x) : (x))

void uart1Config(float baudrate) {
	float tmp;

	if ((baudrate > 152) && (baudrate < 10E6)) {
		U1MODEbits.UARTEN = 0;				// désactive le périphérique avant de le configurer
		U1MODE = 0;					// spécifie le mode de fonctionnement
		tmp = FCY/(16*baudrate) - 1;			// on calcule la valeur exacte à placer dans le compteur pour obtenir le baudrate désiré
		U1MODEbits.BRGH = 0;				// on décide de toujours fonctionner en high speed (cela nous limite à 152 baud minimum)
		U1BRG = (unsigned int)tmp;			// on assigne la valeur calculée pour le baudrate, ce qui la tronque à l'entier inférieur
		if (tmp-U1BRG > 0.5) {				// on vérifie si il n'aurait pas mieux valu arrondir à l'entier supérieur
			U1BRG++;							// si oui, on le fait
		}
		tmp = FCY/(16*(float)(U1BRG + 1));
		if (ABS(tmp-baudrate) > baudrate/50) {
			U1MODEbits.BRGH = 1;				// on décide de toujours fonctionner en high speed (cela nous limite à 152 baud minimum)
			tmp = FCY/(4*baudrate) - 1;
			U1BRG = (unsigned int)tmp;			// on assigne la valeur calculée pour le baudrate, ce qui la tronque à l'entier inférieur
			if (tmp-U1BRG > 0.5) {				// on vérifie si il n'aurait pas mieux valu arrondir à l'entier supérieur
				U1BRG++;							// si oui, on le fait
			}
		}
		U1MODEbits.UARTEN = 1;				// on active l'UART
		U1STAbits.UTXEN = 1;				// on active l'émission (Attention, cela active aussi U1TXIF)
	}
}


inline void uart1SendChar(unsigned char data) {
	while(U1STAbits.UTXBF);
	U1TXREG = data;
}


inline unsigned char uart1GetChar(void) {
	while(U1STAbits.URXDA == 0);
	return U1RXREG;
}

inline int uart1RxDataAvailable(void) {
	return U1STAbits.URXDA;
}

inline int uart1TxReady(void) {
	return !U1STAbits.UTXBF;
}
