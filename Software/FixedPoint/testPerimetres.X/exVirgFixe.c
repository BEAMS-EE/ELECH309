/*
 * File:   exVirgFixe.c
 * Author: mosee
 *
 * Created on 25 mars 2020, 11:18
 */

#include "xc.h"


#define ABS(x) ((x)<0 ? -(x) : (x))


uint32_t perimetre1( uint16_t l, uint16_t L ) {
	return( 2*( (uint32_t)l + (uint32_t)L ) );
}


uint16_t perimetre2( uint16_t l, uint16_t L ) {
	return( 2*( (l >> 2) + (L >> 2) ) );
}


uint16_t perimetre3( uint16_t l, uint16_t L ) {
    uint32_t result;

    result = 2*( (uint32_t)l + (uint32_t)L );
    return( (uint16_t)(result >> 2) );
}


uint32_t surface(uint16_t l, uint16_t L) {
	return( (uint32_t)l * (uint32_t)L );
}


uint32_t volume(uint16_t l, uint16_t L, uint16_t h) {
    uint32_t tmp;
    
    tmp = (uint32_t)l * (uint32_t)L;
    tmp = tmp >> 5;
    h = h >> 2;
	return( tmp * (uint32_t)h );
}


int main(void) {
    uint16_t l16, L16, h16, p16;
    uint32_t p32;
    float lf, Lf, pf, pQ, pCalc, maxErrQ, maxErr1, maxErr2, maxErr3;
    
    
    /* Test 1 : valeurs exactes de l et L
     * 
     * On commence par utiliser des valeurs qui ne sont pas arrondies 
     * par l'utilisation de la virgule fixe 
     * On choisit l = 10.25cm et L = 50.5cm
     * => Le périmètre réel est de 121.5cm 
     * 
     * Le but de ce test est de vérifier si les fonctions renvoie le
     * résultat attendu.
     * On pourra aussi voir le nombre d'instructions des fonctions en utilisant
     * la Stopwatch du simulateur. Elle donne le nombre de cycles d'instructions
     * qui se sont écoulés depuis le début du programme. */


    /* Initialisation de l et L au format u16.7
     *   Pour cela, on les multiplie par 2^7 (=128) 
     *   On l'obtient en décalant 1 de 7 bits vers la gauche, ce qui revient
     *   à multiplier 1 par 2, 7 fois d'affilée. */
    l16 = 10.25*(1u << 7);  // l16 = 1312 ( 1312/128 = 10.25cm )
    L16 = 50.5*(1u << 7);   // L16 = 6464 ( <=> 50.5cm )
    
    // avec perimetre1, on ne doit pas avoir de perte de précision
    p32 = perimetre1(l16, L16);
    // Résultat du test : p32 = 15552 (<=> 121,5cm), en 22 Tcy
    
    // avec perimetre2 et perimetre3, on risque une perte de précision 
    p16 = perimetre2(l16, L16);
    // Résultat du test : p16 = 3888 (<=> 121,5cm), en 19 Tcy

    p16 = perimetre3(l16, L16);
    // Résultat du test : p16 = 3888 (<=> 121,5cm), en 30 Tcy

    /* Conclusions du test 1:
     *   Les fonctions renvoient les bonnes valeurs.
     *   Comme prévu, le temps d'exécution de perimetre3 est plus grand */
    
    
    
    /* Test 2 : valeurs approchées de l et L
     * 
     * On utilise maintenant des valeurs qui sont arrondies 
     * par l'utilisation de la virgule fixe 
     * On choisit l = 10.23cm et L = 50.52cm
     * => Le périmètre réel est toujours de 121.5cm 
     * 
     * Le but de ce test est d'observer la précision de chaque fonction */

    // Initialisation de l et L au format u16.7
    l16 = 10.23*(1u << 7)+0.5;    // l16 = 1309 ( <=> 10,2265625cm )
    L16 = 50.52*(1u << 7)+0.5;    // L16 = 6467 ( <=> 50,5234375cm )
    // Les valeurs contenues dans l16 et L16 ne sont pas les valeurs exactes
    // On peut remarquer que le périmètre vaut toujours 121.5cm
    
    // avec perimetre1, on ne doit pas avoir de perte de précision
    p32 = perimetre1(l16, L16);
    // Résultat du test : p32 = 15552 (<=> 121,5cm), en 22 Tcy
    
    // avec perimetre2 et perimetre3, on risque une perte de précision 
    p16 = perimetre2(l16, L16);
    // Résultat du test : p16 = 3886 (<=> 121,4375cm), en 19 Tcy

    p16 = perimetre3(l16, L16);
    // Résultat du test : p16 = 3888 (<=> 121,5cm), en 30 Tcy

    /* Conclusions du test 2 :
     *   perimetre1 renvoie le périmètre exact, malgré le fait que
     *   ses paramètres soient arrondis.  C'est dû au fait que les
     *   erreurs d'arrondis se compensent.
     *   Par contre, on remarque que perimetre2 ne renvoieplus la bonne
     *   valeur. C'est dû à l'arrondi qu'elle fait sur ses paramètres.
     *   perimetre3 donne la bonne valeur, parce que le perimetre peut être
     *   exprimé exactement en u16.5 et que contrairement à perimetre2,
     *   elle ne fait pas d'arrondi en interne. */


    /* Test 3 : Evaluation de l'erreur maximale
     * 
     * Le but de ce test est de mesurer l'erreur maximale due à  
     * l'utilisation de la virgule fixe.
     * Il y a 2 causes possibles pour cette erreur :
     *   1. la quantification due au codage
     *   2. la perte de précision due aux arrondis dans les fonctions
     * Pour ce test, on balayera les valeurs possibles, par pas de 1cm
     * On utilisera des variables flottantes comme référence.
     * L'erreur de perimetre1 est l'erreur de quantification uniquement */
    maxErrQ = 0;  maxErr1 = 0;  maxErr2 = 0;  maxErr3 = 0;
    
    for (lf=0; lf<=5; lf=lf+0.01) {
        l16 = lf*(1u << 7)+0.5;
        if ( ABS((float)l16/128.0 - lf) > maxErrQ ) {
            maxErrQ = ABS((float)l16/128.0 - lf);
        }
        for (Lf=0; Lf<=5; Lf=Lf+0.01) {
            L16 = Lf*(1u << 7)+0.5;
            pf = 2*(lf+Lf);
            p32 = perimetre1(l16, L16);
            pQ = (float)p32/128.0;
            if ( ABS(pf - pQ) > maxErr1) {
                maxErr1 = ABS(pf - pQ);
            }
            p16 = perimetre2(l16, L16);
            pCalc = (float)p16/32.0;
            if ( ABS(pQ - pCalc) > maxErr2 ) {
                    maxErr2 = ABS( pQ - pCalc);
            }
            p16 = perimetre3(l16, L16);
            pCalc = (float)p16/32.0;
            if ( ABS(pQ - pCalc) > maxErr3 ) {
                    maxErr3 = ABS( pQ - pCalc);
            }
        }
    }
    
    /*
     * Résultat du test 3 :
     * 
     * maxErrQ = 0.00313m
    
    
/*    
    l = 10.23*(1u << 4)+0.5;    // a = 164 ( <=> 10,25 cm), en 2 Tcy
    L = 50.6*(1u << 4)+0.5;    // b = 810 ( <=> 50,625 cm), en 2 Tcy
    c = 100*(1u << 4)+0.5;      // c = 1600 ( <=> 100 cm), en 2 Tcy
    // Le volume réel est 51 681,96 cm³
    // Le volume basé sur les valeurs de a,b et c est 51 762,5 cm³
    cc = volume(a, b, c);   // cc = 1660400 (<=> 51 887,5 cm³), en 50 Tcy
    cc = volume(c, a, b);   // cc = 1656400 (<=> 51 762,5 cm³), en 50 Tcy
    cc = volume(b, c, a);   // cc = 1660500 (<=> 51 890,625 cm³), en 50 Tcy
*/
    
    while(1);
    
    return 0;
}
