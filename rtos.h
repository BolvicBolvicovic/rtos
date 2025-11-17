#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

typedef uint8_t		u8;
typedef uint32_t	u32;
typedef int32_t		s32;

extern void		ets_printf(const char* fmt, ...);
extern void		ets_delay_us(u32 us);
extern void		ets_install_putc1(void (*)(u8));
extern void		uart_tx_one_char(u8);
extern void		ets_update_cpu_frequency(u32 mhz);

#define INTERNAL			static
#define BIT(a)				(1 << (a))
#define REG32(a)			(*(volatile u32*)(a))

#define CLEAR_PERI_REG_MASK(reg, mask)	(REG32(reg) = REG32(reg) & (~(mask)))
#define SET_PERI_REG_MASK(reg, mask)	(REG32(reg) = REG32(reg) | (mask))

#define PERI_IO_MUX			0x60000800
#define PERI_IO_MUX_FUNC		0x13
#define PERI_IO_MUX_FUNC_S		4
#define PERI_IO_MUX_PULLUP		BIT(7)
#define PERI_IO_MUX_U0TXD_U		(PERI_IO_MUX + 0x18)
#define FUNC_U0TXD			0

#define PIN_PULLUP_DISABLE(pin_name)	CLEAR_PERI_REG_MASK(pin_name, PERI_IO_MUX_PULLUP)
#define PIN_FUNC_SELECT(pin_name, func)	\
	do { \
		REG32(pin_name) = REG32(pin_name) \
		& (~(PERI_IO_MUX_FUNC << PERI_IO_MUX_FUNC_S)) \
		| (((func & BIT(2) << 2) | (func & 3)) << PERI_IO_MUX_FUNC_S); \
	} while (0)

#define GPIO_OUT_REG			REG32(0x60000300)
#define GPIO_OUT_SET_REG		REG32(0x60000304)
#define GPIO_OUT_CLR_REG		REG32(0x60000308)
#define GPIO_ENABLE_REG			REG32(0x6000030C)

#define UART_BASE_REG(a)		(0x60000000 + (a) * 0xf00)
#define UART_CLKDIV(a)			(UART_BASE_REG(a) + 0x14)
#define UART_CONF0(a)			(UART_BASE_REG(a) + 0x20)
#define UART_BIT_NUM			3
#define UART_BIT_NUM_S			4
#define UART_STOP_BIT_NUM		3
#define UART_STOP_BIT_NUM_S		2
#define UART_PARITY_ENABLE		BIT(1)
#define UART_PARITY			BIT(0)

#endif
