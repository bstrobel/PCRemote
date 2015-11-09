/*
 * PS2KeybDevice.c
 *
 * Created: 24.10.2015 08:34:18
 *  Author: Bernd
 */ 
#include <avr/interrupt.h>
#include <avr/cpufunc.h> // for _NOP()
#include <stdio.h>
#include <stdbool.h>
#include "PS2KBDevice.h"


#define ENABLE_PCINT_DATA PCMSK0 |= _BV(_data_pin_port_b)
#define ENABLE_PCINT_CLK PCMSK0 |= _BV(_clock_pin_port_b)
#define DISABLE_PCINT_DATA PCMSK0 &= ~_BV(_data_pin_port_b)
#define DISABLE_PCINT_CLK PCMSK0 &= ~_BV(_clock_pin_port_b)
#define IS_CLOCK_OUTPUT bit_is_clear(DDRB,_clock_pin_port_b)

#define ENABLE_PCINT0S PCICR |= _BV(PCIE0)
#define DISABLE_PCINT0S PCICR &= ~_BV(PCIE0)

#define BIT_ACK_INDEX 0
#define BIT_STOP_INDEX BIT_ACK_INDEX + 1
#define BIT_PARITY_INDEX BIT_STOP_INDEX + 1
#define BIT_7_INDEX BIT_PARITY_INDEX + 1
#define BIT_6_INDEX BIT_7_INDEX + 1
#define BIT_5_INDEX BIT_6_INDEX + 1
#define BIT_4_INDEX BIT_5_INDEX + 1
#define BIT_3_INDEX BIT_4_INDEX + 1
#define BIT_2_INDEX BIT_3_INDEX + 1
#define BIT_1_INDEX BIT_2_INDEX + 1
#define BIT_0_INDEX BIT_1_INDEX + 1

#define RCV_BUF_SIZE 16
#define SND_BUF_SIZE 16
static volatile char _recv_buf[RCV_BUF_SIZE];
static volatile char _send_buf[SND_BUF_SIZE];
static volatile uint8_t _recv_buf_head = 0;
static volatile uint8_t _recv_buf_len = 0;
static volatile bool _recv_buf_overflow = false;
static volatile uint8_t _send_buf_head = 0;
static volatile uint8_t _send_buf_len = 0;
static volatile	bool _next_byte_typematic_rate = false;
static volatile bool _next_byte_led = false;

static volatile	bool _ps2dev_enabled = true;
static volatile uint8_t _rcvd_char;
static volatile uint8_t _rcvd_char_parity;
static volatile uint8_t _char_to_send;
static volatile uint8_t _char_to_send_parity;
static volatile bool _trans_in_progress = false;
static volatile bool _trans_interrupted = false;

static volatile int8_t _bit_index = BIT_ACK_INDEX;

static uint8_t _clock_pin_port_b;
static uint8_t _data_pin_port_b;

static volatile bool _host_req_dev_rcv = false; // signals that the host wants us to receive something
static volatile bool _recv_in_progress = false;
static volatile uint8_t _clock_state = 1;

static inline void _enable_timer0() {
	TCNT0 = 0; // reset the timer
	TCCR0B |= _BV(CS01); // enable the counter with iclk/8
	TCCR0B &= ~(_BV(CS02) | _BV(CS00)); // enable the counter with iclk/8
	TCCR0A |= _BV(WGM01); // CTC mode
	TIFR0 |= _BV(OCF0A); // clear output compare match A interrupt flag
	TIMSK0 |= _BV(OCIE0A); // enable interrupt generation on output compare match A
	OCR0A = 40; // iclk = 8MHz, iclk/8, ocr=40 -> 12.5kHz clock
}

static inline void _disable_timer0() {
	TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // disable the counter
	TIMSK0 &= ~_BV(OCIE0A);	// disable interrupt generation
}

static inline void _conf_pins_input() {
	DISABLE_PCINT0S;
	DISABLE_PCINT_DATA;
	DISABLE_PCINT_CLK;
	DDRB &= ~(_BV(_clock_pin_port_b) | _BV(_data_pin_port_b)); // defines pins as inputs
	PORTB |= _BV(_clock_pin_port_b) | _BV(_data_pin_port_b); // activates pull-up resistors
	_NOP();
	ENABLE_PCINT_CLK;
	ENABLE_PCINT_DATA;
	ENABLE_PCINT0S;
}

