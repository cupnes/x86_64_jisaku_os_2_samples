#ifndef _X86_H_
#define _X86_H_

#define SS_KERNEL_CODE	0x0008
#define SS_KERNEL_DATA	0x0010

#define MAX_INTR_NO	256

struct __attribute__((packed)) interrupt_descriptor {
	unsigned short offset_00_15;
	unsigned short segment_selector;
	unsigned short ist	: 3;
	unsigned short _zero1	: 5;
	unsigned short type	: 4;
	unsigned short _zero2	: 1;
	unsigned short dpl	: 2;
	unsigned short p	: 1;
	unsigned short offset_31_16;
	unsigned int   offset_63_32;
	unsigned int   _reserved;
};

void enable_cpu_intr(void);
void cpu_halt(void);
unsigned char io_read(unsigned short addr);
void io_write(unsigned short addr, unsigned char value);
void gdt_init(void);

#endif
