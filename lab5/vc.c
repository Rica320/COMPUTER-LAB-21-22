#include "vc.h"

static void *video_mem;
unsigned hres, vres;
unsigned numBytesPerPixel;

vbe_mode_info_t mode_info;

/*    Functions for 1      */

int set_vc_mode(uint16_t mode) {

  /* ========== Get Mode Info ========== */

  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  // Store VC Settings
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;

  numBytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  unsigned vram_base = mode_info.PhysBasePtr;
  unsigned vram_size = hres * vres * numBytesPerPixel;

  /* ========== Mapping Memory ========== */

  // Allow memory mapping
  struct minix_mem_range mr;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if ((sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed\n");

  // Map memory
  video_mem = vm_map_phys(SELF, (void *) vram_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  /* ========== Change Video Card Mode ========== */

  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = VIDEO_CARD;
  r.ax = SET_VBE_MODE;
  r.bx = BIT(14) | mode; // (bit 14 --> linear buffer)

  // Make the BIOS call
  if (sys_int86(&r)) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

  return 0;
}

/*     Functions for 2     */

int(changePixelColor)(uint16_t x, uint16_t y, uint32_t color) {

  if (x >= mode_info.XResolution || y >= mode_info.YResolution)
    return 1;

  // pointer para a pos na mem do pixel a pintar
  uint8_t *ptr = (uint8_t *) video_mem + numBytesPerPixel * (x + (y * hres));

  // i << 3 é equivalente a i * 8 --> deslocar o byte da cor para a próxima parte do R G B
  for (unsigned i = 0; i < numBytesPerPixel; i++)
    ptr[i] = (color >> (i << 3)) & 0xFF;

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned i = 0; i < len; i++)
    changePixelColor(x + i, y, color);

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (unsigned i = 0; i < height; i++)
    vg_draw_hline(x, y + i, width, color);

  return 0;
}

/*    Functions for 3     */

int(video_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {

  unsigned height = vres / no_rectangles;
  unsigned width = hres / no_rectangles;

  for (unsigned h = 0; h < no_rectangles; h++)
    for (unsigned w = 0; w < no_rectangles; w++) {

      uint32_t color;

      if (mode_info.MemoryModel == DIRECT_COLOR)
        color = direct_color(R(w, step, first), G(h, step, first), B(w, h, step, first));
      else // Indexed
        color = indexed_color(w, h, step, first, no_rectangles);

      vg_draw_rectangle(w * width, h * height, width, height, color);
    }

  return 0;
}

// _________________________________________________________________________________________

uint32_t(indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
  return (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t(direct_color)(uint32_t red, uint32_t green, uint32_t blue) {
  return (blue << mode_info.BlueFieldPosition) | (green << mode_info.GreenFieldPosition) | (red << mode_info.RedFieldPosition);
}

// _________________________________________________________________________________________

uint32_t(R_First)(uint32_t first) {
  return (first >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
}

uint32_t(G_First)(uint32_t first) {
  return (first >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
}

uint32_t(B_First)(uint32_t first) {
  return (first >> mode_info.BlueFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
}

// _________________________________________________________________________________________

uint32_t(R)(unsigned w, uint8_t step, uint32_t first) {
  return (R_First(first) + w * step) % (1 << mode_info.RedMaskSize);
}

uint32_t(G)(unsigned h, uint8_t step, uint32_t first) {
  return (G_First(first) + h * step) % (1 << mode_info.RedMaskSize);
}

uint32_t(B)(unsigned w, unsigned h, uint8_t step, uint32_t first) {
  return (B_First(first) + (w + h) * step) % (1 << mode_info.RedMaskSize);
}

// _________________________________________________________________________________________

/*    Functions for 5    */

int XPMmove(xpm_map_t xpm, moveCords_t *cords, int16_t speed, uint8_t fr_rate) {

  int frameCounter = 0;
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_INDEXED, &img);

  // Atualiza valores das posicoes conforme speed e fr_rate

  if (speed > 0) {
    // limpar posicao anterior (pinta por cima da zona do xpm com preto)
    vg_draw_rectangle(cords->newX, cords->newY, img.width, img.height, 0);
    cordsCalc(cords, speed);
  }

  else {
    if (frameCounter++ % abs(speed) == 0) {
      // limpar posicao anterior (pinta por cima da zona do xpm com preto)
      vg_draw_rectangle(cords->newX, cords->newY, img.width, img.height, 0);
      cordsCalc(cords, 1);
    }
  }

  // desenhar xpm na nova posição
  map = xpm_load(xpm, XPM_INDEXED, &img);
  for (unsigned int height = 0; height < img.height; height++)
    for (unsigned int width = 0; width < img.width; width++)
      changePixelColor(cords->newX + width, cords->newY + height, *map++);

  return 0;
}

int cordsCalc(moveCords_t *cords, int16_t speed) {

  if (cords->xi == cords->xf) {
    if (cords->yi < cords->yf) {
      if (cords->newY + speed > cords->yf)
        cords->newY = cords->yf;
      else
        cords->newY = cords->newY + speed;
    }
    else {
      if (cords->newY - speed < cords->yf)
        cords->newY = cords->yf;
      else
        cords->newY = cords->newY - speed;
    }
  }
  else {
    if (cords->xi < cords->xf) {
      if (cords->newX + speed > cords->xf)
        cords->newX = cords->xf;
      else
        cords->newX = cords->newX + speed;
    }
    else {
      if (cords->newX - speed < cords->xf)
        cords->newX = cords->xf;
      else
        cords->newX = cords->newX - speed;
    }
  }

  return 0;
}

/*  Auxiliar func */

int waitForEscPress() {
  int ipc_status;
  message msg;
  uint8_t bit_no;

  kbd_subscribe_int(&bit_no);

  bool another_read = false;
  uint8_t codes[2];

  while (scancode != ESC_BREAKCODE) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    // if there is a KBD Interrupt
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        kbc_ih(); // chamar o nosso handler

        if (!another_read) {
          codes[0] = scancode;
          if (scancode == MSB_2B_SCANCODE)
            another_read = true;
        }
        else {
          codes[1] = scancode;
          another_read = false;
        }
      }
  }

  kbd_unsubscribe_int(); // devolve controlo do kbd ao minix
  return 0;
}
