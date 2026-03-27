#include "encoder.h"

//PB6 TIM4_CH1
//PB7 TIM4_CH2 
//ALT F NO2

void Encoder_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 
	RCC->APB1ENR |= (1 << 2); // TIM4EN

	GPIOB->MODER &= ~(3 << (PB6*2)); // clear PB6 
	GPIOB->MODER |=(2 << (PB6*2)); // AF PB6

	GPIOB->MODER &= ~(3 << (PB7*2)); // clear PB7 
	GPIOB->MODER |=(2 << (PB7*2)); // AF PB7

	GPIOB->AFR[0] &= ~((0xF << (PB6*4))|(0xF << (PB7*4)));  
	GPIOB->AFR[0] |= (2 << (PB6*4))|(2 << (PB7*4)); //AF2

    TIM4->CCMR1 = 0;
    TIM4->SMCR = 0;
    TIM4->CCER = 0;
    TIM4->CR1 = 0;

// TIMx capture/compare mode register 1 
    TIM4->CCMR1 |= (1<<0)|(1<<8); //Bits 1:0 CC1S[1:0]: Capture/Compare 1 selection
// CC1 channel is configured as input, IC1 is mapped on TI1
// CC2 channel is configured as input, IC2 is mapped on TI2


    TIM4->SMCR |= (3 << 0); 
// SMS /0011: Encoder mode 3 - Counter counts up/down on both TI1FP1 and TI2FP2 edges 
// depending on the level of the other input.
 
    TIM4->CCER |= (1<<4)|(1<<0);
// CC1E: Capture/Compare 1 output enable.
// CC2E: Capture/Compare 2 output enable.

 
    TIM4->CR1 |= (1 << 0);//Bit 0 CEN: Counter enable
}

int32_t Encoder_Get_TIM4(){

	return (int32_t)TIM4->CNT;
}





















