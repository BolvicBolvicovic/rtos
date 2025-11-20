#include "multitasking.h"

#ifdef UNIT_TESTS
INTERNAL void
_task_context_switch(u32** task_old_sp, u32* task_new_sp)
{
	(void)task_old_sp;
	(void)task_new_sp;
}
INTERNAL void
_task_context_init(u32* __unused, u32* task_new_sp)
{
	(void)__unused;
	(void)task_new_sp;
}
#else
extern void	_task_context_switch(u32** task_old_sp, u32* task_new_sp);
extern void	_task_context_init(u32* __unused, u32* task_new_sp);
#endif

Task	_task_pool[TASK_POOL_SIZE] = {0};
u32	_task_pool_index= 0;
Task*	_task_running	= 0;
Task*	_task_ready_tail= 0;
//Task*	_task_suspended	= 0;
//Task*	_task_blocked	= 0;

//void
//task_suspend(Task* task)
//{
//
//}
//
//void
//task_resume(Task* task)
//{
//
//}

void
task_yield(void)
{
	if (_task_ready_tail)
		_task_ready_tail->next = _task_running;
	_task_ready_tail	= _task_running;
	_task_running		= _task_running->next;
	_task_ready_tail->next	= 0;
	_task_context_switch(&(_task_ready_tail->sp), _task_running->sp);
}

INTERNAL void
task_end(void)
{
	DISABLE_INTERRUPTS();

	Task*	next = _task_running->next;
	_task_running->state	= UNINITIALIZED;
	_task_running->next	= 0;
	_task_running->sp	= 0;
	_task_running		= next;
}

INTERNAL Task*
task_find_next_initialized(void)
{
	u32	start_index = _task_pool_index;

	do
	{
		_task_pool_index++;
		if (_task_pool_index == TASK_POOL_SIZE)
			_task_pool_index = 0;
		if (_task_pool[_task_pool_index].state == UNINITIALIZED)
			return &_task_pool[_task_pool_index];
	}
	while (_task_pool_index != start_index);

	return 0;
}

s32
task_create(void* task_routine, void* task_routine_param, u32* task_stack_top)
{
	if (!task_routine || !task_stack_top) return -1;

	Task*	task = task_find_next_initialized();
	if (!task) return -1;

	// Note: Align down stack to 16 bytes (Xtensa requirement).
	task->sp	= (u32*)((uptr)task_stack_top & ~0xF);

	task_stack_frame*	frame = (task_stack_frame*)(task->sp) - 1;
	frame->entry	= (u32)task_routine;
	frame->exit	= (u32)task_end;
	frame->param	= (u32)task_routine_param;
	frame->ps	= 0x20; // Note: Turn the interrupts on.
	frame->sar	= 0;

	task->state = READY;
	return task->id;
}


s32
task_init(void* task_routine, void* task_routine_param, u32* task_stack_top)
{
	for (u32 i = 0; i < TASK_POOL_SIZE; i++)
	{
		_task_pool[i].id	= i;
		_task_pool[i].state	= UNINITIALIZED;
		_task_pool[i].next	= 0;
		_task_pool[i].sp	= 0;
	}

	s32	task_main_id = task_create(task_routine, task_routine_param, task_stack_top);
	if (task_main_id == -1) return -1;
	_task_pool[task_main_id].next	= &_task_pool[task_main_id];
	_task_pool[task_main_id].state	= RUNNING;

	_task_running	= &_task_pool[task_main_id];
	_task_ready_tail= _task_running;
	_task_context_init(0, _task_running->sp);

	// Note: Should never be reached.
	return 0;
}
