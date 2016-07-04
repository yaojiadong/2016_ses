/*
 * alarm.h
 *
 *  Created on: Jun 25, 2016
 *      Author: Jiadong
 */

#ifndef ALARM_H_
#define ALARM_H_

/* INCLUDES ******************************************************************/
#include "ses_lcd.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_scheduler.h"
#include "ses_rotary.h"
#include "ses_uart.h" //testing

/* DEFINES & MACROS **********************************************************/

#define TRANSITION(newState) (fsm->state = newState, RET_TRANSITION)

/* TYPES ********************************************************************/

/** return values */
enum {
	RET_HANDLED, //< event was handled
	RET_IGNORED, //< event was ignored; not used in this implementation
	RET_TRANSITION //< event was handled and a state transition occurred
};

typedef uint8_t fsmReturnStatus; //< typedef to be used with above enum

typedef struct event_s {
	uint8_t signal; //< identifies the type of event
} Event;

typedef struct time_t Time_t;
typedef struct fsm_s Fsm;
typedef struct data data_t;

/** typedef for state event handler functions */
typedef fsmReturnStatus (*State)(Fsm *, const Event*);

struct time_t {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t milli;
};

struct fsm_s {
	State state; //< current state, pointer to event handler
	bool isAlarmEnabled; //< flag for the alarm status
	Time_t timeSet; //< multi-purpose var for system time and alarm time
};

/*	a list of events	*/
enum {
	ENTRY,
	EXIT,
	ROTARY_PRESSED,
	JOYSTICK_PRESSED,
	MATCHING,
	ROTARY_INC,
	ROTARY_DEC
};

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * Initialize the alarm clock.
 * Set up the functions and go to set_hour state.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus alarm_init(Fsm* fsm, const Event* event);

/*
 * Set hour for alarm clock.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus sethour(Fsm* fsm, const Event* event);


/*
 * Set minute for alarm clock.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus setmin(Fsm* fsm, const Event* event);

/*
 * Normal operation mode will show and update time on the screen.
 * The green led also indicates the updating of the clock.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus normal_operating(Fsm* fsm, const Event* event);

/*
 * Set alarm hour.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus set_alarm_hour(Fsm* fsm, const Event* event);

/*
 * Set alarm minute and schedule the task
 * to check if the clock and alarm matches
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus set_alarm_min(Fsm* fsm, const Event* event);

/*
 * When alarm and clock match, red led will toggle in 4Hz.
 * The red led will be turned off if any button is pressed or 5s has elapsed.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
fsmReturnStatus led_toggle(Fsm* fsm, const Event* event);


/*
 * It executes the action in accordance to the event.
 * If the event decides state transition, the current state will execute
 * the exit action and the next state will execute the entry action.
 *
 * @param fsm 	points to the Fsm struct which contains the updated states.
 * @param event decides which action the state is going to execute.
 * */
inline static void fsm_dispatch(Fsm* fsm, const Event* event) {
	static Event entryEvent = { .signal = ENTRY };
	static Event exitEvent = { .signal = EXIT };
	State s = fsm->state;
	fsmReturnStatus r = fsm->state(fsm, event);
	if (r == RET_TRANSITION) {
		s(fsm, &exitEvent); //< call exit action of last state
		fsm->state(fsm, &entryEvent); //< call entry action of new state
	}
}

#endif /* ALARM_H_ */
