/*
 * pscheduler.c
 *
 *  Created on: Jun 1, 2016
 *      Author: Jiadong
 */

#include "pscheduler.h"


#define TIMER1_CYC_FOR_1MILLISEC 	249
#define NUM_CONTEXT 	20

void **pxCurrentTCB; //ptr to sp.

//static uint8_t num_task;
static context contextArray[NUM_CONTEXT];

//put the system sp to the my sp.
#define save_context() \
	asm volatile (	\
			"push r0 \n\t" \
			"in r0, __SREG__ \n\t" \
			"cli \n\t"	\
			"push r0 \n\t"	\
			"push r1 \n\t"	\
			"clr r1 \n\t"	\
			"push r2 \n\t"	\
			"push r3 \n\t"	\
			"push r4 \n\t"	\
			"push r5 \n\t"	\
			"push r6 \n\t"	\
			"push r7 \n\t"	\
			"push r8 \n\t"	\
			"push r9 \n\t"	\
			"push r10 \n\t"	\
			"push r11 \n\t"	\
			"push r12 \n\t"	\
			"push r13 \n\t"	\
			"push r14 \n\t"	\
			"push r15 \n\t"	\
			"push r16 \n\t"	\
			"push r17 \n\t"	\
			"push r18 \n\t"	\
			"push r19 \n\t"	\
			"push r20 \n\t"	\
			"push r21 \n\t"	\
			"push r22 \n\t"	\
			"push r23 \n\t"	\
			"push r24 \n\t"	\
			"push r25 \n\t"	\
			"push r26 \n\t"	\
			"push r27 \n\t"	\
			"push r28 \n\t"	\
			"push r29 \n\t"	\
			"push r30 \n\t"	\
			"push r31 \n\t"	\
			"lds r26, pxCurrentTCB \n\t"	\
			"lds r27, pxCurrentTCB + 1 \n\t"	\
			"in r0, __SP_L__ \n\t" 	\
			"st x+, r0 \n\t"	\
			"in r0, __SP_H__ \n\t"	\
			"st x+, r0"	\
	)

//assign the sp to the system sp
#define restore_context()	\
	asm volatile (	\
			"lds r26, pxCurrentTCB \n\t"	\
			"lds r27, pxCurrentTCB + 1 \n\t"	\
			"ld r28, x+ \n\t"	\
			"out __SP_L__, r28 \n\t"	\
			"ld r29, x+ \n\t"	\
			"out __SP_H__, r29 \n\t"	\
			"pop r31 \n\t"	\
			"pop r30 \n\t"	\
			"pop r29 \n\t"	\
			"pop r28 \n\t"	\
			"pop r27 \n\t"	\
			"pop r26 \n\t"	\
			"pop r25 \n\t"	\
			"pop r24 \n\t"	\
			"pop r23 \n\t"	\
			"pop r22 \n\t"	\
			"pop r21 \n\t"	\
			"pop r20 \n\t"	\
			"pop r19 \n\t"	\
			"pop r18 \n\t"	\
			"pop r17 \n\t"	\
			"pop r16 \n\t"	\
			"pop r15 \n\t"	\
			"pop r14 \n\t"	\
			"pop r13 \n\t"	\
			"pop r12 \n\t"	\
			"pop r11 \n\t"	\
			"pop r10 \n\t"	\
			"pop r9 \n\t"	\
			"pop r8 \n\t"	\
			"pop r7 \n\t"	\
			"pop r6 \n\t"	\
			"pop r5 \n\t"	\
			"pop r4 \n\t"	\
			"pop r3 \n\t"	\
			"pop r2 \n\t"	\
			"pop r1 \n\t"	\
			"pop r0 \n\t"	\
			"out __SREG__, r0 \n\t"	\
			"pop r0 \n\t"	\
	)

static void timer1_init() {

	// CTC mode for compare A  0x04
	TCCR1B &= ~(1 << WGM13);
	TCCR1B |= (1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
	//prescaler
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);
	//Set interrupt mask
	TIMSK1 |= (1 << OCIE1A);
	//Clear the interrupt flag
	TIFR1 |= (1 << OCF1A);
	//set value for comparing.
	OCR1A = TIMER1_CYC_FOR_1MILLISEC;
}

void pscheduler_run(task_t *task_list, uint8_t num_task) {

	uint8_t index;
	timer1_init();

	for (index = 0; index < num_task; index++) {

		contextArray[index].pstack = &(contextArray[index].stack[220]);
		contextArray[index].stack[252] = 0x80; //enable global interrupt
		task_t my_task = task_list[index]; // without & in front of func ptr

		//put the function pointer onto the top of the stack
		contextArray[index].stack[254] = (uint8_t) ((uint16_t) my_task >> 8); //high
		contextArray[index].stack[255] = (uint8_t) ((uint16_t) my_task & 0xFF); //low

		if (index == num_task - 1) //last node points to the first node.
			contextArray[index].next = &contextArray[0];
		else
			contextArray[index].next = &contextArray[index + 1];
	}

	pxCurrentTCB = &(contextArray[0].pstack); // will call the task 1
	restore_context();

	//ret, not reti, to execute the function.
	asm volatile (
			"ret \n\t"
	);
}

ISR(TIMER1_COMPA_vect, ISR_NAKED) {
	//here inside the save context, the global interrupt is disabled.
	save_context();

	static context *my_context = contextArray;
	my_context = my_context->next;
	pxCurrentTCB = &((my_context->next)->pstack);

	restore_context();
	asm volatile (
			"reti \n\t"
	);
}


