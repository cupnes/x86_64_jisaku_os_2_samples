#include <cover.h>
#include <fb.h>
#include <font.h>
#include <fbcon.h>
#include <iv.h>

#define UNIT	10
#define FC_MAX	255
/* #define TOP_MARGIN	10 */
#define TOP_MARGIN	0
#define BOTTOM_MARGIN	25

unsigned char fc_r = 0, fc_g = 0, fc_b = 0;

void dump_titles(void)
{
	/* 座標計算用変数 */
	unsigned int screen_center_x = fb.hr / 2;
	unsigned int str_width_half;
	unsigned int start_x, start_y;

	/* 文字色設定 */
	set_fg(fc_r, fc_g, fc_b);

	/* フルスクラッチで作る!x86_64自作OS パート2(26文字) */
	set_font_size(FONT_SIZE_13x18);
	str_width_half = (FONT_13x18_WIDTH * 26) / 2;
	start_x = screen_center_x - str_width_half;
	start_y = TOP_MARGIN;
	move_cursor(start_x, start_y);
	char series_title[] = {
		FONT_kata_fu, FONT_kata_ru, FONT_kata_su, FONT_kata_ku,
		FONT_kata_ra, FONT_kata_xtu, FONT_kata_chi, FONT_hira_de,
		FONT_x_saku, FONT_hira_ru, '!',
		'x', '8', '6', '_', '6', '4',
		FONT_x_ji, FONT_x_saku, 'O', 'S', ' ',
		FONT_kata_pa, '-', FONT_kata_to, '2', '\0'};
	puts(series_title);

	/* ＡＣＰＩで
	 * ＨＰＥＴ取
	 * 得してスケ
	 * ジューラを
	 * 作る本 */
	set_font_size(FONT_SIZE_68x73);
	str_width_half = (FONT_68x73_WIDTH * 5) / 2;
	start_x = screen_center_x - str_width_half;
	/* start_y += FONT_13x18_HEIGHT * 2; */
	start_y += FONT_13x18_HEIGHT;
	move_cursor(start_x, start_y);
	set_fg(0xff, 0, 0);
	char title_1[] = {'A', 'C', 'P', 'I', '\0'};
	puts(title_1);
	set_fg(0, 0, 0);
	putc(FONT_hira_de);

	start_y += FONT_68x73_HEIGHT;
	move_cursor(start_x, start_y);
	set_fg(0xff, 0, 0);
	char title_2[] = {'H', 'P', 'E', 'T', '\0'};
	puts(title_2);
	set_fg(0, 0, 0);
	putc(FONT_x_shu);

	start_y += FONT_68x73_HEIGHT;
	move_cursor(start_x, start_y);
	char title_3[] = {
		FONT_x_toku, FONT_hira_shi, FONT_hira_te, '\0'};
	puts(title_3);
	set_fg(0xff, 0, 0);
	putc(FONT_kata_su);
	putc(FONT_kata_ke);

	start_y += FONT_68x73_HEIGHT;
	move_cursor(start_x, start_y);
	char title_4[] = {
		FONT_kata_ji, FONT_kata_xyu, '-', FONT_kata_ra, '\0'};
	puts(title_4);
	set_fg(0, 0, 0);
	putc(FONT_hira_wo);

	start_y += FONT_68x73_HEIGHT;
	move_cursor(start_x, start_y);
	char title_5[] = {FONT_x_saku, FONT_hira_ru, FONT_x_hon, '\0'};
	puts(title_5);

	/* 大神　祐真　著(7文字) */
	set_font_size(FONT_SIZE_DEFAULT);
	str_width_half = (FONT_WIDTH * 7) / 2;
	start_x = screen_center_x - str_width_half;
	start_y += FONT_68x73_HEIGHT;
	move_cursor(start_x, start_y);
	char author[] = {
		FONT_x_dai, FONT_x_kami, ' ', FONT_x_yu, FONT_x_shin,
		' ', FONT_x_cho, '\0'};
	puts(author);

	/* 2018-08-10版(11文字) */
	str_width_half = (FONT_WIDTH * 11) / 2;
	start_x = screen_center_x - str_width_half;
	/* start_y += FONT_HEIGHT * 2; */
	start_y += FONT_HEIGHT;
	move_cursor(start_x, start_y);
	char pub[] = {
		'2', '0', '1', '8', '-', '0', '8', '-', '1', '0', FONT_x_han,
		'\0'};
	puts(pub);

	/* へにゃぺんて 発行(9文字) */
	str_width_half = (FONT_WIDTH * 9) / 2;
	start_x = screen_center_x - str_width_half;
	/* start_y += FONT_HEIGHT * 2; */
	start_y += FONT_HEIGHT;
	move_cursor(start_x, start_y);
	char circle[] = {
		FONT_hira_he, FONT_hira_ni, FONT_hira_xya, FONT_hira_pe,
		FONT_hira_n, FONT_hira_te, ' ', FONT_x_hatsu, FONT_x_yuki, '\0'};
	puts(circle);
}

void cover_main(void)
{
	/* 背景画像表示 */
	iv_init();
	view(0);		/* ファイルシステム1番目のファイルを使う */

	/* 以降、独自フォントを使った文字の描画 */

	dump_titles();
}
