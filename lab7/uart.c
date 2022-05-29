#include "uart.h"
#include "handlers.h"
#include "uart_defines.h"



// int ser_test_conf(unsigned short base_addr) {
//     /* To be completed */
// }

int ser_set_bits_per_char(unsigned short base_addr, unsigned long bits) {
    uint32_t lcr;

	CHECKCall(sys_inb( base_addr + UART_LCR, &lcr));

    lcr = (lcr & 0xfc) | bits; // TODO: MAGIC ::: CHECK INPUT TOO

    CHECKCall(sys_outb(base_addr + UART_LCR, lcr));

    return EXIT_SUCCESS;
}



int ser_set_stop(unsigned short base_addr, unsigned long stop) {

    stop-=1;
    stop = (uint8_t)((stop&1)<<2); // se for 1 ent liga senão n, deve dar

    uint32_t lcr;

	CHECKCall(sys_inb( base_addr + UART_LCR, &lcr));

    lcr = (lcr & ~UART_STOP_BIT) | stop;

    CHECKCall(sys_outb( base_addr + UART_LCR, lcr));

    return EXIT_SUCCESS;
}

int ser_set_par(unsigned short base_addr, long parity) {
    uint32_t lcr;

	CHECKCall(sys_inb( base_addr + UART_LCR, &lcr));

    lcr = (lcr & 0xC7) | parity; // TODO: MAGIC ::: CHECK INPUT TOO

    CHECKCall(sys_outb(base_addr + UART_LCR, lcr));
    return EXIT_SUCCESS;
}

int enable_dlab(unsigned short base_addr) {
    uint32_t lcr;
    CHECKCall(sys_inb(base_addr + UART_LCR, &lcr));
    
    lcr = lcr | LCR_DLAB;
    CHECKCall(sys_outb(base_addr + UART_LCR, lcr));
    return EXIT_SUCCESS;
}

int disable_dlab(unsigned short base_addr) {
    uint32_t lcr;
    CHECKCall(sys_inb(base_addr + UART_LCR, &lcr));
    
    lcr = lcr & ~LCR_DLAB;
    CHECKCall(sys_outb(base_addr + UART_LCR, lcr));
    return EXIT_SUCCESS;
}

int ser_set_bitrate(unsigned short base_addr, unsigned long bitrate) {

    CHECKCall(enable_dlab(base_addr));

    uint8_t lsb, msb;
    uint16_t rate = UART_BITRATE / bitrate;
    CHECKCall(util_get_LSB(rate, &lsb));
    CHECKCall(util_get_MSB(rate, &msb));
    CHECKCall(sys_outb(COM1_ADDR + UART_DLL, lsb));
    CHECKCall(sys_outb(COM1_ADDR + UART_DLM, msb));

    CHECKCall(disable_dlab(base_addr));

    return EXIT_SUCCESS;
}

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	          long parity, unsigned long rate) { 

	// uint32_t lcr;

    CHECKCall(ser_set_bits_per_char(base_addr ,bits));

    //CHECKCall(sys_inb( COM1_ADDR + UART_LCR, &lcr));

    CHECKCall(ser_set_stop(base_addr , stop));

    CHECKCall(ser_set_par(base_addr , parity));

    CHECKCall(ser_set_bitrate(base_addr ,rate));

    return EXIT_SUCCESS;
}

int set_ier(unsigned short base_addr, uint8_t ierToSet, bool enable) {
    uint8_t ier = 0;

    CHECKCall(util_sys_inb(base_addr + UART_IER, &ier));

    if (enable)
        ier = ier | ierToSet;
    else
        ier = ier & ~ierToSet;


    CHECKCall(sys_outb(base_addr + UART_IER, ier));
    
    return 0;
}

int ser_subscribe_int(uint8_t *bit_no, int* hook_id) {
    *hook_id = 4;
    *bit_no = *hook_id;

    CHECKCall(sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), hook_id));

    return 0;
}

int ser_unsubscribe_int(int* hook_id) {
    CHECKCall(sys_irqrmpolicy(hook_id));
    return 0;
}

//
//int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
//                    unsigned long stop, long parity, unsigned long rate, 
//                    int stringc, char *strings[]) {
//    /* To be completed */
//}
//
//int ser_test_int(/* details to be provided */) { 
//    /* To be completed */
//}
//
//int ser_test_fifo(/* details to be provided */) {
//    /* To be completed */
//}
//
