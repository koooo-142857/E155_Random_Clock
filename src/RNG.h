// RNG header file
#ifndef RNG_H
#define RNG_H

#include <stdint.h>
#include <stm32l432xx.h> 
// function prototypes. 
void initRNG();
int getRN();
char* getSequence12_SPI();
// long long getSequence12(); 
#endif
