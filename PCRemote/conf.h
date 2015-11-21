/*
 * conf.h
 *
 * Created: 21.11.2015 12:49:16
 *  Author: Bernd
 */ 


#ifndef CONF_H_
#define CONF_H_

#define F_CPU 8000000UL
#define LOG_BUF_SIZE 512
#define AUTO_REPEAT_LIMIT 4


#define IR_RECV_PORT_D
#define IR_RECV_PIN PORTD2

#define BLINK_LED

#define BLINK_LED_PORT_C
#define BLINK_LED_PIN PORTC5

#define PWR_SENSE_PORT PORTD
#define PWR_SENSE_DDR DDRD
#define PWR_SENSE_PIN PIND
#define PWR_SENSE_BIT PORTD6

#define PWR_BTN_PORT PORTD
#define PWR_BTN_DDR DDRD
#define PWR_BTN_BIT PORTD7

#endif /* CONF_H_ */