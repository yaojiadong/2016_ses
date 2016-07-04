/*
 * ses_pwm.h
 *
 *  Created on: Jun 11, 2016
 *      Author: Jiadong
 */

#ifndef SES_PWM_H_
#define SES_PWM_H_

#include "ses_common.h"

/*
 * Initialize the timer0 with pwm mode
 * */
void pwm_init(void);

/*
 * Set the duty cycle for the PWM signal.
 *
 * @param dutyCycle 	the duty cycle of the PWM
 * */
void pwm_setDutyCycle(uint8_t dutyCycle);




#endif /* SES_PWM_H_ */
