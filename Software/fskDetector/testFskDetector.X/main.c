/*
 * File:   main.c
 * Author: M. Osée
 *
 * V1.0		09/10/2013
 */


#include "xc.h"
#include "FskDetector.h"
#include <stdio.h>

#define AVANCE	0
#define DROITE	1
#define GAUCHE	2
#define STOP	3

void Test(int message, int parity);
void PrintMessage(int data);
void PrintTrame(int data);
void Silence(void);
void Bruit(void);


int main(void) {
	printf("\n\nTrame correcte, parite paire");
	Test(0x0AA, 0);
	printf("\nTrame correcte, parite impaire");
	Test(0x155, 1);
	printf("\nTrame avec erreur de parite (paire)");
	Test(0x30F, 1);
	// On "envoie" un bit de silence pour réinitialiser la machine d'état après une trame erronnée
	printf("\nUne periode de bit de silence\n");
	Silence();
	// On teste une trame correcte pour vérifier que la machine d'état est bien réinitialisée
	printf("\nTrame correcte, parite paire");
	Test(0x1AB, 0);
	printf("\nTrame avec erreur de parite (impaire)");
	Test(0x2F0,0);
	// On "envoie" un bit de bruit pour réinitialiser la machine d'état après une trame erronnée
	printf("\nUne periode de bit de bruit\n");
	Bruit();
	// On teste une trame correcte pour vérifier que la machine d'état est bien réinitialisée
	printf("\nTrame correcte, parite impaire");
	Test(0x2AB, 0);

	return 0;
}



void Test(int message, int parity) {
	int trame;
	int bitNb, sampNb;		// indexs de boucle
	int detLow, detHigh;	// sorties des détecteurs

	printf("\nMessage initial :  ");
	PrintMessage(message);							// on affiche le message en binaire
	trame = (message<<2) + (parity<<1);				// on construit la trame
	printf("Trame initiale : ");
	PrintTrame(trame);								// on affiche la trame en binaire
	// Test
	for (bitNb=12; bitNb>=0; bitNb--) {
		if (trame&(1<<bitNb)) {
			detLow = 0;
			detHigh = 1;
		} else {
			detLow = 1;
			detHigh = 0;
		}
		for(sampNb=0; sampNb<OSR; sampNb++){
			message = FskDetector(detLow, detHigh);
		}
	}	if (message) {
		printf("Message decode :   ");
		PrintMessage(message);
	}
}


void Silence(void) {
	int sampNb;

	for(sampNb=0; sampNb<OSR; sampNb++){FskDetector(0, 0);}
}


void Bruit(void) {
	int sampNb;

	for(sampNb=0; sampNb<OSR; sampNb++){FskDetector(1, 1);}
}


void PrintMessage(int data) {
	int i;

	for (i=9; i>7; i--) {					// l'ordre se trouve dans les bits b9-b8 de data
		printf("%i", (data&(1<<i))>>i);		// on utilise un masque pour récupérer le bit qui nous intéresse
	}										// et on le shifte à droite pour qu'il deviennent b0
	printf("|");
	for (i=7; i>=0; i--) {
		printf("%i", (data&(1<<i))>>i);		// On fait de même pour les 8 bits du paramètre
	}
	printf("\n");
}


void PrintTrame(int data) {
	int i;

	printf("%i|", (data&(1<<12))>>12);		// le START BIT est le 'b12' de la trame
	for (i=11; i>9; i--) {
		printf("%i", (data&(1<<i))>>i);		// ensuite, viennent les 2 bits de l'ordre
	}
	printf("|");
	for (i=9; i>1; i--) {
		printf("%i", (data&(1<<i))>>i);		// puis les 8 bits du paramètre,
	}
	printf("|");
	printf("%i|", (data&2)>>1);				// le bit de parité
	printf("%i\n", (data&1));				// et le STOP BIT
}