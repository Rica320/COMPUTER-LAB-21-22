#include "font.h"

static char kbd_US [128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',     
    0, /* <-- control key */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',  0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

// LETTERS
#include "../../assets/font/char_065.xpm" // A
#include "../../assets/font/char_066.xpm" // B
#include "../../assets/font/char_067.xpm" // C
#include "../../assets/font/char_068.xpm" // D
#include "../../assets/font/char_069.xpm" // E
#include "../../assets/font/char_070.xpm" // F
#include "../../assets/font/char_071.xpm" // G
#include "../../assets/font/char_072.xpm" // H
#include "../../assets/font/char_073.xpm" // I
#include "../../assets/font/char_074.xpm" // J
#include "../../assets/font/char_075.xpm" // K
#include "../../assets/font/char_076.xpm" // L
#include "../../assets/font/char_077.xpm" // M
#include "../../assets/font/char_078.xpm" // N
#include "../../assets/font/char_079.xpm" // O
#include "../../assets/font/char_080.xpm" // P
#include "../../assets/font/char_081.xpm" // Q
#include "../../assets/font/char_082.xpm" // R
#include "../../assets/font/char_083.xpm" // S
#include "../../assets/font/char_084.xpm" // T
#include "../../assets/font/char_085.xpm" // U
#include "../../assets/font/char_086.xpm" // V
#include "../../assets/font/char_087.xpm" // W
#include "../../assets/font/char_088.xpm" // X
#include "../../assets/font/char_089.xpm" // Y
#include "../../assets/font/char_090.xpm" // Z

xpm_map_t letters[] = {xpm_char_065, xpm_char_066, xpm_char_067, xpm_char_068, xpm_char_069, xpm_char_070, xpm_char_071, xpm_char_072, xpm_char_073, xpm_char_074, xpm_char_075, xpm_char_076, xpm_char_077, xpm_char_078, xpm_char_079, xpm_char_080, xpm_char_081, xpm_char_082, xpm_char_083, xpm_char_084, xpm_char_085, xpm_char_086, xpm_char_087, xpm_char_088, xpm_char_089, xpm_char_090};

// NUMBERS
#include "../../assets/font/char_048.xpm" // 0
#include "../../assets/font/char_049.xpm" // 1
#include "../../assets/font/char_050.xpm" // 2
#include "../../assets/font/char_051.xpm" // 3
#include "../../assets/font/char_052.xpm" // 4
#include "../../assets/font/char_053.xpm" // 5
#include "../../assets/font/char_054.xpm" // 6
#include "../../assets/font/char_055.xpm" // 7
#include "../../assets/font/char_056.xpm" // 8
#include "../../assets/font/char_057.xpm" // 9

xpm_map_t numbers[] = {xpm_char_048, xpm_char_049, xpm_char_050, xpm_char_051, xpm_char_052, xpm_char_053, xpm_char_054, xpm_char_055, xpm_char_056, xpm_char_057};

// SPECIAL CHARS
#include "../../assets/font/char_032.xpm" // SPACE
#include "../../assets/font/char_033.xpm" // !
#include "../../assets/font/char_035.xpm" // #
#include "../../assets/font/char_047.xpm" // /
#include "../../assets/font/char_058.xpm" // :
#include "../../assets/font/char_063.xpm" // ?

// xpm_map_t specials[] = {}; // por implementar se necessário

void draw_char(xpm_map_t xpm, int x0, int y0, int hexColor) {
  sprite_t *sprite = make_sprite(xpm, XPM_8_8_8_8);
  set_sprite_X(sprite, x0);
  set_sprite_Y(sprite, y0);
  draw_text_sprite(sprite, hexColor);
  free_sprite(sprite);
}

void draw_text(char *text, int x0, int y0, int hexColor, bool small) {
  if (f_small == NULL) {
    a = make_sprite(font_small, XPM_8_8_8_8);
    f_small = create_animSprite(a, 96, 16, 9 , 20);
  }

  if (small)
  {
    int margin = 0;
    for (unsigned i = 0; i < strlen(text); i++) {
      int l = (int) (text[i]) - 19;

      draw_animSprite(f_small, l, x0 + margin, y0);
      margin += 10;
    }
    return;
  }
  
  for (unsigned i = 0; i < strlen(text); i++) {

    int l = (int) (text[i]);

    if (l == 32) {
      x0 += 40;
      continue;
    }
    else if (l == 58)
      draw_char(xpm_char_058, x0, y0, hexColor);
    else if (l == 47) // /
      draw_char(xpm_char_047, x0, y0, hexColor);
    else if (l >= 65 && l <= 90)
      draw_char(letters[l - 65], x0, y0, hexColor);
    else if (l >= 48 && l <= 57)
      draw_char(numbers[l - 48], x0, y0, hexColor);
    else
      draw_char(xpm_char_035, x0, y0, hexColor);
    x0 += 40;
  }
}

char get_ascii_from_scancode(uint8_t scancode) {
  return kbd_US[scancode];
}
