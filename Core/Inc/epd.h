/*
 * epd.h
 *
 *  Created on: Jan 30, 2025
 *      Author: ukashu
 */

#ifndef INC_EPD_H_
#define INC_EPD_H_

#include <stdint.h>

// Display resolution
#define EPD_1IN54_V2_WIDTH       200
#define EPD_1IN54_V2_HEIGHT      200

#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

void EPD_1IN54_V2_Init(void);
void EPD_1IN54_V2_Init_Partial(void);
void EPD_1IN54_V2_Clear(void);
void EPD_1IN54_V2_Display(uint8_t *Image);
void EPD_1IN54_V2_DisplayPartBaseImage(uint8_t *Image);
void EPD_1IN54_V2_DisplayPart(uint8_t *Image);
void EPD_1IN54_V2_Sleep(void);

#endif /* INC_EPD_H_ */