static inline void _conf_pins_output() {
	DISABLE_PCINT0S;
	DISABLE_PCINT_DATA;
	DISABLE_PCINT_CLK;
	DDRB |= _BV(_clock_pin_port_b) | _BV(_data_pin_port_b);
}

static inline void _conf_clock_input_high() {
	_clock_state = 1;
	DDRB &= ~_BV(_clock_pin_port_b); // defines as input
	PORTB |= _BV(_clock_pin_port_b); // activates pull-up resistor
	_NOP();
}

static inline void _conf_clock_out_low() {
	_clock_state = 0;
	DDRB |= _BV(_clock_pin_port_b); // defines as output
	PORTB &= ~_BV(_clock_pin_port_b); // set to low
}

// is meant to be called from ISR so no interrupt disabling etc.
// returning a 0 means buffer is full
static bool _put_char_into_rcv_buf(char c) {
	if (_recv_buf_len >= RCV_BUF_SIZE)
		return false;
	uint8_t i = _recv_buf_head + _recv_buf_len;
	if (i >= RCV_BUF_SIZE)
		i -= RCV_BUF_SIZE;
	_recv_buf[i] = c;
	_recv_buf_len++;
	return true;
}

// Scan codes never contain 0xff or 0x00, so we use 0x00 to signal that the buffer is empty
static char _get_char_from_rcv_buf()
{
	cli();
	if (_recv_buf_len == 0)
		return 0x0;
	char c = _recv_buf[_recv_buf_head];
	_recv_buf_head++;
	_recv_buf_len--;
	if (_recv_buf_head >= RCV_BUF_SIZE)
		_recv_buf_head = 0;
	sei();
	return c;
}

// returning a 0 means buffer is full
static bool _put_char_into_snd_buf(char c) {
	cli();
	if (_send_buf_len >= SND_BUF_SIZE)
		return false;
	uint8_t i = _send_buf_head + _send_buf_len;
	if (i >= SND_BUF_SIZE)
		i -= SND_BUF_SIZE;
	_send_buf[i] = c;
	_send_buf_len++;
	sei();
	return true;
}

// Scan codes never contain 0xff or 0x00, so we use 0x00 to signal that the buffer is empty
static inline char _get_char_from_snd_buf()
{
	if (_send_buf_len == 0) {
		return 0x0;
	}
	return _send_buf[_send_buf_head];
}

static void _remove_char_from_snd_buf() {
	cli();
	if (_send_buf_len == 0) { // just to make sure...
		return;
	}
	_send_buf_head++;
	_send_buf_len--;
	if (_send_buf_head >= SND_BUF_SIZE) {
		_send_buf_head = 0;
	}
	sei();
}

static uint8_t _parity(uint8_t x) {
	uint8_t y = x ^ (x >> 1);
	y ^= (y >> 2);
	y ^= (y >> 4);
	return y & 1;
}

static bool _send_char_to_host( uint8_t c) {
	if (!c || !_ps2dev_enabled) { // since the buffer returns 0x0 when empty
		return false;
	}
	while(_trans_in_progress) {}
	_trans_in_progress = true;
	_char_to_send = c;
	_char_to_send_parity = _parity(c);
	while(bit_is_clear(PINB,_clock_pin_port_b)) {
		//wait for the clock pin to become high
	}
	_conf_pins_output();
	PORTB &= ~_BV(_data_pin_port_b); // send the start bit (LOW)
	PORTB &= ~_BV(_clock_pin_port_b);
	_bit_index = BIT_0_INDEX;
	_clock_state = 0;
	_enable_timer0();
	while(_trans_in_progress) {
		//block until transmission to finish
	}
	bool x = _trans_interrupted;
	_trans_interrupted = false;
	return x;
}

ISR(PCINT0_vect) {
	uint8_t p = PINB; // save the value of the interrupt source pins
	if (bit_is_clear(p,_clock_pin_port_b) && bit_is_clear(p,_data_pin_port_b)) {
		_host_req_dev_rcv = 1; // get ready for the action
	} else if (bit_is_set(p,_clock_pin_port_b) && bit_is_clear(p,_data_pin_port_b) && _host_req_dev_rcv && !_recv_in_progress) {
		DISABLE_PCINT0S;
		DISABLE_PCINT_DATA;
		DISABLE_PCINT_CLK;
		_clock_state = 1;
		_bit_index = BIT_0_INDEX;
		_host_req_dev_rcv = 0;
		_recv_in_progress = 1;
		_enable_timer0();
	} else {
		// reset the state
		_host_req_dev_rcv = 0;
	}
}

