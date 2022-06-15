/**
 * @file video_graphic.h 
 * @brief deals with the graphic driver. Defining useful api drawing etc..
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef VIDEO_GRAPHIC_H
#define VIDEO_GRAPHIC_H

#include <lcom/lcf.h>

#include "../utils/handlers.h"
#include "lcom/video_gr.h"
#include "vbe.h"

static void *video_mem; /* frame-buffer VM address */
static void *buf;

static unsigned h_res;          /* Horizontal resolution in pixels */
static unsigned v_res;          /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t color_model;

static unsigned int vram_size;

static uint8_t RedMaskSize;
static uint8_t GreenMaskSize;
static uint8_t BlueMaskSize;

static uint8_t RedFieldPosition;
static uint8_t GreenFieldPosition;
static uint8_t BlueFieldPosition;

/**
 * @brief Gets VBE Mode info for the specified mode and sets it with a BIOS Call
 * @param mode Mode for the VBE to be set
 * @return EXIT_SUCCESS if it finishes successfully
 */
int(vbe_set_mode)(uint16_t mode);

/**
 * @brief Maps Video Memory to VRAM according to mode info
 * @param mode Graphics Mode to be used
 * @return EXIT_SUCCESS if it finishes successfully
 */
int(map_vram)(uint16_t mode);

/**
 * @brief Maps Video Memory to VRAM calling (map_vram) and then sets VBE mode with (vbe_set_mode)
 * @param mode Mode to initialize the video graphic to
 */
void *(video_graphic_init) (uint16_t mode);

/**
 * @brief Copies auxiliar buffer memory into the physicall memory buffer
 * @return EXIT_SUCCESS if it finishes successfully
 */
int(flush_screen)();

/**
 * @brief Changes Color of a pixel
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Color code for the desired color for the pixel
 */
int(fill_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Uses (fill_pixel) to draw an horizontal line
 * @param x Leftmost Pixel's X Coordinate
 * @param y Leftmost Pixel´s Y Coordinate
 * @param len Line's Length in pixels
 * @param color Color code for the desired color for the line
 * @return EXIT_SUCCESS if it finishes successfully
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a Rectangle using (vg_draw_hline)
 * @param x X Coordinate of the pixel in the upper left corner of the rectangle
 * @param y Y Coordinate of the pixel in the upper left corner of the rectangle
 * @param width Rectangle's width in pixels
 * @param height Rectangle's height in pixels
 * @param color Rectangle's color code
 * @return EXIT_SUCCESS if it finishes successfully
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Returns boolean if given mode is indexed
 * @return boolean if given mode is indexed
 */
bool(is_indexed_mode)(uint16_t mode);

/**
 * @brief Returns Horizontal Resolution in Pixels of Current Mode
 * @return Value of the horizontal resolution of the current graphics mode
 */
unsigned(get_hres)(void);

/**
 * @brief Returns Vertical Resolution in Pixels of Current Mode
 * @return Value of the Vertical resolution of the current graphics mode
 */
unsigned(get_vres)(void);

/**
 * @brief Returns Amount of Bits that Represent a Pixel in Current Mode
 * @return Bits per pixel in Current Mode
 */
unsigned(get_bits_per_pixel)(void);

/**
 * @brief Returns Red Mask Size
 * @return Red Mask Size
 */
uint8_t(getRedMaskSize)(void);

/**
 * @brief Returns Green Mask Size
 * @return Green Mask Size
 */
uint8_t(getGreenMaskSize)(void);

/**
 * @brief Returns Blue Mask Size
 * @return Blue Mask Size
 */
uint8_t(getBlueMaskSize)(void);

/**
 * @brief Returns Red Field Position
 * @return Red Field Position
 */
uint8_t(getRedFieldPosition)(void);

/**
 * @brief Returns Green Field Position
 * @return Green Field Position
 */
uint8_t(getGreenFieldPosition)(void);

/**
 * @brief Returns Blue Field Position
 * @return Blue Field Position
 */
uint8_t(getBlueFieldPosition)(void);

#endif
