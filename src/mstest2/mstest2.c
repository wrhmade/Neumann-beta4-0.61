#include "apilib.h"
#include <stdio.h>

void pmain(void)
{
	char s[40];
	sprintf(s,"Left:%d Right:%d\n",api_mouseleftchick(),api_mouserightchick());
	api_putstr0(s);
	api_end();
}
