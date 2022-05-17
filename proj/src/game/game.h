#ifndef MAIN_H
#define MAIN_H

#include "../drivers/graph/video_graphic.h"
#include "../drivers/kbc/i8042.h"
#include "../drivers/keyboard/kbd_keys.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "int_handlers/dependent/dependent_ih.h"
#include "int_handlers/independent/independent_ih.h"
#include "sprite/sprite.h"
#include "state_machine/mouse_state.h"
#include <lcom/lcf.h>

// include assets
#include "../assets/menu_bg.h"

#include "../assets/menu_button.xpm"

static mouse_ptr cursor;
static sprite_t *menu_img;

// menu buttons
static sprite_t *exit_b;
static sprite_t *help_b;
static sprite_t *multi_b;
static sprite_t *single_b;

void game_loop();

void draw_update();

void mouse_update_pos(int x, int y);

#endif
