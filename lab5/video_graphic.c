#include "video_graphic.h"
#include "lcom/video_gr.h"

int(vbe_set_mode)(uint16_t mode) {
  vbe_mode_info_t vmi;
  CHECKCall(vbe_get_mode_info(mode, &vmi));

  reg86_t r86;

  /* Specify the appropriate register values */

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4f;
  r86.al = VBE_SET_MODE;
  r86.bx = mode | BIT(14);

  /* Make the BIOS call */

  CHECKCall(sys_int86(&r86));

  return EXIT_SUCCESS;
}

int(map_vram)(uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base; /* VRAM's physical addresss */ /* VRAM's size, but you can use
             the frame-buffer size, instead */
  int r;

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  vbe_mode_info_t vmi;
  CHECKCall(vbe_get_mode_info(mode, &vmi));

  vram_base = vmi.PhysBasePtr;
  h_res = vmi.XResolution;
  v_res = vmi.YResolution;
  bits_per_pixel = vmi.BitsPerPixel;
  vram_size = v_res * h_res * (vmi.BitsPerPixel + 7) / 8;
  color_model = vmi.MemoryModel;

  RedMaskSize = vmi.RedMaskSize;
  GreenMaskSize = vmi.GreenMaskSize;
  BlueMaskSize = vmi.BlueMaskSize;

  RedFieldPosition = vmi.RedFieldPosition;
  GreenFieldPosition = vmi.GreenFieldPosition;
  BlueFieldPosition = vmi.BlueFieldPosition;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  buf = (void *)malloc(vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return EXIT_SUCCESS;
}

int(fill_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t *ptr;
  uint8_t bytes_per_color = (bits_per_pixel + 7) / 8;

  ptr = (uint8_t *) buf + (x + (y *  h_res)) * bytes_per_color;

  /*
    DOESN'T THIS APROCHE RIGHTS PARTS THAT IT SHOULDN'T ? 
  */
  //memcpy((void *) (ptr), &color, bytes_per_color);

  for (unsigned i = 0; i < bytes_per_color; i++)
    ptr[i] = (color >> (i * 8)) & 0xFF;

  return 0;

  return EXIT_SUCCESS;
}

int (flush_screen)() {

  memcpy((void *)(video_mem), buf, vram_size);
  return EXIT_SUCCESS;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (uint16_t i = 0; i < len; i++) {
    CHECKCall(fill_pixel(x + i, y, color));
  }

  return EXIT_SUCCESS;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    CHECKCall(vg_draw_hline(x, y + i, width, color));
  }

  return EXIT_SUCCESS;
}

void *(video_graphic_init) (uint16_t mode) {
  // TODO :: change the return

  CHECKCall(map_vram(mode));
  CHECKCall(vbe_set_mode(mode));

  return NULL;
}


bool (is_indexed_mode)(uint16_t mode) {
  return mode == 0x105;
}

unsigned (get_hres)(void) {
  return h_res;
}

unsigned (get_vres)(void) {
  return v_res;
}

unsigned (get_bits_per_pixel)(void) {
  return bits_per_pixel;
}

uint8_t (getRedMaskSize)(void) {
  return RedMaskSize;
}
uint8_t (getGreenMaskSize)(void){
  return GreenMaskSize;
}
uint8_t (getBlueMaskSize)(void) {
  return BlueMaskSize;
}

uint8_t (getRedFieldPosition)(void) {
  return RedFieldPosition;
}
uint8_t (getGreenFieldPosition)(void){
  return GreenFieldPosition;
}
uint8_t (getBlueFieldPosition)(void) {
  return BlueFieldPosition;
}
