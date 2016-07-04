/*
 * scheduler_test.h
 *
 *  Created on: Jun 10, 2016
 *      Author: Jiadong
 */

#ifndef SCHEDULER_TEST_H_
#define SCHEDULER_TEST_H_


#include "ses_button.h"
#include "ses_led.h"
#include "ses_scheduler.h"
#include "ses_lcd.h"


/*
 * A wrapper of function led_greenToggle().
 * */
void wrapper_greenToggle(void*);

/*
 * A wrapper of function  button_checkState().
 * */
void wrapper_button_checkState(void*);

/*
 * A wrapper of function  led_yellowOf().
 * It also keep track of the state of the led.
 * */
void wrapper_yellowOff(void*);

/*
 * It controls the yellow led by schedule the function wrapper_yellowOff().
 * It is also attached to JoystickButton. LED will be turned off by pressing the button as well.
 * */
void control_yellow();

/*
 * Keep track of the state of the stop watch.
 * Determine whether it is stopped or running.
 * */
void stopWatch();

/*
 * Calculate the stop watch values, which are to be shown on the LCD.
 * */
void calc_time();



#endif /* SCHEDULER_TEST_H_ */
