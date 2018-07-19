#ifndef _HPET_H_
#define _HPET_H_

#define MS_TO_US	1000
#define SEC_TO_US	1000000

void hpet_init(void);
void dump_gcidr(void);
void dump_gcr(void);
void dump_mcr(void);
void sleep(unsigned long long us);

#endif
