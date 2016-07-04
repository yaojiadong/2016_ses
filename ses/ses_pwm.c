/*
 * ses_pwm.c
 *
 *  Created on: Jun 11, 2016
 *      Author: Jiadong
 */


#include "ses_pwm.h"

void pwm_init(void)
{
	//enable timer 0.
	PRR0 &= ~(1<<PRTIM0);
	//set as output
	DDRG |= (1<<DDG5);//DDR_REGISTER(PORTG) |= (1<<DDG5);
	//set fast PWM mode  WGM02:0 =011
	TCCR0B &= ~(1<<WGM02);
	TCCR0A |= 1<<WGM01;
	TCCR0A |= 1<<WGM00;
	// no prescaling 001
	TCCR0B &= ~(1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS00);
	//BOTTOM: clear, Compare match: set. COM0B1:0=3. Inverting
	TCCR0A |= 1<<COM0B1;
	TCCR0A |= 1<<COM0B0;
}


void pwm_setDutyCycle(uint8_t dutyCycle)
{
	//OCR0B = (1-dutyCycle)*255;
	OCR0B =255-dutyCycle;

}
