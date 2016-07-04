/*
 * main.c
 *
 *  Created on: Jun 2, 2016
 *      Author: Jiadong
 */

#include "pscheduler.h"

void taskA(void) {

	led_redInit();

	while (1) {
		led_redToggle();
		_delay_ms(1000);
	}
}
void taskB(void) {

	led_yellowInit();

	while (1) {
		led_yellowToggle();
		_delay_ms(2000);
	}
}
void taskC(void) {

	led_greenInit();

	while (1) {
		led_greenToggle();
		_delay_ms(3000);
	}
}

task_t taskList[] = { taskA, taskB, taskC };

int main(void) {

	pscheduler_run(taskList, 3);
	return 0;
}
