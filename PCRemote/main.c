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
static unsigned long prev_ir_value = 0;

static void send_make_for_ircode(unsigned long ircode)
{
			    debug_log("M 0x");
			    sprintf(strbuf1,"%lx",decode_results.value);
			    debug_log(strbuf1);
			    debug_log("\r\n");
}

static void send_break_for_ircode(unsigned long ircode)
{
			    debug_log("B 0x");
			    sprintf(strbuf1,"%lx",decode_results.value);
			    debug_log(strbuf1);
			    debug_log("\r\n");
}

int main(void)
{
	init_debug_log();
	enableIRRecv();
	setup_ps2device(PINB2, PINB1);
	DDRB &= ~_BV(DDB0); // data direction input for B0
	PORTB |= _BV(PORTB0); // enable pullup
    while (1) 
    {
		do_ps2device_work();
        switch(decodeHashIRRecv(&decode_results))
        {
            case DECODED:
            {
			    /*debug_log("-R:");
			    sprintf(strbuf1,"%d",decode_results.rawlen);
			    debug_log(strbuf1);
			    debug_log("\r\n");
			    for (int i = 0; i< decode_results.rawlen; i++)
			    {
				    sprintf(strbuf1, "%d",decode_results.rawbuf[i]);
				    debug_log(strbuf1);
				    if (i + 1 < decode_results.rawlen)
                    {
					    debug_log(",");
				    }
			    }*/
                if (prev_ir_value && prev_ir_value != decode_results.value)
                {
                    send_break_for_ircode(prev_ir_value);
                }
                send_make_for_ircode(decode_results.value);
                prev_ir_value = decode_results.value;
                break;
            }
            case BUTTON_RELEASED:
            {
                send_break_for_ircode(prev_ir_value);
                prev_ir_value = 0;
                break;
            }
            default:
            {
                break;
            }
        }            
		if (bit_is_clear(PINB,PINB0)) {
			debug_log("Make U ARROW\r\n");
			SEND_EXT_MAKE(PS2DC_U_ARROW_EXT);
			_delay_ms(100);
			debug_log("Break U ARROW\r\n");
			SEND_EXT_BREAK(PS2DC_U_ARROW_EXT);
			_delay_ms(100);
		}
    }
}

