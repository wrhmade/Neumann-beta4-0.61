#include "apilib.h"

void pmain(void)
{
	char inputstr[40];
	int i;
	for(;;)
	{
		i=api_getkey(1);
		if(i>=0x20 && i<=0xff)
		{
			if(api_getconscurx()<480)
			{
				api_putchar(i);
			}
		}
		else if(i==0x08+256)
		{
			if(api_getconscurx()>0)
			{
				api_consbackspace();	
			}
		}
		else if(i==0x0A)
		{
			break;
		}
	}
	api_end();
}
