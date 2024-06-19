#include "apilib.h"
#include <stdio.h>
void pmain(void)
{
	char s[40];
	sprintf(s,"%d",5/0);
	api_putstr0(s);
	api_end();
}
