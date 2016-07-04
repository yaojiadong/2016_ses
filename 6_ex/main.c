/*
 * main.c
 *
 *  Created on: Jun 11, 2016
 *      Author: Jiadong
 */

#include "ses_button.h"
#include "ses_scheduler.h"
#include "ses_lcd.h"
#include "pid.h"
#include "ses_uart.h"

//#define SET_POINT		100
#define P	0.01
#define I	0.04
#define D	0
#define SCREEN_LENGTH	122
#define BASE_LINE		3

static void wrapper_pwm();
static void display();
static void wrapper_pid(void*);

static uint8_t set_point = 0;

int main() {

//task 6.1
	pButtonCallback joystickButton = wrapper_pwm;
	button_init(true); //debouncing, timer1 is initialized in this function
	button_setJoystickButtonCallback(joystickButton);

	pwm_init();
	pwm_setDutyCycle(0); //initially motor idle

//task 6.2,6.3
	lcd_init();
	led_yellowInit();
	led_greenInit();
	scheduler_init(); // timer2_start() is called there.
	motorFrequency_init(); //timer 5 and external interrupt settings

	taskDescriptor td1; //= (taskDescriptor*) malloc(sizeof(taskDescriptor));
	td1.task = &display;
//td.param = &;  //void pointer point to void, param is of type void
	td1.expire = 0;
	td1.period = 500; //timer2 INT every 1ms
	td1.execute = 0;
	td1.next = NULL;
	scheduler_add(&td1);

//task6.4
	uart_init(57600); //debugging

	pidData_t pidData;
	pid_Init(P * SCALING_FACTOR, I* SCALING_FACTOR, D *SCALING_FACTOR, &pidData); //set p=0; i=1.5, d=0;
	taskDescriptor td2; //= (taskDescriptor*) malloc(sizeof(taskDescriptor));
	td2.task = &wrapper_pid;
	td2.param = &pidData;  //void pointer point to void, param is of type void
	td2.expire = 0;
	td2.period = 200; //timer2 INT every 1ms
	td2.execute = 0;
	td2.next = NULL;
	scheduler_add(&td2);

	sei();
	scheduler_run();
	return 0;
}

static void wrapper_pwm() {
	static bool stop = true;

	stop = !stop;
	if (stop) {
		set_point = 0;
	} else {
		set_point = 100;
	}
	//task 6.2, 6.3
	//	pwm_setDutyCycle(85); //duty cycle is 85.

}

static void display() {
	uint16_t recent_freq = motorFrequency_getRecent();

//task 6.2, 6.3
//	uint16_t rpm_recent = 60 * recent_freq;
//	uint16_t rpm_median = 60 * motorFrequency_getMedian();

	int16_t step = 0;
	uint8_t static pos = 0;

	step = ((int16_t) recent_freq - set_point) / 10;
	if (step > 3)
		step = 3;
	if (step < -4)
		step = -4;

	lcd_setCursor(0, 2);
	fprintf(lcdout, "%d\t %d      \n", set_point, recent_freq);	//space

	if (set_point == 0)
		lcd_setPixel(0, pos, 1);
	else
		lcd_setPixel(BASE_LINE - step, pos, 1);	//row 0 automatically. How to change row??
	pos++;
	if (pos == SCREEN_LENGTH) { //end of screen
		lcd_clear();
		lcd_setCursor(0, 2);
		fprintf(lcdout, "%d\t %d\n", set_point, recent_freq);
		pos = 0;
	}

//task 6.2 6.3
//	fprintf(lcdout, "The recent frequency is %d rpm.\nThe median frequency is %d rpm\n ",rpm_recent, rpm_median);
}

static void wrapper_pid(void * pidData) {

	uint8_t pid_out = pid_control(set_point, (pidData_t*) pidData); //setpoint 100Hz
	pwm_setDutyCycle(pid_out);
	fprintf(uartout, "real dutycycle: %d\n", pid_out);
}

/*static void wrapper_pwm() {
 static bool stop = true;

 static pidData_t pidData;
 static taskDescriptor td2;

 stop = !stop;
 fprintf(uartout, "terminate 1");
 if (stop) {
 fprintf(uartout, "terminate 2");

 scheduler_remove(&td2);
 pwm_setDutyCycle(0); //stop the motor
 } else {
 SET_POINT = 100;
 pid_Init(0 * SCALING_FACTOR, 0.04 * SCALING_FACTOR, 0 * SCALING_FACTOR,
 &pidData);
 td2.task = &wrapper_pid;
 td2.param = &pidData; //void pointer point to void, param is of type void
 td2.expire = 0;
 td2.period = 200; //task executes every 0.2s
 td2.execute = 0;
 td2.next = NULL;
 scheduler_add(&td2);
 //task 6.2, 6.3
 //	pwm_setDutyCycle(85); //duty cycle is 85.
 }
 }*/