ISR(TIMER0_COMPA_vect) {
	if (_clock_state) {
		if (bit_is_set(PINB,_clock_pin_port_b)) {
			if (_recv_in_progress) {
				if (_bit_index == BIT_ACK_INDEX) {
					DDRB |= _BV(_data_pin_port_b); //switch data to output
					PORTB &= ~_BV(_data_pin_port_b); //set to LOW - this is the ACK bit
				}
				if (_bit_index <0) {
					_host_req_dev_rcv = 0;
					_disable_timer0();
					_conf_pins_input();
					_recv_in_progress = 0;
					_recv_buf_overflow = !_put_char_into_rcv_buf(_rcvd_char);
				} else {
					_conf_clock_out_low(); // don't bring the clock line low after everything has been received!
				}
			}
			if (_trans_in_progress) {
				if (_bit_index < 0) {
					_disable_timer0();
					_conf_pins_input();
					_trans_in_progress = 0;
				} else {
					_conf_clock_out_low();
				}
			}
		} else {
			// host has pulled clock low -> bail out and get ready to receive something
			_disable_timer0();
			_conf_pins_input();
			_trans_in_progress = 0;
			_recv_in_progress = 0;
			_host_req_dev_rcv = 0;
		}
	} else {
		// we're just after the rising edge of the clock -> do something
		if (_recv_in_progress) {
			// read the value while clock is LOW
			uint8_t data_pin_val = (PINB & _BV(_data_pin_port_b)) ? 1 : 0;
			// set clock HIGH
			_conf_clock_input_high();
			// we receive something from the host
			switch (_bit_index) {
				case BIT_0_INDEX:
					_rcvd_char = data_pin_val;
					break;
				case BIT_1_INDEX:
					_rcvd_char |= (data_pin_val << 1);
					break;
				case BIT_2_INDEX:
					_rcvd_char |= (data_pin_val << 2);
					break;
				case BIT_3_INDEX:
					_rcvd_char |= (data_pin_val << 3);
					break;
				case BIT_4_INDEX:
					_rcvd_char |= (data_pin_val << 4);
					break;
				case BIT_5_INDEX:
					_rcvd_char |= (data_pin_val << 5);
					break;
				case BIT_6_INDEX:
					_rcvd_char |= (data_pin_val << 6);
					break;
				case BIT_7_INDEX:
					_rcvd_char |= (data_pin_val << 7);
					break;
				case BIT_PARITY_INDEX:
					_rcvd_char_parity = data_pin_val;
					break;
				case BIT_STOP_INDEX:
					break;
				case BIT_ACK_INDEX:
					break;
			}
		}
		if (_trans_in_progress) {
			_conf_clock_input_high();
			// we send something to the host
			switch (_bit_index) {
				case BIT_0_INDEX:
				case BIT_1_INDEX:
				case BIT_2_INDEX:
				case BIT_3_INDEX:
				case BIT_4_INDEX:
				case BIT_5_INDEX:
				case BIT_6_INDEX:
				case BIT_7_INDEX:
					if (_char_to_send & 1) {
						PORTB |= _BV(_data_pin_port_b);
						} else {
						PORTB &= ~_BV(_data_pin_port_b);
					}
					_char_to_send >>= 1;
					break;
				case BIT_PARITY_INDEX:
					if (_char_to_send_parity & 1) {
						PORTB &= ~_BV(_data_pin_port_b);
						} else {
						PORTB |= _BV(_data_pin_port_b);
					}
					break;
				case BIT_STOP_INDEX:
					PORTB |= _BV(_data_pin_port_b);
					break;
				case BIT_ACK_INDEX:
					break;
			}
		}
		_bit_index--;
	}
}

static void _clear_rcv_buf() {
	cli();
	_recv_buf_head=0;
	_recv_buf_len=0;
	sei();
}

static void _clear_send_buf() {
	cli();
	_send_buf_head=0;
	_send_buf_len=0;
	sei();
}

