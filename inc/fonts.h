/*
 * fonts.h
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: dima
 * https://github.com/stDstm/Example_STM32F103/blob/master/Display_TFT_ILI9341/Inc/fonts.h
 */

#ifndef FONTS_H_
#define FONTS_H_

//#include "main.h"
#include <stdint.h>

typedef enum ST7735_Colors {
    BLACK    = 0x0000,
    WHITE    = 0xFFFF,
    RED      = 0xF800,
    GREEN    = 0x07E0,
    BLUE     = 0x001F,
    YELLOW   = 0xFFE0,
    CYAN     = 0x07FF,
    MAGENTA  = 0xF81F,
} ST7735_Color;

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;



#endif /* FONTS_H_ */
