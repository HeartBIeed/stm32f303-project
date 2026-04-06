#ifndef ENC_H_
#define ENC_H_

#include "main.h"

//PB6 TIM4_CH1
//PB7 TIM4_CH2 
//ALT F NO4

#define PB5 5 // BUTTON
#define PB6 6
#define PB7 7

void Encoder_init();
int32_t Encoder_Get_TIM4();

#endif /* ENC_H_ */
