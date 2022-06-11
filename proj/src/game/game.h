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
 * @brief Starts the Game Loop and Finishes It when the game is over
 */
void game_loop();

#endif
