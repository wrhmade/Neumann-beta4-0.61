/* bootpack�̃��C�� */

#include "bootpack.h"
#include "bootchr.h"
#include <stdio.h>

#define KEYCMD_LED		0xed


//_169Mode�������(16:9)ģʽ 


int y,m,d, i, panic=0,hzkfound=0;
int leftchick,rightchick; 
struct SHEET *sht_back;
char panic_reason[50];
void keywin_off(struct SHEET *key_win);
void keywin_on(struct SHEET *key_win);
void close_console(struct SHEET *sht);
void close_constask(struct TASK *task);
unsigned char buf_mouse[24*24];
struct SHEET *sht_mouse;
struct TASK *task_a,*task_b;
struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

void task_b_process();
#define PROCESS_COLOR 1
#define PROCESS_BACKCOLOR BACKC
#define PROCESS_SUM 11
int tmpx=0,tmpy=0;
int mx, my; 
int process=0;
void init_taskb()
{
	task_b=task_alloc();
	fifo32_init(&task_b->fifo, 128, 0, task_b);
	task_b->cons_stack = memman_alloc_4k(memman, 64 * 1024); /* 64Kbջ�ڴ� */
    task_b->tss.esp = task_b->cons_stack + 64 * 1024 - 12; /* ����ջ��λ�� */
    task_b->tss.eip=(int) &task_b_process;
    task_b->tss.es = 1 * 8; /* �������ݶ�ѡ��� */
    task_b->tss.cs = 2 * 8; /* ��������ѡ��� */
    task_b->tss.ss = 1 * 8; /* �������ݶ�ѡ��� */
    task_b->tss.ds = 1 * 8; /* �������ݶ�ѡ��� */
    task_b->tss.fs = 1 * 8; /* �������ݶ�ѡ��� */
    task_b->tss.gs = 1 * 8; /* �������ݶ�ѡ��� */
	
}
void syspanic(char* reason)
{
	strcpy(panic_reason,reason);
	panic=1;
	return ;
}


void process_forward(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	int _process=binfo->scrnx/2-320/2+60+process;
	boxfill8(binfo->vram,binfo->scrnx,PROCESS_COLOR,_process,binfo->scrny/2-240/2+200,_process+(200/PROCESS_SUM),binfo->scrny/2-240/2+220);
	process+=200/PROCESS_SUM;
}

void colortest(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	/*boxfill8(binfo->vram,binfo->scrnx,0,0,0,20,500);
	boxfill8(binfo->vram,binfo->scrnx,1,20,0,40,500);
	boxfill8(binfo->vram,binfo->scrnx,2,40,0,60,500);
	boxfill8(binfo->vram,binfo->scrnx,3,60,0,80,500);
	boxfill8(binfo->vram,binfo->scrnx,4,80,0,100,500);
	boxfill8(binfo->vram,binfo->scrnx,5,100,0,120,500);
	boxfill8(binfo->vram,binfo->scrnx,6,120,0,140,500);
	boxfill8(binfo->vram,binfo->scrnx,7,140,0,160,500);
	boxfill8(binfo->vram,binfo->scrnx,8,160,0,180,500);
	boxfill8(binfo->vram,binfo->scrnx,9,180,0,200,500);
	boxfill8(binfo->vram,binfo->scrnx,10,200,0,220,500);
	boxfill8(binfo->vram,binfo->scrnx,11,220,0,240,500);
	boxfill8(binfo->vram,binfo->scrnx,12,240,0,260,500);
	boxfill8(binfo->vram,binfo->scrnx,13,260,0,280,500);
	boxfill8(binfo->vram,binfo->scrnx,14,280,0,300,500);
	boxfill8(binfo->vram,binfo->scrnx,15,300,0,320,500);*/
	int i,j,s=0;
	for(i=1;i<=16;i++)
	{
		for(j=1;j<=16;j++)
		{
			boxfill8(binfo->vram,binfo->scrnx,s,20*(j-1),20*(i-1),20*(j-1)+20,20*(i-1)+20);
			char a[2];
			sprintf(a,"%d",s);
			putfonts8_asc(binfo->vram, binfo->scrnx, 20*(j-1), 20*(i-1), 0, a);
			s++;
		}
	}
}
int tmpx2=0;
int correct;

