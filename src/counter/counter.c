#include <stdio.h>
#include "apilib.h"

void pmain(void)
{
	char *buf, s[12];
	int win, timer;
	long int c=0;
	api_initmalloc();
	buf = api_malloc(150 * 50);
	win = api_openwin(buf, 150, 50, -1, "¼ÆÊıÆ÷");
	for (;;) {
		sprintf(s, "%09ld",c);
		api_boxfilwin(win, 28, 27, 115, 41, 7 /* ”’ */);
		api_putstrwin(win, 28, 27, 0 /* • */, 11, s);
		if (api_getkey(1) == 0x20) {
			c++;
		}
	}
	api_end();
}
