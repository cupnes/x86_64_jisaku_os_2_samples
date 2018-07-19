#include <acpi.h>
#include <fbcon.h>

#define US_TO_FS	1000000000

struct __attribute__((packed)) HPET_TABLE {
	struct SDTH header;
	unsigned int event_timer_block_id;
	struct ACPI_ADDRESS base_address;
	unsigned char hpet_number;
	unsigned short minimum_tick;
	unsigned char flags;
};

unsigned long long reg_base;

/* General Capabilities and ID Register */
#define GCIDR_ADDR	(reg_base)
#define GCIDR	(*(volatile unsigned long long *)GCIDR_ADDR)
union gcidr {
	unsigned long long raw;
	struct __attribute__((packed)) {
		unsigned long long rev_id:8;
		unsigned long long num_tim_cap:5;
		unsigned long long count_size_cap:1;
		unsigned long long _reserved:1;
		unsigned long long leg_rt_cap:1;
		unsigned long long vendor_id:16;
		unsigned long long counter_clk_period:32;
	};
};

/* General Configuration Register */
#define GCR_ADDR	(reg_base + 0x10)
#define GCR	(*(volatile unsigned long long *)GCR_ADDR)
union gcr {
	unsigned long long raw;
	struct __attribute__((packed)) {
		unsigned long long enable_cnf:1;
		unsigned long long leg_rt_cnf:1;
		unsigned long long _reserved:62;
	};
};

/* Main Counter Register */
#define MCR_ADDR	(reg_base + 0xf0)
#define MCR	(*(volatile unsigned long long *)MCR_ADDR)

void hpet_init(void)
{
	/* HPET tableを取得 */
	struct HPET_TABLE *hpet_table = (struct HPET_TABLE *)get_sdt("HPET");

	/* レジスタの先頭アドレスを取得 */
	reg_base = hpet_table->base_address.address;

	/* 使うまでHPETは止めておく */
	union gcr gcr;
	gcr.raw = GCR;
	gcr.enable_cnf = 0;
	GCR = gcr.raw;
}

void dump_gcidr(void)
{
	puts("GCIDR\r\n");

	union gcidr r;
	r.raw = GCIDR;

	puts("REV ID             ");
	putd(r.rev_id, 3);
	puts("\r\n");

	puts("NUM TIM CAP        ");
	putd(r.num_tim_cap, 2);
	puts("\r\n");

	puts("COUNT SIZE CAP     ");
	putd(r.count_size_cap, 1);
	puts("\r\n");

	puts("LEG RT CAP         ");
	putd(r.leg_rt_cap, 1);
	puts("\r\n");

	puts("COUNTER CLK PERIOD ");
	putd(r.counter_clk_period, 10);
	puts("\r\n");
}

void dump_gcr(void)
{
	puts("GCR\r\n");

	union gcr r;
	r.raw = GCR;

	puts("ENABLE CNF ");
	putd(r.enable_cnf, 1);
	puts("\r\n");

	puts("LEG RT CNF ");
	putd(r.leg_rt_cnf, 1);
	puts("\r\n");
}

void dump_mcr(void)
{
	puts("MCR ");
	puth(MCR, 16);
	puts("\r\n");
}

void sleep(unsigned long long us)
{
	/* 現在のmain counterのカウント値を取得 */
	unsigned long long mc_now = MCR;

	/* usマイクロ秒後のmain counterのカウント値を算出 */
	unsigned long long fs = us * US_TO_FS;
	union gcidr gcidr;
	gcidr.raw = GCIDR;
	unsigned long long mc_duration = fs / gcidr.counter_clk_period;
	unsigned long long mc_after = mc_now + mc_duration;

	/* HPETが無効であれば有効化する */
	union gcr gcr;
	gcr.raw = GCR;
	unsigned char to_disable = 0;
	if (!gcr.enable_cnf) {
		gcr.enable_cnf = 1;
		GCR = gcr.raw;

		/* sleep()を抜ける際に元に戻す(disableする) */
		to_disable = 1;
	}

	/* usマイクロ秒の経過を待つ */
	while (MCR < mc_after);

	/* 元々無効であった場合は無効に戻しておく */
	if (to_disable) {
		gcr.raw = GCR;
		gcr.enable_cnf = 0;
		GCR = gcr.raw;
	}
}
