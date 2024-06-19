#include "apilib.h"

void pmain()
{
	int fh;
	char filebuf[1024*1024*1024];;
	fh=api_fopen("font.ttf");
	if(fh!=0)
	{
		end("font.ttf not found.\n");
	}
	else
	{
		api_fread(filebuf,1024*1024*1024,fh);
	}
	api_end();
}

void end(char* mess)
{
	api_putstr0(mess);
	api_end();
}
