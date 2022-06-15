#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "lcom/lcf.h"

#include "../../../drivers/graphics/video_graphic.h"

/**
 * @brief Sprite struct defining an XPM loaded image
 *
 */
struct sprite {
  unsigned x, y;
  unsigned w, h;
  uint8_t *map;
};

typedef struct sprite sprite_t;

/**
 * @brief Allocates a sprite_t object with the given xpm and the image type
 *
 * @param xpm xpm to be converted into a sprite
 * @param type image type to be considered
 * @return sprite_t* sprite_t object created
 */
sprite_t *make_sprite(const xpm_map_t xpm, enum xpm_image_type type);

/**
 * @brief Frees a sprite_t struct memory
 *
 * @param p sprite to have its memory cleared
 */
void free_sprite(sprite_t *p);

/**
 * @brief Get the given sprite width
 *
 * @param sprite sprite to be used
 * @return unsigned int width of the given sprite
 */
unsigned int get_sprite_W(sprite_t *sprite);

/**
 * @brief Get the given sprite height
 *
 * @param sprite sprite to be used
 * @return unsigned int height of the given sprite
 */
unsigned int get_sprite_H(sprite_t *sprite);

/**
 * @brief Get the given sprite y pos
 *
 * @param sprite sprite to be used
 * @return unsigned int y pos of the given sprite
 */
unsigned int get_sprite_Y(sprite_t *sprite);

/**
 * @brief Get the given sprite x pos
 *
 * @param sprite sprite to be used
 * @return unsigned int x pos of the given sprite
 */
unsigned int get_sprite_X(sprite_t *sprite);

/**
 * @brief Set the sprite Y pos
 *
 * @param sprite sprite to be updated
 * @param y new sprite's y pos
 */
void set_sprite_Y(sprite_t *sprite, unsigned int y);

/**
 * @brief Set the sprite X pos
 *
 * @param sprite sprite to be updated
 * @param y new sprite's x pos
 */
void set_sprite_X(sprite_t *sprite, unsigned int x);

/**
 * @brief Set the sprite (x,y) position
 *
 * @param sprite sprite to be updated
 * @param x new sprite's x pos
 * @param y new sprite's y pos
 */
void set_sprite_pos(sprite_t *sprite, unsigned x, unsigned y);

/**
 * @brief Gets the sprite's color map
 *
 * @param sprite sprite to be used
 * @return uint8_t* sprite's color map pointer
 */
uint8_t *get_sprite_map(sprite_t *sprite);

/**
 * @brief Draws the given sprite in the 0x14C mode
 *
 * @param sprite sprite to be drawn
 * @return int 0 if draw = Ok
 */
int draw_sprite_in_mode_14c(sprite_t *sprite);

/**
 * @brief Draws a chess piece in the given (x,y) pos
 * using the provided color map
 * @param map color map for the piece to be drawn
 * @param x x cord of where to draw the piece
 * @param y y cord of where to draw the piece
 * @param size number of width/height piece's pixels
 * @return int 0 if draw = Ok
 */
int draw_piece_in_mode_14c(uint8_t *map, int x, int y, unsigned int size);

/**
 * @brief Draw the given text sprite using the given color map
 *
 * @param sprite text sprite to be drawn
 * @param color_map color map to be used in the draw
 * @return int 0 if draw = Ok
 */
int draw_text_sprite(sprite_t *sprite, int color_map);

/**
 * @brief Draws the given sprite in the 0x105 mode
 *
 * @param sprite sprite to be drawn
 * @return int 0 if draw = Ok
 */
int draw_sprite_in_mode_105(sprite_t *sprite);

#endif // SPRITE_H_INCLUDED
