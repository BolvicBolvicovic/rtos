#ifndef ESP8266_H
#define ESP8266_H

#include "c_types.h"

#define BIT(a)			(1 << (a))
#define REG32(a)		(*(volatile u32*)(a))
#define CPU_FREQ_MHZ	80
#define BAUDRATE		115200
#define IRAM_ATTR		__attribute__((section(".iram1")))


#define CLEAR_PERI_REG_MASK(reg, mask)	(REG32(reg) = REG32(reg) & (~(mask)))
#define SET_PERI_REG_MASK(reg, mask)	(REG32(reg) = REG32(reg) | (mask))

#define PERI_IO_MUX			0x60000800
#define PERI_IO_MUX_FUNC	0x13
#define PERI_IO_MUX_FUNC_S	4
#define PERI_IO_MUX_PULLUP	BIT(7)
#define PERI_IO_MUX_U0TXD_U	(PERI_IO_MUX + 0x18)
#define PERI_IO_MUX_GPIO2_U	(PERI_IO_MUX + 0x38)
#define PERI_IO_MUX_GPIO5_U	(PERI_IO_MUX + 0x40)

#define PIN_PULLUP_DISABLE(pin_name)	CLEAR_PERI_REG_MASK(pin_name, PERI_IO_MUX_PULLUP)
#define PIN_FUNC_SELECT(pin_name, func) \
    do { \
        u32 v = REG32(pin_name); \
        v &= ~(PERI_IO_MUX_FUNC << PERI_IO_MUX_FUNC_S); \
        v |= ((func & PERI_IO_MUX_FUNC) << PERI_IO_MUX_FUNC_S); \
        REG32(pin_name) = v; \
    } while (0)

#define GPIO_OUT_REG			REG32(0x60000300)
#define GPIO_OUT_SET_REG		REG32(0x60000304)
#define GPIO_OUT_CLR_REG		REG32(0x60000308)
#define GPIO_ENABLE_REG			REG32(0x6000030C)

#define UART_BASE_REG(u)        (0x60000000 + (u) * 0xF00)
#define UART_FIFO(u)            (UART_BASE_REG(u) + 0x00)
#define UART_STATUS(u)          (UART_BASE_REG(u) + 0x1C)
#define UART_TXFIFO_CNT         0x000000FF
#define UART_TXFIFO_CNT_S       16
#define UART_STOP_BIT_NUM		3
#define UART_STOP_BIT_NUM_S		4
#define UART_PARITY_ENABLE		BIT(1)
#define UART_PARITY			BIT(0)

/* --- BOOTLOADER FUNCTIONS --- */

/* ETS FUNCTIONS */

/* Name: ets_bzero
 * Description: zeros n bytes of array s.
 * */
extern void	ets_bzero(void* s, u32 n);

/* Name: ets_char2xdigit
 * Description: converts a char to an hex digit signed integer.
 * */
extern s32	ets_char2xdigit(char c);

/* Name: ets_delay_us
 * Description: delays for input micro seconds.
 * */
extern void	ets_delay_us(u32 us);

/* Name: ets_enter_sleep
 * Description: calls ets_set_idle_cb with rtc_enter_sleep as a parameter and enter sleep mode.
 * */
extern void	ets_enter_sleep(void);

/* Name: ets_external_printf
 * Description: same implementation as ets_printf.
 * I suppose that it is there until one replaces it with ets_install_external_printf
 * so one can have one's own printf in the ROM?
 * */
extern s32	ets_external_printf(const char* fmt, ...);

/* Name: ets_get_cpu_frequency
 * Description: reads from the register at 0x3fffc704 the cpu frequency.
 * */
extern u32	ets_get_cpu_frequency(void);

/* Name: ets_getc
 * Description: calls uart_rx_one_char_block which waits until a char is received and then retuns it.
 * */
extern char	ets_getc(void);

/* Name: ets_install_external_printf
 * Description: installs an external printf by calling ets_install_putc2.
 * I am not 100% sure about the functions signature.
 * */
extern void	ets_install_external_printf(void (*external_printf)(const char*));

/* Name: ets_install_putc1
 * Description: installs a putc function by updating the register at 0x3fffdd48
 * with the input function pointer.
 * */
extern void	ets_install_putc1(void (*putc)(u8 c));

/* Name: ets_install_putc2
 * Description: installs a putc function by updating the register at 0x3fffdd4c
 * with the input function pointer.
 * */
extern void	ets_install_putc2(void (*putc)(u8 c));

