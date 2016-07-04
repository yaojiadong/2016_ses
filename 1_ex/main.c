/*
 * main.c
 *
 *  Created on: Apr 13, 2016
 *      Author: Jiadong
 */

/*
 #include <avr/io.h>
 #include <util/delay.h>
 */
//#include "blink.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

/*void shortDelay (void)
 {
 uint32_t i; // 32 bit unsigned integer
 for (i = 0x000823D8; i > 0 ; i--) {
 // prevent code optimization by using inline assembler
 asm volatile ( "nop" ); // one cycle with no operation
 }
 }*/

void wait(uint16_t millis) {
	uint32_t i = millis * 16000L / 18000L;
	//uint32_t i =941;

	for (; i > 0; i--) {
		uint16_t innerLoop = 1000;
		for (; innerLoop > 0; innerLoop--) { // question
			asm volatile("nop");
		}
	}
}

/**Toggles the red LED of the SES-board*/
int main(void) {
	DDRG |= 0x02;
	//DDRF |= 0x40; //green LED PF6 as output (bit 6)
	while (1) {
		wait(10000);
		//_delay_ms(1000);
		//PORTF ^= 0x40;
		PORTG ^= 0x02;
	}
	return 0;
}
