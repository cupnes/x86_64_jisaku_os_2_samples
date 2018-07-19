#include <fbcon.h>
#include <fb.h>
#include <font.h>

/* 64bit unsignedの最大値0xffffffffffffffffは
 * 10進で18446744073709551615(20桁)なので'\0'含め21文字分のバッファで足りる */
#define MAX_STR_BUF	21

unsigned int cursor_x = 0, cursor_y = 0;
unsigned char font_size = FONT_SIZE_DEFAULT;

static void draw_char_default(unsigned int c)
{
	unsigned int x, y;

	/* カーソル座標(cursor_x,cursor_y)へ文字を描画 */
	for (y = 0; y < FONT_HEIGHT; y++)
		for (x = 0; x < FONT_WIDTH; x++)
			if (font_bitmap[c][y][x])
				draw_px_fg(cursor_x + x, cursor_y + y);

	/* カーソル座標の更新 */
	cursor_x += FONT_WIDTH;
	if ((cursor_x + FONT_WIDTH) >= fb.hr) {
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
		if ((cursor_y + FONT_HEIGHT) >= fb.vr) {
			cursor_x = cursor_y = 0;
			clear_screen();
		}
	}
}

static void draw_char_13x18(unsigned int c)
{
	unsigned int x, y;

	/* カーソル座標(cursor_x,cursor_y)へ文字を描画 */
	for (y = 0; y < FONT_13x18_HEIGHT; y++)
		for (x = 0; x < FONT_13x18_WIDTH; x++)
			if (font_bitmap_13x18[c][y][x])
				draw_px_fg(cursor_x + x, cursor_y + y);

	/* カーソル座標の更新 */
	cursor_x += FONT_13x18_WIDTH;
	if ((cursor_x + FONT_13x18_WIDTH) >= fb.hr) {
		cursor_x = 0;
		cursor_y += FONT_13x18_HEIGHT;
		if ((cursor_y + FONT_13x18_HEIGHT) >= fb.vr) {
			cursor_x = cursor_y = 0;
			clear_screen();
		}
	}
}

static void draw_char_68x73(unsigned int _c)
{
	unsigned int x, y;

	unsigned int c = font_idx_to_68x73[_c];

	/* カーソル座標(cursor_x,cursor_y)へ文字を描画 */
	for (y = 0; y < FONT_68x73_HEIGHT; y++)
		for (x = 0; x < FONT_68x73_WIDTH; x++)
			if (font_bitmap_68x73[c][y][x])
				draw_px_fg(cursor_x + x, cursor_y + y);

	/* カーソル座標の更新 */
	cursor_x += FONT_68x73_WIDTH;
	if ((cursor_x + FONT_68x73_WIDTH) >= fb.hr) {
		cursor_x = 0;
		cursor_y += FONT_68x73_HEIGHT;
		if ((cursor_y + FONT_68x73_HEIGHT) >= fb.vr) {
			cursor_x = cursor_y = 0;
			clear_screen();
		}
	}
}

void putc(char _c)
{
	unsigned int font_height;

	unsigned int c = (unsigned char)_c;
	switch(c) {
	case '\r':
		cursor_x = 0;
		break;

	case '\n':
		switch (font_size) {
		case FONT_SIZE_13x18:
			font_height = FONT_13x18_HEIGHT;
			break;
		case FONT_SIZE_68x73:
			font_height = FONT_68x73_HEIGHT;
			break;
		default:
			font_height = FONT_HEIGHT;
		}

		cursor_y += font_height;
		if ((cursor_y + font_height) >= fb.vr) {
			cursor_x = cursor_y = 0;
			clear_screen();
		}
		break;

	default:
		switch (font_size) {
		case FONT_SIZE_13x18:
			draw_char_13x18(c);
			break;
		case FONT_SIZE_68x73:
			draw_char_68x73(c);
			break;
		default:
			draw_char_default(c);
		}
	}
}

void puts(char *s)
{
	while (*s != '\0')
		putc(*s++);
}

void putd(unsigned long long val, unsigned char num_digits)
{
	char str[MAX_STR_BUF];

	int i;
	for (i = num_digits - 1; i >= 0; i--) {
		int digit = val % 10;
		str[i] = '0' + digit;
		val /= 10;
	}
	str[num_digits] = '\0';

	puts(str);
}

void puth(unsigned long long val, unsigned char num_digits)
{
	char str[MAX_STR_BUF];

	int i;
	for (i = num_digits - 1; i >= 0; i--) {
		unsigned char v = (unsigned char)(val & 0x0f);
		if (v < 0xa)
			str[i] = '0' + v;
		else
			str[i] = 'A' + (v - 0xa);
		val >>= 4;
	}
	str[num_digits] = '\0';

	puts(str);
}

void move_cursor(unsigned int x, unsigned int y)
{
	cursor_x = x;
	cursor_y = y;
}

void set_font_size(unsigned char _font_size)
{
	font_size = _font_size;
}
