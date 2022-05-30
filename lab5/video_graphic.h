#include <lcom/lcf.h>

//Valores explicados no .c em vg_map_vram
static void *video_mem;         
static void *buf;
static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel; 
static uint8_t color_model;
static unsigned int vram_size;
static uint8_t RedMaskSize;
static uint8_t GreenMaskSize;
static uint8_t BlueMaskSize;
static uint8_t RedFieldPosition;
static uint8_t GreenFieldPosition;
static uint8_t BlueFieldPosition;
unsigned int vram_base;



void* (vd_init)(uint16_t mode);
int vg_map_vram(uint16_t mode);
int vg_set_mode(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int vg_fill_pixel(uint16_t x, uint16_t y, uint32_t color);
int vg_drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
