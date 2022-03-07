/*
 * File:   testFIR.c
 * Author: mosee
 *
 * Created on 25 mars 2020, 11:18
 */


#include "xc.h"

#define M   7
#define D   (1ul << M)
#define T   7

int32_t g = -0.04288*D-.5;
int32_t a[T] = { D, -0.34932*D-.5, -5.97957*D-.5, -11.95342*D-.5, -5.97957*D-.5,
                 -0.34932*D-.5, D };

int32_t max = 0;
int32_t filterInt(int16_t newSample) {
    static int16_t oldSamples[T-1] = {0, 0, 0, 0};
    int32_t y;
    int16_t i;

    y = a[0]*newSample;
    for (i = 1; i<T; i++) {
        y = y + a[i]*oldSamples[i-1];
    }
    if (y > max) {
        max = y;
    }
    else if (-y > max) {
        max = -y;
    }
    y = (y*g);
    y = y >> (2*M);

    // Stored samples are updated
    for (i=T-1; i>0; i--) {
        oldSamples[i] = oldSamples[i-1];
    }
    oldSamples[0] = newSample;
    
    return(y);
}

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


uint32_t volume(uint16_t l, uint16_t L, uint16_t h) {
    uint32_t tmp;
    
    tmp = (uint32_t)l * (uint32_t)L;
    tmp = tmp >> 5;
    h = h >> 2;
	return( tmp * (uint32_t)h );
}


int main(void) {
    int i;
    int32_t x[10] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    int32_t out[10];
    
    uint16_t a, b, c;
    uint32_t aa, bb, cc;
    
    a = 10.23*(1u << 7)+0.5;  // a = 1309 ( <=> 10,2265625 cm), en 2 Tcy
    b = 50.52*(1u << 7)+0.5;  // b = 6467 ( <=> 50,5234375 cm), en 2 Tcy
    // Le périmètre réel est de 121.5 cm
    cc = perimetre1(a, b);  // cc = 15552 (<=> 121,5? cm), en 22 Tcy
    
    c = perimetre2(a, b);   // c = 3886 (<=> 121,4375? cm), en 19 Tcy
    
    c = perimetre3(a, b);   // cc = 3888 (<=> 121,5? cm), en 30 Tcy

    
    a = 10.23*(1u << 4)+0.5;    // a = 164 ( <=> 10,25 cm), en 2 Tcy
    b = 50.6*(1u << 4)+0.5;    // b = 810 ( <=> 50,625 cm), en 2 Tcy
    c = 100*(1u << 4)+0.5;      // c = 1600 ( <=> 100 cm), en 2 Tcy
    // Le volume réel est 51 681,96 cm³
    // Le volume basé sur les valeurs de a,b et c est 51 762,5 cm³
    cc = volume(a, b, c);   // cc = 1660400 (<=> 51 887,5 cm³), en 50 Tcy
    cc = volume(c, a, b);   // cc = 1656400 (<=> 51 762,5 cm³), en 50 Tcy
    cc = volume(b, c, a);   // cc = 1660500 (<=> 51 890,5 cm³), en 50 Tcy
    
    for (i=0; i<10; i++) {
        out[i] = filterInt(x[i]);
    }
    
    while(1);
    
    return 0;
}
