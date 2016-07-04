
#if 0
/**This is a so-called "naked" interrupt and thus the compiler
 * creates no additional assembler code when entering/ leaving
 * the function. Note that a "reti" (return from interrupt)
 * becomes necessary.
 *
 */
ISR(TIMER1_COMPA_vect, ISR_NAKED) {
	asm volatile (
			"reti \n\t"
	);

}

//------------------------------

/**In preemptive multi-tasking each
 * task needs an own stack.
 */
#define THREAD_STACK_SIZE	256


//------------------------------


/**The state of a task must be stored in
 * some struct
 */
struct context_t {
	uint8_t stack[THREAD_STACK_SIZE];
	void *pstack;
	struct context_t *next;
};



/*
 void save_context(uint8_t index) {
 context *this_context = contextArray + index;
 uint8_t *this_stack = this_context->stack;
 void *this_pstack = this_context->pstack;

 asm volatile(
 "lds r26, this_pstack" "\n\t");

 //uint16_t *this_pstack = (uint16_t *)this_context->pstack;

 //register uint8_t *this_stack asm("r1");
 //this_pstack = this_context->pstack;
 asm volatile(
 //"EXTERN this_stack"
 //"EXTERN this_pstack"
 //"push r0 \n\t"
 //"in r0 this_stack \n\t"
 "st this_stack+ ,r0 \n\t"
 "in r0, __SREG__ \n\t"
 "cli \n\t"
 "st this_stack+, r0 \n\t"
 "st this_stack+, r1 \n\t"
 "st this_stack+ ,r2 \n\t"
 "st this_stack+, r3 \n\t"
 "st this_stack+, r4 \n\t"
 "st this_stack+, r5 \n\t"
 "st this_stack+, r6 \n\t"
 "st this_stack+, r7 \n\t"
 "st this_stack+, r8 \n\t"
 "st this_stack+, r9 \n\t"

 //...
 "st this_stack+, r31 \n\t"

 "in r0, __SP_L__ \n\t"
 "st this_pstack+, r0 \n\t"
 "in r0, __SP_H__ \n\t"
 "st this_pstack+, r0"

 : "=r"(this_stack)
 );

 //asm volatile("lds r26 %0": : "e"(this_stack));
 //asm volatile("st %0, r0" : "=r"(this_stack));
 //register uint8_t *this_stack asm("r26");
 //asm volatile("": "=r" (this_stack));
 //asm volatile("in %0, %1": "=r" (*this_stack) : "I" (_SFR_IO_ADDR(SPL)));
 //	asm volatile(
 //			"mov %0, r0" "\n\t"
 //			"in r0, __SREG__" "\n\t"
 //			"cli""\n\t"
 //			"mov %0, r0""\n\t"
 //			"mov %0, r1""\n\t"
 //			"mov %0, r2""\n\t"
 //			"lds r26, this_pstack" "\n\t"
 //"in r0, __SP_L__""\n\t"
 //"mov %1, r0""\n\t"
 //"in r0, __SP_H__""\n\t"
 //"mov %2, r0""\n\t"
 //"in %1, %3""\n\t"
 //"in %2, %4""\n\t"
 //: "=r" (*(this_stack++)), "=r"(this_pstack),"=r"(*(&(this_pstack)+1))
 //: "I"(_SFR_IO_ADDR(SPL)),"I"(_SFR_IO_ADDR(SPH))
 //	);

 asm volatile("in %0 ,%1"
 : "=r"(*(&(this_pstack)+1))  //+1 means 1 bit? or 8bit? 1 byte?
 :"I"(_SFR_IO_ADDR(SPH)));

 asm volatile("cli"
 : "=r"(*(this_stack+1)));
 asm volatile("mov %0 ,r1"
 : "=r"(*(this_stack+2)));

 //asm volatile("mov %0, r1": "=r" (*(this_stack++)) : );
 }
 */

#endif
