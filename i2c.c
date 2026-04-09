#include "i2c.h"

uint32_t timeout;

void I2C_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // GPIOB
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // APB1 I2C1

//MODER 0b10 - 0x02 alt func - 2 bit
	GPIOB->MODER &= ~( 0x3 << (SCL *2)); // SCL  clear
	GPIOB->MODER |= ( 0x2 << (SCL *2)); // SCL set TX

	GPIOB->MODER &= ~( 0x3 << (SDA *2)); // SDA  clear
	GPIOB->MODER |= ( 0x2 << (SDA *2)); // SDA set RX

//OTYPER 1- Open Drain  - 1 bit 
	GPIOB->OTYPER |= ((1<<SCL )|(1<<SDA )); 

//OSPEEDR 10 high 2 - bit
	GPIOB->OSPEEDR |= ((0x2<<(SCL *2))|(0x2<<(SDA *2))); 

	GPIOB->AFR[1] &= ~((0xF<<((SCL-8) *4))|(0xF<<((SDA-8) *4))); // clear
	GPIOB->AFR[1] |= ((4<<((SCL-8) *4))|(4<<((SDA-8) *4)));  //0b001/ 4 bit 
//AFR 4 I2C enable mode

	I2C1->CR1 = 0;
	I2C1->CR2 = 0;
//	I2C1->CR1 |= (1<<2); // RX interrupt enable
//	I2C1->CR1 |= (1<<1); // TX interrupt enable
	
	I2C1->TIMINGR = (0x7<<28)|(0x4<<20)|(0x1<<16)|(0x24<<8)|(0x2D<<0);
// PRESC | SCLDEL | SDADEL | SCLH | SCLL 
//setup for fcpu 72MHz / i2c clk 100 kHz

	I2C1->CR1 |= (1<<0); // PE
}


void I2C_writeByte(int addr,uint8_t* data,int nbytes){

	I2C1->CR2 = 0; 
	
	I2C1->CR2 |= (nbytes<< 16); //nbytes
	I2C1->CR2 |= (1 << 25); //autoend
	I2C1->CR2 |= (addr << 1); // set slave address
	I2C1->CR2 |= (1 << 13); //start 

	while (!(I2C1->ISR & I2C_ISR_TXIS));//txis 1 - txdr empty

	for (int i = 0; i < nbytes; ++i)
	{
	while (!(I2C1->ISR & I2C_ISR_TXIS)); //wait empty tx buffer
	I2C1->TXDR = data[i];// send 

	}

	while (!(I2C1->ISR & I2C_ISR_STOPF)); //wait stop
	I2C1->ICR = I2C_ICR_STOPCF; //clear 
}

void I2C_readByte(int addr,uint8_t* data,int len){

		I2C1->CR2 = 0; 
		
		I2C1->CR2 |= (1 << 10); // 1 read
		I2C1->CR2 |= (len<< 16); //len
		I2C1->CR2 |= (1 << 25); //autoend
		I2C1->CR2 |= (addr << 1); // set slave address
		I2C1->CR2 |= (1 << 13); //start 


		for (int i = 0; i < len; ++i)
			{				
				while (!(I2C1->ISR & I2C_ISR_RXNE)); //wait transmit
				data[i] = I2C1->RXDR; // read 
			}
			
	while (!(I2C1->ISR & I2C_ISR_STOPF)); //wait stop
	I2C1->ICR = I2C_ICR_STOPCF; //clear 
}

int I2C_checkAddress(int addr){
	//clear errors
	I2C1->ICR = (1<<4)|(1<<5)|(1<<8)|(1<<9);

			I2C1->CR2 = 0; 
			
			I2C1->CR2 |= (0 << 16); //nbytes
			I2C1->CR2 |= (1 << 25); //autoend
			I2C1->CR2 |= (addr << 1); // set slave address
			I2C1->CR2 |= (1 << 13); //start 

	timeout = TIMEOUT;

	while (!(I2C1->ISR & (I2C_ISR_NACKF | I2C_ISR_STOPF | I2C_ISR_TC))) //wait NACK STOPF TC
		{
			if (--timeout == 0) return 0; //none ack
		}

	if (I2C1->ISR & I2C_ISR_NACKF) // if nack
		{
		   I2C1->ICR = I2C_ICR_NACKCF | I2C_ICR_STOPCF; //clear flags
		   return 0; // no device
		}


	 timeout = TIMEOUT;
	while (!(I2C1->ISR & I2C_ISR_STOPF )) // wait STOPF 
		{
			if (--timeout == 0) break;
		}

	I2C1->ICR = I2C_ICR_STOPCF; //clear stopf
	return 1; 
}


void I2C_scan(){

	uint32_t addr;
	char str[10];
	
	USART1_sendStr("I2C Start Scan \n\r");

	for (addr = 0x08; addr < 0x78; addr++)
	{
		if (I2C_checkAddress(addr)) 
		{
			USART1_sendStr("\n\r I2C FIND:");
			itoa(addr,str,16);
			USART1_sendStr(str);
		} 
	}

	USART1_sendStr("\n\r I2C End Scan \n\r");
}