#include "main.h"


void gpio_init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01

GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

}


int main(void){

gpio_init();

 while( 1 )
	{
		SET_BIT(GPIOC->ODR, 1<<13);
		for (size_t i = 0; i < 1000000; i++)
		{
		}
		
		CLEAR_BIT(GPIOC->ODR, 1<<13);
		for (size_t i = 0; i < 1000000; i++)
		{
		}
	}
}

