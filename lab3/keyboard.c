#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <keyboard.h>
#include <handlers.h>
#include <i8042.h>

int scancode_sz = 1;
bool two_byte_scancode = false;
bool error_flag = false;

int (subscribe_kbc_interrupt)(uint8_t bit_no, int *hook_id) {
  NullSafety(&bit_no);
  NullSafety(hook_id);
  *hook_id = bit_no;
  CHECKCall(sys_irqsetpolicy( KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id));
  return EXIT_SUCCESS;
}

int (unsubscribe_interrupt)(int *hook_id) {
    NullSafety(hook_id);
    CHECKCall(sys_irqrmpolicy(hook_id));
    return EXIT_SUCCESS;
}

int (kbc_read_i)() {

  uint8_t st;

  if (two_byte_scancode) scancode_sz = 2;
  else scancode_sz = 1;
  CHECKCall(util_sys_inb(KBC_ST_REG, &st));

  if (st & (PARITY_ERROR | TIMEOUT_ERROR))
  {
    return EXIT_FAILURE;
  }
  
  if (!(st & OUT_BUF_FULL)) 
  { 
    return EXIT_FAILURE;

  }
  /*
  if (st & AUX_MOUSE) {
    return EXIT_FAILURE;
  }
  */

  uint8_t byte = 0;

  CHECKCall(util_sys_inb(OUT_BUF, &byte));
  
  scancode[scancode_sz - 1] = byte;
  two_byte_scancode = byte == TWO_BYTE_CODE;

  return EXIT_SUCCESS;
}

void (kbc_ih)(void) {

  if (kbc_read_i()) {
    error_flag = true;
  } else {
    error_flag = false;
  }
}

bool (kbc_ready)() {
  return !two_byte_scancode;
}
void (kbc_get_scancode)(unsigned char * scan, int* scan_sz) {
  for (int i = 0; i < scancode_sz; i++)  {
    scan[i] = scancode[i];
  }
  *scan_sz = scancode_sz;
}

bool (kbc_get_error)(){
  return error_flag;
}

int (kbc_read)(uint8_t * code) {
  uint8_t st;

  while (1) {
    CHECKCall(util_sys_inb(KBC_ST_REG, &st));

    if (st & OUT_BUF_FULL) { // WHAT ABOUT THE MOUSE !!! TODO
      CHECKCall(util_sys_inb(OUT_BUF, code));
      if (st & (PARITY_ERROR | TIMEOUT_ERROR)){
        return EXIT_FAILURE;
      }
      else {
        return EXIT_SUCCESS;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  
  return EXIT_FAILURE;
}
/***
 * IMPORTANT :::: kbc_read must not call CHECKCall as it might be inapropriate... 
 * it ends the program just because of a par ERROR or TIMEOUT :::: TO ASK ::: TODO
 * 
 */

int (kbd_poll)(uint8_t code[], uint8_t *size) {
  NullSafety(code);
  NullSafety(size);

  uint8_t byte;

  CHECKCall(kbc_read(&byte));
  code[0] = byte;
  *size = 1;
  
  if (byte == TWO_BYTE_CODE) {
    CHECKCall(kbc_read(&byte)); // CHECKCall(kbc_read(&code[1])); raised the above question (NOTE: THE ABOVE DOES NO HAPPEN AS THERE IS NO EXIT ON CHECKCALL BUT THERE WILL BE ALOT OF PRINTF)
    code[1] = byte;
    *size = 2;
  }

  return EXIT_SUCCESS;
}

int (kbc_issue_cmd)(uint8_t cmd) {

  uint8_t stat;

  while (1) { // TODO : LET IT LIKE THIS ?
    CHECKCall(util_sys_inb(KBC_ST_REG, &stat)); 
    if (!(stat & IN_BUF_FULL)) {
      CHECKCall(sys_outb(KBC_CMD_REG, cmd)); 
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return EXIT_FAILURE;
}

int (kbc_read_cmd)(uint8_t *cmd) {
  CHECKCall(kbc_issue_cmd(KBC_READ_CMD));
  CHECKCall(kbc_read(cmd));
  return EXIT_SUCCESS;
}

int (kbc_write_cmd)(uint8_t cmd) {
  CHECKCall(kbc_issue_cmd(KBC_WRITE_CMD));
  CHECKCall(sys_outb(OUT_BUF, cmd));
  return EXIT_SUCCESS;
}

// this functions can be helpfull in the future

int (kbc_check_cmd)() {
  uint8_t state;
  CHECKCall(kbc_issue_cmd(KBC_CHECK));
  CHECKCall(kbc_read(&state));

  if (state == KBC_CHECK_ERROR)
  {
    printf("KeyBoard Controller is not ok\n");
    exit(1);
  }
  
  return EXIT_SUCCESS;
}

int (kbc_kbd_interface_cmd)() {
  uint8_t state;
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_TEST));
  CHECKCall(kbc_read(&state));

  return state;
}

int (kbc_enable_kbd_cmd)() {
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_ENABLE));

  return EXIT_SUCCESS;
}

int (kbc_disable_kbd_cmd)() {
  CHECKCall(kbc_issue_cmd(KBC_INTERFACE_DISABLE));

  return EXIT_SUCCESS;
}

int (kbd_restore)() {
  // TODO: CHANGE THIS ... make it modular as said in the slides ... use the above func etc.
  uint8_t cmd;

  CHECKCall(kbc_read_cmd(&cmd));

  cmd |= (INT_KBD | INT_MOUSE); // ask why does INT_MOUSE is necessary ... it gives a warning if not used

  CHECKCall(kbc_write_cmd(cmd));

  return EXIT_SUCCESS;
}
