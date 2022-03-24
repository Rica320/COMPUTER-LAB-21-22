#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


/*Changes the operating sequence of a timer
Must use the read back command so it doesn't change the 4 LSB's (mode and BCD binary) of the timer's control word
timer = Timer to configure (ranges from 0 to 2)
freq =  Timer operating frequency
return: 0 on sucess and non-zero otherwise
*/

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  return 1;
}

/*Subscribes and enables Timer 0 interrupts
bit_no	= address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
return: 0 upon success and non-zero otherwise
*/
int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/*Unsubscribes Timer 0 interrupts.
Return: 0 upon success and non-zero otherwise
*/
int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/*Timer 0 interrupt handler.
Increments counter
*/
void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

/*Reads the input timer configuration (status) via read-back command.
timer = Timer whose configuration to read (Ranges from 0 to 2)
st = Address of memory position to be filled with the timer config
Return 0 upon success and non-zero otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/*Shows timer configuration.
Displays, in a human friendly way, the specified field of a timer status, which was read via the read-back command
timer = timer whose configuration should be displayed (Ranges from 0 to 2)
st = status read via the read-back command
field = status field to display in human friendly way
*/
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
