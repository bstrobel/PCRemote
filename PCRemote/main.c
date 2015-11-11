/*
 * main.c
 *
 * Created: 22.10.2015 15:32:58
 * Author : Bernd
 */ 
#define F_CPU 8000000UL
#define LOG_BUF_SIZE 512

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../PS2KBDevice/PS2KBDevice.h"
#include "../IRReceiver/IRReceiver.h"
#include "../IRReceiver/MCECodes.h"
#include "../../DebugLogger/DebugLogger/debug_logger.h"

volatile decode_results_t decode_results;
static char strbuf1[10];

int main(void)
{
	init_debug_log();
	enableIRRecv();
	setup_ps2device(PINB2, PINB1);
	DDRB &= ~_BV(DDB0); // data direction input for B0
	PORTB |= _BV(PORTB0); // enable pullup
	DDRD &= ~(_BV(DDD7) | _BV(DDD6) | _BV(DDD5));
	PORTD |= _BV(PORTD7) | _BV(PORTD6) | _BV(PORTD5);
    while (1) 
    {
		do_ps2device_work();
		if (decodeHashIRRecv(&decode_results)) {
			debug_log("-R:");
			sprintf(strbuf1,"%d",decode_results.rawlen);
			debug_log(strbuf1);
			debug_log("\r\n");
			for (int i = 0; i< decode_results.rawlen; i++)
			{
				sprintf(strbuf1, "%d",decode_results.rawbuf[i]);
				debug_log(strbuf1);
				if (i + 1 < decode_results.rawlen) {
					debug_log(",");
				}
			}
			debug_log("\r\n0x");
			sprintf(strbuf1,"%lx",decode_results.value);
			debug_log(strbuf1);
			debug_log("\r\n");
		}
		if (bit_is_clear(PINB,PINB0)) {
			debug_log("Make U ARROW\r\n");
			SEND_EXT_MAKE(PS2DC_U_ARROW_EXT);
			_delay_ms(100);
			debug_log("Break U ARROW\r\n");
			SEND_EXT_BREAK(PS2DC_U_ARROW_EXT);
			_delay_ms(100);
		}
		if (bit_is_clear(PIND,PIND7)) {
			debug_log("Make D ARROW\r\n");
			SEND_EXT_MAKE(PS2DC_D_ARROW_EXT);
			_delay_ms(100);
			debug_log("Break D ARROW\r\n");
			SEND_EXT_BREAK(PS2DC_D_ARROW_EXT);
			_delay_ms(100);
		}
		if (bit_is_clear(PIND,PIND6)) {
			debug_log("Make R ARROW\r\n");
			SEND_EXT_MAKE(PS2DC_R_ARROW_EXT);
			_delay_ms(100);
			debug_log("Break R ARROW\r\n");
			SEND_EXT_BREAK(PS2DC_R_ARROW_EXT);
			_delay_ms(100);
		}
		if (bit_is_clear(PIND,PIND5)) {
			debug_log("Make L ARROW\r\n");
			SEND_EXT_MAKE(PS2DC_L_ARROW_EXT);
			_delay_ms(100);
			debug_log("Break L ARROW\r\n");
			SEND_EXT_BREAK(PS2DC_L_ARROW_EXT);
			_delay_ms(100);
		}
    }
}

