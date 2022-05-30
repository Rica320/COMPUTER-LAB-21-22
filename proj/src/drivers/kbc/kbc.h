#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

uint8_t scancode[2];
uint8_t mouse_data[3];
uint8_t mouse_received_count;
bool mouse_packet_completed;
int scancode_sz;
bool two_byte_scancode;
bool error_flag;

/**
 * @brief Subscribes interrupts from the keyboard controller
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @param *hook_id Identifier of the interrupt to be subscribed
 * @param kbc IRQ Line for the Keyboard Controller
 * @return EXIT_SUCCESS on success or EXIT_FAILURE if an error occurs
 */
int (subscribe_kbc_interrupt)(uint8_t bit_no, int *hook_id, int kbc); // CHANGE THIS FROM HERE
/**
 * @brief Unsubscribes interrupts from the keyboard controller
 * @param *hook_id Identifier of the interrupt to be unsubscribed
 * @return EXIT_SUCCESS on success or EXIT_FAILURE if an error occurs
 */
int (unsubscribe_interrupt)(int *hook_id);
/**
 * @brief Tries to read keyboard scancodes and mouse packets
 * @return EXIT_SUCCESS on success or EXIT_FAILURE if an error occurs
 */
int (kbc_read_i)();
/**
 * @brief Returns the error flag that was set when the error occurred
 * @return Flag identifying the error that occurred
 */
bool (kbc_get_error)();
/**
 * @brief Handles KBC Interrupts
 */
void (kbc_ih)(void);
/**
 * @brief Checks if the last read byte was the last byte of a scancode or not
 * @return True if the read byte isn't 0xE0 (first byte of the two byte scancode) or False if it is
 */
bool (kbc_ready)();
/**
 * @brief Checks if the currently received packet is already completed
 * @return True if it is or False if it isn't
 */
bool (kbc_mouse_ready)();
/**
 * @brief Get the last scancode read in a given array
 * @param *scan Pointer to the array to receive the scancode
 * @param *scan_sz Pointer to the Size of the Scancode array
 */
void (kbc_get_scancode)(unsigned char *scan, int* scan_sz);
/**
 * @brief Get the last mouse packet read in a given array
 * @param *scan Array to be filled with the packet
 */
void (kbc_get_mouse_data)(unsigned char *scan);
int (kbc_read)(uint8_t * code);

int (kbc_issue_cmd)(uint8_t cmd); 
int (kbc_read_cmd)(uint8_t *cmd);
int (kbc_write_cmd)(uint8_t cmd);
int (kbc_send_mouse_cmd)(uint8_t cmd);
int (kbc_cmd_arg)(uint8_t arg);
int (kbc_check_cmd)();

#endif
