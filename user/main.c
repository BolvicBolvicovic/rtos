#include "esp8266.h"

#define PIN_LED		2
#define BAUDRATE	115200

INTERNAL void
my_putc(u8 c)
{
	uart_tx_one_char(c);
}

int
main(void)
{
	PIN_PULLUP_DISABLE(PERI_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERI_IO_MUX_U0TXD_U, FUNC_U0TXD);
	ets_update_cpu_frequency(80);
	REG32(UART_CONF0(0))  = REG32(UART_CONF0(0)) 
		& ~(UART_PARITY_ENABLE 
			| UART_PARITY
			| (UART_BIT_NUM << UART_BIT_NUM_S)
			| (UART_STOP_BIT_NUM << UART_STOP_BIT_NUM_S))
		| (3 << UART_BIT_NUM_S)
		| (1 << UART_STOP_BIT_NUM_S);
	REG32(UART_CLKDIV(0)) = 80000000 / BAUDRATE;
	ets_install_putc1(my_putc);
	GPIO_ENABLE_REG |= BIT(PIN_LED);
	
	ets_printf("\n\nLet's get started!\n");

	for (;;)
	{
		GPIO_OUT_CLR_REG = BIT(PIN_LED);
		ets_delay_us(500000);
		ets_printf("ON\n");

		GPIO_OUT_SET_REG = BIT(PIN_LED);
		ets_delay_us(500000);
		ets_printf("OFF\n");
	}

	return 0;
}
