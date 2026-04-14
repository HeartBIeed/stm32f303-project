
#include "w25q.h"

void CS_W25Q_high(){
	GPIOA->BSRR |= (1 << CS_W25Q); 
}

void CS_W25Q_low(){
	GPIOA->BSRR |= (1 << (CS_W25Q+16)); 
}



uint32_t W25Q_ReadID(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 

	GPIOA->MODER &= ~(3 << (CS_W25Q * 2)); 
	GPIOA->MODER |= (1 << (CS_W25Q * 2)); // output

	GPIOA->OSPEEDR &= ~(3 << (CS_W25Q * 2));
	GPIOA->OSPEEDR |= (1 << (CS_W25Q * 2)); //high

	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

CS_W25Q_low();

	SPI1_sendByte(0x9F);

	Temp0 = SPI1_sendByte(0xFF);
	Temp1 = SPI1_sendByte(0xFF);
	Temp2 = SPI1_sendByte(0xFF);

CS_W25Q_high();

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}


