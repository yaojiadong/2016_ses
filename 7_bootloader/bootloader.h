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

/* DEFINES & MACROS **********************************************************/
#define BOOTSIZE 	4096

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * Pass the command to bootloader and execute the command.
 *
 * @param cmd	is the command which would be taken to execute certain task.
 * */
void command_parser(uint8_t cmd);

//void select_device_type();
//void set_address(uint32_t *addr);
//void block_read(uint16_t size, uint8_t type, uint32_t *addr) ;
//void start_block_flash_read(uint16_t size, uint32_t *addr);
//void start_block_eeprom_read(uint16_t size, uint32_t *addr);
//void block_load(uint16_t size, uint8_t type, uint32_t *addr);
//void start_block_flash_load(uint16_t size, uint32_t *addr);
//void start_block_eeprom_load(uint16_t size, uint32_t* addr);
//void chip_erase() ;
//void check_block_support();
//void return_support_device_code() ;
//void enter_prog_mode();
//void leave_prog_mode() ;
//void exit_bootloader() ;
//void return_software_identifier();
//void read_signature_byte() ;
//void return_software_version();
//void auto_increment_address();


#endif /* BOOTLOADER_H_ */
