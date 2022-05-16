#include "game.h"
#include <lcom/lcf.h>
#include "../drivers/keyboard/kbd_keys.h"


extern int (* state[])(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

void game_loop() {

  vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00ff);
  
  
  xpm_map_t m = menu_bg;
  sprite_t * sprite = make_sprite(m , XPM_8_8_8_8);
  
  set_sprite_X(sprite, 0);
  set_sprite_Y(sprite, 0);

  draw_sprite_in_mode_14c(sprite);


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

}

