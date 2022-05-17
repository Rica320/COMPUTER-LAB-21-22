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
#include "../assets/Exit.xpm"
#include "../assets/Instructions.xpm"
#include "../assets/Multiplayer.xpm"
#include "../assets/Online.xpm"
#include "../assets/Play.xpm"
#include "../assets/menu_bg.h"

static mouse_ptr cursor;
static sprite_t *menu_img;

void game_loop();

void draw_update();

void mouse_update_pos(int x, int y);

#endif
