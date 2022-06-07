#include "game.h"
#include "../assets/fontbitmap.xpm"
#include "../drivers/keyboard/kbd_keys.h"
#include <lcom/lcf.h>
void game_loop() {

  set_up_view();

  flush_screen();
  set_up_board();

  subscribe_ihs();

  sprite_t *a = make_sprite(font_small, XPM_8_8_8_8);

  set_sprite_pos(a, 90 , 90);

  AnimSprite * b = create_animSprite(a, 96, 16, 9 , 20);

  //AnimSprite * c = create_animSprite(a, 1, 1,  120,144);

  memset((void *) user_msg, 0, sizeof(uint8_t) * 15 * 6);

  while (true) {
    EVENTS event = handle_ihs();

    if (handle_evt(event) & BIT(BREAK_EVT))
      break;

    for (size_t i = 0; i < 10; i++)
    {
      draw_animSprite(b, i + 1, i*10, 10);
    }
    
    
    //draw_animSprite(c, 1, 0, 0);
    
    draw_sprite_in_mode_14c(a);
    flush_screen();
  }

  unsubscribe_ihs();

  free_view();
}
