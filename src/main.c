// Ruth Mueller, Kevin Wan, E155 Random Clock
// 12/08/22
// rmueller@hmc.edu,kewan@hmc.edu
int main(void) {

  counter_init();
  initialize();

  char sec = 1;
  char min = 1;
  char hour = 1;
  char index = 0;
  int old_hour_pos = 180;  // even setting it like this got really weird
  int old_min_pos = 180;
  TIM16->CCR1 = 180; // min starting location
  TIM15->CCR2 = 180; // hour starting location


  int min_angles[12] =  {290, 278, 270, 260, 250, 239, 230, 222, 214, 205, 190, 305};
  int hr_angles[12] = {195, 210, 224, 235, 249, 260, 272, 284, 300, 309, 325, 336};

                 //  pos1, pos2, pos3, ...
  char *random;


  // initial clock
  random = getSequence12_SPI(); // this function inits SPI inside of itself. And sends the SPI by itself. 
       for(int i = 0; i<12; i++){printf("%d \n", random[i]);}// for debugging, check the random sequence
  
  for(int i = 0; i<12; i++){if(random[i] == hour)index = i;}
  hour_AtoB(old_hour_pos, hr_angles[index]);// move hour hand
  old_hour_pos = hr_angles[index];

  for(int i = 0; i<12; i++){if(random[i] == min/5+1)index = i;}
  printf("min calculation min%5+1 = %d", min/5+1);
  min_AtoB(old_min_pos, min_angles[index]); // move minuite hand
  old_min_pos = min_angles[index];

  // randomize


  while(1) {
    
      

      if(sec == 60){  //////////
          
          if(min % 5 == 0 && min != 60){
            for(int i = 0; i<12; i++){if(random[i] == min/5+1)index = i;}
            min_AtoB(old_min_pos, min_angles[index]); // move minuite hand
            old_min_pos = min_angles[index];
            printf("five min passed min: %d min_pos: %d\n", min, old_min_pos);
          }

          if(min == 60){
            printf("an hour passed \n");
            random = getSequence12_SPI(); // this function inits SPI inside of itself. And sends the SPI by itself. 
            for(int i = 0; i<12; i++){printf("%d \n", random[i]);}// for debugging, check the random sequence
            hour++;

            if(hour + 1 == 13){
              hour = 1;
              printf("a day passed \n");
            }
          
          min = 1;
          // move min hand
          // move hour hand
            for(int i = 0; i<12; i++){if(random[i] == hour)index = i;}
            hour_AtoB(old_hour_pos, hr_angles[index]);// move hour hand
            old_hour_pos = hr_angles[index];
            for(int i = 0; i<12; i++){if(random[i] == min/5+1)index = i;}
            min_AtoB(old_min_pos, min_angles[index]); // move minuite hand
            old_min_pos = min_angles[index];
          }

          sec = 1;
          min++;
      }
      

      delay_1s();
      sec++;    
  }

  
}

// looping hands test for testing the angles
void loopHands(int hr_angles[12], int min_angles[12]){     
    for(int i=0; i<11; i++){
      hour_AtoB(hr_angles[i], hr_angles[i+1]);
      min_AtoB(min_angles[i], min_angles[i+1]);
      for(volatile int i = 0; i< 100000; i++){}; 
    }
    hour_AtoB(hr_angles[11], hr_angles[0]);
    min_AtoB(min_angles[11], min_angles[0]);
    for(volatile int i = 0; i< 100000; i++){}; 
}

void delay_1s(){
  //for(volatile int i = 0; i< 358000; i++){}; // for default 4 MHz clock
  //for(volatile long i = 0; i< 358000*20; i++){}; // divide by 60 for a minuite per second
  for(volatile long i = 0; i< 358000*20 / 6; i++){}; // divide by 60 for a minuite per second
}

// this spins both hands together as a test
void spin_hands_test(){ 
     //--------------------------  DEMONSTRATES SPINNING
    for(int j = 0; j<=180; j++){
       for(volatile int i = 0; i< 10000; i++){}; // smooth mostion wait
       TIM16->CCR1 = 180 + j;
       TIM15->CCR2 = 360 - j;
     }
     for(volatile int i = 0; i< 10000; i++){};

     for(int j = 180; j > 0; j--){
       for(volatile int i = 0; i< 10000; i++){}; // smooth motion wait
       TIM16->CCR1 = 180 + j;
       TIM15->CCR2 = 360 - j;
     }
     for(volatile int i = 0; i< 100000; i++){};
     
    //------------------------------
}

