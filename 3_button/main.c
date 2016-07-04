/*
 * main.c
 *
 *  Created on: Apr 24, 2016
 *      Author: Jiadong
 */

#include "ses_led.h"
#include "ses_button.h"
#include "ses_adc.h"
#include "ses_lcd.h"

int main() {
	led_redInit();
	led_greenInit();
	led_yellowInit();
	lcd_init();

	adc_init();
	button_init();//latter : button_init(bool);
	while (1) {
//		uint16_t val = adc_read(ADC_JOYSTICK_CH);
//		lcd_setCursor(0,0);
//		lcd_clear();
//		fprintf(lcdout,"%d\n", val);

		if (button_isJoystickPressed())
			led_greenOn();
		else
			led_greenOff();

		if (button_isRotaryPressed())
			led_redOn();
		else
			led_redOff();

		lcd_setCursor(0, 0);
		lcd_clear();
		if (adc_getJoystickDirection() == RIGHT) {
			led_yellowOn();

			fprintf(lcdout, "right\n");
		} else
			led_yellowOff();

		uint16_t temperature = adc_getTemperature();
		//uint16_t temperature = adc_read(ADC_TEMP_CH);
		uint16_t light = adc_read(ADC_LIGHT_CH);
		fprintf(lcdout, "The temperature is %d\n, the light is %d\n",
				temperature, light);
		//slow down the refresh freq of the lcd
		_delay_ms(1000);
	}
}
