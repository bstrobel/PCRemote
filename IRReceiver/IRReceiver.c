/*
 * IRReceiver.c
 *
 * Created: 08.11.2015 12:15:41
 *  Author: Bernd
 */ 
#include <avr/interrupt.h>

#include "IRReceiver.h"


static volatile irparams_t irparams;

// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
//    -> we don't need that at all. carries no information since the counter overflows without detection
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts
ISR(TIMER_INTR_NAME)
{

	uint8_t irdata = bit_is_set(_IRPIN,IR_RECV_PIN) ? SPACE : MARK;

	irparams.timer++; // One more 50us tick
	if (irparams.rawlen >= RAWBUF) {
		// Buffer overflow
		irparams.rcvstate = STATE_STOP;
	}
	switch(irparams.rcvstate) {
		case STATE_IDLE: // In the middle of a gap
			if (irdata == MARK) {
				if (irparams.timer < GAP_TICKS) {
					// Not big enough to be a gap.
					irparams.timer = 0;
				}
				else {
					// gap just ended, record duration and start recording transmission
					irparams.rawlen = 0;
					//irparams.rawbuf[irparams.rawlen++] = irparams.timer;
					irparams.timer = 0;
					irparams.rcvstate = STATE_MARK;
				}
			}
			break;
		case STATE_MARK: // timing MARK
			if (irdata == SPACE) {   // MARK ended, record time
				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
				irparams.timer = 0;
				irparams.rcvstate = STATE_SPACE;
			}
			break;
		case STATE_SPACE: // timing SPACE
			if (irdata == MARK) { // SPACE just ended, record it
				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
				irparams.timer = 0;
				irparams.rcvstate = STATE_MARK;
			}
			else { // SPACE
				if (irparams.timer > GAP_TICKS) {
					// big SPACE, indicates gap between codes
					// Mark current code as ready for processing
					// Switch to STOP
					// Don't reset timer; keep counting space width
					irparams.rcvstate = STATE_STOP;
				}
			}
			break;
		case STATE_STOP: // waiting, measuring gap
			if (irdata == MARK) { // reset gap timer
				irparams.timer = 0;
			}
			break;
	}

#ifdef BLINK_LED
	if (irdata == MARK) {
		_LED_PORT |= _BV(BLINK_LED_PIN);
		//PORTC |= _BV(PORTC5); //set the bit
	}
	else {
		_LED_PORT &=~ _BV(BLINK_LED_PIN);
		//PORTC &= ~_BV(PORTC5); //clear the bit
	}
#endif
}

// Compare two tick values, returning 0 if newval is shorter,
// 1 if newval is equal, and 2 if newval is longer
// ----Use a tolerance of 20%
// ++++To avoid fp multiplication use an integer tolerance and substract it
// ++++Values are in the range of 7..30 (except lead-in which is 55)
int compare(unsigned int oldval, unsigned int newval) {
//	if (newval < oldval * .8) {
	if (newval < oldval - 4) {
		return 0;
	}
//	else if (oldval < newval * .8) {
	else if (oldval < newval - 4) {
		return 2;
	}
	else {
		return 1;
	}
}

// Use FNV hash algorithm: http://isthe.com/chongo/tech/comp/fnv/#FNV-param
#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

/* Converts the raw code values into a 32-bit hash code.
 * Hopefully this code is unique for each button.
 * This isn't a "real" decoding, just an arbitrary value.
 */
bool decodeHashIRRecv(volatile decode_results_t *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP)
  {
	 return NO_DATA; 
  }
  if (results->rawlen < 5 && irparams.rcvstate == STATE_STOP) {
	  	irparams.rcvstate = STATE_IDLE;
	  	irparams.rawlen = 0;
		return NO_DATA;
  }
  long hash = FNV_BASIS_32;
  for (int i = 0; i+2 < results->rawlen; i++) {
    int value =  compare(results->rawbuf[i], results->rawbuf[i+2]);
    // Add value into the hash
    hash = (hash * FNV_PRIME_32) ^ value;
  }
  results->value = hash;
  results->bits = 32;
  results->decode_type = HASH;
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;
  return DECODED;
}

// initialization
void enableIRRecv() {
	cli();
	// setup pulse clock timer interrupt
	//Prescale /8 (16M/8 = 0.5 microseconds per tick)
	// Therefore, the timer interval can range from 0.5 to 128 microseconds
	// depending on the reset value (255 to 0)
	TIMER_CONFIG_NORMAL();

	//Timer2 Overflow Interrupt Enable
	TIMER_ENABLE_INTR;

	sei();  // enable interrupts

	// initialize state machine variables
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;

	// set pin modes
	_IRDDR &= ~_BV(IR_RECV_PIN);
	
#ifdef BLINK_LED
	_LED_DDR |= _BV(BLINK_LED_PIN);
	_LED_PORT |= _BV(BLINK_LED_PIN);
#endif //BLINK_LED
	
}
