#ifndef ST7735_H_
#define ST7735_H_

#include "main.h"

#define RST 0
#define AO 1
#define CS 2


void ST7735_init();
int ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_fill(uint16_t color);

#endif /* ST7735_H_ */