void pmain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	if(_169Mode)binfo->scrny=binfo->scrnx/16*9;
	extern char hankaku[4096];
	init_palette();
	boxfill8(binfo->vram,binfo->scrnx,PROCESS_BACKCOLOR,0,0,binfo->scrnx,binfo->scrny);
	boxfill8(binfo->vram,binfo->scrnx,COL8_848484,binfo->scrnx/2-320/2-5,binfo->scrny/2-240/2-5,binfo->scrnx/2+320/2-5,binfo->scrny/2+240/2-5);
	boxfill8(binfo->vram,binfo->scrnx,COL8_FFFFFF,binfo->scrnx/2-320/2,binfo->scrny/2-240/2,binfo->scrnx/2+320/2,binfo->scrny/2+240/2);
	boxfill8(binfo->vram,binfo->scrnx,COL8_000000,binfo->scrnx/2-320/2+58,binfo->scrny/2-240/2+198,binfo->scrnx/2-320/2+262,binfo->scrny/2-240/2+222);
	boxfill8(binfo->vram,binfo->scrnx,COL8_C6C6C6,binfo->scrnx/2-320/2+60,binfo->scrny/2-240/2+200,binfo->scrnx/2-320/2+260,binfo->scrny/2-240/2+220);
	putfonts8_asc_lang(binfo->vram,binfo->scrnx,binfo->scrnx/2-16*8/2,binfo->scrny/2-240/2+180,0,"Starting Neumann",0);
	
	
	
	struct SHTCTL *shtctl;
	char s[40];
	char tmp[80];
	struct FIFO32 fifo, keycmd;
	int fifobuf[128], keycmd_buf[32];
	int new_mx = -1, new_my = 0, new_wx = 0x7fffffff, new_wy = 0;
	unsigned long int memtotal;
	struct MOUSE_DEC mdec;
	unsigned char *buf_back;
	
	
	
	struct TASK *task;
	/*static char keytable0[0x80] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@, '[', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
	};*/
	static char keytable0[0x80] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, 0,   0,   0x5c, 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
	};
	/*static char keytable1[0x80] = {
		0,   0,   '!', 0x22, '#', '$', '%', '&', 0x27 '(', ')', '~', '=', '~', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,   0,   '}', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
	};*/
	static char keytable1[0x80] = {
		0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 0x22, 0,   0,   '|', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
	};
	
	int key_shift = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1;
	int j, x, y, mmx = -1, mmy = -1, mmx2 = 0;
	struct SHEET *sht = 0, *key_win, *sht2;
	int *fat;
	unsigned char *nihongo;
	unsigned char *hzk;
	struct FILEINFO *finfo;
	int chick_move=0; 

	process_forward();


	
	
	
	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC�̏��������I������̂�CPU�̊��荞�݋֎~������ */

	process_forward();
	
	fifo32_init(&fifo, 128, fifobuf, 0);
	*((int *) 0x0fec) = (int) &fifo;

	process_forward();
	
	
	init_pit();
	
	process_forward();
	
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);

	process_forward();
	
	io_out8(PIC0_IMR, 0xf8); /* PIT��PIC1�ƃL�[�{�[�h������(11111000) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */

	process_forward();
	
	fifo32_init(&keycmd, 32, keycmd_buf, 0);

	
	process_forward();
	
	init_acpi(); 

	process_forward();

	memtotal = memtest(0x00400000, 0xbfffffff);
	sprintf(tmp,"memtotal=%dMB",memtotal/1024/1024);
	putfonts8_asc_lang(binfo->vram,binfo->scrnx,0,0,COL8_000000,tmp,0);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);
	
	process_forward();
	
	
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	task_a = task_init(memman);
	fifo.task = task_a;
	task_run(task_a, 1, 2);

	process_forward();

	*((int *) 0x0fe4) = (int) shtctl;
	task_a->langmode = 3;

	process_forward();

	correct=0;
	
	/* sht_back */
	sht_back  = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* �����F�Ȃ� */
	init_screen8(buf_back, binfo->scrnx, binfo->scrny);
	
	
	/* sht_cons */
	//key_win = open_console(shtctl, memtotal,"Console");  
	key_win = open_console(shtctl, memtotal,"Console");

	/* sht_mouse */
	sht_mouse = sheet_alloc(shtctl);
	sheet_setbuf(sht_mouse, buf_mouse, 24, 24, 99);
	init_mouse_cursor8(buf_mouse, 99,1);
	//mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	//my = (binfo->scrny - 28 - 16) / 2;
	mx=20;
	my=20;
	sheet_slide(sht_back,  0,  0);
	sheet_slide(key_win,   32, 4);
	sheet_slide(sht_mouse, mx, my);
	sheet_updown(sht_back,  0);
	sheet_updown(key_win,   1);
	sheet_updown(sht_mouse, 2);
	keywin_on(key_win);
	
	/* �ŏ��ɃL�[�{�[�h��ԂƂ̐H���Ⴂ���Ȃ��悤�ɁA�ݒ肵�Ă������Ƃɂ��� */
	fifo32_put(&keycmd, KEYCMD_LED);
	fifo32_put(&keycmd, key_leds);
	
	/* nihongo.fnt�̓ǂݍ��� */
	//nihongo = (unsigned char *) memman_alloc_4k(memman, 16 * 256 + 32 * 94 * 47);
	hzk = (unsigned char *) memman_alloc_4k(memman, 0x5d5d * 32);
	fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
	finfo = file_search("HZK16.hzk", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo != 0) {
		hzkfound=1;
		file_loadfile(finfo->clustno, finfo->size, hzk, fat, (char *) (ADR_DISKIMG + 0x003e00));
	} else {
		for (i = 0; i < 16 * 256; i++) {
			hzk[i] = hankaku[i]; /* �t�H���g���Ȃ������̂Ŕ��p�������R�s�[ */
		}
		for (i = 16 * 256; i < 16 * 256 + 32 * 94 * 47; i++) {
			hzk[i] = 0xff; /* �t�H���g���Ȃ������̂őS�p������0xff�Ŗ��ߐs���� */
		}
	}
		
	
	*((int *) 0x0fe8) = (int) hzk;
	memman_free_4k(memman, (int) fat, 4 * 2880);
	
	init_taskb();
	task_run(task_b, 2, 2);
	
	if(hzkfound==0)putfonts8_asc_sht(sht_back,binfo->scrnx-480,16,1,BACKC,"[Warning]HZK16.HZK not found, Chinese will not be displayed.",60);
	init_desktop(buf_back);
	sheet_refresh(sht_back,0,0,binfo->scrnx,binfo->scrny); 
	
	for (;;) {
		if(panic==1)
		{
			boxfill8(binfo->vram,binfo->scrnx,4,0,0,binfo->scrnx,binfo->scrny);
			putfonts8_asc(binfo->vram,binfo->scrnx,0,0,7,"===System Panic===");
			putfonts8_asc(binfo->vram,binfo->scrnx,0,16,7,"The system crashed due to an issue.");
			putfonts8_asc(binfo->vram,binfo->scrnx,0,32,7,"You can try restarting your computer to fix this problem.");
			putfonts8_asc(binfo->vram,binfo->scrnx,0,48,7,"Reason:");
			putfonts8_asc(binfo->vram,binfo->scrnx,56,48,7,panic_reason);
			putfonts8_asc(binfo->vram,binfo->scrnx,0,64,7,"*** Press F2 to reboot");
			for(;;)
			{
				i = fifo32_get(&fifo);
				io_sti();
				if(i == 256 + 0x3C)
				{
					io_out8(0x64,0xfe); 
				}
			}
		}
		
		
		//putfonts8_asc(binfo->vram, binfo->scrnx, 615, 523, COL8_000000, "Neumann System");
		putfonts8_asc_sht(sht_back, binfo->scrnx-8*15, binfo->scrny-77, 0, BACKC,"Neumann����ϵͳ",15);
		//putfonts8_asc(binfo->vram, binfo->scrnx, 655, 539, COL8_000000, "Version 0.1 [Beta]");
		putfonts8_asc_sht(sht_back, binfo->scrnx-8*19, binfo->scrny-61, 0, BACKC,"�汾��0.61 [Beta 4]",19);

		//putfonts8_asc(binfo->vram, binfo->scrnx, 385, 555, COL8_000000, "WARNING:THIS IS A BETA VERSION THAT MAY BE UNSTABLE.");
		putfonts8_asc_sht(sht_back, binfo->scrnx-8*28, binfo->scrny-45, 0, BACKC,"ע��:���ǲ��԰�,���ܻ᲻�ȶ�",28);
		
		
		sprintf(s, "Mouse Data:(%3d, %3d)", mx, my);
		putfonts8_asc_sht(sht_back, 0, binfo->scrny-16 , 0, BACKC, s, 21);
		
		if((key_leds & 4) != 0)
		{
			putfonts8_asc_sht(sht_back,binfo->scrnx-16*5,0,0,BACKC,"��д������",10);
		}
		else
		{
			putfonts8_asc_sht(sht_back,binfo->scrnx-16*5,0,0,BACKC,"          ",10);
		}
		
		
		if (fifo32_status(&keycmd) > 0 && keycmd_wait < 0) {
			/* �L�[�{�[�h�R���g���[���ɑ���f�[�^������΁A���� */
			keycmd_wait = fifo32_get(&keycmd);
			wait_KBC_sendready();
			io_out8(PORT_KEYDAT, keycmd_wait);
		}
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			/* FIFO��������ۂɂȂ����̂ŁA�ۗ����Ă���`�悪����Ύ��s���� */
			if (new_mx >= 0) {
				io_sti();
				sheet_slide(sht_mouse, new_mx, new_my);
				new_mx = -1;
			} else if (new_wx != 0x7fffffff) {
				io_sti();
				sheet_slide(sht, new_wx, new_wy);
				new_wx = 0x7fffffff;
			} else {
				task_sleep(task_a);
				io_sti();
			}
		} else {
			
			i = fifo32_get(&fifo);
			io_sti();
			if (key_win != 0 && key_win->flags == 0) {	/* �E�B���h�E������ꂽ */
				if (shtctl->top == 1) {	/* �����}�E�X�Ɣw�i�����Ȃ� */
					key_win = 0;
				} else {
					key_win = shtctl->sheets[shtctl->top - 1];
					keywin_on(key_win);
				}
			}
			if (256 <= i && i <= 511) { /* �L�[�{�[�h�f�[�^ */
				if (i < 0x80 + 256) { /* �L�[�R�[�h�𕶎��R�[�h�ɕϊ� */
					if (key_shift == 0) {
						s[0] = keytable0[i - 256];
					} else {
						s[0] = keytable1[i - 256];
					}
				} else {
					s[0] = 0;
				}
				if ('A' <= s[0] && s[0] <= 'Z') {	/* ���͕������A���t�@�x�b�g */
					if (((key_leds & 4) == 0 && key_shift == 0) ||
							((key_leds & 4) != 0 && key_shift != 0)) {
						s[0] += 0x20;	/* �啶�����������ɕϊ� */
					}
				}
				if (s[0] != 0 && key_win != 0) { /* �ʏ핶���A�o�b�N�X�y�[�X�AEnter */
					fifo32_put(&key_win->task->fifo, s[0] + 256);
				}
				if (i == 256 + 0x0f && key_win != 0) {	/* Tab */
					keywin_off(key_win);
					j = key_win->height - 1;
					if (j == 0) {
						j = shtctl->top - 1;
					}
					key_win = shtctl->sheets[j];
					keywin_on(key_win);
				}
				if (i == 256 + 0x2a) {	/* ���V�t�g ON */
					key_shift |= 1;
				}
				if (i == 256 + 0x36) {	/* �E�V�t�g ON */
					key_shift |= 2;
				}
				if (i == 256 + 0xaa) {	/* ���V�t�g OFF */
					key_shift &= ~1;
				}
				if (i == 256 + 0xb6) {	/* �E�V�t�g OFF */
					key_shift &= ~2;
				}
				if (i == 256 + 0x3a) {	/* CapsLock */
					key_leds ^= 4;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x45) {	/* NumLock */
					key_leds ^= 2;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x46) {	/* ScrollLock */
					key_leds ^= 1;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x3b && key_shift != 0 && key_win != 0) {	/* Shift+F1 */
					task = key_win->task;
					if (task != 0 && task->tss.ss0 != 0) {
						if(task->langmode==3)
						{
							cons_putstr0(task->cons, "\n����ر�(Shift+F1) :\n");	
						}
						else
						{
							cons_putstr0(task->cons, "\nBreak(Shift+F1):\n");
						}
						io_cli();	/* �����I���������Ƀ^�X�N���ς��ƍ��邩�� */
						task->tss.eax = (int) &(task->tss.esp0);
						task->tss.eip = (int) asm_end_app;
						io_sti();
						task_run(task, -1, 0);	/* �I���������m���ɂ�点�邽�߂ɁA�Q�Ă�����N���� */
						beep(0);
					}
				}
				if (i == 256 + 0x3c && key_shift != 0) {	/* Shift+F2 */
					/* �V����������R���\�[������͑I����Ԃɂ���i���̂ق����e�؂���ˁH�j */
					if (key_win != 0) {
						keywin_off(key_win);
					}
					key_win = open_console(shtctl, memtotal,"����̨");
					sheet_slide(key_win, 32, 4);
					sheet_updown(key_win, shtctl->top);
					keywin_on(key_win);
				}
				if (i == 256 + 0x5B) {	/* WinKey */
					
				}
				if (i == 256 + 0x57) {	/* F11 */
					sheet_updown(shtctl->sheets[1], shtctl->top - 1);
				}
				if (i == 256 + 0xfa) {	/* �L�[�{�[�h���f�[�^�𖳎��Ɏ󂯎���� */
					keycmd_wait = -1;
				}
				if (i == 256 + 0xfe) {	/* �L�[�{�[�h���f�[�^�𖳎��Ɏ󂯎��Ȃ����� */
					wait_KBC_sendready();
					io_out8(PORT_KEYDAT, keycmd_wait);
				}
			} else if (512 <= i && i <= 767) { /* �}�E�X�f�[�^ */
				if (mouse_decode(&mdec, i - 512) != 0) {
					/* �}�E�X�J�[�\���̈ړ� */
					if(mdec.x !=0 || mdec.y!=0)
					{
						chick_move=1;
					}
					else
					{
						chick_move=0;
					}
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 1) {
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1) {
						my = binfo->scrny - 1;
					}
					new_mx = mx;
					new_my = my;
					
					leftchick=((mdec.btn & 0x01) != 0);
					rightchick=((mdec.btn & 0x02) != 0);
					if ((mdec.btn & 0x01) != 0) {
						/* ���{�^���������Ă��� */
						if((mx >= 20 && mx <=52) && (my >= 20 && my <=52))
						{
							if ((!(0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) || shtctl->top - 1==0) && chick_move==0) 
							{
								while(!((mdec.btn & 0x01) != 0)){};
								if (key_win != 0) {
									keywin_off(key_win);
								}
								key_win = open_console(shtctl, memtotal,"����̨");
								sheet_slide(key_win, 32, 4);
								sheet_updown(key_win, shtctl->top);
								keywin_on(key_win);
							}
						}
						if((mx >= 20 && mx <=52) && (my >= 84 && my <=116))
						{
							if ((!(0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) || shtctl->top - 1==0) && chick_move==0)
							{
								while(!((mdec.btn & 0x01) != 0)){};
								/*if (key_win != 0) {
									keywin_off(key_win);
								}
								key_win = open_console(shtctl, memtotal,"invader");
								sheet_slide(key_win, 32, 4);
								sheet_updown(key_win, shtctl->top);
								keywin_on(key_win);
							
								task = key_win->task;
								cons_runcmd("ncst invader.prg",task->cons,fat,memtotal);*/
								/*task=open_constask(0, memtotal);
								struct FIFO32 *fifo2=&task->fifo;
								char programname[20]="invader.prg";
								for(c=0;c<11;c++)
								{
									fifo32_put(fifo2, programname[c] + 256);
								}
								fifo32_put(fifo2, 10 + 256);*/
								execute_program("invader.prg",memtotal,buf_mouse); 
								//init_mouse_cursor8(buf_mouse,99,2);
								
							}
						}
						if((mx >= 20 && mx <=52) && (my >= 148 && my <=180))
						{
							if ((!(0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) || shtctl->top - 1==0) && chick_move==0)
							{
								while(!((mdec.btn & 0x01) != 0)){};
								/*if (key_win != 0) {
									keywin_off(key_win);
								}
								key_win = open_console(shtctl, memtotal,"invader");
								sheet_slide(key_win, 32, 4);
								sheet_updown(key_win, shtctl->top);
								keywin_on(key_win);
							
								task = key_win->task;
								cons_runcmd("ncst invader.prg",task->cons,fat,memtotal);*/
								/*task=open_constask(0, memtotal);
								struct FIFO32 *fifo2=&task->fifo;
								char programname[20]="fcount.prg";
								for(c=0;c<11;c++)
								{
									fifo32_put(fifo2, programname[c] + 256);
								}
								fifo32_put(fifo2, 10 + 256);*/
								init_mouse_cursor8(buf_mouse, 99,2);
								execute_program("fcount.prg",memtotal,buf_mouse); 
								
							}
						}
						if (mmx < 0) {
							
							/* �ʏ탂�[�h�̏ꍇ */
							/* ��̉��������珇�ԂɃ}�E�X���w���Ă��鉺������T�� */
							for (j = shtctl->top - 1; j > 0; j--) {
								sht = shtctl->sheets[j];
								x = mx - sht->vx0;
								y = my - sht->vy0;
								if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) {
									if (sht->buf[y * sht->bxsize + x] != sht->col_inv) {
										sheet_updown(sht, shtctl->top - 1);
										if (sht != key_win) {
											keywin_off(key_win);
											key_win = sht;
											keywin_on(key_win);
										}
										if (3 <= x && x < sht->bxsize - 3 && 3 <= y && y < 21) {
											mmx = mx;	/* �E�B���h�E�ړ����[�h�� */
											mmy = my;
											mmx2 = sht->vx0;
											new_wy = sht->vy0;
										}
										//if (sht->bxsize - 21 <= x && x < sht->bxsize - 5 && 5 <= y && y < 19) {
										if (5 <= x && x <= 19 && 5 <= y && y < 19) {
											/* �u�~�v�{�^���N���b�N */
											if ((sht->flags & 0x10) != 0) {		/* �A�v����������E�B���h�E���H */
												task = sht->task;
												if(task->langmode==3)
												{
													cons_putstr0(task->cons, "\n����ر�(�رհ�ť):\n");	
												}
												else
												{
													cons_putstr0(task->cons, "\nBreak(Close Button) :\n");
												}
												io_cli();	/* �����I���������Ƀ^�X�N���ς��ƍ��邩�� */
												task->tss.eax = (int) &(task->tss.esp0);
												task->tss.eip = (int) asm_end_app;
												io_sti();
												task_run(task, -1, 0);
												beep(0); 
											} else {	/* �R���\�[�� */
												task = sht->task;
												sheet_updown(sht, -1); /* �Ƃ肠������\���ɂ��Ă��� */
												keywin_off(key_win);
												key_win = shtctl->sheets[shtctl->top - 1];
												keywin_on(key_win);
												io_cli();
												fifo32_put(&task->fifo, 4);
												io_sti();
											}
										}
										break;
									}
								}
								else
								{
								} 
							}
						} else {
							chick_move=1;
							/* �E�B���h�E�ړ����[�h�̏ꍇ */
							x = mx - mmx;	/* �}�E�X�̈ړ��ʂ��v�Z */
							y = my - mmy;
							new_wx = (mmx2 + x + 2) & ~3;
							new_wy = new_wy + y;
							mmy = my;	/* �ړ���̍��W�ɍX�V */
						}
					} else {
						/* ���{�^���������Ă��Ȃ� */
						mmx = -1;	/* �ʏ탂�[�h�� */
						if (new_wx != 0x7fffffff) {
							sheet_slide(sht, new_wx, new_wy);	/* ��x�m�肳���� */
							new_wx = 0x7fffffff;
						}
					}
				}
			} else if (768 <= i && i <= 1023) {	/* �R���\�[���I������ */
				close_console(shtctl->sheets0 + (i - 768));
			} else if (1024 <= i && i <= 2023) {
				close_constask(taskctl->tasks0 + (i - 1024));
			} else if (2024 <= i && i <= 2279) {	/* �R���\�[����������� */
				sht2 = shtctl->sheets0 + (i - 2024);
				memman_free_4k(memman, (int) sht2->buf, 256 * 165);
				sheet_free(sht2);
			}
		}
		
	}
}

