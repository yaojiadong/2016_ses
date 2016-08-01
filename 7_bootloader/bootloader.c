/*
 * bootloader.c
 *
 *  Created on: Jun 29, 2016
 *      Author: Jiadong
 */

#include "bootloader.h"

//uint32_t address; //store addr in 4 bytes
//uint8_t pageBuffer[SPM_PAGESIZE];  //array size 256, each element is 1Byte, in total, one page is 256Bytes
//uint8_t buf[SPM_PAGESIZE] = { 0 };

static void select_device_type();
static void set_address(uint32_t *addr);
//static void block_read(uint16_t size, uint8_t type, uint32_t *addr);
static void start_block_flash_read(uint16_t size, uint32_t *addr);
//static void start_block_eeprom_read(uint16_t size, uint32_t *addr);
//static void block_load(uint16_t size, uint8_t type, uint32_t *addr);
static void start_block_flash_load(uint16_t size, uint32_t *addr);
//static void start_block_eeprom_load(uint16_t size, uint32_t* addr);
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

	static uint32_t address; //static!!!!!!!!!
	uint16_t size;
	uint16_t temp;
	uint8_t type;

	switch (cmd) {
	case 'L':
		leave_prog_mode();
		break;
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

		temp = uart_getc();
		size = (temp << 8) | uart_getc();      //2*dd,  block size in byte
		type = uart_getc();						//'F' or 'E'

		if (type == 'F')
			start_block_flash_load(size, &address);
		else
			uart_putc('?');
		break;
	case 'g':
		temp = uart_getc();
		size = (temp << 8) | uart_getc();      //2*dd,  in byte
		type = uart_getc();						//'F' or 'E'

		if (type == 'F')
			start_block_flash_read(size, &address);
		else
			uart_putc('?');

		break;
	default:
		fprintf(lcdout, "%d", cmd);
		uart_putc('?');
	}
}

//T
static void select_device_type() {
	fprintf(lcdout, "T");
	uart_getc(); //ignore the data
	uart_putc('\r');  //carriage return
}

//A
static void set_address(uint32_t *addr) {
	fprintf(lcdout, "A");
	uint16_t address = uart_getc();
	address = (address << 8) | uart_getc();
	*addr = address;
	lcd_setCursor(0, 1);
	fprintf(lcdout, "%" PRIu32, *addr);  //address is 0, which is correct.
	uart_putc('\r');
}

//g
//static void block_read(uint16_t size, uint8_t type, uint32_t *addr) {
//	fprintf(lcdout, "g");
//	if (type == 'F') {
//
//		start_block_flash_read(size, addr);  // load flash
//
//	} else if (type == 'E') {
//		start_block_eeprom_read(size, addr);  // load EEPROM
//	} else
//		uart_putc('?');
//}

static void start_block_flash_read(uint16_t size, uint32_t *addr) {

	uint8_t data;
	(*addr) <<= 1;

	do {
		data = pgm_read_byte_far((*addr)++); //(uint32_t)0);//(*addr)++); // read_program_memory(address,0x00);
		uart_putc(data);			   				// send byte
		size--;				// reduce number of bytes to read by one
	} while (size);						// loop through size

	(*addr) >>= 1;
}

//B
//write one page.
static void start_block_flash_load(uint16_t size, uint32_t *addr) {
	uint8_t sreg = SREG;
	int i;
	uint8_t buf[SPM_PAGESIZE] = { 0 };
	fprintf(lcdout, "B");

	(*addr) <<= 1;

	if (size <= SPM_PAGESIZE) {

		for (i = 0; i < size; i++) {
			buf[i] = uart_getc();
		}

		boot_page_erase((*addr)); //why we erase again? erase chip is already called.
		boot_spm_busy_wait();

		//if size is odd, then use do-while
		for (i = 0; i < size; i += 2) {
			uint16_t w = buf[i];
			uint16_t temp = buf[i + 1];
			w += temp << 8;  //first one is low byte, second is high???
			boot_page_fill((*addr)+i, w);
		}
		boot_page_write(*addr);
		boot_spm_busy_wait();

		(*addr) >>= 1;

		//Reenable RWW-section to jump back to application after bootloading
		boot_rww_enable ();
		SREG = sreg;

		uart_putc('\r');
	} else
		uart_putc('?');

}

//not used
//static void start_block_eeprom_load(uint16_t size, uint32_t* addr) {
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
//}

//e
static void chip_erase() {  //erase application section only
	fprintf(lcdout, "e");
	uint32_t i;
	for (i = 0; i < (FLASHEND - (BOOTSIZE * 2)); i += SPM_PAGESIZE)
	{  //in bytes
		boot_page_erase(i);
		boot_spm_busy_wait();		// Wait until finished.
	}
	uart_putc('\r');  // respond okay
}

//b
static void check_block_support() {
	fprintf(lcdout, "b");
	uart_putc('Y');
	uart_putc((SPM_PAGESIZE >> 8) & 0xFF); // MSB first.
	uart_putc(SPM_PAGESIZE & 0xFF); 		// send LSB second.
}

//t
static void return_support_device_code() {
	fprintf(lcdout, "t");
	uart_putc('2'); 		//only one device
	uart_putc('\0'); //NULL
}

//P
static void enter_prog_mode() {
	fprintf(lcdout, "P");
	uart_putc('\r');
}

//L
static void leave_prog_mode() {
	fprintf(lcdout, "L");
	uart_putc('\r');
}

//E
static void exit_bootloader() {
	fprintf(lcdout, "E");
	uart_putc('\r');
	asm volatile ("jmp 0" ::);

//goto application section???
}

//S
static void return_software_identifier() {
	fprintf(lcdout, "S");
	uart_putc('A');
	uart_putc('V');
	uart_putc('R');
	uart_putc('B');
	uart_putc('O');
	uart_putc('O');
	uart_putc('T');
}

//s
static void read_signature_byte() {
	fprintf(lcdout, "s");
	uart_putc(SIGNATURE_2);
	uart_putc(SIGNATURE_1);
	uart_putc(SIGNATURE_0);
}

//V
static void return_software_version() {
	fprintf(lcdout, "V");
	uart_putc('0');
	uart_putc('1');
}

//a
static void auto_increment_address() {
	fprintf(lcdout, "a");
	uart_putc('Y');
}
