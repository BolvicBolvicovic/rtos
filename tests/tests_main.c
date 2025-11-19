#include "tests_lib.h"

int
main(void)
{
	_heap_start = (u8*)malloc(TESTER_HEAP_SIZE);

	tests_arena_alloc_basic_cycle();

	return 0;
}