// this slowly steps between two servo inputs
void hour_AtoB(int start, int finish){
  if(finish > start){
    for(int j = 0; j<=(finish - start); j++){
       for(volatile long i = 0; i< 10000*20; i++){}; // smooth mostion wait, mult by 20 for 80 MHz instead of 4MHz
       TIM15->CCR2 = start + j;
     }
  }
  else{  // when finish < start
    for(int j = 0; j<=(start - finish); j++){
       for(volatile long i = 0; i< 10000*20; i++){}; // smooth mostion wait, mult by 20 for 80 MHz instead of 4MHz
       TIM15->CCR2 = start - j;
     }
  }
  
}


// this slowly steps between two servo inputs
void min_AtoB(int start, int finish){
  if(finish > start){
    for(int j = 0; j<=(finish - start); j++){
       for(volatile long i = 0; i< 10000*20; i++){}; // smooth mostion wait, mult by 20 for 80 MHz instead of 4MHz
       TIM16->CCR1 = start + j;
     }
  }
  else{  // when finish < start
    for(int j = 0; j<=(start - finish); j++){
       for(volatile long i = 0; i< 10000*20; i++){}; // smooth mostion wait, mult by 20 for 80 MHz instead of 4MHz
       TIM16->CCR1 = start - j;
     }
  }
  
}

//______________________________________INITIALIZE_________________________________

void initialize(){

  // for random number generation
  configureFlash();
  configureClock(); // DON'T FORGET THESE!  takes a 4 MHz clk to 80 MHz clk
  initRNG();

  // turning on the clock to let GPIOB work:
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; 
  // turning on the clock to let GPIOA work:
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  
  // setting output mode for pin b_3:
  GPIOB->MODER &= ~_VAL2FLD(GPIO_MODER_MODE3, 0b11); // clear the bits first
  GPIOB->MODER |= _VAL2FLD(GPIO_MODER_MODE3, 0b01); // set the bits

  // setting output mode for pin A_6:  // set as '10' for TIM16
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE6, 0b11);  // clear the bits first
  GPIOA->MODER |= _VAL2FLD(GPIO_MODER_MODE6, 0b10);

  // setting the alternate function for pin A_6:
  GPIOA->AFR[0] &= ~_VAL2FLD(GPIO_AFRL_AFSEL6, 0b1111); // clear the bits first  
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 0b1110);   

  // setting output mode for pin A_3:  // currently setting '10' for TIM15
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE3, 0b11); // clear the bits first
  GPIOA->MODER |= _VAL2FLD(GPIO_MODER_MODE3, 0b10);

  // setting the alternate function for pin A_3:
  GPIOA->AFR[0] &= ~_VAL2FLD(GPIO_AFRL_AFSEL3, 0b1111); // clear the bits first
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 0b1110);  // alternate funciton 14

  return;
}


//______________________________________COUNTER_INIT_________________________________

void counter_init(){
  // turning on the clock for timers
  RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;


  
  // TIM15 work
   
    //TIM15_EGR |= (1 << 0); // write 1 to UG flag to get the prescale to work 
    TIM15->EGR |= TIM_EGR_UG;

    // enable preload register 
    TIM15->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC2PE, 0b1);  //*potentially OC1PE

    // set the CCMR1 to 0b0111 for PWM mode 2   
    TIM15->CCMR1 &= _VAL2FLD(TIM_CCMR1_OC2M, 0b111);  
    TIM15->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC2M, 0b111); 

    // set ARPE
    TIM16->CR1 |= _VAL2FLD(TIM_CR1_ARPE, 0b1);
    // enable MOE
    TIM15->BDTR|= _VAL2FLD(TIM_BDTR_MOE, 0b1);

    // enable output 
    TIM15->CCER |=_VAL2FLD(TIM_CCER_CC2E, 0b1);

    // set output active low (flipping it back bc it's already flipped for some reason)
    TIM15->CCER |= _VAL2FLD(TIM_CCER_CC2P, 0b1);  // 


  // turn on PWM mode, OCxM set to 110 for PWM mode 1  
  TIM16->CCMR1 &= ~_VAL2FLD(TIM_CCMR1_OC1M, 0b111); // clear it first
  TIM16->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC1M, 0b110);
  // enable MOE
  TIM16->BDTR|= TIM_BDTR_MOE;

  //// prescaling to divide by 11, for a clock of 4MHz (default)
  //TIM16->PSC = 21;  
  //TIM15->PSC = 21; 

  // prescaling to divide by , for a clock of 80 MHz (when PLL is configured)
  TIM16->PSC = 443;  // psc[15:0]
  TIM15->PSC = 443; 

  // restarting at 200:
  TIM16->ARR = 3600;
  TIM15->ARR = 3600;
  TIM15->CCR2 = 180; // duty cycle
  TIM16->CCR1 = 180;  // duty cycle,

  TIM16->CCER |= _VAL2FLD(TIM_CCER_CC1E, 0b1);  // setting CC1E

    // turn on count enable
  TIM16->CR1 |= TIM_CR1_CEN;
  TIM15->CR1 |= TIM_CR1_CEN;


  return;
}
