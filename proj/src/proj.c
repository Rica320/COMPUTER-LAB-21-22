#include <lcom/lcf.h>

#include "drivers/graph/video_graphic.h"
#include "game/game.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  video_graphic_init(0x14C);
  game_loop();
  vg_exit();
  return EXIT_SUCCESS;
}
