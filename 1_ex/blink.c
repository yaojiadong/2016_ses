/*
 * blink.c
 *
 *  Created on: Apr 13, 2016
 *      Author: Jiadong
 */


#include <avr/io.h>
#include <util/delay.h>
/**Toggles the red LED of the SES-board*/
void blink(void) {
    //DDRG |= 0x02; //RED LED PG1 config as output pin (bit 2)
	DDRF |= 0x40; //green LED PF6 as output (bit 6)
	while (1) {
		_delay_ms(1000);
		//PORTG ^= 0x02;
		PORTF ^= 0x40;
	}

}
