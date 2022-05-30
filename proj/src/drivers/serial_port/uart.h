#include <lcom/lcf.h>

#ifndef _LCOM_UART_H_
#define _LCOM_UART_H_

enum INT_TYPE {
	RECEIVE_DATA,
	TX_EMPTY,
	ERROR,
	OTHER,
	NO_INT
};

static enum INT_TYPE type = NONE;

int ser_test_conf(unsigned short base_addr);

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	     long parity, /* -1: none, 0: even, 1: odd */
	     unsigned long rate);

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 	unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);

int ser_test_int(/* details to be provided */) ;

int ser_test_fifo(/* details to be provided */);

int ser_set_bitrate(unsigned short base_addr, unsigned long bitrate);

int ser_set_stop(unsigned short base_addr, unsigned long stop);

int ser_set_par(unsigned short base_addr, long parity);

int ser_set_bits_per_char(unsigned short base_addr, unsigned long bits);

int enable_dlab(unsigned short base_addr);

int disable_dlab(unsigned short base_addr);

int set_ier(unsigned short base_addr, uint8_t ierToSet, bool enable);

int ser_subscribe_int(uint8_t *bit_no, int* hook_id);

int ser_unsubscribe_int(int* hook_id);

int ser_writeb(unsigned short base_addr, uint8_t byte);

int ser_readb(unsigned short base_addr, uint8_t * byte);

void uart_ih();

int get_int_type();

#endif
