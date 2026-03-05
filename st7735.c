#include "st7735.h"

void ST7735_setPins(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 

	GPIOA->MODER &= ~((1 << (RST*2))|(1 << (AO*2))|(1 << (CS*2))); 
	GPIOA->MODER |= ((1 << (RST*2))|(1 << (AO*2))|(1 << (CS*2))); // output

	GPIOA->OSPEEDR &= ~((2 << (RST*2))|(2 << (AO*2))|(1 << (CS*2))); 
	GPIOA->OSPEEDR |= ((2 << (RST*2))|(2 << (AO*2))|(1 << (CS*2))); //high

	GPIOA->BSRR |= (1 << RST); 


}

void AO_high(){
	GPIOA->BSRR |= (1 << AO); 
}

void AO_low(){
	GPIOA->BSRR |= (1 << (AO+16)); 
}

void CS_high(){
	GPIOA->BSRR |= (1 << CS); 
}

void CS_low(){
	GPIOA->BSRR |= (1 << (CS+16)); 
}

void ST7735_reset(){

	GPIOA->BSRR |= (1 << RST); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << (RST+16)); 
		_delay_ms(10);	
	GPIOA->BSRR |= (1 << RST); 

}

void ST7735_cmd(uint8_t cmd){

	AO_low();

	SPI1_sendByte(cmd);
		_delay_us(5);

}

void ST7735_data(uint8_t data){

	AO_high();

	SPI1_sendByte(data);
		_delay_us(5);
}

void ST7735_init(){

	ST7735_setPins();

	ST7735_reset();
		_delay_ms(10);
	CS_low();
		_delay_ms(10);

	ST7735_cmd(0x11);
		_delay_ms(10);

	ST7735_cmd(0x3A);
	ST7735_data(0x05);

	ST7735_cmd(0x36);
	ST7735_data(0x14);

	ST7735_cmd(0x29);
		_delay_ms(100);

}

void ST7735_fill(uint16_t color){
	CS_low();

	ST7735_cmd(0x2A);
	ST7735_data(0);
	ST7735_data(0);
	ST7735_data(0);
	ST7735_data(127);

	ST7735_cmd(0x2B);
	ST7735_data(0);
	ST7735_data(0);
	ST7735_data(0);
	ST7735_data(159);

	ST7735_cmd(0x2C);

	AO_high();
	
	for (uint32_t i = 0; i < 128*160; ++i)
	{
		SPI1_sendByte(color >> 8);
		SPI1_sendByte(color & 0xFF);

	}
	
	CS_high();

}








