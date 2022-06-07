#include "game.h"
#include <lcom/lcf.h>

#include "views/font.h"
void game_loop() {

  set_up_view();

  flush_screen();
  set_up_board();

  subscribe_ihs();

  memset((void *) user_msg, 0, sizeof(uint8_t) * 15 * 6);

  while (true) {
    EVENTS event = handle_ihs();

    if (handle_evt(event) & BIT(BREAK_EVT))
      break;

    draw_text("OLA", 10, 10, 0xffffff, true);
    flush_screen();
  }

  unsubscribe_ihs();

  free_view();
}
