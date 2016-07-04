/*
 * pscheduler.h
 *
 *  Created on: Jun 1, 2016
 *      Author: Jiadong
 */

#ifndef PSCHEDULER_H_
#define PSCHEDULER_H_

#include "ses_common.h"
#include "ses_led.h"

#define THREAD_STACK_SIZE	256

typedef void (*task_t)(void);

typedef struct context_t {
	uint8_t stack[THREAD_STACK_SIZE];
	void *pstack;
	struct context_t *next;
}context;

/*
 * Run preemptive scheduler.
 * It first initialize stacks for each tasks. By calling restore, the first task will be executed.
 * Later on, the scheduler will take the same amount of time (done by ISR) to execute all tasks.
 * */
void pscheduler_run(task_t *task_list, uint8_t num_task);


#endif /* PSCHEDULER_H_ */
