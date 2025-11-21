#define XT_STK_EXIT		0x00    /* (offset 0) exit point for dispatch */
#define XT_STK_PC		0x04    /* return address */
#define XT_STK_PS		0x08    /* at level 1 PS.EXCM is set here */
#define XT_STK_A0		0x0C
#define XT_STK_A1		0x10    /* stack ptr before interrupt */
#define XT_STK_A2		0x14
#define XT_STK_A3		0x18
#define XT_STK_A4		0x1C
#define XT_STK_A5		0x20
#define XT_STK_A6		0x24
#define XT_STK_A7		0x28
#define XT_STK_A8		0x2C
#define XT_STK_A9		0x30
#define XT_STK_A10		0x34
#define XT_STK_A11		0x38
#define XT_STK_A12		0x3C    /* Call0 callee-save */
#define XT_STK_A13		0x40    /* Call0 callee-save */
#define XT_STK_A14		0x44    /* Call0 callee-save */
#define XT_STK_A15		0x48    /* Call0 callee-save */
#define XT_STK_SAR		0x4C

/* void	_task_context_switch(u32** task_old_sp, u32* task_new_sp) */
.global	_task_context_switch
.align	4
_task_context_switch:
	/* Allocate space on the stack for the frame */
	addi	a1, a1, -(XT_STK_SAR + 4)

	s32i	a0, a1, XT_STK_A0
	s32i	a0, a1, XT_STK_EXIT
	movi	a0, 0
	rsr	a0, EPC1
	s32i	a0, a1, XT_STK_PC
	rsr	a0, PS
	s32i	a0, a1, XT_STK_PS
	rsr	a0, SAR
	s32i	a0, a1, XT_STK_SAR
	
	s32i	a1, a1, XT_STK_A1
	s32i	a2, a1, XT_STK_A2
	s32i	a3, a1, XT_STK_A3
	s32i	a4, a1, XT_STK_A4
	s32i	a5, a1, XT_STK_A5
	s32i	a6, a1, XT_STK_A6
	s32i	a7, a1, XT_STK_A7
	s32i	a8, a1, XT_STK_A8
	s32i	a9, a1, XT_STK_A9
	s32i	a10, a1, XT_STK_A10
	s32i	a11, a1, XT_STK_A11
	s32i	a12, a1, XT_STK_A12
	s32i	a13, a1, XT_STK_A13
	s32i	a14, a1, XT_STK_A14
	s32i	a15, a1, XT_STK_A15
	
	/* Saving old sp and swtich to new sp */
	s32i	a1, a2, 0
/* void	_task_context_init(u32* __unused, u32* task_new_sp) */
.global _task_context_init
_task_context_init:
	mov	a1, a3
	
	/* Restore registers */
	l32i	a0, a1, XT_STK_PS
	wsr	a0, PS
	rsync

	l32i	a0, a1, XT_STK_SAR
	wsr	a0, SAR
	
	l32i	a0, a1, XT_STK_A0
	l32i	a2, a1, XT_STK_A2
	l32i	a3, a1, XT_STK_A3
	l32i	a4, a1, XT_STK_A4
	l32i	a5, a1, XT_STK_A5
	l32i	a6, a1, XT_STK_A6
	l32i	a7, a1, XT_STK_A7
	l32i	a8, a1, XT_STK_A8
	l32i	a9, a1, XT_STK_A9
	l32i	a10, a1, XT_STK_A10
	l32i	a11, a1, XT_STK_A11
	l32i	a12, a1, XT_STK_A12
	l32i	a13, a1, XT_STK_A13
	l32i	a14, a1, XT_STK_A14
	l32i	a15, a1, XT_STK_A15

	
	l32i	a2, a1, XT_STK_PC
	l32i	a1, a1, XT_STK_A1
	jx	a2
