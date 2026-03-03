#include "uart.h"

volatile uint8_t usart_data_buffer[32]; //буффер uart
volatile uint8_t usart_index_buffer =0;
volatile uint8_t usart1_flag =0;

void USART1_init(uint16_t baud)
	{
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //APB bus
	//moder 10 - alt 2 bit
		GPIOA->MODER &= ~(3 << (PA9*2)); 
		GPIOA->MODER |= (2 << (PA9*2)); //pa9 set tx

		GPIOA->MODER &= ~(3 << (PA10*2)); 
		GPIOA->MODER |= (2 << (PA10*2)); //pa10 set rx
	//otyper 0- push-pull 1 bit
		GPIOA->OTYPER &= ~((1<<PA9)|(1<<PA10));
	//speed 10 high 2 bit
		GPIOA->OSPEEDR |= ((2<<(PA9*2))|(2<<(PA10*2))); //pa9/10 to  0b10

		GPIOA->AFR[1] &= ~((0xF<<(1*4))|(0xF<<(2*4))); 
		GPIOA->AFR[1] |= ((7<<(1*4))|(7<<(2*4))); //AF7


	// 0b001/ 4 bit on 1 pin
	// usart tx rx enable mode

		USART1->CR1 = 0;
		USART1->CR2 = 0;
		USART1->CR3 = 0;
			
		USART1->BRR = FCPU/baud; 
		USART1->CR1 |= USART_CR1_RXNEIE ; // interrupts
		USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //tx rx usart enable
		
		NVIC_SetPriority(USART1_IRQn,2);
		NVIC_EnableIRQ(USART1_IRQn);
	}


void USART1_sendByte(char tx_data) // отправка байта
	{
		while ((USART1->ISR & USART_ISR_TXE) == 0); // пока флаг пустого буффера не появится
	 	USART1->TDR = tx_data; 
	}



void USART1_sendStr(char *str) // отправка строки - принимаем строковый массив
	{
		while (*str) USART1_sendByte(*str++); // отправка до \0
	}


void USART1_IRQHandler() // обработчик прерывания
	{

	if (USART1->ISR & USART_ISR_RXNE) 
	//проверяем флаг
	//RXNE сбросится
		{
		char data_rx = USART1->RDR;
		usart1_flag =1; // поставим флаг

		if (data_rx == '\r' || usart_index_buffer >= sizeof(usart_data_buffer)-1) //если конец строки или переполнение
			{
				usart_data_buffer[usart_index_buffer] = '\0'; //вставляем 0-терминатор
				usart_index_buffer = 0;
			}		
				
		else	
			{
				usart_data_buffer[usart_index_buffer] = data_rx;
				usart_index_buffer++;
			}
			
		}


	}	


void USART1_echo()
	{
	if (usart1_flag)
		{
		USART1_sendStr("GET: ");
		USART1_sendStr((char*)usart_data_buffer);
		USART1_sendStr("\r\n");
		usart1_flag = 0;
		}	

	}
