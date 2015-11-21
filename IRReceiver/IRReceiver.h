/*
 * IRReceiver.h
 *
 * Based on the work found here https://github.com/z3t0/Arduino-IRremote
 * 
 * Created: 08.11.2015 12:16:02
 *  Author: Bernd
 */ 


#ifndef IRRECEIVER_H_
#define IRRECEIVER_H_
#include "../PCRemote/conf.h"

#include <stdbool.h>

#ifdef F_CPU
#	define SYSCLOCK F_CPU
#else
#	define SYSCLOCK 8000000UL
#endif


#ifdef IR_RECV_PORT_D
#	define _IRPORT PORTD
#	define _IRPIN PIND
#	define _IRDDR DDRD
#endif
#ifdef IR_RECV_PORT_C
#	define _IRPORT PORTC
#	define _IRPIN PINC
#	define _IRDDR DDRC
#endif
#ifdef IR_RECV_PORT_B
#	define _IRPORT PORTB
#	define _IRPIN PINB
#	define _IRDDR DDRB
#endif

#ifdef BLINK_LED_PORT_D
#	define _LED_PORT PORTD
#	define _LED_DDR DDRD
#endif
#ifdef BLINK_LED_PORT_C
#	define _LED_PORT PORTC
#	define _LED_DDR DDRC
#endif
#ifdef BLINK_LED_PORT_B
#	define _LED_PORT PORTB
#	define _LED_DDR DDRB
#endif



#define HASH -1
typedef enum
{
    DECODED,
    BUTTON_RELEASED,
    IN_TRANSMISSION,
    IDLE
} DECODE_STATE;


// Some useful constants

#define MICROS_PER_TICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

#define BUTTON_RELEASED_GAP_MICROS 150000UL // Time in microseconds after we decide that the button was released
#define TRANSMISSION_GAP_MICROS 5000 // Minimum map between transmissions
#define GAP_TICKS (TRANSMISSION_GAP_MICROS/MICROS_PER_TICK)
#define BTN_RELEASE_GAP_TICKS (BUTTON_RELEASED_GAP_MICROS/MICROS_PER_TICK) 

// receiver states
typedef enum
{
    STATE_IDLE,
    STATE_MARK,
    STATE_SPACE,
    STATE_TRANSMISSION_COMPLETED
} IR_RECV_STATES;

// IR detector output is active low
#define MARK  0
#define SPACE 1

// defines for timer2 (8 bits)
#define TIMER_RESET			 ({TCNT2 = 0; TIFR2 = 0;})
#define TIMER_ENABLE_INTR    ({TIFR2 = 0; TIMSK2 = _BV(OCIE2A);})
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_COUNT_TOP      (SYSCLOCK * MICROS_PER_TICK / 1000000) //should be 400 for 8MHz and 50usec
#define TIMER_CONFIG_NORMAL() ({ \
	TCCR2A = _BV(WGM21); \
	TCCR2B = _BV(CS21); \
	OCR2A = TIMER_COUNT_TOP / 8; \
	TCNT2 = 0; \
})

// information for the interrupt handler
typedef struct {
	uint8_t rcvstate;          // state machine
	unsigned int timer;     // state timer, counts 50uS ticks.
	unsigned int rawbuf[RAWBUF]; // raw data
	uint8_t rawlen;         // counter of entries in rawbuf
}
irparams_t;

// Results returned from the decoder
typedef struct {
	int decode_type; // (NEC, SONY, RC5,) UNKNOWN
	unsigned long value; // Decoded value
	uint8_t bits; // Number of bits in decoded value
	volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
	uint8_t rawlen; // Number of records in rawbuf.
}
decode_results_t;

DECODE_STATE decodeHashIRRecv(volatile decode_results_t *results);
void enableIRRecv();

#endif /* IRRECEIVER_H_ */