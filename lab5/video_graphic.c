#include <lcom/lcf.h>

#include "video_graphic.h"


void*(vg_init)	(	uint16_t 	mode	)	{
  vg_map_vram(mode); //Mapeamento da VRAM
  vg_set_mode(mode); //Configurar Modo da Gráfica
  return NULL;
}

int vg_map_vram ( uint16_t mode) {
//Passo 1) Obter valores de resolução e nº de bits por pixel para o modo desejado. A função vbe_get_mode_info chama  a função 0x01 da VBE (Return VBE Mode Information).
  vbe_mode_info_t info;
  vbe_get_mode_info(mode, &info);

//Passo 2) Separar as informações obtidas na variável info e guardá-las em variáveis globais estáticas:

  //Endereço Físico da VRAM
  vram_base = info.PhysBasePtr;

  //Resoluções Horizontal e Vertical
  h_res = info.XResolution;
  v_res = info.YResolution;

  //Color Depth (Bits Por Pixel)
  bits_per_pixel = info.BitsPerPixel;

  //Tamanho da VRAM em bytes
  vram_size = (h_res*v_res)*(bits_per_pixel + 7)/8;

  //Modo de Cor (Formato)
  color_model = info.MemoryModel;

  //Tamanhos de Masks para Modo Direto de Cor
  RedMaskSize = info.RedMaskSize;
  GreenMaskSize = info.GreenMaskSize;
  BlueMaskSize = info.BlueMaskSize;

  //Tamanhos de Masks para Modo Direto Linear de Cor
  RedFieldPosition = info.RedFieldPosition;
  GreenFieldPosition = info.GreenFieldPosition;
  BlueFieldPosition = info.BlueFieldPosition;

//Passo 3) Conferir ao processo permissão para mapear a região de endereços da VRAM usando sys_privctl(). (CÓDIGO DISPONIBILIZADO NO LAB LIGEIRAMENTE ALTERADO DE ORDEM)

static void *video_mem;
struct minix_mem_range mr;
int r;				    
mr.mr_base = (phys_bytes) vram_base;	
mr.mr_limit = mr.mr_base + vram_size;  

if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

//Passo 4) Usar vm_map_phys para mapear a memória (CÓDIGO DISPONIBILIZADO NOS LABS)

video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

if(video_mem == MAP_FAILED)
   panic("couldn't map video memory");

return 0;
}

int vg_set_mode(uint16_t mode){

  vbe_mode_info_t info;
  vbe_get_mode_info(mode,&info);
  reg86_t r86;
   
  /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.ah = 0x4F;    /* Set VBE Mode Function */
  r86.al = 0x02;    /* VBE Set Mode */
  r86.bx = mode | BIT(14);
  
  /* Make the BIOS call */

  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(int i = y; i < y + height; i++){
    if (vg_draw_hline(x,i,width,color)) return 1;
  }
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(uint16_t i = x; i<= x + len; i++){
    if(vg_fill_pixel(i,y,color)) return 1;
  }
  return 0;
}

int vg_fill_pixel(uint16_t x, uint16_t y, uint32_t color){
  if(x > h_res || y > v_res) return 1;
  uint8_t* mem = video_mem; //pointer mem aponta para o primeiro elemento de video_mem
  mem += (x+(y*h_res))*(bits_per_pixel); //pointer aponta para o píxel a mudar
  memcpy(mem,&color,bits_per_pixel);
  return 0;
}
