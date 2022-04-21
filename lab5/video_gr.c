#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <lcom/video_gr.h>
#include <vbe.h>

#include <handlers.h>


void *(vg_init)(uint16_t mode) {

  vbe_mode_info_t vmi;
  CHECKCall(vbe_get_mode_info(mode, &vmi));

  reg86_t r86;
   
  /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */

  r86.intno = 0x10;
  r86.ah = 0x4f;
  r86.al = VBE_SET_MODE;
  r86.bx = mode | BIT(14);
  
  /* Make the BIOS call */

  if( sys_int86(&r86) != OK ) { // TODO: WHAT ABOUT THAT AH STUFF
    printf("\tvg_exit(): sys_int86() failed \n");
  }

  return NULL;  
}

