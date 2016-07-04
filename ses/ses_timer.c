/* INCLUDES ******************************************************************/
#include "ses_timer.h"

/* DEFINES & MACROS **********************************************************/
#define TIMER1_CYC_FOR_5MILLISEC    1249 //16MHz*5ms/64-1
#define TIMER2_CYC_FOR_1MILLISEC	 249   //16MHz*1ms/64-1

static pTimerCallback timer1Callback, timer2Callback;


/*FUNCTION DEFINITION ********************************************************/
void timer2_setCallback(pTimerCallback cb) {
	timer2Callback = cb;
}

void timer2_start() {
	//CTC mode  0x02
	TCCR2B &= ~(1 << WGM22);
	TCCR2A |= (1 << WGM21);
	TCCR2A &= ~(1 << WGM20);
	//prescaler 64
	TCCR2B |= (1 << CS22);
	TCCR2B &= ~(1 << CS21);
	TCCR2B &= ~(1 << CS20);
	//Set interrupt mask
	TIMSK2 |= (1 << OCIE2A);

	//SREG??? not needed here.

	//Clear the interrupt flag
	TIFR2 |= (1 << OCF2A);
	//set value for comparing.
	OCR2A = TIMER2_CYC_FOR_1MILLISEC;
}

void timer2_stop() {
	//previously it is set to 0100.
	TCCR2B &= ~(1 << CS22);
}

void timer1_setCallback(pTimerCallback cb) {
	timer1Callback = cb;
}

void timer1_start() {
	// CTC mode for compare A  0x04
	TCCR1B &= ~(1 << WGM13);
	TCCR1B |= (1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
	//prescaler
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);
	//Set interrupt mask
	TIMSK1 |= (1<<OCIE1A);
	//Clear the interrupt flag
	TIFR1 |= (1 << OCF1A);
	//set value for comparing.
	OCR1A = TIMER1_CYC_FOR_5MILLISEC;

}

void timer1_stop() {
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS10);
}

ISR(TIMER1_COMPA_vect) {
	timer1Callback();
}

ISR(TIMER2_COMPA_vect) {
	timer2Callback();
}
