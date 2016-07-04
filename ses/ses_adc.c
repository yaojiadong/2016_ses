/*
 * ses_adc.c
 *
 *  Created on: Apr 24, 2016
 *      Author: Jiadong
 */

#include "ses_adc.h"
#include "ses_lcd.h"

#define ADC_PORT	PORTF
#define TEMP_PIN 	2
#define LIGHT_PIN 	4
#define JOYSTICK_PIN 	5
#define MIC_POS_PIN 	1
#define MIC_NEG_PIN 	0

#define ADC_TEMP_MAX	800
#define ADC_TEMP_MIN	400
#define ADC_TEMP_RAW_MAX	256
#define ADC_TEMP_RAW_MIN	482
#define ADC_TEMP_FACTOR 	2

/**
 * Initializes the ADC
 */
void adc_init(void) {
	//Define Macro
	//Later on you only use ADC_PORT, you dont have to remember which port to use
	//config as input
	DDR_REGISTER(ADC_PORT) &= ~(1 << TEMP_PIN);
	DDR_REGISTER(ADC_PORT) &= ~(1 << LIGHT_PIN);
	DDR_REGISTER(ADC_PORT) &= ~(1 << JOYSTICK_PIN);
	DDR_REGISTER(ADC_PORT) &= ~(1 << MIC_POS_PIN);
	DDR_REGISTER(ADC_PORT) &= ~(1 << MIC_NEG_PIN);
	//config as input
	/*DDF(TEMP_PIN) = 0;
	 DDF(LIGHT_PIN) = 0;
	 DDF(JOYSTICK_PIN) = 0;
	 DDF(MIC_POS_PIN) = 0;
	 DDF(MIC_NEG_PIN) = 0;*/
	//config as input
	//DDR_REGISTER(ADC_PORT) &= 0xC8;

	//deactivate pull up resistor
	ADC_PORT &= ~(1 << TEMP_PIN);
	ADC_PORT &= ~(1 << LIGHT_PIN);
	ADC_PORT &= ~(1 << JOYSTICK_PIN);
	ADC_PORT &= ~(1 << MIC_POS_PIN);
	ADC_PORT &= ~(1 << MIC_NEG_PIN);
	//deactivate pull up resistor
	//ADC_PORT &= 0xC8;

	//clearing the PRADC bit at bit0
	PRR0 &= ~(1 << PRADC);
	//set reference voltage, both bit 7 and 6 being '1' move 6 positions
	ADMUX |= ADC_VREF_SRC << REFS0;
	//set right adjust result at bit 5
	ADMUX &= ~(1 << ADLAR);
	//set prescaler 64,
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADPS1);
	ADCSRA &= ~(1 << ADPS0);
	//Do not select auto triggering
	ADCSRA &= ~(1 << ADATE);
	//Enable the ADC
	ADCSRA |= (1 << ADEN);
}

/**
 * Read the raw ADC value of the given channel
 * @adc_channel The channel as element of the ADCChannels enum
 * @return The raw ADC value
 */
uint16_t adc_read(uint8_t adc_channel) {

	if (adc_channel >= 0 && adc_channel < 8) {
		//set the first three bits to three.
		//in fact we don't have to check, since the if statement has checked it
		/*		adc_channel &= ~(1<<7);
		 adc_channel &= ~(1<<6);
		 adc_channel &= ~(1<<5);*/

		//clear the bit4 to bit0
		//ADMUX &= 0xE0;
		ADMUX &= ~(1<<MUX0);
		ADMUX &= ~(1<<MUX1);
		ADMUX &= ~(1<<MUX2);
		ADMUX &= ~(1<<MUX3);
		ADMUX &= ~(1<<MUX4);
		//supply the channel to the converter
		ADMUX |= adc_channel;
		//start conversion
		ADCSRA |= (1 << ADSC);
		//polling, check the Bit 6 – ADSC,  sooo bad
		while ((ADCSRA & (1 << ADSC))) {
		}
		return ADC;		//(raw_value << 8) + temp;
	} else
		return ADC_INVALID_CHANNEL;
}

/**
 * Read the current joystick direction
 * @return The direction as element of the JoystickDirections enum
 */
uint8_t adc_getJoystickDirection() {
	enum JoystickDirections direction;
	uint16_t raw_data = adc_read(ADC_JOYSTICK_CH);

	if (raw_data >= 0 && raw_data < 300) {
		return direction = RIGHT;
	} else if (raw_data >= 300 && raw_data < 500) {
		return direction = UP;
	} else if (raw_data >= 500 && raw_data < 700) {
		return direction = LEFT;
	} else if (raw_data >= 700 && raw_data < 900) {
		return direction = DOWN;
	} else {
		return direction = NO_DIRECTION;
	}
}

/**
 * Read the current temperature
 * @return Temperature in tenths of degree celsius
 */
int16_t adc_getTemperature() {	//the calculation idea is explained in notes.
	int16_t adc = adc_read(ADC_TEMP_CH);
	int16_t slope = (ADC_TEMP_MAX - ADC_TEMP_MIN)
			/ (ADC_TEMP_RAW_MAX - ADC_TEMP_RAW_MIN);
	int16_t offset = ADC_TEMP_MAX - (ADC_TEMP_RAW_MAX * slope);
	return (adc * slope + offset) / ADC_TEMP_FACTOR;
}
