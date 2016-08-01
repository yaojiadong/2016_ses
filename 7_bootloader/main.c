/*
 * main.c
 *
 *  Created on: Jul 2, 2016
 *      Author: Jiadong
 */

#include "bootloader.h"


#define JOYSTICK_PORT	PORTB
#define JOYSTICK_PIN		7


/*****Take a look at the property of the project***/
int main() {

	led_redInit();
	lcd_init();
	uart_init(57600);
	SPMCSR &= ~(1<<SPMIE);//disable interrupt

	DDR_REGISTER(JOYSTICK_PORT) &= ~(1 << JOYSTICK_PIN); //input
	JOYSTICK_PORT |= (1 << JOYSTICK_PIN); //activate pull up resistor

	//check joystick
	bool bootloader_not_used = PIN_REGISTER(JOYSTICK_PORT)
			& (1 << JOYSTICK_PIN);


//	the Boot Reset Fuse can be programmed so that the Reset
//	Vector is pointing to the Boot Flash start address after a reset.
//	It is set in the project property.
	if (!bootloader_not_used) {  //to enter the bootloader, keep joystick pressed during reset
		led_redOn();
		uint8_t cmd;
		cli();
		while (1) {
			cmd = uart_getc();
			command_parser(cmd);
		}
	}

	return 0;
}
