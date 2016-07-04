/*
 * ses_motorFrequency.c
 *
 *  Created on: Jun 11, 2016
 *      Author: Jiadong
 */

#include "ses_motorFrequency.h"

//Function declaration
static void sorting(uint16_t *array);
static uint16_t get_median(uint16_t * array);

#define TIMER5_CYC_FOR_10Hertz 6249 //6250-1
#define ARRAY_SIZE 10

static uint16_t record_one[ARRAY_SIZE];
static uint16_t record_two[ARRAY_SIZE];
static uint16_t (*record_ptr)[ARRAY_SIZE]; //pointer to array, the size of array is ARRAR_SIZE, the type of the value in array is uint16_t

static uint8_t array_index = 0;
static uint16_t freq = 0;

void motorFrequency_init() {
	//choose one of the arrays to store freq values
	record_ptr = &record_one;

	//external interrupt
	//rising edge 0x03
	EICRA |= 1 << ISC01;
	EICRA |= 1 << ISC00;
	//mask
	EIMSK |= 1 << INT0;
	//clear flag
	EIFR |= 1 << INTF0;

	//timer5, Operating mode, 0x04. datasheet seems wrong when compare table 18-5 and 18-28
	TCCR5B &= ~(1 << WGM53);
	TCCR5B |= 1 << WGM52;
	TCCR5A &= ~(1 << WGM51);
	TCCR5A &= ~(1 << WGM50);
	//clock prescaler 0x04; 1/256
	TCCR5B |= (1 << CS52);
	TCCR5B &= ~(1 << CS51);
	TCCR5B &= ~(1 << CS50);
	OCR5A = TIMER5_CYC_FOR_10Hertz;
	//enable interrupt for compare match
	TIMSK5 |= 1 << OCIE5A;
	//clear flag
	TIFR5 |= 1 << OCF5A;

}

uint16_t motorFrequency_getRecent() { //the frequency might be overwritten by INT.(half overwritten)
	uint16_t temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		temp = freq;
	}

	return temp;
}

uint16_t motorFrequency_getMedian() {

	//check which array it points to.Once you want to calculate median, you should store the later values in another array.
	if (*record_ptr == record_one) {
		record_ptr = &record_two;
		//*record_ptr = record_two;
		sorting(record_one);
		return get_median(record_one);

	} else {
		record_ptr = &record_one;
		//*record_ptr = record_one;
		sorting(record_two);
		return get_median(record_two);
	}
}

/*
 *  6-time interrupt counts one revolution.
 * */
ISR(INT0_vect) {
	//motor_stop = false;
	static uint8_t count = 0;
	uint16_t *temp_record = *record_ptr;
	count++;

	if (count % 6 == 0) {

		freq = (uint16_t) TIMER5_CYC_FOR_10Hertz * 10 / TCNT5; //overflow. #define the value will be signed integer. Type casting.

		led_greenOff();

		if (array_index < ARRAY_SIZE) {
			temp_record[array_index] = freq; //time_for_one_revolution;
			array_index++;
		} else {
			array_index = 0;
			temp_record[array_index] = freq; //time_for_one_revolution;
			array_index++;
		}

		TCNT5 = 0;
		count = 0;
	}

	led_yellowToggle();
}

/*
 * If we enter the INT, it mean the frequency is lower than 10 Hz.
 * We take it as stopped motor.
 * */
ISR(TIMER5_COMPA_vect) {
	uint16_t *temp = *record_ptr;
	uint8_t i;
	for (i = 0; i < ARRAY_SIZE; i++)
		temp[i] = 0;

	freq = 0;  //make sure recently freq is zero
	led_greenOn();
}

static void sorting(uint16_t *array) {
	uint8_t i, j;
	uint16_t temp;

	for (i = 0; i < ARRAY_SIZE; ++i) {
		for (j = i + 1; j < ARRAY_SIZE; ++j) {

			if (array[i] > array[j]) {
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

static uint16_t get_median(uint16_t * array) {
	uint16_t temp = 0;
	if (ARRAY_SIZE % 2 == 0) {

		temp = (array[ARRAY_SIZE / 2] + array[ARRAY_SIZE / 2 - 1]) / 2;
		return temp;
	} else {
		temp = array[(ARRAY_SIZE - 1) / 2];
		return temp;
	}
}
