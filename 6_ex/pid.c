/*
 * pid.c
 *
 *  Created on: Jun 16, 2016
 *      Author: Jiadong
 */

#include "pid.h"
#include "ses_uart.h"

void pid_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, pidData_t *pid)
{
	// Start values for PID controller
	pid->I_sumError = 0;
	pid->lastProcessValue = 0;
	// Tuning constants for PID loop
	pid->P_Factor = p_factor ;
	pid->I_Factor = i_factor ;
	pid->D_Factor = d_factor ;
	// Limits to avoid overflow
	pid->I_maxSumError = MAX_ERROR;
}

uint8_t pid_control(int16_t setPoint, pidData_t* pid_st) {
	uint16_t pv;
	int16_t  error, p_term, d_term;
	int32_t i_term, ret, temp;
	pv = motorFrequency_getRecent();
	error = setPoint - pv;

	fprintf(uartout,"error %d\n", error); //debugging

	//calculate P
	p_term = pid_st->P_Factor * error;

	//calculate I
	temp = pid_st->I_sumError + error;
	if (temp > pid_st->I_maxSumError) {
		i_term =MAX_ERROR;
		pid_st->I_sumError = pid_st->I_maxSumError;

	} else if (temp < -pid_st->I_maxSumError) {
		i_term = -MAX_ERROR ;
		pid_st->I_sumError = -pid_st->I_maxSumError;

	} else {
		pid_st->I_sumError = temp;
		i_term = pid_st->I_Factor * pid_st->I_sumError;
	}

	//calculate D
	d_term = pid_st->D_Factor * (pid_st->lastProcessValue - error);
	pid_st->lastProcessValue = error;


	ret = (p_term + i_term + d_term) / SCALING_FACTOR;
	if (ret > 255) {
		return 255;
	} else if (ret < 0) {
		return 0;
	}else{
		return (uint8_t)ret;
	}
}
