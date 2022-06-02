#ifndef _FONT_H_
#define _FONT_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"

static char kbd_US [128];

void draw_text(char *text, int x0, int y0, int hexColor);
char get_ascii_from_scancode(uint8_t scancode);

#endif // _FONT_H_