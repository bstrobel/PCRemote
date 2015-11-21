/*
 * PS2KBDevice.h
 *
 * Created: 24.10.2015 08:33:48
 *  Author: Bernd
 */ 


#ifndef PS2KBDEVICE_H_
#define PS2KBDEVICE_H_
#include "../PCRemote/conf.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include "PS2HostCmds.h"
#include "PS2DevCodes.h"

#define SEND_MAKE(code) \
	add_char_to_send_buf(code);\
	do_ps2device_work();
#define SEND_BREAK(code) \
	add_char_to_send_buf(PS2DC_BREAK_CODE);\
	add_char_to_send_buf(code);\
	do_ps2device_work();
#define SEND_EXT_MAKE(code) \
	add_char_to_send_buf(PS2DC_EXT_CODE);\
	add_char_to_send_buf(code);\
	do_ps2device_work();
#define SEND_EXT_BREAK(code) \
	add_char_to_send_buf(PS2DC_BREAK_CODE);\
	add_char_to_send_buf(PS2DC_EXT_CODE);\
	add_char_to_send_buf(code);\
	do_ps2device_work();


extern void setup_ps2device(uint8_t clock_pin_port_b, uint8_t data_pin_port_b);
extern void do_ps2device_work();
extern bool add_char_to_send_buf(char c);
#endif /* PS2KBDEVICE_H_ */