#include <acpi.h>
#include <fbcon.h>

struct __attribute__((packed)) RSDP {
	char Signature[8];
	unsigned char Checksum;
	char OEMID[6];
	unsigned char Revision;
	unsigned int RsdtAddress;
	unsigned int Length;
	unsigned long long XsdtAddress;
	unsigned char Extended_Checksum;
	unsigned char Reserved[3];
};

struct __attribute__((packed)) XSDT {
	struct SDTH Header;
	struct SDTH *Entry[0];
};

struct XSDT *xsdt;

void acpi_init(void *rsdp)
{
	xsdt = (struct XSDT *)((struct RSDP *)rsdp)->XsdtAddress;
}

void dump_sdth_sig(struct SDTH *h)
{
	unsigned char i;
	for (i = 0; i < 4; i++)
		putc(h->Signature[i]);
}

void dump_xsdt(void)
{
	dump_sdth_sig(&xsdt->Header);
	puts("\r\n");

	unsigned long long num_sdts =
		(xsdt->Header.Length - sizeof(struct SDTH))
		/ sizeof(struct SDTH *);
	puts("NUM SDTS ");
	putd(num_sdts, 2);
	puts("\r\n");

	unsigned long long i;
	for (i = 0; i < num_sdts; i++) {
		dump_sdth_sig(xsdt->Entry[i]);
		putc(' ');
	}
	puts("\r\n");
}
