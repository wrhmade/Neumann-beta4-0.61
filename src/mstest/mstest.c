#include "apilib.h"

void pmain(void)
{
	char cmdline[40];
	api_cmdline(cmdline,40);
	int i=0;
	for(;cmdline[i]!=' ';i++)
	{
		
	}
	i++;
	api_putstr0(cmdline[i]-48);
	api_updatemouse(cmdline[i]-48);
	api_end();
}
