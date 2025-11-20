#ifndef MULTITASKING_H
#define MULTITASKING_H

#include "c_types.h"

#define TASK_POOL_SIZE		20
#ifdef UNIT_TESTS
#define DISABLE_INTERRUPTS()
#define HALT()
#else
#define DISABLE_INTERRUPTS()	asm volatile ("rsil a0, 15")
#define HALT()			asm volatile ("waiti 0")
#endif

typedef struct
{
	uptr	exit;
	uptr	sp;
	uptr	param;
	u32	a3;
	u32	a4;
	u32	a5;
	u32	a6;
	u32	a7;
	u32	a8;
	u32	a9;
	u32	a10;
	u32	a11;
	u32	a12;
	u32	a13;
	u32	a14;
	u32	a15;
	u32	sar;
	uptr	entry;
	u32	ps;
} task_stack_frame;

typedef enum
{
	UNINITIALIZED,
	RUNNING,
	READY,
	BLOCKED,
	SUSPENDED
} task_state;

typedef struct Task_s
{
	u32		id;
	task_state	state;
	u32*		sp;	
	struct Task_s*	next;
} Task;

/* name: task_yield
 * task_yield saves the running tasks, adds it to the tail of ready tasks and does the context switch
 * to the next task.
 * */
void	task_yield(void);

//void	task_suspend(Task*);
//void	task_resume(Task*);

/* name: task_init
 * @params: all parameters are only used in the task_create call that generates the first task.
 * @ret: On success, should not return since the context switch should happen. On error, returns -1.
 * 	 For the unit tests, should return 0 on success.
 * */
s32	task_init(void* task_routine, void* task_routine_param, u32* task_stack_top);

/* name: task_create
 * @param task_routine:		Function pointer to the entry point of the task.
 * @param task_routine_param:	Pointer to the arguments of the task.
 * @param task_stack_top:	Top of the stack where the stack frame will be stored.
 * @ret: On success, returns the id of the task. On error, returns -1.
 * */
s32	task_create(void* task_routine, void* task_routine_param, u32* task_stack_top);

#endif
