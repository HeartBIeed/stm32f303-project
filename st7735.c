#include "st7735.h"

void ST7735_setPins(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 

	GPIOA->MODER &= ~((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); 
	GPIOA->MODER |= ((1 << (RST*2))|(1 << (AO*2))|(1 << (CS*2))); // output

	GPIOA->OSPEEDR &= ~((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); 
	GPIOA->OSPEEDR |= ((3 << (RST*2))|(3 << (AO*2))|(3 << (CS*2))); //high

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
		_delay_ms(120);

	ST7735_cmd(0x3A);
	ST7735_data(0x05);

	ST7735_cmd(0x36);
	ST7735_data(0x14);

	ST7735_cmd(0x29);
		_delay_ms(100);

}


int ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {



  if((x >= 128) || (y >= 160)) return 0;
  if(w <= 0 || h <= 0) return 0;

  if(x + w >  128)  w = 128  - x;
  if(y + h > 160) h = 160 - y;

CS_low();

	ST7735_cmd(0x2A);
	ST7735_data(0);
	ST7735_data(x);
	ST7735_data(0);
	ST7735_data(x+w-1);

	ST7735_cmd(0x2B);
	ST7735_data(0);
	ST7735_data(y);
	ST7735_data(0);
	ST7735_data(y+h-1);

	ST7735_cmd(0x2C);

	AO_high();

for(int16_t i=0; i<h; i++) {
    for(int16_t j=0; j<w; j++) {

	SPI1_sendByte(color >> 8);
	SPI1_sendByte(color & 0xFF);

    }
  }

 CS_high();
 return 1;
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
	
	while(SPI1->SR & SPI_SR_BSY);
	CS_high();

}


