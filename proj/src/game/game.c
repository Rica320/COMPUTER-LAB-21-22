#include "game.h"
#include <lcom/lcf.h>
#include "../drivers/keyboard/kbd_keys.h"


mouse_ptr getMouse() {
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

  // bool move_right = false, move_up = false, move_down=false, move_left=false;
  // int counter = 0;
  // int frames = 0;
  // int ticks_frame = sys_hz() / 10;
  // int16_t speed = 10;
  // int mov = 1;
  // if (speed > 0)
  //   mov = speed;


  while (true) {
    EVENT_T event = handle_ihs();

    if (handle_evt(event) == BREAK_EVT)
      break;
    
  }

  unsubscribe_ihs();


  free_sprite(cursor);
  free_sprite(menu_img);

}
