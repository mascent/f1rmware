#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include <libopencm3/cm3/common.h>

#define RESX 130
#define RESY 130

#define TYPE_CMD    0
#define TYPE_DATA   1

/* Display buffer */
extern uint8_t lcdBuffer[];

void lcdInit(void);
void lcdFill(char f);
void lcdDisplay(void);
void lcdSetPixel(char x, char y, uint8_t f);
uint8_t lcdGetPixel(char x, char y);
void lcdShift(int x, int y, int wrap);
void lcdSetContrast(int c);

void lcd_select();
void lcd_deselect();
void lcdWrite(uint8_t cd, uint8_t data);

#endif
