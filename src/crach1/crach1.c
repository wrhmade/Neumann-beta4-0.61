#include "apilib.h"
#include <stdio.h>
char s[50];
int i=0;
void f1()
{
	i++;
	sprintf(s,"%d\n",i);
	api_putstr0(s);
	f1();
}
void pmain(void)
{
	f1();
}
