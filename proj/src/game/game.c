#include "game.h"
#include "../drivers/keyboard/kbd_keys.h"
#include <lcom/lcf.h>


void game_loop() {

  set_up_view();

  flush_screen();
  set_up_board();

  subscribe_ihs();

  while (true) {
    EVENTS event = handle_ihs();

    if (handle_evt(event) & BIT(BREAK_EVT))
      break;
  }

  unsubscribe_ihs();

  free_view();
}
