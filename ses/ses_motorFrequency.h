/*
 * ses_motorFrequency.h
 *
 *  Created on: Jun 11, 2016
 *      Author: Jiadong
 */

#ifndef SES_MOTORFREQUENCY_H_
#define SES_MOTORFREQUENCY_H_

#include "ses_common.h"
#include "ses_led.h"
#include "util/atomic.h"

/*
 *Initialize the external interrupt  and timer5. Both are used to calculate the frequency of the motor.
 * */
void motorFrequency_init();

/*
 * Get the recent frequency of the motor. Basically it is the number of revolutions during a period of time.
 *
 * @reutrn The recent frequency
 * */
uint16_t motorFrequency_getRecent();

/*
 * Get the median frequency of the motor. There are two arrays which are used to store frequency values.
 * When the median values is being calculated,  the later values will be stored in another array.
 * The median value is calculated by first sorting the array that is currently used for storing values and then taking the median value.
 *
 * @reutrn The median frequency
 * */
uint16_t motorFrequency_getMedian();

#endif /* SES_MOTORFREQUENCY_H_ */
