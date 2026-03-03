#include "rcc.h"

void SystemClock_HSI_8MHz(void){

	RCC->CR |=RCC_CR_HSION; // enable HSI
	while((RCC->CR & RCC_CR_HSIRDY)==0); // wait stable HSI

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |=RCC_CFGR_SW_HSI; // HSI source SYSCLK
	//SW 0b00 HSI, 0b01-HSE, 0b10-PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); //SWS status CFGR ->HSI EN
}


void SystemClock_HSE_8MHz(void){

	RCC->CR |=RCC_CR_HSEON; // enable HSE
	while((RCC->CR & RCC_CR_HSERDY)==0); // wait stable HSE

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |=RCC_CFGR_SW_HSE; // HSE source SYSCLK
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE); //SWS status CFGR ->HSE EN
}


void PLL_enable(){

	RCC->CFGR = (RCC->CFGR & ( ~RCC_CFGR_PLLMUL)) | (RCC_CFGR_PLLMUL6); //set mul 6

	RCC->CR |= RCC_CR_PLLON;// PLL enable  
	while((RCC->CR & RCC_CR_PLLRDY)==0); // wait PLL is set

	RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL as system clock
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) //wait switched SC to PLL

	SystemCoreClockUpdate();

}
