#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <keyboard.h>
#include <handlers.h>
#include <i8042.h>

int scancode_sz = 1;
bool two_byte_scancode = false;

int (keyboard_subscribe_kbc_interrupts)(uint8_t bit_no, int *hook_id) {
  NullSafety(&bit_no);
  NullSafety(hook_id);
  *hook_id = bit_no;
  CHECKCall(sys_irqsetpolicy( KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id));
  return EXIT_SUCCESS;
}

int (unsubscribe_kbc_interrupt)(int *hook_id) {
    NullSafety(hook_id);
    CHECKCall(sys_irqrmpolicy(hook_id));
    return EXIT_SUCCESS;
}

void (kbc_ih)(void) {
  uint8_t st;

  CHECKCall(util_sys_inb(KBC_ST_REG, &st));

  // DOES THE TYPE OF ERROR MATTERS ?
  // TO ASK: IF THERE IS AN ERROR WILL THE PREVIOUS INPUT BE READ AGAIN?
  if (st & (PARITY_ERROR | TIMEOUT_ERROR))
  {
    // TELL SOMEONE THAT THIS WAS AN ERROR ::: TODO
    return;
  }
  
  if (!(st & OUT_BUF_FULL)) 
  { 
    // TELL SOMEONE THAT THIS WAS AN ERROR ::: TODO
    return;

  }
  if (st & AUX_MOUSE) {
    // TELL SOMEONE THAT THIS WAS AN ERROR ::: TODO
    return;
  }

  uint8_t byte = 0;

  CHECKCall(util_sys_inb(OUT_BUF, &byte));
  
  scancode[scancode_sz - 1] = byte;
  two_byte_scancode = byte == TWO_BYTE_CODE;
  if (two_byte_scancode) scancode_sz = 2;
}

