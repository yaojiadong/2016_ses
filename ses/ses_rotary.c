/*
 * ses_rotary.c
 *
 *  Created on: Jun 26, 2016
 *      Author: Jiadong
 */

#include "ses_rotary.h"
//#include "ses_led.h"
//#include "ses_uart.h"

static pTypeRotaryCallback CW, CCW;

void rotary_init() {
	//input
	DDR_REGISTER(A_ROTARY_PORT) &= ~(1 << A_ROTARY_PIN);
	DDR_REGISTER(B_ROTARY_PORT) &= ~(1 << B_ROTARY_PIN);

	//otherwise it does not work!!!!!!!
	//pull up resistor activate
	A_ROTARY_PORT |= (1 << A_ROTARY_PIN);    //Pull-UP R
	B_ROTARY_PORT |= (1 << B_ROTARY_PIN);    //Pull-UP R

	/*
	 //timer5, Operating mode, 0x04. datasheet seems wrong when compare table 18-5 and 18-28
	 TCCR5B &= ~(1 << WGM53);
	 TCCR5B |= 1 << WGM52;
	 TCCR5A &= ~(1 << WGM51);
	 TCCR5A &= ~(1 << WGM50);
	 //clock prescaler 0x04; 1/256
	 TCCR5B |= (1 << CS52);
	 TCCR5B &= ~(1 << CS51);
	 TCCR5B &= ~(1 << CS50);
	 OCR5A = PERIOD;
	 //enable interrupt for compare match
	 TIMSK5 |= 1 << OCIE5A;
	 //clear flag
	 TIFR5 |= 1 << OCF5A;*/
}

void rotary_setClockwiseCallback(pTypeRotaryCallback cb) {
	CW = cb;
}

void rotary_setCounterClockwiseCallback(pTypeRotaryCallback cb) {
	CCW = cb;
}

void check_rotary() {

	static bool pre_a = 0;

	bool a = PIN_REGISTER(A_ROTARY_PORT) & (1 << A_ROTARY_PIN);
	bool b = PIN_REGISTER(B_ROTARY_PORT) & (1 << B_ROTARY_PIN);
//	uart_putc('C'); //debug
	if ((!a) && (pre_a)) { // A has gone from high to low
		if (b) {
			CW(); // B is high so clockwise
		}
		else {
			CCW();			// B is low so counter-clockwise
		}
	}
	pre_a = a;
}

//ISR(TIMER5_COMPA_vect) {
//	check_rotary();
//}

