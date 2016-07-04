/*INCLUDES ************************************************************/

#include "ses_scheduler.h"

/* PRIVATE VARIABLES **************************************************/
/** list of scheduled tasks */
static taskDescriptor* taskList = NULL;
static systemTime_t timeVar = 0;

/*FUNCTION DEFINITION *************************************************/
/*
 static void scheduler_update(void) {

 taskDescriptor* temp = taskList;
 while (temp != NULL) {
 if (temp->execute) {
 } else {
 if ((temp->expire) == 0) {
 temp->execute = 1; //mark the expired tasks
 temp->expire = temp->period; //period should be updated in the update funciton.
 } else {
 (temp->expire)--;
 }
 }
 temp = temp->next;
 }
 }
 */

//this function is correct. The previous one will only count down after the execution of the task.
//Here, it will count down immediately after the 'expire' is reset.
static void scheduler_update(void) {

	timeVar++;

	taskDescriptor* temp = taskList;
	while (temp != NULL) {

		if ((temp->expire) == 0) {
			temp->execute = 1; //mark the expired tasks
			temp->expire = temp->period; //period should be updated in the update funciton.
		} else {
			(temp->expire)--;
		}
		temp = temp->next;
	}
}

void scheduler_init() {

	timer2_start();
	timer2_setCallback(scheduler_update);
}

void scheduler_run() {

	taskDescriptor* temp;
	while (1) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			temp = taskList; // every time you put the temp from the first node!!!!!!!
			while (temp != NULL) {

				if (temp->execute == 1) {
					task_t the_task = temp->task;
					void* the_param = temp->param;

					NONATOMIC_BLOCK( NONATOMIC_RESTORESTATE)
					{
						the_task(the_param);			// execute the function
					}

					temp->execute = 0;
					if (temp->period == 0)
						scheduler_remove(temp);
				}
				temp = temp->next;
			}
		}
	}
}

bool scheduler_add(taskDescriptor * toAdd) {
	bool ret;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		//also have to check if it is already added.
		taskDescriptor* temp = taskList;
		while (temp != NULL) {
			if (temp->task == toAdd->task) { //check the struct or the task??
				return false;
			}
			temp = temp->next;
		}

		if (toAdd == NULL)
			ret = false;
		else {	//add in the beginning.
			temp = taskList;
			taskList = toAdd;
			toAdd->next = temp;
			ret = true;
		}
	}
	return ret;
}

void scheduler_remove(taskDescriptor * toRemove) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		taskDescriptor* temp = taskList;
		taskDescriptor* pre_temp = NULL;
		bool found = false;
		while (temp != NULL && !found) {

			if (temp == toRemove) {
				if (temp == taskList) {	//first node
					taskList = taskList->next;
					found = true;
				} else {
					pre_temp->next = temp->next;
					found = true;
				}
			} else {
				pre_temp = temp;
				temp = temp->next;
			}
		}
	}
}

systemTime_t scheduler_getTime() {
	systemTime_t temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		temp = timeVar;
	}
	return temp;
}

void scheduler_setTime(systemTime_t time) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		timeVar = time;
	}
}

