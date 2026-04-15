#include "main.h"

char string[41];

void GPIO_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	GPIOC->MODER |= (1 << (13*2)); //PC13 / output 01
	GPIOC->PUPDR |= (1 << (13*2)); //PC13 / up 01

	GPIOC->MODER |= (1 << (14*2)); 
	GPIOC->PUPDR |= (1 << (14*2)); 
}

void PC13_blink(){

	static uint32_t	start = 0;

	if (ms_ticks - start >= 1000){

	GPIOC->ODR ^= (1 << 13);
	start = ms_ticks;
	}
}



void Print_Radiation(uint8_t x, uint8_t y){

	static uint8_t state = 1;	

	if (state){

		ST7735_DrawString(x+50,y,"uSv",MAGENTA,Font_11x18);
		ST7735_DrawString(x+50,y+18,"uR",MAGENTA,Font_11x18);

		state = 0;
	}
	
	static char text_uR_old[8]={"\0"};	
	static char text_uSv_old[8]={"\0"};	

	char text_uR[8]={"\0"};	
	char text_uSv[8]={"\0"};	

	static uint32_t	start = 0;

	if (ms_ticks - start >= 5000){

	uint16_t current_radiation = Radiation();

	sprintf(text_uR,"0.%02u",current_radiation); 
	sprintf(text_uSv,"%u ",current_radiation); 		

		if (!(text_uR_old == text_uR)){

		ST7735_DrawString(x,y+18, text_uR_old,BLACK,Font_11x18);
		ST7735_DrawString(x,y+18, text_uR,MAGENTA,Font_11x18);
		}


		if (!(text_uSv_old == text_uSv)){

		ST7735_DrawString(x+10,y, text_uSv_old,BLACK,Font_11x18);
		ST7735_DrawString(x+10,y, text_uSv,MAGENTA,Font_11x18);
		}

	strcpy(text_uR_old,text_uR);
	strcpy(text_uSv_old,text_uSv);

	start = ms_ticks;
	}
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

ST7735_FillRect(0,0, 128, 160, BLACK); 

uint32_t start[4] = {0};

 while(1)
{
	USART_commands();
	USART1_echo();
//	_delay_ms(300);
 



 if (ms_ticks - start[3] >= 1000){

	start[3] = ms_ticks;
	AHT_output(3,30);
	}
	

 if (ms_ticks - start[2] >= 900){

	start[2] = ms_ticks;
	Print_Time(10,3);
	}

if (ms_ticks - start[1] >= 5000)
	{

	sprintf(string,"\n SUMofCounts: %02u \n\r",SUMofCounts); 
	USART1_sendStr(string);	

	sprintf(string,"uSv: 0.%02u \n\r",Radiation()); 
	USART1_sendStr(string);	

	sprintf(string,"uR: %02u \n\n\r",Radiation()); 
	USART1_sendStr(string);	
	start[1] = ms_ticks;
	}

Print_Radiation(30, 80);

PC13_blink();


}
}


