#include "game.h"
#include "../drivers/keyboard/kbd_keys.h"
#include <lcom/lcf.h>

/*____________________________DRAW LOGIC_________________________*/

void mouse_update_pos(int x, int y) {
  int nx = get_sprite_X(cursor) + x;
  int ny = get_sprite_Y(cursor) - y;
  set_sprite_X(cursor, (nx > 0 ? nx : get_hres() + nx) % get_hres());
  set_sprite_Y(cursor, (ny > 0 ? ny : get_vres() + ny) % get_vres());
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

int get_cursor_X() {
  return get_sprite_X(cursor);
}

int get_cursor_Y() {
  return get_sprite_Y(cursor);
}

void draw_bg() {
  draw_sprite_in_mode_14c(menu_img);
}

void draw_cursor() {
  draw_sprite_in_mode_14c(cursor);
}

void draw_button(const char *xpm[], int x, int y) {
  sprite_t *sprite = make_sprite(xpm, XPM_8_8_8_8);
  set_sprite_X(sprite, x);
  set_sprite_Y(sprite, y);
  draw_sprite_in_mode_14c(sprite);
  free_sprite(sprite);
}

void draw_menu() {
  switch (game_cur_state) {
    case menu_entry:
      draw_button(play_b_xpm, 400, 200);
      draw_button(instructions_b_xpm, 400, 400);
      draw_button(exit_b_xpm, 400, 600);
      break;
    case menu_play:
      draw_button(multiplayer_b_xpm, 400, 200);
      draw_button(online_b_xpm, 400, 400);
      draw_button(back_b_xpm, 400, 600);
      break;
    case instructions:
      draw_button(instructions_b_xpm, 400, 200);
      draw_button(back_b_xpm, 400, 400);
      break;
    case multiplayer:
      //draw_button(multiplayer_b_xpm, 400, 200);
      draw_board();
      draw_pieces(board);

      //draw_button(back_b_xpm, 400, 400);
      break;
    case online:
      draw_button(online_b_xpm, 400, 200);
      draw_button(back_b_xpm, 400, 400);
      break;
    case menu_end:
      draw_button(back_b_xpm, 400, 200);
      draw_button(back_b_xpm, 400, 400);
      break;
    default:
      break;
  }
}

void game_set_state(enum menu_state_codes state) {
  game_cur_state = state;
}

void draw_update() {
  draw_bg();
  draw_menu();
  draw_cursor();
  flush_screen();
}

/*____________________________/DRAW LOGIC_________________________*/

void set_up_board() {
  board[0][0] = make_piece(wB, XPM_8_8_8_8, 0104, Knight, BLACK);
}

void game_loop() {

  xpm_map_t m = menu_bg;
  menu_img = make_sprite(m, XPM_8_8_8_8);

  xpm_map_t cm = cursor_xpm;
  cursor = make_sprite(cm, XPM_8_8_8_8);

  set_sprite_X(cursor, 200);
  set_sprite_Y(cursor, 200);

  set_sprite_X(menu_img, 0);
  set_sprite_Y(menu_img, 0);

  draw_sprite_in_mode_14c(menu_img);
  draw_sprite_in_mode_14c(cursor);

  flush_screen();

  set_up_board();

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
