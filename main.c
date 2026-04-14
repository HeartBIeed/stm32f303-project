#include "main.h"

void GPIO_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}



void EXINT_Test(){

	char buffer[16];  
	sprintf(buffer, "%ld", count); 

	ST7735_FillRect(50,30, 100, 18, BLACK);
	ST7735_DrawString(50,30,buffer, MAGENTA,Font_7x10);
}

void W25Q_printID(){

	char buffer[32];  
	sprintf(buffer, "%ld",  W25Q_ReadID()); 

	ST7735_FillRect(30,100, 100, 18, BLACK);
	ST7735_DrawString(30,100,buffer, RED,Font_7x10);

}



// ***************** main *****************
int main(void){

PLL_72MHz_enable();
SysTick_init();

GPIO_init();
USART1_init(9600);
SPI1_init();
ST7735_init();
Encoder_init();
RTC_init();
//ADC_init();
I2C_init();
EXTI_init();

ST7735_FillRect(0,0, 128, 160, BLACK); // фоновая заливка
bresenhamCircleFill(40, 140, 10, RED);

	_delay_ms(10);
W25Q_printID();
	_delay_ms(10);

uint32_t start[4] = {0};
uint8_t screen_state = 0;

 while(1)
{
	USART_commands();
	USART1_echo();
//	_delay_ms(300);
 
	MENU_Encoder();



	

 if (ms_ticks - start[2] >= 7000){

	start[2] = ms_ticks;
	I2C_scan();
	}

// Тест энкодера с выводом на дисплей
 if (ms_ticks - start[3] >= 100){

	start[3] = ms_ticks;
	Encoder_Test(); 
	EXINT_Test();
	}

// Вывод отладочных квадратов 1Гц на дисплей
// и времени с РТС
	if (ms_ticks - start[1] >= 1000){

	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state){

		ST7735_FillRect(108,140, 20, 20, RED);
		print_Time();
		} else {

		ST7735_FillRect(108,140, 20, 20, GREEN);  
		print_Time();
		}
	}

// блинкер на РС13 1Гц
if (ms_ticks - start[0] >= 1000){

	GPIOC->ODR ^= (1 << 13);
	start[0] = ms_ticks;
	}


}
}



