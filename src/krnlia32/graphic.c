/* 僌儔僼傿僢僋張棟娭學 */

#include "bootpack.h"

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:崟 */
		0xff, 0x00, 0x00,	/*  1:柧傞偄愒 */
		0x00, 0xff, 0x00,	/*  2:柧傞偄椢 */
		0xff, 0xff, 0x00,	/*  3:柧傞偄墿怓 */
		0x00, 0x00, 0xff,	/*  4:柧傞偄惵 */
		0xff, 0x00, 0xff,	/*  5:柧傞偄巼 */
		0x00, 0xff, 0xff,	/*  6:柧傞偄悈怓 */
		0xff, 0xff, 0xff,	/*  7:敀 */
		0xc6, 0xc6, 0xc6,	/*  8:柧傞偄奃怓 */
		0x84, 0x00, 0x00,	/*  9:埫偄愒 */
		0x00, 0x84, 0x00,	/* 10:埫偄椢 */
		0x84, 0x84, 0x00,	/* 11:埫偄墿怓 */
		0x00, 0x00, 0x84,	/* 12:埫偄惵 */
		0x84, 0x00, 0x84,	/* 13:埫偄巼 */
		0x00, 0x84, 0x84,	/* 14:埫偄悈怓 */
		0x84, 0x84, 0x84	/* 15:埫偄奃怓 */
	};
	unsigned char table2[216 * 3];
	int r, g, b;
	set_palette(0, 15, table_rgb);
	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
				table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
				table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
			}
		}
	}
	set_palette(16, 231, table2);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 妱傝崬傒嫋壜僼儔僌偺抣傪婰榐偡傞 */
	io_cli(); 					/* 嫋壜僼儔僌傪0偵偟偰妱傝崬傒嬛巭偵偡傞 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 妱傝崬傒嫋壜僼儔僌傪尦偵栠偡 */
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}



void init_screen8(char *vram, int x, int y)
{
	int *fat;
	unsigned char c;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;	

	boxfill8(vram, x, BACKC, 0, 0, x - 1, y);

	if(WALLPAPER)
	{
		fat = (int *) memman_alloc_4k(memman, 4 * 2880);
		file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
		read_picture(fat, vram, x, y);
		memman_free_4k(memman, (int) fat, 4 * 2880);
	}
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfont32(char *vram, int xsize, int x, int y, char c, char *font1, char *font2)
{
	char New1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char New2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int i=0,j=0;
	while(i<8)
	{
		New1[i]=font1[j];
		i++;
		j+=2;
	}
	j=0;
	while(i<16)
	{
		New1[i]=font2[j];
		i++;
		j+=2;
	}
	i=0;j=1;
	while(i<8)
	{
		New2[i]=font1[j];
		i++;
		j+=2;
	}
	j=1;
	while(i<16)
	{
		New2[i]=font2[j];
		i++;
		j+=2;
	}
	putfont8(vram,xsize,x,y,c,New1);
	putfont8(vram,xsize,x+8,y,c,New2);
}

void putfonts8_asc_lang(char *vram, int xsize, int x, int y, char c, unsigned char *s,int lmode)
{
	extern char hankaku[4096];
	struct TASK *task = task_now();
	char *hzk = (char *) *((int *) 0x0fe8), *font;
	char *nihongo = (char *) *((int *) 0x0fe8);
	int k, t;

	if (lmode == 0) {
		for (; *s != 0x00; s++) {
			putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
			x += 8;
		}
	}
	if (lmode == 1) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if ((0x81 <= *s && *s <= 0x9f) || (0xe0 <= *s && *s <= 0xfc)) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				if (0x81 <= task->langbyte1 && task->langbyte1 <= 0x9f) {
					k = (task->langbyte1 - 0x81) * 2;
				} else {
					k = (task->langbyte1 - 0xe0) * 2 + 62;
				}
				if (0x40 <= *s && *s <= 0x7e) {
					t = *s - 0x40;
				} else if (0x80 <= *s && *s <= 0x9e) {
					t = *s - 0x80 + 63;
				} else {
					t = *s - 0x9f;
					k++;
				}
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );	/* 嵍敿暘 */
				putfont8(vram, xsize, x    , y, c, font + 16);	/* 塃敿暘 */
			}
			x += 8;
		}
	}
	if (lmode == 2) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if (0x81 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );	/* 嵍敿暘 */
				putfont8(vram, xsize, x    , y, c, font + 16);	/* 塃敿暘 */
			}
			x += 8;
		}
	}
	if (lmode == 3) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if (0xa1 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, hankaku + *s * 16);//只要是半角就使用hankaku里面的字符
				}
			} else {
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = hzk + (k * 94 + t) * 32;
				
				putfont32(vram,xsize,x-8,y,c,font,font+16);
			}
			x += 8;
		}
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	struct TASK *task=task_now();
	putfonts8_asc_lang(vram,xsize,x,y,c,s,task->langmode);
	return;
}


