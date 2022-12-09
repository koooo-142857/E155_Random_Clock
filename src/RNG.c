#include "STM32L432KC_RCC.h" // this won't work yet. But that is OK. need to copy paste some RCC.h and RCC.c files over first. 
#include "RNG.h" 
#include "STM32L432KC_TIM.h"
#include <stm32l432xx.h>
#include <stdlib.h>
#include <math.h>
#include "STM32L432KC_SPI.h"
#include "STM32L432KC.h"


void initRNG() {
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN; 
  // look into the ConfigureClock module! There, we make sure the dedicated RNG clock is selected to avoid clocking errors. 
  // There is a "three line RNG module" comment over there. Look for it. 
  RNG->CR |= RNG_CR_RNGEN; 
  
}
int getRN() {
  


  int random_integer;
  int CEIS_;
  int DRDY_;
  int SEIS_;
  CEIS_ = (RNG->SR & RNG_SR_CEIS);
  DRDY_ = (RNG->SR | RNG_SR_DRDY); 
  SEIS_ = (RNG->SR & RNG_SR_SEIS);

  while ((CEIS_==1) || (DRDY_==0)|| (SEIS_==1)); 
  //if any of these is true, stall.
  random_integer = (volatile int) RNG->DR;
  return random_integer; 

} 


char* getSequence12_SPI(){
  // big old function that does the random number generation, arranging and SPI transaction in one go. 
  static char out[12]; //the output random assignment of the 12 numbers. 
  char numbers[12] = {1,2,3,4,5,6,7,8,9,10,11,12}; 
  int i,j,k;
  
  for (j=0;j<12;j++) {
    int modrandom;
    modrandom = (abs(getRN())%(12-j)); 
    out[j] = numbers[modrandom]; 
    for (i=modrandom;i<(11-j);i++)
    { numbers[i] = numbers[i+1];}
  }
  
  // SPI part begins
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
  initSPI(1, 0, 0); //br = 1 cpol = 0 cpha = 0
  // some debugger pins (like print statements!)
  /*pinMode(PA5, GPIO_OUTPUT);  // LOAD
  pinMode(PA6, GPIO_INPUT);   // DONE
  pinMode(PA9, GPIO_OUTPUT);
  pinMode(PA10, GPIO_OUTPUT);
  digitalWrite(PA9, 0);
  digitalWrite(PA10, 0);
  // end debugger pins. 
  // begin sending SPI transactions of each character. 
  digitalWrite(PA5,1); // Assert LOAD*/
  for (k = 0;k < 12;k++) {
    spiSendReceive(out[k]); // SPI send is working !!! 12051718 verified with oscilloscope on SCK and MOSI. (3,1,10). 
  }
  while(SPI1->SR & SPI_SR_BSY); // Confirm all SPI transactions are completed
  return out; 
} 




  


   
   

