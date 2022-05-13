#include <lcom/lcf.h>

void*(vg_init)	(	uint16_t 	mode	)	{
  vbe_mode_info_t info;
  vbe_get_mode_info(mode, &info);
  return NULL;
}
