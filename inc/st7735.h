#ifndef ST7735_H_
#define ST7735_H_

#include "main.h"

#define RST 0
#define AO 1
#define CS 2


void ST7735_init();
void ST7735_fill(uint16_t color);

#endif /* ST7735_H_ */
