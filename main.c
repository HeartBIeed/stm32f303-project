#include "main.h"


void gpio_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}


int main(void){

PLL_72MHz_enable();
//SystemClock_HSE_8MHz();
SysTick_init();

gpio_init();
USART1_init(9600);
	USART1_sendStr("USART EN \n\r");
SPI1_init();
	USART1_sendStr("SPI INIT \n\r");
ST7735_init();
	USART1_sendStr("ST7735 INIT \n\r");

ST7735_fill(0xF800);

uint32_t start[3] = {0};
uint8_t led_state = 0;
uint8_t screen_state = 0;



 while( 1 )
{

	USART1_echo();


	if (ms_ticks - start[1] >= 1000)
	{
	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state) 	
		{
			ST7735_fill(0xF800);

		} else {
			ST7735_fill(0x07E0);
		}
	}



	if (ms_ticks - start[0] >= 1000)
	{
	start[0] = ms_ticks;
	led_state ^= 1;

		if (led_state) 	
		{
			CLEAR_BIT(GPIOC->ODR, 1<<13);
		} else {
			SET_BIT(GPIOC->ODR, 1<<13);
		}
	}

			CLEAR_BIT(GPIOC->ODR, 1<<14);
		_delay_us(10);
			SET_BIT(GPIOC->ODR, 1<<14);
		_delay_us(10);

}
}



