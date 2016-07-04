/*
 * alarm.c
 *
 *  Created on: Jun 25, 2016
 *      Author: Jiadong
 */

#include "alarm.h"

static Time_t wrapper_time_format(systemTime_t);
static void wrapper_red_led(void*);
static void wrapper_turnoff_led(void*);
static void time_increment(void*);
static void check_matching(void*);
static void wrapper_check_rotary(void* param);

static taskDescriptor td_toggle_led; //toggleing led
static taskDescriptor td_turnoff_led;//turnoff led after 5s
static taskDescriptor td_time_increment;// time increment
static taskDescriptor td_check_matching;//check matching
static taskDescriptor td_check_rotary_encoder; //check rotary encoder

fsmReturnStatus alarm_init(Fsm * fsm, const Event* e) {

	td_toggle_led.task = &wrapper_red_led;
	//td.param = ;
	td_toggle_led.expire = 0;
	td_toggle_led.period = 250;   //0.25s = 4Hz
	td_toggle_led.execute = 0;
	td_toggle_led.next = NULL;

	td_turnoff_led.task = &wrapper_turnoff_led;
	td_turnoff_led.param = fsm;
	td_turnoff_led.expire = 5000;  //count 5s
	td_turnoff_led.period = 0;
	td_turnoff_led.execute = 0;
	td_turnoff_led.next = NULL;

	td_time_increment.task = &time_increment;
//	td3.param ;
	td_time_increment.expire = 0;
	td_time_increment.period = 1000; // every second update the time of the clock
	td_time_increment.execute = 0;
	td_time_increment.next = NULL;

	td_check_matching.task = &check_matching;
//	td4.param
	td_check_matching.expire = 0;
	td_check_matching.period = 1000; // every second check if matching
	td_check_matching.execute = 0;
	td_check_matching.next = NULL;


	td_check_rotary_encoder.task = &wrapper_check_rotary;
	td_check_rotary_encoder.expire = 0;  //immediately execute the task
	td_check_rotary_encoder.period = 1; //
	td_check_rotary_encoder.execute = 0;
	td_check_rotary_encoder.next = NULL;
	scheduler_add(&td_check_rotary_encoder);


	switch (e->signal) {
	case ENTRY:
		fsm->isAlarmEnabled = false;
		fprintf(lcdout, "%02d:%02d\n", 0, 0);
		fprintf(lcdout, "set clock hour");
		(fsm->timeSet).hour = 0;
		(fsm->timeSet).minute = 0;
		(fsm->timeSet).second = 0;
		return TRANSITION(sethour);
	default:
		return RET_IGNORED;
	}
}

