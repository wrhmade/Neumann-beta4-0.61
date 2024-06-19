#include "bootpack.h"

void winkey_menu(void)
{

}
void init_desktop(char* vram)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	static char console_icon_chr[32][32]={
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
		"@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$@",
		"@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$@",
		"@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
		"@..............................@",
		"@.00.000.000.000.000.00........@",
		"@.0..000.000.000.000..0........@",
		"@.0..000.000.000.000..0........@",
		"@.0..000.000.000.000..0........@",
		"@.00.000.000.000.000.00.000....@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@..............................@",
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	};
	
	char console_icon[32][32];
	int x,y;
	for(x=0;x<32;x++)
	{
		for(y=0;y<32;y++)
		{
			if(console_icon_chr[y][x]=='@')
			{
				console_icon[y][x]=COL8_848484;
			}
			if(console_icon_chr[y][x]=='$')
			{
				console_icon[y][x]=COL8_C6C6C6;
			}
			else if(console_icon_chr[y][x]=='.')
			{
				console_icon[y][x]=COL8_000000;
			}
			else if(console_icon_chr[y][x]=='0')
			{
				console_icon[y][x]=COL8_FFFFFF;
			}
			else if(console_icon_chr[y][x]=='N')
			{
				console_icon[y][x]=BACKC;
			}
		}
	}
	/*char* vram=binfo->vram;
	for(x=0;x<32;x++)
	{
		for(y=0;y<32;y++)
		{
			vram[(y+20)* binfo->scrnx + (x+20)]=console_icon[y][x];
		}
	}*/
	putfonts8_asc(vram, binfo->scrnx, (20-(48/2-32/2)) ,52, COL8_FFFFFF,"����̨");
	putblock8_8(vram,binfo->scrnx,32,32,20,20,console_icon,32);
	

	static char invader_icon_chr[32][32]={
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"............@@@@@@@@............",
		".........@@@@@@@@@@@@@@.........", 
		".@....@@@@@@@@@@@@@@@@@@@@....@.",
		".@.@@@@@@@..@@@@@@@@..@@@@@@@.@.",
		".@.@@@@@@@..@@@@@@@@..@@@@@@@.@.",
		".@.@@@@@@@..@@@@@@@@..@@@@@@@.@.",
		".@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.",
		"...@@@@@@@@@@@@@@@@@@@@@@@@@@...",
		"...@@@@@@@@@@@@@@@@@@@@@@@@@@...",
		"...@@@@@@@@@@@@@@@@@@@@@@@@@@...",
		"...@@@@@@@@@@@@@@@@@@@@@@@@@@...",
		"........@@............@@........",
		"..@.....@@............@@.....@..",
		"..@@@@@@@@............@@@@@@@@..",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"WWW.W...W.W.W...W...WW..WWW.WW..",
		".W..WW..W.W.W..W.W..W.W.W...W.W.",
		".W..W.W.W.W.W..W.W..W.W.WWW.WW..",
		".W..W..WW.W.W.WWWWW.W.W.W...W.W.",
		"WWW.W...W..W..W...W.WW..WWW.W.W.",
		"................................"
	};
	
	char invader_icon[32][32];
	for(x=0;x<32;x++)
	{
		for(y=0;y<32;y++)
		{
			if(invader_icon_chr[y][x]=='@')
			{
				invader_icon[y][x]=2;
			}
			else if(invader_icon_chr[y][x]=='W')
			{
				invader_icon[y][x]=COL8_FFFFFF;
			}
			else if(invader_icon_chr[y][x]=='.')
			{
				invader_icon[y][x]=BACKC;
			}
		}
	}
	putfonts8_asc(vram, binfo->scrnx, (20-(56/2-32/2)) ,20+32+32+32, COL8_FFFFFF,"Invader");
	putblock8_8(vram,binfo->scrnx,32,32,20,20+32+32,invader_icon,32);
	
	static char counter_icon_chr[32][32]={
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		"................................",
		".$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$.",
		".$$$&&&&&&&&&&&&&&&&&&&&&&&*$$$&",
		".$$$&***********************$$$&",
		".$$$&**$$$**$$$**$$$**$$$***$$$&",
		".$$$&**$*$**$*$**$*$**$*$***$$$&",
		".$$$&**$*$**$*$**$*$**$*$***$$$&",
		".$$$&**$*$**$*$**$*$**$*$***$$$&",
		".$$$&**$$$**$$$**$$$**$$$***$$$&",
		".$$$&***********************$$$&",
		".$$$************************$$$&",
		".$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$&",
		"..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
		"................................",
		".........$$$..$...$$.$$$........",
		".........$...$.$.$....$.........",
		".........$$$.$.$..$...$.........",
		".........$...$$$...$..$.........",
		".........$...$.$.$$...$.........",
		"................................",
		"................................",
		"................................"
	};
	
	char counter_icon[32][32];
	for(x=0;x<32;x++)
	{
		for(y=0;y<32;y++)
		{
			if(counter_icon_chr[y][x]=='*')
			{
				counter_icon[y][x]=COL8_FFFFFF;
			}
			else if(counter_icon_chr[y][x]=='$')
			{
				counter_icon[y][x]=COL8_000000;
			}
			else if(counter_icon_chr[y][x]=='&')
			{
				counter_icon[y][x]=COL8_848484;
			}
			else if(counter_icon_chr[y][x]=='.')
			{
				counter_icon[y][x]=BACKC;
			}
		}
	}
	putfonts8_asc(vram, binfo->scrnx, (20-(64/2-32/2)) ,20+32+32+32+32+32, COL8_FFFFFF,"10ms����");
	putblock8_8(vram,binfo->scrnx,32,32,20,20+32+32+32+32,counter_icon,32);
} 
