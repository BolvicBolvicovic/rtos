#include "tests_lib.h"

void
tests_arena_alloc_basic_cycle()
{
	Arena	arena = arena_init(0x1000);
	u32		mark = arena.sp;

	s32*	s32_array = (s32*)arena_push(&arena, sizeof(int) * 15);

	assert(s32_array);

	for (s32 i = 0; i < 15; i++)
	{
		s32_array[i] = i;
	}

	for (s32 i = 0; i < 15; i++)
	{
		assert(s32_array[i] == i);
	}

	arena_pop(&arena, mark);
	
	assert(arena.sp == 0);
}
