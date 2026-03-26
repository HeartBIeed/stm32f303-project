#include "main.h"

void GPIO_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}


void Encoder_Test(){

	int32_t position = Encoder_Get_TIM4();
	char buffer[16];  
	sprintf(buffer, "%ld", position); 

	ST7735_FillRect(10,112, 100, 10, 0x0000);
	ST7735_DrawString(10,112,buffer, 0xFFFF);
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
 


// Тест энкодера с выводом на дисплей
 if (ms_ticks - start[3] >= 200)
	{
	start[3] = ms_ticks;
	Encoder_Test(); 
	}
		

// Вывод  АЦП и температуры на дисплей
	if (ms_ticks - start[2] >= 1000)
	{
	start[2] = ms_ticks;

	ST7735_FillRect(0,82, 120, 40, 0x0000);

	uint32_t data_adc = ADC_read()*3300/4095;
	uint32_t data_temp = (data_adc - 907)/15;

	char string[32];
	sprintf(string, "ADC: %lu mV",data_adc);
	ST7735_DrawString(3,82, string, 0xFFFF); // ADC

		sprintf(string, "ADC: %lu \r\n",data_adc);
		USART1_sendStr(string);

	sprintf(string, "T: %lu *C",data_temp);
	ST7735_DrawString(10,92, string, 0xFFFF); // temperature

		sprintf(string, "T: %lu \r\n",data_temp);
		USART1_sendStr(string);

	}


// Вывод отладочных квадратов 1Гц на дисплей
// и времени с РТС
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


// блинкер на РС13 1Гц
if (ms_ticks - start[0] >= 1000) 
	{
		GPIOC->ODR ^= (1 << 13);
		start[0] = ms_ticks;
	}


}
}



