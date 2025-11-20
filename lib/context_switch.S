/* void	_task_context_switch(u32** task_old_sp, u32* task_new_sp) */
.global	_task_context_switch
.align	4
_task_context_switch:
	/* Allocate space on the stack for the frame */
	addi	a1, a1, -76
	/* Save a0 as pointer counter */
	s32i	a0, a1, 72
	rsr	a0, PS
	s32i	a0, a1, 76
	/* Save SAR and general registers */
	rsr	a0, SAR
	s32i	a0, a1, 68

	s32i	a2, a1, 8
	s32i	a3, a1, 12
	s32i	a4, a1, 16
	s32i	a5, a1, 20
	s32i	a6, a1, 24
	s32i	a7, a1, 28
	s32i	a8, a1, 32
	s32i	a9, a1, 36
	s32i	a10, a1, 40
	s32i	a11, a1, 44
	s32i	a12, a1, 48
	s32i	a13, a1, 52
	s32i	a14, a1, 56
	s32i	a15, a1, 60
	
	movi	a0, 0
	s32i	a0, a1, 0
	/* Saving old sp and swtich to new sp */
	s32i	a1, a2, 0
/* void	_task_context_init(u32* __unused, u32* task_new_sp) */
.global _task_context_init
_task_context_init:
	mov	a1, a3
	
	/* Restore registers */
	l32i	a0, a1, 76
	wsr	a0, PS
	rsync

	l32i	a0, a1, 68
	wsr	a0, SAR

	l32i	a2, a1, 8
	l32i	a3, a1, 12
	l32i	a4, a1, 16
	l32i	a5, a1, 20
	l32i	a6, a1, 24
	l32i	a7, a1, 28
	l32i	a8, a1, 32
	l32i	a9, a1, 36
	l32i	a10, a1, 40
	l32i	a11, a1, 44
	l32i	a12, a1, 48
	l32i	a13, a1, 52
	l32i	a14, a1, 56
	l32i	a15, a1, 60
	
	l32i	a0, a1, 0

	l32i	a2, a1, 72
	addi	a1, a1, 76
	jx	a2
