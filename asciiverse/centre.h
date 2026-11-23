/* Centres Ambassador of Pain in the terminal without touching aop.c.
 *
 * build.sh compiles aop.c with -include asciiverse/centre.h. This header
 * pulls in curses.h first, then sends the game's only two drawing calls,
 * mvaddch() for the 80x24 field and mvprintw() for the lives and points
 * line, through the two functions below, which add an offset that puts the
 * field in the middle of the terminal. When aop.c includes curses.h itself,
 * the header's include guard makes that a no-op, so the 64 lines compile
 * exactly as written.
 *
 * The game redraws the whole field every frame, so the offset is worked out
 * on every call and follows a resize; when it changes, the screen is erased
 * once so the old frame does not linger.
 */
#ifndef AOP_CENTRE_H
#define AOP_CENTRE_H

#include <curses.h>
#include <stdarg.h>

/* the field array has 25 rows, but a level is 24 lines and the 25th is
 * always blank: centre what shows */
#define AOP_FIELD_ROWS 24
#define AOP_FIELD_COLS 80

static int aop_oy = -1, aop_ox = -1;

static void aop_origin(void) {
	int oy, ox;
#if defined(PDCURSES)
	/* PDCurses only takes the console's new size when asked */
	if (is_termresized()) resize_term(0, 0);
#endif
	oy = LINES > AOP_FIELD_ROWS ? (LINES - AOP_FIELD_ROWS) / 2 : 0;
	ox = COLS > AOP_FIELD_COLS ? (COLS - AOP_FIELD_COLS) / 2 : 0;
	if (oy != aop_oy || ox != aop_ox) {
		erase();
		aop_oy = oy;
		aop_ox = ox;
	}
}

static int aop_mvaddch(int y, int x, chtype ch) {
	aop_origin();
	return mvaddch(aop_oy + y, aop_ox + x, ch);
}

static int aop_mvprintw(int y, int x, const char *fmt, ...) {
	va_list ap;
	int r;
	aop_origin();
	if (move(aop_oy + y, aop_ox + x) == ERR) return ERR;
	va_start(ap, fmt);
	r = vw_printw(stdscr, fmt, ap);
	va_end(ap);
	return r;
}

#undef mvaddch
#undef mvprintw
#define mvaddch(y, x, ch) aop_mvaddch((y), (x), (chtype)(unsigned char)(ch))
#define mvprintw aop_mvprintw

#endif
