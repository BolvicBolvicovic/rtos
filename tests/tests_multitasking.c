#include "tests_lib.h"

INTERNAL s32
tests_multitasking_dummy(void)
{
	return 1 + 2 == 4;
}

INTERNAL void
tests_multitasking_basic_cycle(void)
{
#define TESTS_MULTITASKING_STACK_BUFFER_SIZE 0x1000
	u8*	stack_buffer[TESTS_MULTITASKING_STACK_BUFFER_SIZE] = {0};
	u32*	stack_top = (u32*)(stack_buffer + TESTS_MULTITASKING_STACK_BUFFER_SIZE - 1);
	
	assert(task_init((void*)tests_multitasking_dummy, 0, stack_top) == 0);
	task_stack_frame*	frame = (task_stack_frame*)((uptr)stack_top & ~0xF) - 1;
	assert(frame->entry	== (uptr)tests_multitasking_dummy);
	assert(frame->a[2]	== 0);
	assert(frame->ps	== 0x20);
	assert(frame->sar	== 0);
}

void
tests_multitasking(void)
{
	tests_multitasking_basic_cycle();
}
