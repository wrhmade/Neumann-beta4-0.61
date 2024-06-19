#include "apilib.h"

void pmain(void)
{
	int langmode=api_getlang();
	for(;;)
	{
		if(langmode==3)
		{
			api_putstr0("ĞÂÄê¿ìÀÖ");
		}
		else
		{
			api_putstr0("Happy New Year");
		}
	}
	api_end();
}
