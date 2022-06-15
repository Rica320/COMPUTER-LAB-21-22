#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// Needed for BIOS Call
#define VIDEO_CARD 0x10
#define SET_VBE_MODE 0x4F02

// Color modes
#define DIRECT_COLOR 0X06
#define INDEXED_COLOR 0X105
#define BETTER_COLOR 0x14C

/**
 * @brief Changes the video card mode
 *
 * @param mode mode to be set
 * @return int boolean if success
 */
int set_vc_mode(uint16_t mode);

/**
 * @brief Maps the physical video card buffer
 *
 * @return int boolean if success
 */
int map_phys_frame_buffer(vbe_mode_info_t *mode_info);

/**
 * @brief Sets the graphic mode
 *
 */
int(set_graphics_card_mode)(uint16_t mode);

/**
 * @brief Change pixel (x,y) color in the correct mem position
 *
 */
int(changePixelColor)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw a horizontal line starting at (x,y) of color
 * and going to the right len pixels
 *
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draw a rectangle of color given. Left up corner (x,y).
 * Going right width pixels and down height pixels.
 *
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Fills entire screen with given color
 *
 */
int(vg_draw_fill)(uint32_t color);

int(video_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

uint32_t(indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles);
uint32_t(direct_color)(uint32_t red, uint32_t green, uint32_t blue);

uint32_t(R_First)(uint32_t first);
uint32_t(G_First)(uint32_t first);
uint32_t(B_First)(uint32_t first);

uint32_t(R)(unsigned int w, uint8_t step, uint32_t first);
uint32_t(G)(unsigned int h, uint8_t step, uint32_t first);
uint32_t(B)(unsigned int w, unsigned int h, uint8_t step, uint32_t first);

/*  Last Func */

struct _moveCords_t {
  uint16_t xi, yi, xf, yf;
  uint16_t newX, newY;
};

typedef struct _moveCords_t moveCords_t;

int XPMmove(xpm_map_t xpm, moveCords_t *cords, int16_t speed, uint8_t fr_rate);

int cordsCalc(moveCords_t *cords, int16_t speed);

/*  Connection to Timer and Keyboard */

#include "kbd.h"
extern uint8_t scancode;

int waitForEscPress();
