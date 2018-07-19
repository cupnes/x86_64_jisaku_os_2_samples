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

unsigned long long xsdt;

void acpi_init(void *rsdp)
{
	xsdt = ((struct RSDP *)rsdp)->XsdtAddress;
}
