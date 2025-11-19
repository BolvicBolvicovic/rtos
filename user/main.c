#include "esp8266.h"
#include "arena_alloc.h"

#define PIN_LED			2

int
main(void)
{
	Arena*	arena = arena_new();
	PIN_FUNC_SELECT(PERI_IO_MUX_GPIO2_U, 0);
	PIN_FUNC_SELECT(PERI_IO_MUX_GPIO5_U, 0);
	GPIO_ENABLE_REG |= BIT(PIN_LED);

	ets_printf("\n\nLet's get started!\n");

	while (1)
	{
		u32	mark = arena->sp;
		char*	on = (char*)arena_push(arena, 4);
		char*	off= (char*)arena_push(arena, 5);
		
		strcpy(on, "on\n");
		strcpy(off, "off\n");

		GPIO_OUT_CLR_REG = BIT(PIN_LED);
		ets_delay_us(500000);
		ets_printf(on);

		GPIO_OUT_SET_REG = BIT(PIN_LED);
		ets_delay_us(500000);
		ets_printf(off);

		arena_pop(arena, mark);
	}

	return 0;
}
