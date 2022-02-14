/* 
 * File:   adc.h
 * Author: mosee
 */

#ifndef ADC_H
#define	ADC_H

/**
 * En-tête de la librairie de l'ADC pour ELEC-H309
 * 
 * Ces fonctions utilisent l'ADC1 du dsPIC pour convertir le signal de AN0.
 * 
 * L'ADC peut être utilisé dans 2 modes, par adcInit() :
 *   - en "manuel" : les conversions sont démarrées en appelant adcStart(). 
 *     C'est donc la boucle principale qui rythme l'échantillonnage
 *   - déclenché par le timer3 : dans ce mode, une conversion est démarrée 
 *     à chaque débordement du timer3. La fréquence d'échantillonnage est alors
 *     définie par PR3.
 * Comme une conversion prend plusieurs cycles d'horloge, il faut utiliser 
 * adcConversionDone() pour "poller" la fin d'une conversion.
 * L'échantillon peut alors être lu avec adcRead().
 */


#define ADC_MANUAL_SAMPLING 0
#define ADC_TIMER3_SAMPLING 2

/**
 * Initialise l'ADC1 pour convertir AN0, sur 10 bits
 * Paramètre:
 *  - mode : définit le mode de fonctionnement. Il peut prendre 2 valeurs :
 *     - ADC_MANUAL_SAMPLING : les conversions sont lancées par adcStart()
 *     - ADC_TIMER3_SAMPLING : la fréquence d'échantillonnage est définie
 *          par le TIMER3. NE PAS UTILISER adcStart() DANS CE MODE !
 */
void adcInit(int mode);


/**
 * Démarre une conversion. A n'utiliser qu'en mode ADC_MANUAL_SAMPLING
 */
void adcStart(void);

/**
 * renvoie 1 si la conversion est terminée, 0 sinon
 */
int adcConversionDone(void);

/**
 * Renvoie le résultat de la dernière conversion.
 * Si on essaie de lire pendant une conversion, la fonction le détecte et 
 * renvoie -32768, qui est une valeur impossible comme résultat de conversion.
 */
int adcRead(void);


#endif	/* ADC_H */

