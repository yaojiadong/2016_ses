/*
 * main.c
 *
 *  Created on: Jun 25, 2016
 *      Author: Jiadong
 */

#include "alarm.h"
#include "ses_rotary.h"



inline static void fsm_init(Fsm* fsm, State init);
//inline static void fsm_dispatch(Fsm* fsm, const Event* event);

void schedule_joystick();
void schedule_rotary();
void task_joystick(void *);
void task_rotary(void *);

void wrapper_cw();
void wrapper_ccw();

/*****Global Variable******/
static Fsm fsm;

int main() {
//	uart_init(57600); //debugging

	fsm_init(&fsm, alarm_init);
	sei();
	scheduler_run();
	return 0;
}

inline static void fsm_init(Fsm* fsm, State init) {
	scheduler_init();
	button_init(true);
	led_redInit();
	led_yellowInit();
	led_greenInit();
	lcd_init();
	rotary_init();

	rotary_setClockwiseCallback(wrapper_cw);
	rotary_setCounterClockwiseCallback(wrapper_ccw);

	button_setJoystickButtonCallback(schedule_joystick);
	button_setRotaryButtonCallback(schedule_rotary);

	Event entryEvent = { .signal = ENTRY };
	fsm->state = init;
	fsm->state(fsm, &entryEvent);
}

void schedule_joystick() {
	static taskDescriptor td2;
	td2.task = &task_joystick;
//	td2.param = ;
	td2.expire = 0;  //immediately execute the task
	td2.period = 0;
	td2.execute = 0;
	td2.next = NULL;
	scheduler_add(&td2);
}

void task_joystick(void * param) {
	Event event = { .signal = JOYSTICK_PRESSED };
	fsm_dispatch(&fsm, &event);
}

void schedule_rotary() {

	static taskDescriptor td; //lifetime of local variable
	td.task = &task_rotary;
	td.expire = 0;  //immediately execute the task
	td.period = 0;
	td.execute = 0;
	td.next = NULL;
	scheduler_add(&td);
}

void task_rotary(void * param) {
	Event event = { .signal = ROTARY_PRESSED };
	fsm_dispatch(&fsm, &event);
}



void wrapper_cw() {
	Event event = { .signal = ROTARY_INC };
	fsm_dispatch(&fsm, &event);
}

void wrapper_ccw() {
	Event event = { .signal = ROTARY_DEC };
	fsm_dispatch(&fsm, &event);
}

/* dispatches events to state machine, called in application*/
//inline static void fsm_dispatch(Fsm* fsm, const Event* event) {
//	static Event entryEvent = { .signal = ENTRY };
//	static Event exitEvent = { .signal = EXIT };
//	State s = fsm->state;
//	fsmReturnStatus r = fsm->state(fsm, event);
//	if (r == RET_TRANSITION) {
//		s(fsm, &exitEvent); //< call exit action of last state
//		fsm->state(fsm, &entryEvent); //< call entry action of new state
//	}
//}
