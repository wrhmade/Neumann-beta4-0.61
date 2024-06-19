#include "apilib.h"
#include <stdio.h>

void pmain(void)
{
	char cmdline[60];
	api_cmdline(cmdline, 60);
	int i=0;
	while(cmdline[i]!=' ')
	{
		i++;
	}
	i++;
	while(cmdline[i]!='\0')
	{
		while(!(cmdline[i]==' ' || cmdline[i]=='\0')) 
		{
			api_putchar(cmdline[i]);
			i++;
		}
		i++;
		api_putchar(0x0A);
	} 
	api_end();
}
