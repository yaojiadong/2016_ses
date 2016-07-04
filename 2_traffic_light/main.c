/*
 * main.c
 *
 *  Created on: Apr 17, 2016
 *      Author: Jiadong
 */

#include "ses_led.h"
#include "ses_common.h"
#include "ses_lcd.h"
#include "ses_uart.h"

void printState(const char*);

int main(void) {
	uart_init(57600);
	lcd_init();

	fprintf(uartout,"START,Greeting from Jiadong\n"); //Send from board to PC
	fprintf(lcdout,"START,Hello Jiadong"); //directly shown on LCD.

//traffic light
	led_redInit();
	led_yellowInit();
	led_greenInit();
	while (1) {
		led_greenOn();
		printState("The state is green");
		_delay_ms(5000);


		led_greenToggle();
		led_yellowOn();
		printState("The state is yellow");
		_delay_ms(500);


		led_yellowToggle();
		led_redOn();
		printState("The state is red");
		_delay_ms(2000);

		led_yellowToggle();
		printState("The state is red and yellow");
		_delay_ms(300);

		led_yellowOff();
		led_redOff();
	}
	return 1;
}

void printState(const char* state)
{
	fprintf(uartout,"%s\n", state);
	fprintf(lcdout,"%s\n", state);
}


