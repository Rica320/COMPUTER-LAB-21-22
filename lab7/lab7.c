#include <lcom/lcf.h>

#include "uart.h"
#include "stdio.h"
#include "uart_defines.h"
#include "handlers.h"
#include "i8254.h"
#include "kbc.h"
#include "keyboard.h"
#include "i8042.h"

extern uint32_t n_interrupts;

int main(int argc, char **argv) {

	lcf_set_language("EN-US");

  	// enables to log function invocations that are being "wrapped" by LCF
  	// [comment this out if you don't want/need it]
  	lcf_trace_calls("/home/lcom/labs/lab7/trace.txt");

  	// enables to save the output of printf function calls on a file
  	// [comment this out if you don't want/need it]
  	lcf_log_output("/home/lcom/labs/lab7/output.txt");

  	// handles control over to LCF
  	// [LCF handles command line arguments and invokes the right function]
  	if (lcf_start(argc, argv))
  	  return 1;

  	// LCF clean up tasks
  	// [must be the last statement before return]
  	lcf_cleanup();


	return 0;
}

int(proj_main_loop)(int argc, char *argv[]){
	uint8_t lcr;

	CHECKCall(util_sys_inb( COM1_ADDR + UART_IER, &lcr));

	ser_test_set(COM1_ADDR, BIT(0) | BIT(1), 1, BIT(3), 115200);

	int r, ipc_status, hook_id;
	uint8_t bit_n;	
	ser_subscribe_int(&bit_n, &hook_id);
	set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, true);

	uint32_t iir;

	message msg;
	uint16_t irq_set = BIT(bit_n);
	int i = 0;

	uint8_t timer_id = 0; // THE WAY WE IMPLEMENTED WE ALREADY KNOW THE TIMER ID
	uint16_t irqt_set;		 // TODO: 16 bits ?
	CHECKCall(timer_subscribe_int(&timer_id));
	irqt_set = BIT(timer_id);
	uint8_t bt;

	printf("\n---------READ----------\n");
	util_sys_inb(COM1_ADDR + UART_RBR, &bt);
	printf("\n-----%d-----\n", bt);

	uint8_t kbc_bit_no = 1;
	int kbc_hook_id = 0;
	bool esc_pressed = false;
	uint16_t irqk_set = BIT(kbc_bit_no);


	unsigned char scan[2];
	int scan_size;

	bool write = false;
	uint8_t writeByte = 0;

	CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id, KEYBOARD_IRQ));

	while (!esc_pressed)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.m_notify.interrupts & irq_set)
				{
					
					CHECKCall(sys_inb(COM1_ADDR + UART_IIR, &iir));
					
					if (!(iir & IER_RECEIVED_INT))
					{
						switch (((iir & SER_INT_ID) >> 1))
						{
						case SER_RX_INT:
						/* ... read received character */
							printf("\n---------READ----------\n");
							util_sys_inb(COM1_ADDR + UART_RBR, &bt);
							printf("\n-----%d-----\n", bt);
							
						break;
						case SER_TX_INT:
						/* ... put character to sent */
							printf("\n---------WRITE---------\n");
							write = true;
							
						break;
						case SER_RLS_INT:
							
							printf("----UPPER-----\n");

							
						break;
						case 0 :
							printf("MODEEM\n");

							break;
						}
					}
				}
				if (msg.m_notify.interrupts & irqt_set)
				{ 
					timer_int_handler();
					
					if (!(n_interrupts % TIMER_ASEC_FREQ))
					{						
						i++;
					}

					
				}
				if (msg.m_notify.interrupts & irqk_set)
				{
					kbc_ih();
					if (!kbc_get_error())
					{
						if (kbc_ready())
						{
							kbc_get_scancode(scan, &scan_size);
							if (scan[scan_size - 1] == ESC_BREAK_CODE)
							{
								esc_pressed = true;
							}

							else if (write)
							{
								printf("\nsending request:\n");
								write = false;
								CHECKCall(sys_outb(COM1_ADDR + UART_THR, writeByte));
								writeByte = scan[scan_size -1];
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
		else
		{
		}
		//i++;
	}

	set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, false);

	//CHECKCall(sys_outb(COM1_ADDR + UART_IER, 0));
	CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
	ser_unsubscribe_int(&hook_id);
	


	return 0;
}
