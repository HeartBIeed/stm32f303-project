#include "main.h"

void GPIO_init(){

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

GPIO_init();
USART1_init(9600);
	USART1_sendStr("USART EN \n\r");
SPI1_init();
	USART1_sendStr("SPI INIT \n\r");
ST7735_init();
	USART1_sendStr("ST7735 INIT \n\r");

	ST7735_FillRect(0,0, 128, 160, 0x0000);
	ST7735_DrawString(3,3, "ST7735 INIT", 0xFFFF);

RTC_init();
	USART1_sendStr("RTC INIT \n\r");
	ST7735_DrawString(3,17, "RTC INIT", 0xFFFF);

ADC_init();
	ST7735_DrawString(3,29, "ADC INIT", 0xFFFF);


uint32_t start[3] = {0};
uint8_t screen_state = 0;

 while(1)
{
	USART_commands();
	USART1_echo();
//	_delay_ms(300);





	if (ms_ticks - start[2] >= 1000)
	{
	start[2] = ms_ticks;

	ST7735_FillRect(0,82, 120, 40, 0x0000);

	uint32_t data_adc = ADC_read()*3300/4095;
	uint32_t data_temp = (data_adc - 907)/15;

	char string[32];
	sprintf(string, "ADC: %lu mV",data_adc);
	ST7735_DrawString(3,82, string, 0xFFFF);

	sprintf(string, "T: %lu *C",data_temp);
	ST7735_DrawString(10,92, string, 0xFFFF);
	}



	if (ms_ticks - start[1] >= 1000)
	{
	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state) 	
		{

		ST7735_FillRect(108,140, 20, 20, 0xF800); // red

		ST7735_FillRect(40,70, 80, 10, 0x0000); // clear time black 
		print_Time();

		} else {

		ST7735_FillRect(108,140, 20, 20, 0x00FF); // blue

		ST7735_FillRect(40,70, 80, 10, 0x0000); // clear time black 
		print_Time();

		}
	}



if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
	}


}
}



