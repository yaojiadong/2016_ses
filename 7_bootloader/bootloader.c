/*
 * bootloader.c
 *
 *  Created on: Jun 29, 2016
 *      Author: Jiadong
 */

#include "bootloader.h"

//uint32_t address; //store addr in 4 bytes
//uint8_t pageBuffer[SPM_PAGESIZE];  //array size 256, each element is 1Byte, in total, one page is 256Bytes

static void select_device_type();
static void set_address(uint32_t *addr);
static void block_read(uint16_t size, uint8_t type, uint32_t *addr);
static void start_block_flash_read(uint16_t size, uint32_t *addr);
static void start_block_eeprom_read(uint16_t size, uint32_t *addr);
static void block_load(uint16_t size, uint8_t type, uint32_t *addr);
static void start_block_flash_load(uint16_t size, uint32_t *addr);
static void start_block_eeprom_load(uint16_t size, uint32_t* addr);
static void chip_erase();
static void check_block_support();
static void return_support_device_code();
static void enter_prog_mode();
static void leave_prog_mode();
static void exit_bootloader();
static void return_software_identifier();
static void read_signature_byte();
static void return_software_version();
static void auto_increment_address();

void command_parser(uint8_t cmd) {

	uint32_t address;
	uint16_t size;
	while (1) {

		switch (cmd) {
		case 'P':
			enter_prog_mode();
			break;
		case 'a':
			auto_increment_address();
			break;
		case 'A':
			set_address(&address);
			break;
		case 'e':
			chip_erase();
			break;
		case 'T':
			select_device_type();
			break;
		case 's':
			read_signature_byte();
			break;
		case 't':
			return_support_device_code();
			break;
		case 'S':
			return_software_identifier();
			break;
		case 'V':
			return_software_version();
			break;
		case 'E':
			exit_bootloader();
			break;
		case 'b':
			check_block_support();
			break;
		case 'B':
			size = (uart_getc() << 8) | uart_getc();      //2*dd,  in byte
			block_load(size, uart_getc(), &address);		//'F' or 'E'
			break;
		case 'g':
			size = (uart_getc() << 8) | uart_getc();      //2*dd,  in byte
			block_read(size, uart_getc(), &address);		//'F' or 'E'
			break;
		}
	}

}

//T
void select_device_type() {
	uart_getc(); //ignore the data
	uart_putc('\r');  //carriage return
}

//A
void set_address(uint32_t *addr) {

	uint8_t address = uart_getc();
	address = (address << 8) | uart_getc();
	*addr = address;
	uart_putc('\r');
}

//g
void block_read(uint16_t size, uint8_t type, uint32_t *addr) {
	if (type == 'F') {
		(*addr) <<= 1;
		start_block_flash_read(size, addr);  // load flash
		(*addr) >>= 1;
	} else if (type == 'E') {
		start_block_eeprom_read(size, addr);  // load EEPROM
	} else
		uart_putc('?');
}

void start_block_flash_read(uint16_t size, uint32_t *addr) {
//for reading, interrupt protection is not needed.
//	uint8_t sreg = SREG;
//	cli();
	uint8_t data;
	do {

		data = pgm_read_byte_far((*addr)++); // read_program_memory(address,0x00);
		uart_putc(data);			   				// send byte
		size--;							// reduce number of bytes to read by one
	} while (size);						// loop through size

//	SREG = sreg;
}

void start_block_eeprom_read(uint16_t size, uint32_t *addr) {
}

//B
void block_load(uint16_t size, uint8_t type, uint32_t *addr) {

	if (type == 'F') {
		// Convert address to bytes temporarily. The first 9 bits indicate page. 7 bits for words.
		//moving one bit left will give LSB the ability to address byte.
		(*addr) <<= 1;
		start_block_flash_load(size, addr);  // load flash
		(*addr) >>= 1;
	} else if (type == 'E') {
		start_block_eeprom_load(size, addr);  // load EEPROM
	} else
		uart_putc('?');
}

//write one page.
void start_block_flash_load(uint16_t size, uint32_t *addr) {
	uint16_t data_word;
	uint8_t sreg = SREG;
	cli();
	// Disable interrupts

	if (size <= SPM_PAGESIZE) {
		boot_page_erase(*addr);
		boot_spm_busy_wait();

		uint16_t i;
		for (i = 0; i < size; i += 2) { //if size is odd, then use do-while

			data_word = uart_getc();
			data_word = (data_word << 8) | uart_getc();
			boot_page_fill((*addr)+i, data_word);
		}

		//boot_page_fill_safe()
		boot_page_write(*addr);
		boot_spm_busy_wait();

		uart_putc('\r');
	} else
		uart_putc('?');

//Reenable RWW-section to jump back to application after bootloading
	boot_rww_enable ();
	SREG = sreg;
}

//not used?
void start_block_eeprom_load(uint16_t size, uint32_t* addr) {
//	uint8_t buf[SPM_PAGESIZE];
//
//	if (size <= SPM_PAGESIZE) {
//		uint16_t i;
//		for (i = 0; i < size; i++) {
//			buf[i] = uart_getc();
//		}
//		eeprom_write_block((const void*) buf, (void*) addr, size);
//		uart_putc('\r');
//	} else
//		uart_putc('?');
}

//e
void chip_erase() {  //erase application section or all???
	uint32_t i;
	for (i = 0; i < (FLASHEND - (BOOTSIZE * 2)); i += SPM_PAGESIZE)
	{//in bytes
		boot_page_erase_safe(i);	// Erase the page
		boot_spm_busy_wait();		// Wait until finished.
	}
	uart_putc('\r');  // respond okay
}

//b
void check_block_support() {
	uart_putc('Y');
	uart_putc((SPM_PAGESIZE >> 8) & 0xFF); // MSB first.
	uart_putc(SPM_PAGESIZE & 0xFF); 		// send LSB second.
}

//t
void return_support_device_code() {
	uart_putc('0');	//only one device
}

//P
void enter_prog_mode() {
	uart_putc('\r');
}

//L
void leave_prog_mode() {
	uart_putc('\r');
}

//E
void exit_bootloader() {
	uart_putc('\r');
	//goto application section???
	__asm__ __volatile__ ("jmp 0" ::);
}

//S
void return_software_identifier() {
	uart_putc('A');
	uart_putc('V');
	uart_putc('R');
	uart_putc('B');
	uart_putc('O');
	uart_putc('O');
	uart_putc('T');
}

//s
void read_signature_byte() {
	uart_putc(SIGNATURE_0);
	uart_putc(SIGNATURE_1);
	uart_putc(SIGNATURE_2);
}

//V
void return_software_version() {
	uart_putc('0');
	uart_putc('1');
}

//a
void auto_increment_address() {
	uart_putc('Y');
}
