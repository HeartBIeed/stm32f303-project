#ifndef EXTINT_H_
#define EXTINT_H_

#include "main.h"

#define PB0 0 

void EXTI_init();
void EXTI0_1_IRQHandler(void);

volatile extern uint32_t count;


#endif /* EXTINT_H_ */
