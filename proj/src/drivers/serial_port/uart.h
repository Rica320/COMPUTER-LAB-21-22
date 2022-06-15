#include <lcom/lcf.h>

#ifndef _LCOM_UART_H_
#  define _LCOM_UART_H_

enum INT_TYPE {
  RECEIVE_DATA,
  TX_EMPTY,
  ERROR,
  OTHER,
  NO_INT
};

static enum INT_TYPE type = NONE;
/**
 * @brief Sets the approriate configuration to use in the uart communication
 * 
 * @param base_addr base addr to use in comunnication
 * @param bits bits per char
 * @param stop nr.stop bits
 * @param parity parity ... -1: none, 0: even, 1: odd
 * @param rate baund rate  
 * @return int 
 */
int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop,
                 long parity,
                 unsigned long rate);

/**
 * @brief Sets the bit rate / baund rate
 * 
 * @param base_addr base addr to use in comunnication
 * @param bitrate the desired bit rate
 * @return int 
 */
int ser_set_bitrate(unsigned short base_addr, unsigned long bitrate);

/**
 * @brief Sets the nr of stop bits
 * 
 * @param base_addr base addr to use in comunnication
 * @param stop stops bits 
 * @return int 
 */
int ser_set_stop(unsigned short base_addr, unsigned long stop);

/**
 * @brief Sets the parity 
 * 
 * @param base_addr base addr to use in comunnication
 * @param parity the desired parity
 * @return int 
 */
int ser_set_par(unsigned short base_addr, long parity);

/**
 * @brief Sets the nr of bits to send in a communication 
 * 
 * @param base_addr  base addr to use in comunnication
 * @param bits the desired number
 * @return int 
 */
int ser_set_bits_per_char(unsigned short base_addr, unsigned long bits);

/**
 * @brief Enables the Divisor Latch 
 * 
 * @param base_addr base addr to use in comunnication
 * @return int 
 */
int enable_dlab(unsigned short base_addr);

/**
 * @brief Disables the Divisor Latch
 * 
 * @param base_addr base addr to use in comunnication
 * @return int 
 */
int disable_dlab(unsigned short base_addr);

/**
 * @brief Set the ier interrups desired, or disables them
 * 
 * @param base_addr base addr to use in comunnication
 * @param ierToSet a byte with the wanted interrups set
 * @param enable name is explanatory, true to enable, else disable
 * @return int 
 */
int set_ier(unsigned short base_addr, uint8_t ierToSet, bool enable);

/**
 * @brief subscribes an serial interrupt 
 * 
 * @param bit_no the bit num attached to the interrupt
 * @param hook_id hook id that identifies the interrupt
 * @return int 
 */
int ser_subscribe_int(uint8_t *bit_no, int *hook_id);

/**
 * @brief unsubscribes an serial interrupt 
 * 
 * @param hook_id the identifier of the interrupt
 * @return int 
 */
int ser_unsubscribe_int(int *hook_id);

/**
 * @brief allows writing to the serial port transmitter
 * 
 * @param base_addr base addr to use in comunnication
 * @param byte the byte to send to the uart
 * @return int 
 */
int ser_writeb(unsigned short base_addr, uint8_t byte);

/**
 * @brief reads a byte from the serial port received data
 * 
 * @param base_addr base addr to use in comunnication
 * @param byte the place to store the read
 * @return int 
 */
int ser_readb(unsigned short base_addr, uint8_t *byte);

/**
 * @brief Handles a uart ih
 * 
 */
void uart_ih();

/**
 * @brief Get the type of interrupt that last occured on the serial port. This is used to try to make our serial 
 * ih more independent from the app, the fifo would be great for that. 
 * 
 * @return int 
 */
int get_int_type();

#endif