fsmReturnStatus sethour(Fsm * fsm, const Event* e) {
	Time_t *temp = &(fsm->timeSet);

	switch (e->signal) {
	case ENTRY:
		return RET_HANDLED;
	case ROTARY_PRESSED:
		if (++(temp->hour) > 23)
			temp->hour = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set clock hour");
		return RET_HANDLED;
	case ROTARY_INC:
		if (++(temp->hour) > 23)
			temp->hour = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set clock hour");
		return RET_HANDLED;
	case ROTARY_DEC:
		if ((temp->hour) == 0)
			temp->hour = 24;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", --(temp->hour), temp->minute);
		fprintf(lcdout, "set clock hour");
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(setmin);
	case EXIT:
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}
fsmReturnStatus setmin(Fsm * fsm, const Event* e) {
	Time_t *temp = &(fsm->timeSet);
	systemTime_t now;

	switch (e->signal) {
	case ENTRY:
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set clock min");
		return RET_HANDLED;
	case ROTARY_PRESSED:
		if (++(temp->minute) > 59)
			temp->minute = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set clock min");
		return RET_HANDLED;
	case ROTARY_INC:
		if (++(temp->minute) > 59)
			temp->minute = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set clock min");
		return RET_HANDLED;
	case ROTARY_DEC:
		if ((temp->minute) == 0)
			temp->minute = 60;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, --(temp->minute));
		fprintf(lcdout, "set clock min");
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(normal_operating);
	case EXIT: //set the time variable in scheduler. The variable is the real clock.
		temp->second = 0;
		now = (systemTime_t) ((temp->hour) * 3600 + (temp->minute) * 60) * 1000;
		scheduler_setTime(now);
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}
fsmReturnStatus normal_operating(Fsm * fsm, const Event* e) {
	static bool enable = false;

	switch (e->signal) {
	case ENTRY:
		td_time_increment.expire = 0;
		scheduler_add(&td_time_increment);
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(set_alarm_hour);
	case ROTARY_PRESSED:
		enable = !enable;
		fsm->isAlarmEnabled = enable;
		if (enable)
			led_yellowOn();
		else
			led_yellowOff();
		return RET_HANDLED;
	case MATCHING:
		if (fsm->isAlarmEnabled) {
			return TRANSITION(led_toggle);
		} else
			return RET_HANDLED;
	case EXIT:
		scheduler_remove(&td_time_increment);
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}
fsmReturnStatus set_alarm_hour(Fsm * fsm, const Event* e) {
	Time_t *temp = &(fsm->timeSet);
	switch (e->signal) {
	case ENTRY:
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm hour");
		return RET_HANDLED;
	case ROTARY_PRESSED:
		if (++(temp->hour) > 23)
			temp->hour = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm hour");
		return RET_HANDLED;
	case ROTARY_INC:
		if (++(temp->hour) > 23)
			temp->hour = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm hour");
		return RET_HANDLED;
	case ROTARY_DEC:
		if ((temp->hour) == 0)
			temp->hour = 24;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", --(temp->hour), temp->minute);
		fprintf(lcdout, "set alarm hour");
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(set_alarm_min);
	case EXIT:
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}
fsmReturnStatus set_alarm_min(Fsm * fsm, const Event* e) {

	Time_t *temp = &(fsm->timeSet);

	switch (e->signal) {
	case ENTRY:
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm min");
		return RET_HANDLED;
	case ROTARY_PRESSED:
		if (++(temp->minute) > 59)
			temp->minute = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm min");
		return RET_HANDLED;
	case ROTARY_INC:
		if (++(temp->minute) > 59)
			temp->minute = 0;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, temp->minute);
		fprintf(lcdout, "set alarm min");
		return RET_HANDLED;
	case ROTARY_DEC:
		if ((temp->minute) == 0)
			temp->minute = 60;
		lcd_clear();
		lcd_setCursor(0, 0);
		fprintf(lcdout, "%02d:%02d\n", temp->hour, --(temp->minute));
		fprintf(lcdout, "set alarm min");
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(normal_operating);
	case EXIT:
		td_check_matching.param = fsm;//in order to get the timeSet in the check matching
		td_check_matching.expire = 0;
		scheduler_add(&td_check_matching);
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}
fsmReturnStatus led_toggle(Fsm * fsm, const Event* e) {

//!!!!!!make all tasks global.
//!!!mistake: set td.next=NULL will disrupt scheduler!!!!
//	static taskDescriptor td;
//	td.task = &wrapper_red_led;
//	//td.param = &;  //void pointer point to void, param is of type void
//	td.expire = 0;
//	td.period = 250;   //0.25s = 4Hz
//	td.execute = 0;
//	td.next = NULL;
//
//	static taskDescriptor td2;
//	td2.task = &wrapper_turnoff_led;
//	td2.param = fsm;  //void pointer point to void, param is of type void
//	td2.expire = 5000;  //count 5s
//	td2.period = 0;
//	td2.execute = 0;
//	td2.next = NULL;
//
//	static taskDescriptor td3;
//	td3.task = &time_increment;
//	td3.param = fsm;
//	td3.expire = 0;
//	td3.period = 1000; // every second update the time of the clock
//	td3.execute = 0;
//	td3.next = NULL;

	switch (e->signal) {
	case ENTRY:
		td_toggle_led.expire = 0;
		td_turnoff_led.expire = 5000;
		td_time_increment.expire = 0;
		scheduler_add(&td_toggle_led);
		scheduler_add(&td_turnoff_led);
		scheduler_add(&td_time_increment);
		return RET_HANDLED;
	case JOYSTICK_PRESSED:
		return TRANSITION(normal_operating);
	case ROTARY_PRESSED:
		return TRANSITION(normal_operating);
	case EXIT:
		led_redOff();
		scheduler_remove(&td_toggle_led);
		scheduler_remove(&td_turnoff_led);
		scheduler_remove(&td_time_increment);
		return RET_HANDLED;
	default:
		return RET_IGNORED;
	}
}

static void wrapper_red_led(void* a) {
	led_redToggle();
}

static void wrapper_turnoff_led(void* fsm) {

	Event event = { .signal = JOYSTICK_PRESSED }; //choose any button
	fsm_dispatch((Fsm*) fsm, &event);
}

static void time_increment(void* dummy) {

	led_greenToggle();
	Time_t clock = wrapper_time_format(scheduler_getTime());
//	systemTime_t temp = scheduler_getTime();
//	clock = wrapper_time_format(temp);

	lcd_clear();
	lcd_setCursor(0, 0);
	fprintf(lcdout, "%02d:%02d:%02d\n", clock.hour, clock.minute, clock.second);
//	fprintf(lcdout, "system time: ""%"PRIu32, temp);//debugging
}

static void check_matching(void *fsm) {

	Fsm* theFsm = (Fsm*)fsm;
	Time_t alarm = theFsm->timeSet;
	Event event = { .signal = MATCHING };

	systemTime_t alarm_time = alarm.hour * 60 + alarm.minute;
	if (alarm_time == (scheduler_getTime() / 60000)) {
		fsm_dispatch(theFsm, &event);  //goto led_toggle
		scheduler_remove(&td_check_matching);
	}
}
static Time_t wrapper_time_format(systemTime_t time) {
	Time_t clock_time;

	uint32_t second = (uint32_t) time / 1000;
	uint16_t milli = (uint16_t) time % 1000;
	uint32_t minute = (uint32_t) second / 60;

	clock_time.milli = milli;
	clock_time.second = (uint8_t) (second % 60); //parenthese are important.
	clock_time.minute = (uint8_t) (minute % 60);
	clock_time.hour = (uint8_t) (minute / 60);

//debug
//	lcd_clear();
//	lcd_setCursor(0, 0);
//	fprintf(lcdout, "In wrapper,%u:%u:%u\n", clock_time.hour, clock_time.minute,
//			clock_time.second);

	return clock_time;
}

static void wrapper_check_rotary(void* param) {
	check_rotary();
}
