#include "apilib.h"

void pmain(void)
{
	int i;
	for(i=0x00;i<=0xFF;i++)
	{
		api_putchar(i);
	}
	api_putchar(0x0A);
	api_end();
}
