#include <x86.h>
#include <intr.h>
#include <pic.h>
#include <acpi.h>
#include <fb.h>
#include <kbc.h>
#include <fbcon.h>
#include <fs.h>
#include <hpet.h>
#include <sched.h>
#include <common.h>

struct __attribute__((packed)) platform_info {
	struct framebuffer fb;
	void *rsdp;
};

void do_taskA(void);

void start_kernel(void *_t __attribute__((unused)), struct platform_info *pi,
		  void *_fs_start)
{
	/* フレームバッファ周りの初期化 */
	fb_init(&pi->fb);
	set_fg(255, 255, 255);
	set_bg(0, 70, 250);
	clear_screen();

	/* ACPIの初期化 */
	acpi_init(pi->rsdp);

	/* CPU周りの初期化 */
	gdt_init();
	intr_init();

	/* 周辺ICの初期化 */
	pic_init();
	hpet_init();
	kbc_init();

	/* ファイルシステムの初期化 */
	fs_init(_fs_start);

	/* スケジューラの初期化 */
	sched_init();

	/* CPUの割り込み有効化 */
	enable_cpu_intr();

	/* スケジューラの開始 */
	sched_start();

	/* タスクAの開始 */
	do_taskA();

	/* haltして待つ */
	while (1)
		cpu_halt();
}

void do_taskA(void)
{
	while (1) {
		putc('A');
		volatile unsigned long long wait = 10000000;
		while (wait--);
	}
}