/*void init_mouse_cursor8(char *mouse, char bc,int mouse_status)
{
	static char cursor_arrow[16][16] = {
		"OO..............",
		"O*O.............",
		"O**O............",
		"O***O...........",
		"O****O..........",
		"O*****O.........",
		"O******O........",
		"O*******O.......",
		"O********O......",
		"O*****OOOOO.....",
		"O**O**O.........",
		"O*O.O**O........",
		"OO..O**O........",
		"O....O**O.......",
		".....O**O.......",
		"......OO........"
	};
	static char cursor_wait[16][16] = {
		"****************",
		".*OOOOOOOOOOOO*.",
		".**OOOOOOOOOO**.",
		".***OO****OO***.",
		".**.*OO**OO*.**.",
		".**..*OOOO*..**.",
		".**...*OO*...**.",
		".**....**....**.",
		".**....**....**.",
		".**...*OO*...**.",
		".**..*OOOO*..**.",
		".**.*OO**OO*.**.",
		".***OO****OO***.",
		".**OO******OO**.",
		".*OOOOOOOOOOOO*.",
		"****************"
	};
	int x, y;
	static char cursor[16][16];
	for(x=0;x<16;x++)
	{
		for(y=0;y<16;y++)
		{
			if(mouse_status==1)
			{
				cursor[x][y]=cursor_arrow[x][y];
			}
			else if(mouse_status==2)
			{
				cursor[x][y]=cursor_wait[x][y];
			}
		}
	}
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}*/

void init_mouse_cursor8(char *mouse, char bc,int mouse_status)
{
	static char cursor_arrow[24][24] = {
		"O.......................", 
		"OO......................", 
		"O*O.....................", 
		"O**O....................", 
		"O***O...................", 
		"O****O..................", 
		"O*****O.................", 
		"O******O................", 
		"O*******O...............", 
		"O********O..............", 
		"O*********O.............", 
		"O******OOOOO............", 
		"O***O**O................", 
		"O**OO**O................", 
		"O*O..O**O...............", 
		"OO...O**O...............", 
		"O.....O**O..............", 
		"......O**O..............", 
		".......O**O.............", 
		".......O**O.............", 
		"........OO..............", 
		"........................", 
		"........................", 
		"........................"
	};
	static char cursor_wait[24][24] = {
		"......OOOOOOOOOOOOO.....",
		"......OO*********OO.....",
		"......OOOOOOOOOOOOO.....",
		".......O*********O......",
		".......O*********O......",
		".......O**O*O*O**O......",
		".......O***O*O***O......",
		".......OO***O***OO......",
		"........OO*****OO.......",
		".........OO*O*OO........",
		"..........OO*OO.........",
		"..........OO*OO.........",
		"..........OO*OO.........",
		".........OO***OO........",
		"........OO**O**OO.......",
		".......OO*******OO......",
		".......O****O****O......",
		".......O***O*O***O......",
		".......O**O*O*O**O......",
		".......O*O*O*O*O*O......",
		"......OOOOOOOOOOOOO.....",
		"......OO*********OO.....",
		"......OOOOOOOOOOOOO.....",
		"........................"
	};

	int x, y;
	static char cursor[24][24];
	for(x=0;x<24;x++)
	{
		for(y=0;y<24;y++)
		{
			if(mouse_status==1)
			{
				cursor[x][y]=cursor_arrow[x][y];
			}
			else if(mouse_status==2)
			{
				cursor[x][y]=cursor_wait[x][y];
			}
		}
	}
	for (y = 0; y < 24; y++) {
		for (x = 0; x < 24; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 24 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 24 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 24 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

int read_picture(int *fat, char *vram, int x, int y)
{
	int i, j, x0, y0, fsize, info[4];
	unsigned char *filebuf, r, g, b;
	struct RGB *picbuf;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FILEINFO *finfo;
	struct DLL_STRPICENV *env;
	finfo = file_search("desktop1.jpg", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0) {
		return -1;
	}
	fsize = finfo->size;
	filebuf = (unsigned char *) memman_alloc_4k(memman, fsize);
	filebuf = file_loadfile2(finfo->clustno, &fsize, fat);
	env = (struct DLL_STRPICENV *) memman_alloc_4k(memman, sizeof(struct DLL_STRPICENV));
	info_JPEG(env, info, fsize, filebuf);
	picbuf = (struct RGB *) memman_alloc_4k(memman, info[2] * info[3] * sizeof(struct RGB));
	decode0_JPEG(env, fsize, filebuf, 4, (unsigned char *) picbuf, 0);
	x0 = (int) ((x - info[2]) / 2);
	y0 = (int) ((y - info[3]) / 2);
	for (i = 0; i < info[3]; i++) {
		for (j = 0; j < info[2]; j++) {
			r = picbuf[i * info[2] + j].r;
			g = picbuf[i * info[2] + j].g;
			b = picbuf[i * info[2] + j].b;
			vram[(y0 + i) * x + (x0 + j)] = rgb2pal(r, g, b, j, i, binfo->vmode);
		}
	}
	memman_free_4k(memman, (int) filebuf, fsize);
	memman_free_4k(memman, (int) picbuf , info[2] * info[3] * sizeof(struct RGB));
	memman_free_4k(memman, (int) env , sizeof(struct DLL_STRPICENV));
	return 0;
}
unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb)
{
	if (cb == 8) {
		static int table[4] = { 3, 1, 0, 2 };
		int i;
		x &= 1; /* 是偶数还是奇数呢？ */
		y &= 1;
		i = table[x + y * 2];
		r = (r * 21) / 256;
		g = (g * 21) / 256;
		b = (b * 21) / 256;
		r = (r + i) / 4;
		g = (g + i) / 4;
		b = (b + i) / 4;
		return((unsigned short) (16 + r + g * 6 + b * 36));
	} else {
		return((unsigned short) (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | (b >> 3)));
	}
}
