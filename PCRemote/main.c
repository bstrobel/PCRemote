/*
 * main.c
 *
 * Created: 22.10.2015 15:32:58
 * Author : Bernd
 */ 
#define F_CPU 8000000UL
#define LOG_BUF_SIZE 512
#define AUTO_REPEAT_LIMIT 4

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "MCEMapper.h"
#include "../PS2KBDevice/PS2KBDevice.h"
#include "../IRReceiver/IRReceiver.h"
#include "../../DebugLogger/DebugLogger/debug_logger.h"

volatile decode_results_t decode_results;
static char strbuf1[10];
static unsigned long prev_ir_value = 0;
static unsigned int same_ir_code_count = 0;

static void send_make_for_ircode(unsigned long ircode)
{
    send_kbcode_for_ir(ircode,MAKE);
    debug_log("M 0x");
    sprintf(strbuf1,"%lx",decode_results.value);
    debug_log(strbuf1);
    debug_log("\r\n");
}

static void send_break_for_ircode(unsigned long ircode)
{
    send_kbcode_for_ir(ircode,BREAK);
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
    while (1) 
    {
		do_ps2device_work();
        switch(decodeHashIRRecv(&decode_results))
        {
            case DECODED:
            {
			    /*
                 * This logs the whole received IR code to the UART.
                 * Enable it only when needed.
                 *
                debug_log("-R:");
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
			    }
                */
                if (prev_ir_value && prev_ir_value != decode_results.value)
                {
                    send_break_for_ircode(prev_ir_value);
                    same_ir_code_count = 1;
                } else {
                    same_ir_code_count++;
                }
                if (same_ir_code_count == 1 || same_ir_code_count > AUTO_REPEAT_LIMIT)
                {
                    send_make_for_ircode(decode_results.value);
                }
                prev_ir_value = decode_results.value;
                break;
            }
            case BUTTON_RELEASED:
            {
                send_break_for_ircode(prev_ir_value);
                prev_ir_value = 0;
                same_ir_code_count = 0;
                break;
            }
            default:
            {
                break;
            }
        }            
    }
}

