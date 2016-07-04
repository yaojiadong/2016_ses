/*
 * ses_rotary.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Jiadong
 */

#ifndef SES_ROTARY_H_
#define SES_ROTARY_H_


/* INCLUDES ******************************************************************/
#include "ses_common.h"


/* DEFINES & MACROS **********************************************************/
#define A_ROTARY_PORT	PORTB
#define B_ROTARY_PORT	PORTG
#define A_ROTARY_PIN	5
#define B_ROTARY_PIN	2


/* TYPES ********************************************************************/
typedef void (*pTypeRotaryCallback)();

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * Initialize the rotary encoder.
 * */
void rotary_init();

/*
 * Set the callback function for encoder when turning clockwise.
 *
 * @param cb   is the function that is called when the
				encoder turns clockwise.
 * */
void rotary_setClockwiseCallback(pTypeRotaryCallback cb);

/*
 * Set the callback function for encoder when turning counterclockwise.
 *
 * @param cb   is the function that is called when the
				encoder turns counterclockwise.
 * */
void rotary_setCounterClockwiseCallback(pTypeRotaryCallback cb);


/*
 * The rotary encoder is checked at a reasonable frequency
 * and decides which direction it is turning to.
 * */
void check_rotary();

#endif /* SES_ROTARY_H_ */
