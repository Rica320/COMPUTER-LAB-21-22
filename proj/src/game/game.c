#include "game.h"
#include "../drivers/keyboard/kbd_keys.h"
#include <lcom/lcf.h>

void mouse_update_pos(int x, int y) {
  int nx = get_sprite_X(cursor) + x;
  int ny = get_sprite_Y(cursor) - y;
  set_sprite_X(cursor, (nx > 0 ? nx : 1152 + nx) % 1152); // TODO:MAGIC
  set_sprite_Y(cursor, (ny > 0 ? ny : 864 + ny) % 864);
}

void set_bg() {
  menu_img = make_sprite(menu_bg, XPM_8_8_8_8);
  set_sprite_X(menu_img, 0);
  set_sprite_Y(menu_img, 0);
}

void set_cursor() {
  cursor = make_sprite(cursor_xpm, XPM_8_8_8_8);
  set_sprite_X(cursor, 200);
  set_sprite_Y(cursor, 200);
}

void draw_bg() {
  draw_sprite_in_mode_14c(menu_img);
}

void draw_cursor() {
  draw_sprite_in_mode_14c(cursor);
}

void draw_exit_b(int x, int y) {
  exit_b = make_sprite(menu_button_xpm, XPM_8_8_8_8);
  set_sprite_X(exit_b, x);
  set_sprite_Y(exit_b, y);
  draw_sprite_in_mode_14c(exit_b);
}

void draw_update() {
  draw_bg();
  draw_exit_b(400, 200);
  draw_exit_b(400, 400);
  draw_exit_b(400, 600);
  draw_cursor();
  flush_screen();
}

void game_loop() {

  set_bg();
  set_cursor();
  draw_update();

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
