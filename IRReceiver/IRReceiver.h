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

#include <stdbool.h>

#ifdef F_CPU
#	define SYSCLOCK F_CPU
#else
#	define SYSCLOCK 8000000UL
#endif

#define IR_RECV_PORT_D
#define IR_RECV_PIN PORTD2

#define BLINK_LED

#define BLINK_LED_PORT_C
#define BLINK_LED_PIN PORTC5


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



#define UNKNOWN -1
#define ERR false
#define DECODED true


// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#define TOLERANCE 25  // percent tolerance in measurements
#define LTOL (1.0 - TOLERANCE/100.)
#define UTOL (1.0 + TOLERANCE/100.)

#define _GAP 5000 // Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)

#define TICKS_LOW(us) (int) (((us)*LTOL/USECPERTICK))
#define TICKS_HIGH(us) (int) (((us)*UTOL/USECPERTICK + 1))

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5

// IR detector output is active low
#define MARK  0
#define SPACE 1

// defines for timer2 (8 bits)
#define TIMER_RESET			 ({TCNT2 = 0; TIFR2 = 0;})
#define TIMER_ENABLE_INTR    ({TIFR2 = 0; TIMSK2 = _BV(OCIE2A);})
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_INTR_NAME      TIMER2_COMPA_vect
#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000) //should be 400 for 8MHz and 50usec

// WGM21=1 -> CTC mode
#if (TIMER_COUNT_TOP < 256)
// CS20=1 -> SYSCLOCK without prescaling
#  define TIMER_CONFIG_NORMAL() ({ \
	TCCR2A = _BV(WGM21); \
	TCCR2B = _BV(CS20); \
	OCR2A = TIMER_COUNT_TOP; \
	TCNT2 = 0; \
})
#else
// CS21=1 -> SYSCLOCK/8
#  define TIMER_CONFIG_NORMAL() ({ \
	TCCR2A = _BV(WGM21); \
	TCCR2B = _BV(CS21); \
	OCR2A = TIMER_COUNT_TOP / 8; \
	TCNT2 = 0; \
})
#endif

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
	union { // This is used for decoding Panasonic and Sharp data - so unused here.
		unsigned int panasonicAddress;
		unsigned int sharpAddress;
	};
	unsigned long value; // Decoded value
	int bits; // Number of bits in decoded value
	volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
	int rawlen; // Number of records in rawbuf.
}
decode_results_t;

volatile irparams_t irparams;

bool decodeHashIRRecv(volatile decode_results_t *results);
void enableIRRecv();
void resumeIRRecv();




#endif /* IRRECEIVER_H_ */