static void _reset_state() {
	cli();
	_conf_pins_input();
	_ps2dev_enabled = true;
	_recv_buf_overflow=false;
	_rcvd_char = 0;
	_rcvd_char_parity = 0;
	_char_to_send = 0;
	_char_to_send_parity = 0;
	_trans_in_progress = false;
	_recv_in_progress = false;
	_host_req_dev_rcv = false;
	_clock_state = 1;
	_bit_index = BIT_ACK_INDEX;
	_next_byte_led = false;
	_next_byte_typematic_rate = false;
	_clear_rcv_buf();
	_clear_send_buf();
	sei();
	_send_char_to_host(PS2DEVICE_BAT_OK);
}

void setup_ps2device(uint8_t clock_pin_port_b, uint8_t data_pin_port_b) {
	_clock_pin_port_b = clock_pin_port_b;
	_data_pin_port_b = data_pin_port_b;
	_reset_state();
}

void do_ps2device_work() {
	while (_recv_buf_len > 0) {
		_clear_send_buf();
		if (_recv_buf_overflow) {
			//printf"_recv_buf overflow!\n");
			_recv_buf_overflow = false;
		}
		uint8_t c = _get_char_from_rcv_buf();
		if (c < 0b11000000 && (_next_byte_led || _next_byte_typematic_rate)) {
			if (_next_byte_typematic_rate) {
				//printf"Typematic Rate: 0x%x\n",c);
			}
			if (_next_byte_led) {
				//printf"LED string: 0x%x\n",c);
			}
			_next_byte_led = _next_byte_typematic_rate = false;
			return;
		}
		switch (c) {
			case PS2HOST_CMD_ECHO:
				_send_char_to_host(PS2DEVICE_CMD_ECHO);
				//printf"Echo!\n");
				return;
			case PS2HOST_CMD_RESEND:
				//we don't care - so we send just an ack
				_send_char_to_host(PS2DEVICE_CMD_ACK);
				//printf"Resend requested!\n");
				continue;
			case PS2HOST_CMD_RESET:
				_send_char_to_host(PS2DEVICE_CMD_ACK);
				_reset_state();
				//printf"Reset requested!\n");
				return;
			case PS2HOST_CMD_SET_KEY_TYPE_MAKE:
			case PS2HOST_CMD_SET_KEY_TYPE_MAKE_BREAK:
			case PS2HOST_CMD_SET_KEY_TYPE_TYPEMATIC:
			case PS2HOST_CMD_SET_ALL_KEYS_TYPEMATIC_MAKE_BREAK:
			case PS2HOST_CMD_SET_ALL_KEYS_MAKE:
			case PS2HOST_CMD_SET_ALL_KEYS_MAKE_BREAK:
			case PS2HOST_CMD_SET_ALL_KEYS_TYPEMATIC:
				// this is generally not the correct handling but we dont care.
				_send_char_to_host(PS2DEVICE_CMD_ACK);
				//printf"One of the set make/break/typematic received! c=0x%x\n", c);
				return;
			case PS2HOST_CMD_SET_TYPEMATIC_RATE_DELAY:
				_next_byte_typematic_rate = true;
				continue;
			case PS2HOST_CMD_SET_RESET_LEDS:
				_next_byte_led = true;
				continue;
			case PS2HOST_CMD_READ_ID:
				_send_char_to_host(PS2DEVICE_ID_1);
				_send_char_to_host(PS2DEVICE_ID_2);
				//printf"Send Id requested!\n");
				return;
			case PS2HOST_CMD_SET_DEFAULT:
				_send_char_to_host(PS2DEVICE_CMD_ACK);
				//printf"Set default!\n");
				return;
			case PS2HOST_CMD_DISABLE:
				_ps2dev_enabled = false;
				_next_byte_led = _next_byte_typematic_rate = false;
				//printf"Device disabled!\n");
				return;
			case PS2HOST_CMD_ENABLE:
				_ps2dev_enabled = true;
				//printf"Device enabled!\n");
				return;
			default:
				//printf"Unrecognized command received! c=0x%x\n", c);
				break;
		}
	}	
	while(_send_buf_len > 0) {
		if (!_send_char_to_host(_get_char_from_snd_buf())) {
			_remove_char_from_snd_buf();
		}
	}
}

bool add_char_to_send_buf(char c) {
	return _put_char_into_snd_buf(c);
}