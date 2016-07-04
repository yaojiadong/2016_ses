/*
 * pid.h
 *
 *  Created on: Jun 16, 2016
 *      Author: Jiadong
 */

#ifndef PID_H_
#define PID_H_

#include <stdint.h>
#include "ses_pwm.h"
#include "ses_motorFrequency.h"

// Maximum value of variables
#define MAX_ERROR      	INT16_MAX
#define SCALING_FACTOR  100

//data structure of the PID controller
typedef struct PID_DATA {
	int16_t lastProcessValue;
	int32_t I_sumError;
	int16_t P_Factor;
	int16_t I_Factor;
	int16_t D_Factor;
	int32_t I_maxSumError;
} pidData_t;


/*
 * Set up PID controller parameters
 *
 * @param p_factor	P factor of PID controller
 * @param i_factor	I factor of PID controller
 * @param d_factor	D factor of PID controller
 * @param pid	The structure that stores all the parameters of PID controller.
 *  *
 * */
void pid_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, pidData_t *pid);

/*
 * Calculate the sum of the PID controllers.
 *
 * @param setPoint	The target frequency
 * @param pid_st	The initialized pid controller parameters.
 *
 * @return the value of the sum of the controllers.
 * */
uint8_t pid_control(int16_t setPoint, pidData_t* pid_st);

#endif /* PID_H_ */
