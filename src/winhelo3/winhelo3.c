#include "apilib.h"

void pmain(void)
{
	char *buf;
	int win;

	api_initmalloc();
	buf = api_malloc(150 * 50);
	win = api_openwin(buf, 150, 50, -1, "hello");
	api_boxfilwin(win,  8, 36, 141, 43, 6 /* ���F */);
	api_putstrwin(win, 28, 28, 0 /* �� */, 12, "hello, world");
	for (;;) {
		if (api_getkey(1) == 0x0a) {
			break; /* Enter�Ȃ�break; */
		}
	}
	api_end();
}
