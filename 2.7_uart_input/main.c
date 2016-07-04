/*
 * main.c
 *
 *  Created on: Apr 17, 2016
 *      Author: Jiadong
 */


#include "ses_uart.h"
#include "ses_led.h"
#include "ses_common.h"
#include <stdint.h>

int main(void){

	uart_init(57600);
	led_redInit();
	led_greenInit();
	led_greenOn();

	while(1){
	uint8_t charater = uart_getc();
	if(charater == 'a')
	{
		led_greenToggle();
		led_redToggle();
	}
	}

	return 1;
}
