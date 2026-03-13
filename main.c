#include "main.h"

uint8_t hour;
uint8_t min;
uint8_t sec;

void GPIO_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}

int USART_commands(){

	if (strncmp((char*)usart_data_buffer,"st",2) == 0) 
	{

		char *command = strtok((char*)usart_data_buffer," ");
		char *h_char = strtok(NULL, ",");
		char *m_char = strtok(NULL, ",");

		int h = atoi(h_char);
		int m = atoi(m_char);

		RTC_setTime(h,m);


		char string[32];
		sprintf(string, "SET TIME -> %2d:%2d \r\n",h,m);
		USART1_sendStr(string);
	 	usart_data_buffer[0] = '\0';

	 		return 1;

	} else {
			return 0;
	}
		
}

void print_Time(){

RTC_getTime(&hour,&min,&sec);
char text[20]={"\0"};	
sprintf(text, "TIME %02d:%02d:%02d \r\n",hour,min,sec);
ST7735_DrawString(3,72, text, 0xFFFF);

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


uint32_t start[3] = {0};
uint8_t led_state = 0;
uint8_t screen_state = 0;

 while(1)
{
	USART_commands();
	USART1_echo();

	if (ms_ticks - start[1] >= 1000)
	{
	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state) 	
		{

		ST7735_FillRect(108,140, 20, 20, 0xF800);

		ST7735_FillRect(40,70, 80, 30, 0x0000);
		print_Time();

		} else {

		ST7735_FillRect(108,140, 20, 20, 0x00FF);

		ST7735_FillRect(40,70, 80, 30, 0x0000);
		print_Time();

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

}
}



