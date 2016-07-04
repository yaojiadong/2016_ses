#ifndef SES_BUTTON_H_
#define SES_BUTTON_H_

/* INCLUDES ******************************************************************/

#include "ses_common.h"
#include "ses_timer.h"

/* DEFINES & MACROS **********************************************************/
#define BUTTON_PORT       			PORTB
#define ROTARY_ENCODER_PIN  		6
#define JOYSTICK_PIN       			7
#define BUTTON_NUM_DEBOUNCE_CHECKS  5

/* TYPES ********************************************************************/
typedef void (*pButtonCallback)();

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * Set the callback function for rotary button.
 *
 * @param callback   The function is called when the rotary button is pressed.
 * */
void button_setRotaryButtonCallback(pButtonCallback callback);


/*
 * Set the callback function for joystick button.
 *
 * @param callback   The function is called when the joystick button is pressed.
 * */
void button_setJoystickButtonCallback(pButtonCallback callback);


/*
 * The button is decided as pressed after several checks.
 * Each check takes 5ms.
 * */
void button_checkState();

/**
 * Initializes rotary encoder and joystick button
 */
void button_init(bool);

/** 
 * Get the state of the joystick button.
 */
bool button_isJoystickPressed(void);

/** 
 * Get the state of the rotary button.
 */
bool button_isRotaryPressed(void);

#endif /* SES_BUTTON_H_ */
