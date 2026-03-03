#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define PA9 9
#define PA10 10


void USART1_init(uint16_t baud);
void USART1_sendByte(char tx_data); // отправка байта
void USART1_sendStr(char *str); // отправка строки - принимаем строковый массив
void USART1_IRQHandler(); // обработчик прерывания
void USART1_echo();

extern volatile uint8_t usart_data_buffer[32]; //буффер uart
extern volatile uint8_t usart_index_buffer;
extern volatile uint8_t usart1_flag;

#endif /* UART_H_ */
