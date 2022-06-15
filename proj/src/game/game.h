#ifndef MAIN_H
#define MAIN_H

#include <lcom/lcf.h>

#include "../drivers/graphics/video_graphic.h"
#include "../drivers/kbc/i8042.h"
#include "../drivers/keyboard/kbd_keys.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "int_handlers/dependent/dependent_ih.h"
#include "int_handlers/independent/independent_ih.h"
#include "views/font/font.h"
#include "views/sprite/sprite.h"

/**
 * @brief Allocs game sprites, subscrives interrupts and manages event handlers.
 * Also unsubscribes the IHs and frees the sprites allocation at the end of the program
 *
 */
void game_loop();

#endif
