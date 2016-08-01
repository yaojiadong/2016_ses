/*
 * bootloader.h
 *
 *  Created on: Jun 29, 2016
 *      Author: Jiadong
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

/* INCLUDES ******************************************************************/
#include "ses_common.h"
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_led.h"

/* DEFINES & MACROS **********************************************************/
#define BOOTSIZE 	4096

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * Pass the command to bootloader and execute the command.
 *
 * @param cmd	is the command which would be taken to execute certain task.
 * */
void command_parser(uint8_t cmd);

#endif /* BOOTLOADER_H_ */
