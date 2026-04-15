#include "gcounter.h"

uint16_t SENS  = 34; //чувствительность
uint16_t Counts[10] = {0}; // десять периодов
uint16_t SUMofCounts = 0; // Сумма событий

uint8_t i = 0; // индекс периодов
uint8_t periods_passed = 0; // количество прошедших периодов
uint32_t total_sec = 1; // общее время работы


uint16_t Radiation(){ 
// подпрограмма вычисления Radiation вызывается каждые 5 сек по таймеру

	if (periods_passed >= 10) SUMofCounts -= Counts[i];    
// удаляем из суммы старое значение
// до 10 периодов  - накопление
// после - затирание старых периодов

	   __disable_irq();

	Counts[i] = count; // count - число из прерывания 
	count = 0;// обнуляем count (раз в 5 сек)

	   __enable_irq();

	SUMofCounts += Counts[i]; // добавляем к сумме число из периода i 

	i++; // инкремент периода
	if (i >= 10) i = 0; // всего 10 от 0 до 9

	if (periods_passed < 10){
//если время замера меньше 50 сек.
		periods_passed++;
		total_sec = periods_passed *5; 
// общее время = кол-во периодов * 5 сек каждый
		} else {

		total_sec = 10 *5; // over 50 sec 
		}
// свыше 50 сек общий диапазон 50 сек. 10 периодов по 5 сек.

//counts per second = Динамическая сумма событий / общее время 
	uint16_t  Rad = (SUMofCounts * SENS) / total_sec;

return Rad;
}
