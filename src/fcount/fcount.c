#include <stdio.h>
#include "apilib.h"

void pmain(void)
{
	char *buf, s[12];
	int win, timer;
	unsigned long int c=0;
	api_initmalloc();
	buf = api_malloc(150 * 50);
	win = api_openwin(buf, 150, 50, -1, "���ټ�����");
	timer = api_alloctimer();
	api_inittimer(timer, 128);
	for (;;) {
		sprintf(s, "%09ld",c);
		api_boxfilwin(win, 28, 27, 115, 41, 7 /* �� */);
		api_putstrwin(win, 28, 27, 0 /* �� */, 11, s);
		api_settimer(timer, 1);	/* 1�b�� */
		if (api_getkey(1) != 128) {
			break;
		}
		c++;
	}
	api_end();
}
