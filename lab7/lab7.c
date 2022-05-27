#include <lcom/lcf.h>

#include "test7.h"
#include "stdio.h"
#include "uart.h"
#include "handlers.h"
#include "i8254.h"

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
	//printf("0x%08x \n", lcr);

	// we WILL HAVE TO DEVELOP SMTH TO INIT ... THERE SEEMS TO BE A DIFF BETWEEN THE STOP AND THE OTHER
	ser_test_set(COM1_ADDR, BIT(0) | BIT(1), 1, BIT(3), 115200);

	int r, ipc_status, hook_id;
	uint8_t bit_n;	
	ser_subscribe_int(&bit_n, &hook_id);
	set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, true);

	uint32_t iir;

	message msg;
	uint16_t irq_set = BIT(bit_n);
	int i = 0;

	// sleep(2);
	uint8_t byte = 0x3;
	//sys_outb(COM1_ADDR + UART_THR, byte);

	uint8_t timer_id = 0; // THE WAY WE IMPLEMENTED WE ALREADY KNOW THE TIMER ID
	uint16_t irqt_set;		 // TODO: 16 bits ?
	CHECKCall(timer_subscribe_int(&timer_id));
	irqt_set = BIT(timer_id);
	uint8_t bt;
	while (i < 10)
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
						//printf("SMTHG");

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
							sys_outb(COM1_ADDR + UART_THR, 0xff);
						break;
						case SER_TX_INT:
						/* ... put character to sent */
							printf("\n---------WRITE---------\n");
							// sys_outb(COM1_ADDR + UART_THR, byte);
						break;
						case SER_RLS_INT:
							/*... notify upper level */
							printf("----UPPER-----\n");
							// printf("\n---------READ----------\n");
							// util_sys_inb(COM1_ADDR + UART_RBR, &bt);
							// printf("\n-----%d-----\n", bt);
							// util_sys_inb(COM1_ADDR + UART_RBR, &bt);
							// printf("\n-----%d-----\n", bt);
							if (i == 0)
								sys_outb(COM1_ADDR + UART_THR, byte);
							
						break;
						case 0 :
							printf("MODEEM\n");

							break;
						}
						//printf("%d", iir & SER_INT_ID);
					} //else {
						//printf("SMTHG");
					//}
				}
				if (msg.m_notify.interrupts & irqt_set)
				{ /* subscribed interrupt */
					timer_int_handler();
					if (!(n_interrupts % TIMER_ASEC_FREQ))
					{								/* second elapsed */
						//timer_print_elapsed_time(); // WHAT TODO WITH THE RETURN VALUE
						//sys_outb(COM1_ADDR + UART_THR, byte);
							sys_outb(COM1_ADDR + UART_THR, ++byte);

						i++;
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
	ser_unsubscribe_int(&hook_id);
	


	return 0;
}