void keywin_off(struct SHEET *key_win)
{
	change_wtitle8(key_win, 0);
	if ((key_win->flags & 0x20) != 0) {
		fifo32_put(&key_win->task->fifo, 3); /* �R���\�[���̃J�[�\��OFF */
	}
	return;
}

void keywin_on(struct SHEET *key_win)
{
	change_wtitle8(key_win, 1);
	if ((key_win->flags & 0x20) != 0) {
		fifo32_put(&key_win->task->fifo, 2); /* �R���\�[���̃J�[�\��ON */
	}
	return;
}

struct TASK *open_constask(struct SHEET *sht, unsigned long int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task = task_alloc();
	int *cons_fifo = (int *) memman_alloc_4k(memman, 128 * 4);
	task->cons_stack = memman_alloc_4k(memman, 64 * 1024);
	task->tss.esp = task->cons_stack + 64 * 1024 - 12;
	task->tss.eip = (int) &console_task;
	task->tss.es = 1 * 8;
	task->tss.cs = 2 * 8;
	task->tss.ss = 1 * 8;
	task->tss.ds = 1 * 8;
	task->tss.fs = 1 * 8;
	task->tss.gs = 1 * 8;
	*((int *) (task->tss.esp + 4)) = (int) sht;
	*((int *) (task->tss.esp + 8)) = memtotal;
	task_run(task, 2, 2); /* level=2, priority=2 */
	fifo32_init(&task->fifo, 128, cons_fifo, task);
	return task;
}

struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal,char* title)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct SHEET *sht = sheet_alloc(shtctl);
	unsigned char *buf = (unsigned char *) memman_alloc_4k(memman, 600 * 450);
	sheet_setbuf(sht, buf, 600, 450, -1); /* �����F�Ȃ� */
	make_window8(buf, 600, 450, title, 0);
	make_textbox8(sht, 8, 28, 600-8-8, 450-37, COL8_000000);
	sht->task = open_constask(sht, memtotal);
	sht->flags |= 0x20;	/* �J�[�\������ */
	return sht;
}

void close_constask(struct TASK *task)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	task_sleep(task);
	memman_free_4k(memman, task->cons_stack, 64 * 1024);
	memman_free_4k(memman, (int) task->fifo.buf, 128 * 4);
	task->flags = 0; /* task_free(task); �̑��� */
	return;
}

void close_console(struct SHEET *sht)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task = sht->task;
	memman_free_4k(memman, (int) sht->buf, 600 * 450);
	sheet_free(sht);
	close_constask(task);
	return;
}

void update_cmostime(struct CONSOLE *cons,int _correct)
{
	correct=_correct;
	char s[10];
	if(_correct==0)
	{
		sprintf(s,"CMOS+0h\n"); 
	}
	else if(_correct>0)
	{
		sprintf(s,"CMOS+%dh\n",_correct);
	}
	else if(_correct<0)
	{
		sprintf(s,"CMOS-%dh\n",-correct);
	}
	cons_putstr0(cons,s);
}

void update_mouse_status(int status)
{
	init_mouse_cursor8(buf_mouse,99,status);
	sheet_slide(sht_mouse, mx, my);
}

int getmousex(void)
{
	return mx;
}

int getmousey(void)
{
	return my;
}

int getmousechick(int key)
{
	if(key==1)
	{
		return leftchick;
	}
	else if(key==2)
	{
		return rightchick;
	}
	return 0;
}

void task_b_process()
{
	for(;;)
	{
		update_date_time(sht_back,correct);
	}
} 

