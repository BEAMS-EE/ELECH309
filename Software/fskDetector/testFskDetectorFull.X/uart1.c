#include <xc.h>
#include "init.h"
#include "uart1.h"


#define ABS(x) ((x) < 0 ? - (x) : (x))

void uart1Config(float baudrate) {
	float tmp;

	if ((baudrate > 152) && (baudrate < 10E6)) {
		U1MODEbits.UARTEN = 0;				// d�sactive le p�riph�rique avant de le configurer
		U1MODE = 0;					// sp�cifie le mode de fonctionnement
		tmp = FCY/(16*baudrate) - 1;			// on calcule la valeur exacte � placer dans le compteur pour obtenir le baudrate d�sir�
		U1MODEbits.BRGH = 0;				// on d�cide de toujours fonctionner en high speed (cela nous limite � 152 baud minimum)
		U1BRG = (unsigned int)tmp;			// on assigne la valeur calcul�e pour le baudrate, ce qui la tronque � l'entier inf�rieur
		if (tmp-U1BRG > 0.5) {				// on v�rifie si il n'aurait pas mieux valu arrondir � l'entier sup�rieur
			U1BRG++;							// si oui, on le fait
		}
		tmp = FCY/(16*(float)(U1BRG + 1));
		if (ABS(tmp-baudrate) > baudrate/50) {
			U1MODEbits.BRGH = 1;				// on d�cide de toujours fonctionner en high speed (cela nous limite � 152 baud minimum)
			tmp = FCY/(4*baudrate) - 1;
			U1BRG = (unsigned int)tmp;			// on assigne la valeur calcul�e pour le baudrate, ce qui la tronque � l'entier inf�rieur
			if (tmp-U1BRG > 0.5) {				// on v�rifie si il n'aurait pas mieux valu arrondir � l'entier sup�rieur
				U1BRG++;							// si oui, on le fait
			}
		}
		U1MODEbits.UARTEN = 1;				// on active l'UART
		U1STAbits.UTXEN = 1;				// on active l'�mission (Attention, cela active aussi U1TXIF)
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
