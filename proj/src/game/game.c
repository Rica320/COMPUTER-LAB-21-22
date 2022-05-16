#include "game.h"
#include <lcom/lcf.h>
#include "../drivers/keyboard/kbd_keys.h"


mouse_ptr getMouse() { // TODO: SHOULD BE get_mouse
  return cursor;
}

sprite_t * getBgImg() {
  return menu_img;
}

void game_loop() {
  
  xpm_map_t m = menu_bg;
  menu_img = make_sprite(m , XPM_8_8_8_8);

  xpm_map_t cm = cursor_xpm;
  cursor = make_sprite(cm , XPM_8_8_8_8);
  
  set_sprite_X(cursor, 200);
  set_sprite_Y(cursor, 200);

  set_sprite_X(menu_img, 0);
  set_sprite_Y(menu_img, 0);

  draw_sprite_in_mode_14c(menu_img);
  draw_sprite_in_mode_14c(cursor);


  flush_screen();

  subscribe_ihs();

  while (true) {
    EVENT_T event = handle_ihs();

    if (handle_evt(event) == BREAK_EVT)
      break;
    
  }

  unsubscribe_ihs();

  free_sprite(cursor);
  free_sprite(menu_img);

}
