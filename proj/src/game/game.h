#ifndef MAIN_H
#define MAIN_H

#include <lcom/lcf.h>

#include "../drivers/graph/video_graphic.h"
#include "../drivers/kbc/i8042.h"
#include "../drivers/keyboard/kbd_keys.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "int_handlers/dependent/dependent_ih.h"
#include "int_handlers/independent/independent_ih.h"
#include "sprite/sprite.h"
#include "views/font.h"

void game_loop();

#endif
