#ifndef MAIN_H
#define MAIN_H

#include <lcom/lcf.h>
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/keyboard/kbd_keys.h"
#include "../drivers/mouse/mouse.h"
#include "../drivers/kbc/i8042.h"
#include "../drivers/graph/video_graphic.h"
#include "sprite/sprite.h"
#include "state_machine/mouse_state.h"
#include "int_handlers/independent/independent_ih.h"
#include "int_handlers/dependent/dependent_ih.h"
#include "../assets/menu_bg.h"

static mouse_ptr cursor;
static sprite_t * menu_img;

void game_loop();

mouse_ptr getMouse();
sprite_t * getBgImg();

#endif