/* Name: ets_install_external_printf
 * Description: installs the uart printf by calling ets_install_putc1.
 * I am not 100% sure about the functions signature as it possibly .
 * It seems to load the word _unknown_3 (0xcc1d0040) which I have no idea what that is.
 * */
extern void	ets_install_uart_printf(void (*uart_printf)(const char*));

/* Name: ets_intr_lock
 * Description: sets the interrupt level to 3. All interrupts under or of level 3 are locked.
 * Sets the register at 0x3fffdcc0 to 3. Maybe remembering the new interrupt level?
 * */
extern void	ets_intr_lock(void);

/* Name: ets_intr_unlock
 * Description: sets the interrupt level to 0. Effectively enables all interrupts.
 * */
extern void	ets_intr_unlock(void);

/* Name: ets_isr_attach
 * Description: calls _xtos_set_interrupt_handler_arg, setting up the interrupt handler.
 * */
extern void	ets_isr_attach(s32 interrupt_nb, void* handler, void* args); 

/* Name: ets_isr_mask
 * Description: calls _xtos_ints_off, clearing the bits in the mask from the interrupt flag.
 * */
extern void	ets_isr_mask(u32 mask);

/* Name: ets_isr_unmask
 * Description: calls _xtos_ints_on, setting the bits in the mask from the interrupt flag.
 * */
extern void	ets_isr_unmask(u32 mask);

extern s32	ets_memcmp(const void* b1, const void* b2, u32 n);
extern void*	ets_memcpy(void* dst, const void* src, u32 n);
extern void*	ets_memmove(void* dst, const void* src, u32);
extern void*	ets_memset(void* s, s32 c, u32 n);

extern s32	ets_printf(const char* fmt, ...);
extern void	ets_putc(u8 byte);
extern void	ets_update_cpu_frequency(u32 mhz);
extern s32	ets_strcmp(const char* s1, const char* s2);
extern char*	ets_strcpy(char* dst, const char* src);
extern u32	ets_strlen(const char* s);
extern s32	ets_strncmp(const char* s1, const char* s2, u32 n);	
extern char*	ets_strncpy(char* dst, const char* src, u32 n);
extern char*	ets_strstr(const char* haystack, const char* needle);

/* _XTOS FUNCTIONS*/

/* Name: _xtos_set_interrupt_handler_arg
 * Description: called by ets_isr_attach. Set up the interrupt handler.
 * Use only if you want to avoid a function call.
 * */
extern void	_xtos_set_interrupt_handler_arg(s32 interrupt_nb, void* handler, void* args); 

/* Name: _xtos_ints_off
 * Description: clear the interrupts flags with the mask by writing to the register at 0x3fffc200.
 * Seems to return something but I don't know what.
 * Called by ets_isr_mask.
 * */
extern void	_xtos_ints_off(u32 mask);

/* Name: _xtos_ints_on
 * Description: sets the interrupts flags with by writing to the register at 0x3fffc200.
 * Seems to return something but I don't know what.
 * Called by ets_isr_unmask.
 * */
extern void	_xtos_ints_on(u32 mask);

/* UART FUNCTIONS */
extern void	uart_div_modify(u8 uart, u32 div);

/* ROM FUNCTIONS */
extern void	rom_i2c_writeReg(u8 slave_addr, u8 reg_addr_len, u8 data_len, u32 data);

/* 'STD' FUNCTIONS */
extern int	rand(void);
extern void	srand(u32 seed);
extern s32	strcmp(const char* s1, const char* s2);
extern char*	strcpy(char* dst, const char* src);
extern u32	strlen(const char* s);
extern s32	strncmp(const char* s1, const char* s2, u32 n);	
extern char*	strncpy(char* dst, const char* src, u32 n);
extern char*	strstr(const char* haystack, const char* needle);

/* MEMORY FUNCTIONS */
extern void*	mem_calloc(u32 n_elements, u32, element_size);
extern void*	mem_malloc(u32 size);
extern void	mem_free(void* p);
extern void*	mem_realloc(void* p, u32 size);
extern void	mem_init(void);
extern s32	mem_trim(u32 pad); 
extern void*	mem_zalloc(u32 size); 
extern s32	memcmp(const void* b1, const void* b2, u32 n);
extern void*	memmove(void* dst, const void* src, u32);
extern void*	memcpy(void* dst, const void* src, u32 n);
extern void*	memset(void* s, s32 c, u32 n);

#endif
