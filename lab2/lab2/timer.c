#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;        //Incremental Counter Global Variable For Interrupts
static int hook_id;            //For timer_subscribe_int and timer_unsubscribe_int


/*Changes the operating sequence of a timer
Must use the read back command so it doesn't change the 4 LSB's (mode and BCD binary) of the timer's control word
timer = Timer to configure (ranges from 0 to 2)
freq =  Timer operating frequency
return: 0 on sucess and non-zero otherwise
*/

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  //Reads current timer configuration  
  uint8_t conf;
  timer_get_conf(timer, &conf); //conf = Read-Back Command lido

  //Control Word keeping conf 4 LSB's, Setting Initialization Mode to LSB followed by MSB (TIMER_LSB_MSB)
  conf = TIMER_SEL(timer)| TIMER_LSB_MSB | LSHUB_IN_BYTE(conf);
  
  //Relative Frequency
  uint16_t relFreq = (uint16_t)(TIMER_FREQ / freq);

  //Reading LSB and RSB from Relative Frequency
  uint8_t lsb, msb;
  util_get_LSB(relFreq, &lsb);
  util_get_MSB(relFreq, &msb);

  //Changing Timer Configuration
  sys_outb(TIMER_CTRL, conf);
  sys_outb(TIMER_0 + timer, lsb);
  sys_outb(TIMER_0 + timer, msb);
  return 0;
}

/*Subscribes and enables Timer 0 interrupts
bit_no	= address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
return: 0 upon success and non-zero otherwise
*/
int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  return 0;
}

/*Unsubscribes Timer 0 interrupts.
Return: 0 upon success and non-zero otherwise
*/
int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

/*Timer 0 interrupt handler.
Increments counter
*/
void (timer_int_handler)() {
  counter++;
}

/*Reads the input timer configuration (status) via read-back command.
timer = Timer whose configuration to read (Ranges from 0 to 2)
st = Address of memory position to be filled with the timer config
Return 0 upon success and non-zero otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rbc = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL, rbc);
  util_sys_inb(TIMER_0 + timer, st);
  return 0;
}

/*Shows timer configuration.
Displays, in a human friendly way, the specified field of a timer status, which was read via the read-back command
timer = timer whose configuration should be displayed (Ranges from 0 to 2)
st = status read via the read-back command
field = status field to display in human friendly way
*/
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;
    switch (field) {
    case (tsf_all):
        val.byte = st;
        break;
    case (tsf_initial):
        val.in_mode = (st & (BIT(4)|BIT(5))) >> 4;
        break;
    case (tsf_mode):
        val.count_mode = (st & (BIT(1)|BIT(2)|BIT(3)))>> 1;
        break;
    case (tsf_base):
        val.bcd = 1 & st;
        break;
    default:
        break;
  }
    timer_print_config(timer,field,val);
  return 0;
}
