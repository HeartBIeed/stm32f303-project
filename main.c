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

	ST7735_FillRect(10,13, 100, 10, 0x0000);
	ST7735_DrawString(10,13,buffer, 0xFFFF);
}

//**********************************************


typedef enum {
	START,
	IDLE,
	UP,
	DOWN
} State_Menu;


void MENU_Encoder(){

static int old_position =0;
int new_position =  Encoder_Get_TIM4();
static uint8_t y;
static State_Menu state = START;

	if (new_position  > old_position)
	{
	state = UP;

	} else if (new_position  < old_position) {

	state = DOWN;
	} 

switch(state) {
    case START:
	y = 80;
	ST7735_FillRect(10,y, 10, 10, 0x0FF0);

    state = IDLE;
    break;
//-----------------------------------

    case IDLE:

    break;
//-----------------------------------
    case UP:
	ST7735_FillRect(10,y, 10, 10, 0x0000);

	y = y - 10;
	if (y < 80) y = 80;

	ST7735_FillRect(10,y, 10, 10, 0x0FF0);

    state = IDLE;
    break;
//-----------------------------------
    case DOWN:
    ST7735_FillRect(10,y, 10, 10, 0x0000);

	y = y + 10;
	if (y > 140) y = 140;

	ST7735_FillRect(10,y, 10, 10, 0x0FF0);

    state = IDLE;
    
    break;
	}

old_position = new_position;
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

ST7735_FillRect(0,0, 128, 160, 0x0000); // фоновая заливка


uint32_t start[4] = {0};
uint8_t screen_state = 0;

 while(1)
{
	USART_commands();
	USART1_echo();
//	_delay_ms(300);
 
MENU_Encoder();

 /*if (ms_ticks - start[4] >= 1000)
	{
	start[4] = ms_ticks;
	I2C_scan();
	}
		
*/


 if (ms_ticks - start[2] >= 10000)
	{
	start[2] = ms_ticks;
	I2C_scan();
	USART1_sendStr("\r\n end scan\r\n ");
	}

// Тест энкодера с выводом на дисплей
 if (ms_ticks - start[3] >= 100)
	{
	start[3] = ms_ticks;
	Encoder_Test(); 
	}
		

// Вывод  АЦП и температуры на дисплей
/*	if (ms_ticks - start[2] >= 1000)
	{
	start[2] = ms_ticks;

	ST7735_FillRect(0,13, 120, 40, 0x0000);

	uint32_t data_adc = ADC_read()*3300/4095;
	uint32_t data_temp = (data_adc - 907)/15;

	char string[32];
	sprintf(string, "ADC: %lu mV",data_adc);
	ST7735_DrawString(3,13, string, 0xFFFF); // ADC

		sprintf(string, "ADC: %lu \r\n",data_adc);
		USART1_sendStr(string);

	sprintf(string, "T: %lu *C",data_temp);
	ST7735_DrawString(10,23, string, 0xFFFF); // temperature

		sprintf(string, "T: %lu \r\n",data_temp);
		USART1_sendStr(string);

	}
*/

// Вывод отладочных квадратов 1Гц на дисплей
// и времени с РТС
	if (ms_ticks - start[1] >= 1000)
	{
	start[1] = ms_ticks;
	screen_state ^= 1;

		if (screen_state) 	
		{

		ST7735_FillRect(108,140, 20, 20, 0xF800); // red

		ST7735_FillRect(40,3, 80, 10, 0x0000); // clear time black 
		print_Time();

		} else {

		ST7735_FillRect(108,140, 20, 20, 0x00FF); // blue

		ST7735_FillRect(40,3, 80, 10, 0x0000); // clear time black 
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



