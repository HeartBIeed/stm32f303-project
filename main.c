#include "main.h"

void GPIO_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}







// ***************** main *****************
int main(void){

PLL_72MHz_enable();
SysTick_init();

GPIO_init();
USART1_init(9600);
SPI1_init();
ST7735_init();
//Encoder_init();
RTC_init();
//ADC_init();
I2C_init();
EXTI_init();
	
	USART1_sendStr("INIT COMPLETE");

ST7735_FillRect(0,0, 128, 160, BLACK); // фоновая заливка


uint32_t start[4] = {0};

 while(1)
{
	USART_commands();
	USART1_echo();
//	_delay_ms(300);
 

 if (ms_ticks - start[3] >= 1000){

	start[3] = ms_ticks;
	AHT_to_USART();
	}
	

 if (ms_ticks - start[2] >= 900){

	start[2] = ms_ticks;
	print_Time();
	}



// блинкер на РС13 1Гц
if (ms_ticks - start[0] >= 1000){

	GPIOC->ODR ^= (1 << 13);
	start[0] = ms_ticks;
	}


}
}



