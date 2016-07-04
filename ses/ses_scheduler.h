#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"
#include "util/atomic.h"
#include "ses_timer.h"

/* TYPES ********************************************************************/

/**type of function pointer for tasks */
typedef void (*task_t)(void*);

/** Task structure to schedule tasks
 */
typedef struct taskDescriptor_s {
	task_t task;          ///< function pointer to call
	void * param;        ///< pointer, which is passed to task when executed
	uint16_t expire;      ///< time offset in ms, after which to call the task
	uint16_t period;    ///< period of the timer after firing; 0 means exec once
	uint8_t execute :1;    ///< for internal use
	uint8_t reserved :7;   ///< reserved
	struct taskDescriptor_s * next; ///< pointer to next task, internal use
} taskDescriptor;

/***** Alarm clock****************/
typedef uint32_t systemTime_t;

/* FUNCTION PROTOTYPES *******************************************************/

/*
 * @return the time which is updated by scheduler.
 * */
systemTime_t scheduler_getTime();

/*
 * @param time   Initialize the system time which the
 * 				 scheduler is responsible for updating.
 * */
void scheduler_setTime(systemTime_t time);

/**
 * Initializes the task scheduler. Uses hardware timer2 of the AVR.
 */
void scheduler_init();

/**
 * Runs scheduler in an infinite loop.
 */
void scheduler_run();

/**
 * Adds a new task to the scheduler.
 *
 * @param td   Pointer to taskDescriptor structure. The scheduler takes
 *             possession of the memory pointed at by td until the task
 *             is removed by scheduler_remove or -- in case of a 
 *             non-periodic task -- the task is executed. td->expire 
 *             and td->execute are changed to by the task scheduler.
 *
 * @return     false, if task is already scheduled or invalid (NULL)
 *             true, if task was successfully added to scheduler and 
 *             will be executed after td->expire ms.
 */
bool scheduler_add(taskDescriptor * td);

/**
 * Removes a task from the scheduler.
 *
 * @param td	pointer to task descriptor to remove
 * */
void scheduler_remove(taskDescriptor * td);

#endif /* SCHEDULER_H_ */
