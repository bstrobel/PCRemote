/*
 * main.c
 *
 * Created: 22.10.2015 15:32:58
 * Author : Bernd
 */ 
#define F_CPU 8000000UL
#define UART_BAUD  9600

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../UART/uart.h"
#include "../PS2KBDevice/PS2KBDevice.h"
#include "../IRReceiver/IRReceiver.h"

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
volatile decode_results_t decode_results;

int main(void)
{

	uart_init();
	enableIRRecv();
	setup_ps2device(PINB2, PINB1);
	stdin = stdout = &uart_str;
	DDRB &= ~_BV(DDB0); // data direction input for B0
	PORTB |= _BV(PORTB0); // enable pullup
	DDRD &= ~(_BV(DDD7) | _BV(DDD6) | _BV(DDD5));
	PORTD |= _BV(PORTD7) | _BV(PORTD6) | _BV(PORTD5);
	resumeIRRecv();
    while (1) 
    {
		do_ps2device_work();
		if (decodeHashIRRecv(&decode_results)) {
			resumeIRRecv();
			printf("Received bytes %d\n",decode_results.rawlen);
			for (int i = 0; i< decode_results.rawlen; i++)
			{
				printf("rawbuf[%d] %#x\n",i,decode_results.rawbuf[i]);
			}
		}
		if (bit_is_clear(PINB,PINB0)) {
			printf("Make U ARROW\n");
			SEND_EXT_MAKE(PS2DC_U_ARROW_EXT);
			_delay_ms(100);
			printf("Break U ARROW\n");
			SEND_EXT_BREAK(PS2DC_U_ARROW_EXT);
		}
		if (bit_is_clear(PIND,PIND7)) {
			printf("Make D ARROW\n");
			SEND_EXT_MAKE(PS2DC_D_ARROW_EXT);
			_delay_ms(100);
			printf("Break D ARROW\n");
			SEND_EXT_BREAK(PS2DC_D_ARROW_EXT);
		}
		if (bit_is_clear(PIND,PIND6)) {
			printf("Make R ARROW\n");
			SEND_EXT_MAKE(PS2DC_R_ARROW_EXT);
			_delay_ms(100);
			printf("Break R ARROW\n");
			SEND_EXT_BREAK(PS2DC_R_ARROW_EXT);
		}
		if (bit_is_clear(PIND,PIND5)) {
			printf("Make L ARROW\n");
			SEND_EXT_MAKE(PS2DC_L_ARROW_EXT);
			_delay_ms(100);
			printf("Break L ARROW\n");
			SEND_EXT_BREAK(PS2DC_L_ARROW_EXT);
		}
		//_delay_ms(100);
    }
}

