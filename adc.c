#include "adc.h"

// пин  PBO/ модуль ADC3/ канал IN12
void ADC_init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 
	RCC->AHBENR |= (1<<29); //ADC34 EN
	RCC->CFGR2 |= (0b10001<<9); //10000: PLL clock divided by 2

	GPIOB->MODER |= ( 3 << (0*2)); // B0 pin/ 0x11 analog mode
	GPIOB->PUPDR &= ~( 3 << (0*2)); // pull down

// Bits 17:16 CKMODE[1:0]: ADC clock mode
ADC34_COMMON->CCR |=(2<<16); //2: HCLK/2 (Synchronous clock mode). 

//ADVREGEN[1:0]: ADC voltage regulator enable
ADC3->CR |= (1<<28); //01: ADC Voltage regulator enabled.
	_delay_us(20);

ADC3->CR &= ~(1<<0); //AD DISABLE

ADC3->CR |= (1<<31); //calibration
	while(ADC3->CR & (1<<31)); //wait 0

ADC3->ISR |= (1<<0);
ADC3->CR |= (1<<0); //AD EN
	while(!(ADC3->ISR & (1<<0))); //wait ADRDY

ADC3->SMPR2 &= ~(7<<6);
//Bits 29:3 SMPx[2:0]: Channel x sampling time selection
ADC3->SMPR2 |= (0b100<<6); //100 19.5 ADC clock cycles

ADC3->SQR1 &= ~(7<<0); //0000: 1 conversion / 1 измерение 
ADC3->SQR1 |= (12<<6); //num channel (12) / номер канала 1 измерения в очереди
// SQR+номер в очереди измерения

ADC3->CFGR |= (1<<13); //1: Continuous conversion mode
//Bit 13 CONT: Single / continuous conversion mode for regular conversions


ADC3->CR |= (1<<2); //AD start

}


uint32_t ADC_read(){

	int timeout = 1000000;

	while(!(ADC3->ISR & (1<<2))) //wait Bit 2 EOC: End of conversion flag
		{
			if (--timeout ==0)
			{
				break;
			}
		}
	return (uint16_t)ADC3->DR;
}







