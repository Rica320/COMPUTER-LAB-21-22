#ifndef _FONT_H_
#define _FONT_H_

#include <lcom/lcf.h>

#include "../animation/animation.h"
#include "../sprite/sprite.h"

static char kbd_US[128];

static sprite_t *a = NULL;
static AnimSprite *f_small = NULL;

/**
 * @brief Draws the given string in the given position
 * The color must also be specifified and a boolean telling if
 * font size is small or large
 *
 * @param text text to be drawn
 * @param x0 start x position of the drawn string
 * @param y0 start y position of the drawn string
 * @param hexColor color the text should have
 * @param small boolean if text should be small
 */
void draw_text(char *text, int x0, int y0, int hexColor, bool small);

/**
 * @brief Get the ascii from scancode object
 *
 * @param scancode scancode where to get ascii frome
 * @return char ascii code retrieved
 */
char get_ascii_from_scancode(uint8_t scancode);

#endif // _FONT_H_
