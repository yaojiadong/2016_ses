/*
 * scheduler_test.c
 *
 *  Created on: May 30, 2016
 *      Author: Jiadong
 */

#include "scheduler_test.h"

static bool stop = true;
static bool state_on = false;

int main(void) {
	button_init(true);
	led_greenInit();
	led_yellowInit();
	led_redInit();//testing
	lcd_init();
	scheduler_init();

//5.2.1:toggle green led every 2s
	taskDescriptor td1; //= (taskDescriptor*) malloc(sizeof(taskDescriptor));
	td1.task = &wrapper_greenToggle;
	//td.param = &;  //void pointer point to void, param is of type void
	td1.expire = 2000;
	td1.period = 2000;
	td1.execute = 0;
	td1.next = NULL;
	scheduler_add(&td1);

//5.2.2: schedule button_checkState
	taskDescriptor td2; //= (taskDescriptor*) malloc(sizeof(taskDescriptor));
	td2.task = &wrapper_button_checkState;
	//td.param = &;  //void pointer point to void, param is of type void
	td2.expire = 0;
	td2.period = 5;
	td2.execute = 0;
	td2.next = NULL;
	scheduler_add(&td2);

//5.2.3: scheduler or button click determines the situation
	button_setJoystickButtonCallback(control_yellow);

//5.2.4 :schedule stopwatch
	fprintf(lcdout, "Please press the rotary button to start the stopwatch");
	button_setRotaryButtonCallback(stopWatch);

	taskDescriptor td5; //= (taskDescriptor*) malloc(sizeof(taskDescriptor));
	td5.task = &calc_time;
	//td5.param = ;  //void pointer point to void, param is of type void
	td5.expire = 0; //immediately run
	td5.period = 100; //0.1s
	td5.execute = 0;
	td5.next = NULL;
	scheduler_add(&td5);

	sei();
	scheduler_run();
	return 1;
}



//task 5.2.1
void wrapper_greenToggle(void* a) {
	led_greenToggle();
}

//task 5.2.2
void wrapper_button_checkState(void* a) {
	button_checkState();
}

//task 5.2.3 :following two functions
void wrapper_yellowOff(void* td) {
	led_yellowOff();
	state_on = false;
}


void control_yellow() {
	static taskDescriptor td3;
	//td3 should remain same as before.
	//without static, the parameter in wrapper_yellowOff(&td3); would be null.
	state_on = !state_on;

	if (state_on) {
		led_yellowOn();
		td3.task = &wrapper_yellowOff;
		td3.param = &td3;
		td3.expire = 5000; //count 5s
		td3.period = 0;//run once
		td3.execute = 0;
		td3.next = NULL;
		scheduler_add(&td3);
	} else {
		led_redOff();
		wrapper_yellowOff(&td3);
		scheduler_remove(&td3);
	}
}

//task 5.2.4 :following two functions
void stopWatch(void) {
	stop = !stop;
}

void calc_time(void *a) {
	static uint16_t time = 0;
	uint8_t lcd_second, lcd_tenthsec;
	if (stop) {
	} else {
		time++;
		lcd_second = time / 10;
		lcd_tenthsec = time % 10;
		lcd_clear();
		fprintf(lcdout, "stopwatch \n%d,%d second\n", lcd_second,
				lcd_tenthsec);
		lcd_setCursor(0, 0); //otherwise the cursor keep going down.
	}
}
