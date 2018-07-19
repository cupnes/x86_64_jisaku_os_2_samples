#ifndef _FBCON_H_
#define _FBCON_H_

void putc(char c);
void puts(char *s);
void putd(unsigned long long val, unsigned char num_digits);
void puth(unsigned long long val, unsigned char num_digits);
void move_cursor(unsigned int x, unsigned int y);
void set_font_size(unsigned char _font_size);

#endif
