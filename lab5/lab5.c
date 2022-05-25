#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "vc.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  // set video card graphic mode
  if (set_vc_mode(mode)) {
    printf("Error changing video card mode.\n");
    return 1;
  }

  sleep(delay); // wait delay seconds to exit graphic mode

  // return to minix's text mode
  if (vg_exit()) {
    printf("Error exiting graphic mode into text mode.\n");
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  return 0;
}

// meus xpm para testar
#include "heart.xpm"

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
}


