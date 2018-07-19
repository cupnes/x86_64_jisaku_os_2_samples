#include <hpet.h>
#include <pic.h>
#include <fbcon.h>

#define SCHED_PERIOD	(5 * MS_TO_US)
#define NUM_TASKS		2
#define TASK_B_STASK_BYTES	4096

unsigned long long task_sp[NUM_TASKS];
volatile unsigned int current_task = 0;
unsigned char taskB_stack[TASK_B_STASK_BYTES];

void do_taskB(void)
{
	while (1) {
		putc('B');
		volatile unsigned long long wait = 10000000;
		while (wait--);
	}
}

void schedule(unsigned long long current_rsp)
{
	task_sp[current_task] = current_rsp;
	current_task = (current_task + 1) % NUM_TASKS;
	set_pic_eoi(HPET_INTR_NO);
	asm volatile ("mov %[sp], %%rsp"
		      :: [sp]"a"(task_sp[current_task]));
	asm volatile (
		"pop	%rdi\n"
		"pop	%rsi\n"
		"pop	%rbp\n"
		"pop	%rbx\n"
		"pop	%rdx\n"
		"pop	%rcx\n"
		"pop	%rax\n"
		"iretq\n");
}

void sched_init(void)
{
	/* 5ms周期の周期タイマー設定 */
	ptimer_setup(SCHED_PERIOD, schedule);

	/* 予めTaskBのスタックを適切に積んでおき、スタックポインタを揃える */
	unsigned long long *sp =
		(unsigned long long *)((unsigned char *)taskB_stack
				       + TASK_B_STASK_BYTES);
	unsigned long long old_sp = (unsigned long long)sp;

	/* push SS */
	--sp;
	*sp = 0x10;

	/* push old RSP */
	--sp;
	*sp = old_sp;

	/* push RFLAGS */
	--sp;
	*sp = 0x202;

	/* push CS */
	--sp;
	*sp = 8;

	/* push RIP */
	--sp;
	*sp = (unsigned long long)do_taskB;

	/* push GR */
	unsigned char i;
	for (i = 0; i < 7; i++) {
		--sp;
		*sp = 0;
	}

	task_sp[1] = (unsigned long long)sp;
}

void sched_start(void)
{
	/* 周期タイマースタート */
	ptimer_start();
}
