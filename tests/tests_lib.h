#ifndef TESTS_LIB_H
#define TESTS_LIB_H

static unsigned char*	_heap_start = 0;

#include <stdlib.h>
#include <assert.h>
#include "arena_alloc.h"

#define TESTER_HEAP_SIZE	0x10000


void	tests_arena_alloc_basic_cycle();

#endif
