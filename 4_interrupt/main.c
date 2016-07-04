/*
 * main.c
 *
 *  Created on: May 9, 2016
 *      Author: Jiadong
 */

#include "ses_button.h"
#include "ses_led.h"
#include "ses_timer.h"

void softwareTimer(void);
void test_timer1(void);

int count1, count2;
//where to put ISR???

int main() {
	pButtonCallback redLed, greenLed;
	pTimerCallback timer1Callback, timer2Callback;

//initialization
	led_redInit();
	led_greenInit();
	led_yellowInit();
	button_init(true); //timer1_start is called there

//	timer1_start();
	timer2_start();

	count1 = 0;
	count2 = 0;

	redLed = &led_redToggle;
	greenLed = &led_greenToggle;

//	timer1Callback = &test_timer1;
	timer2Callback = &softwareTimer;

//pass function
	button_setRotaryButtonCallback(redLed);
	button_setJoystickButtonCallback(greenLed);

//if button_init(false) then we have to set a callback function.
//when button_init(true), the debouncing function is set as callback function
//which has been done in the button.c

//	timer1_setCallback(timer1Callback);
	timer2_setCallback(timer2Callback);

	sei();

	while (1) {
	}

}

void softwareTimer() {
	count2++;
	if (count2 == 1000) {
		led_yellowToggle();
		count2 = 0;
	}
}

void test_timer1()
{
	count1++;
	if(count1 == 200)
	{
		led_redToggle();
		count1 = 0;
	}
}
