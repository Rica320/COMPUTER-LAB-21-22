#include "game.h"
#include "../assets/fontbitmap.xpm"
#include "../drivers/keyboard/kbd_keys.h"
#include <lcom/lcf.h>
void game_loop() {

  set_up_view();

  flush_screen();
  set_up_board();

  subscribe_ihs();

  sprite_t *a = make_sprite(font, XPM_8_8_8_8);

  set_sprite_pos(a, 0 , 0);

  AnimSprite * b = create_animSprite(a, 96, 16, 65, 147);

  memset((void *) user_msg, 0, sizeof(uint8_t) * 15 * 6);

  while (true) {
    EVENTS event = handle_ihs();

    if (handle_evt(event) & BIT(BREAK_EVT))
      break;

    draw_animSprite(b, 1, 10, 10);
    
    draw_sprite_in_mode_14c(a);
    flush_screen();
  }

  unsubscribe_ihs();

  free_view();
}
