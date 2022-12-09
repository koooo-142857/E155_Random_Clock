
int main(void) {

  counter_init();
  initialize();

  char sec = 0;
  char min = 0;
  char hour = 0;
  char old_hour_pos = 180;


  int min_angles[12] =  {290, 278, 270, 260, 250, 239, 230, 222, 214, 205, 190, 305};
  int hr_angles[12] = {240, 247, 258, 269, 278, 287, 298, 309, 190, 207, 217, 227};

                      //  pos1, pos2, pos3, ...
  int rand_sequence[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  int min_hand = 180; // minuite hand starting location
  int hr_hand = 180; // hour hand starting location
  for(volatile int i = 0; i< 100000; i++){}; // smooth mostion wait
  TIM16->CCR1 = min_hand;
  TIM15->CCR2 = hr_hand;

  // randomize

  while(1) {
     
     
 
   
    int angle_list[12] = {195, 210, 224, 235, 249, 260, 272, 284, 300, 309, 325, 336};

    for(int i=0; i<11; i++){
      CCR2_AtoB(angle_list[i], angle_list[i+1]);
      CCR1_AtoB(angle_list[i], angle_list[i+1]);
      for(volatile int i = 0; i< 100000; i++){}; 
    }
    CCR2_AtoB(angle_list[11], angle_list[0]);
    CCR1_AtoB(angle_list[11], angle_list[0]);
    for(volatile int i = 0; i< 100000; i++){}; 
        
  }
}



void delay_1s(){
  for(volatile int i = 0; i< 358000; i++){};
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
void CCR2_AtoB(int start, int finish){
  if(finish > start){
    for(int j = 0; j<=(finish - start); j++){
       for(volatile int i = 0; i< 10000; i++){}; // smooth mostion wait
       TIM15->CCR2 = start + j;
     }
  }
  else{  // when finish < start
    for(int j = 0; j<=(start - finish); j++){
       for(volatile int i = 0; i< 10000; i++){}; // smooth mostion wait
       TIM15->CCR2 = start - j;
     }
  }
  
}


// this slowly steps between two servo inputs
void CCR1_AtoB(int start, int finish){
  if(finish > start){
    for(int j = 0; j<=(finish - start); j++){
       for(volatile int i = 0; i< 10000; i++){}; // smooth mostion wait
       TIM16->CCR1 = start + j;
     }
  }
  else{  // when finish < start
    for(int j = 0; j<=(start - finish); j++){
       for(volatile int i = 0; i< 10000; i++){}; // smooth mostion wait
       TIM16->CCR1 = start - j;
     }
  }
  
}

//______________________________________INITIALIZE_________________________________

void initialize(){

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

  // prescaling to divide by 11
  TIM16->PSC = 21;  
  TIM15->PSC = 21; 

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
