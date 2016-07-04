/*
 * ses_button.c
 *
 *  Created on: Apr 24, 2016
 *      Author: Jiadong
 */

#include "ses_button.h"

static pButtonCallback rotaryCallback, joystickCallback; //has to be volatile???

void button_init(bool debouncing) {
	//set as input
	DDR_REGISTER(BUTTON_PORT) &= ~(1 << ROTARY_ENCODER_PIN);
	DDR_REGISTER(BUTTON_PORT) &= ~(1 << JOYSTICK_PIN);
	// Activate the internal pull-up resistor
	BUTTON_PORT |= (1 << ROTARY_ENCODER_PIN);
	BUTTON_PORT |= (1 << JOYSTICK_PIN);

	if (debouncing) {
		//also work?
		timer1_start();
		timer1_setCallback(button_checkState);
//		timer1_setCallback(&button_checkState);

//		pTimerCallback buttonCheckCallback = &button_checkState;
//		timer1_setCallback(buttonCheckCallback);

	} else {
		//activate the pin change interrupt
		PCICR |= (1 << PCIE0);
		// should I set the flag??????????????????????
		//since sei() will be called at main.c
		//SREG |= (1 << 7); //here symbol I cannot be resolved
		//enable mask
		PCMSK0 |= (1 << ROTARY_ENCODER_PIN);
		PCMSK0 |= (1 << JOYSTICK_PIN);
	}

}

/**
 * Get the state of the joystick button.
 */
bool button_isJoystickPressed(void) {
	if (PIN_REGISTER(BUTTON_PORT) & (1 << JOYSTICK_PIN))
		return false;
	else
		return true;
}

/**
 * Get the state of the rotary button.
 */
bool button_isRotaryPressed(void) {
	if ((PIN_REGISTER(BUTTON_PORT) & (1 << ROTARY_ENCODER_PIN)))
		return false;
	else
		return true;
}

//This ISR is for the direct interrupts without debouncing.
ISR(PCINT0_vect) {
	if (button_isRotaryPressed()) {
		if (rotaryCallback != NULL)
			rotaryCallback();
	}
	if (button_isJoystickPressed()) {
		if (joystickCallback != NULL)
			joystickCallback();
	}
}

void button_setRotaryButtonCallback(pButtonCallback callback) {
	//we check if it is NULL at the ISR.
	//because when we want to disable the ISR, we could feed this function by null fun ptr
	rotaryCallback = callback;
}
void button_setJoystickButtonCallback(pButtonCallback callback) {
	joystickCallback = callback;
}

void button_checkState(void) {
	static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = { };
	static uint8_t index = 0;
	static uint8_t debouncedState = 0;
	uint8_t lastDebouncedState = debouncedState;
//because index is static, it keeps the previous value when it enters.

// each bit in every state byte represents one button
	state[index] = 0;
	//here is the link to the real world
	//all these codes will be executed when you press the button(30ms).
	if (button_isJoystickPressed()) {
		state[index] |= 1;
	}
	if (button_isRotaryPressed()) {
		state[index] |= 2;
	}
	index++;
	if (index == BUTTON_NUM_DEBOUNCE_CHECKS) {
		index = 0;
	}
// init compare value and compare with ALL reads, only if
// we read BUTTON_NUM_DEBOUNCE_CHECKS consistent "0" in the state
// array, the button at this position is considered pressed
	uint8_t j = 0xFF;
	for (uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++) {
		j = j & state[i];
	}
	debouncedState = j;

//check the button in the corresponding bit. only when 0->1, meaning button pressed, we execute button.
//it has nothing to do with active low. We design it as it is. when button pressed, we assign the bit to 1.

//Better to use !=0 since the result might be 0010, which is 2.
//when if test is used, we could do if((debouncedState & 1)), value 2 will also be evaluated to TRUE
	if (((debouncedState & 1) != 0) && ((lastDebouncedState & 1) == 0)) {
		if (joystickCallback != NULL)
			joystickCallback();
	}
	if (((debouncedState & 2) != 0) && ((lastDebouncedState & 2) == 0)) {
		if (rotaryCallback != NULL) //if it is null, you dont call it. if you call, the board resets.
			rotaryCallback();
	}
	//dont have to do it here. it is done in the beginning. same.
	//lastDebouncedState = debouncedState;

}
