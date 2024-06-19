/* 僐儞僜乕儖娭學 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

unsigned int cursor_speed=20; 


void console_task(struct SHEET *sheet, unsigned long int memtotal)
{
	
	struct TASK *task = task_now();
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	int i,*fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	struct CONSOLE cons;
	struct FILEHANDLE fhandle[8];
	char cmdline[60];
	//char lastcmdline[60];
	unsigned char *nihongo = (char *) *((int *) 0x0fe8);

	char s[20];

	cons.sht = sheet;
	cons.cur_x =  8;
	cons.cur_y = 28;
	cons.cur_c = -1;
	cons.flag  = 0;
	cons.scroll  = 1;
	task->cons = &cons;
	task->cmdline = cmdline;

	if (cons.sht != 0) {
		cons.timer = timer_alloc();
		timer_init(cons.timer, &task->fifo, 1);
		timer_settime(cons.timer, cursor_speed);
	}
	file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
	for (i = 0; i < 8; i++) {
		fhandle[i].buf = 0;	/* 枹巊梡儅乕僋 */
	}
	task->fhandle = fhandle;
	task->fat = fat;
	if (nihongo[4096] != 0xff) {	/* 擔杮岅僼僅儞僩僼傽僀儖傪撉傒崬傔偨偐丠 */
		task->langmode = 1;
	} else {
		task->langmode = 0;
	}
	task->langmode = 3;//表示每次都选择汉字
	task->langbyte1 = 0;
	
	cons_putstr0(&cons,"Neumann Console\n\tCopyright(c) 2023-2024 W24 Stduio\n\n");
	/* 僾儘儞僾僩昞帵 */
	//cons_putchar(&cons, '#', 1);
	cons_putchar(&cons, '[' ,1);
	cons_putchar(&cons, 'C' ,1);
	cons_putchar(&cons, 'o' ,1);
	cons_putchar(&cons, 'm' ,1);
	cons_putchar(&cons, 'm' ,1);
	cons_putchar(&cons, 'a' ,1);
	cons_putchar(&cons, 'n' ,1);
	cons_putchar(&cons, 'd' ,1);
	cons_putchar(&cons, ']' ,1);
	int old_flag=0;
	static char history[100][60];
	int historyindex=0;
	int times=0;
	for (;;) {
		if(old_flag!=cons.flag)
		{
			old_flag=cons.flag;
			cmd_cls(&cons);	
			cons.cur_x =  8;
			cons.cur_y = 28;
			cons.scroll=   1;
		}
		io_cli();
		if (fifo32_status(&task->fifo) == 0) {
			task_sleep(task);
			io_sti();
		} else {
			if(cons.flag==0)
			{
				i = fifo32_get(&task->fifo);
				io_sti();
				if (i <= 1 && cons.sht != 0) { /* 僇乕僜儖梡僞僀儅 */
					if (i != 0) {
						timer_init(cons.timer, &task->fifo, 0); /* 師偼0傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_FFFFFF;
						}
					} else {
						timer_init(cons.timer, &task->fifo, 1); /* 師偼1傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_000000;
						}
					}
					timer_settime(cons.timer, cursor_speed);
				}
				if (i == 2) {	/* 僇乕僜儖ON */
					cons.cur_c = COL8_FFFFFF;
				}
				if (i == 3) {	/* 僇乕僜儖OFF */
					if (cons.sht != 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
						cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
					}
					cons.cur_c = -1;
				}
				if (i == 4) {	/* 僐儞僜乕儖偺乽亊乿儃僞儞僋儕僢僋 */
					cmd_exit(&cons, fat);
				}
				if (256 <= i && i <= 511) { /* 僉乕儃乕僪僨乕僞乮僞僗僋A宱桼乯 */
								
					if (i == 8 + 256) {
						/* 僶僢僋僗儁乕僗 */
						if (cons.cur_x > 16+8*8 ){
							historyindex=times;
							/* 僇乕僜儖傪僗儁乕僗偱徚偟偰偐傜丄僇乕僜儖傪1偮栠偡 */
							cons_putchar(&cons, ' ', 0);
							cons.cur_x -= 8;
							boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
							cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
						}
					} else if (i == 10 + 256) {
						/* Enter */
						/* 僇乕僜儖傪僗儁乕僗偱徚偟偰偐傜夵峴偡傞 */
						cons_putchar(&cons, ' ', 0);
						cmdline[cons.cur_x / 8 - 2-8] = 0;
						cons_newline(&cons);
						if(strcmp(cmdline,"")!=0)
						{
							historyindex++;
							strcpy(history[times++],cmdline); 
						}
						cons_runcmd(cmdline, &cons, fat, memtotal);	/* 僐儅儞僪幚峴 */
						if (cons.sht == 0) {
							cmd_exit(&cons, fat);
						}
						/* 僾儘儞僾僩昞帵 */
							cons_putchar(&cons, '[' ,1);
							cons_putchar(&cons, 'C' ,1);
							cons_putchar(&cons, 'o' ,1);
							cons_putchar(&cons, 'm' ,1);
							cons_putchar(&cons, 'm' ,1);
							cons_putchar(&cons, 'a' ,1);
							cons_putchar(&cons, 'n' ,1);
							cons_putchar(&cons, 'd' ,1);
							cons_putchar(&cons, ']' ,1);
					} else 	{
						/* 堦斒暥帤 */
						historyindex=times;
						if (cons.cur_x < 480) {
							/* 堦暥帤昞帵偟偰偐傜丄僇乕僜儖傪1偮恑傔傞 */
							cmdline[cons.cur_x / 8 - 2-8]= i - 256;
							cons_putchar(&cons, i - 256, 1);
						}
					}
				} 
				if (0){
						if(historyindex>0)
						{
							historyindex--;
							cons.cur_x=80;
							boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
								cons.cur_x, cons.cur_y, cons.cur_x + string_len(cmdline)*8-1, cons.cur_y + 15);
							cons_putstr0(&cons,history[historyindex]);
							strcpy(cmdline,history[historyindex]);
						}
					}
				if (cons.sht != 0) {
				if (cons.cur_c >= 0) {
					boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
						cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
				}
				sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
				/* 僇乕僜儖嵞昞帵 */
				if (cons.sht != 0) {
					if (cons.cur_c >= 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
							cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
					}
						sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
					}
				}
			}
			else if(cons.flag==1)
			{
				i = fifo32_get(&task->fifo);
				io_sti();
				if (i <= 1 && cons.sht != 0) { /* 僇乕僜儖梡僞僀儅 */
					if (i != 0) {
						timer_init(cons.timer, &task->fifo, 0); /* 師偼0傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_FFFFFF;
						}
					} else {
						timer_init(cons.timer, &task->fifo, 1); /* 師偼1傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_000000;
						}
					}
					timer_settime(cons.timer, cursor_speed);
				}
				if (i == 2) {	/* 僇乕僜儖ON */
					cons.cur_c = COL8_FFFFFF;
				}
				if (i == 3) {	/* 僇乕僜儖OFF */
					if (cons.sht != 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
						cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
					}
					cons.cur_c = -1;
				}
				if (i == 4) {	/* 僐儞僜乕儖偺乽亊乿儃僞儞僋儕僢僋 */
					cmd_exit(&cons, fat);
				}
				if (256 <= i && i <= 511) { /* 僉乕儃乕僪僨乕僞乮僞僗僋A宱桼乯 */
					if (i == 8 + 256) {
						/* 僶僢僋僗儁乕僗 */
						if (cons.cur_x > 8 ){
							/* 僇乕僜儖傪僗儁乕僗偱徚偟偰偐傜丄僇乕僜儖傪1偮栠偡 */
							cons_putchar(&cons, ' ', 0);
							cons.cur_x -= 8;
							boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
							cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
						}
						else
						{
							if(cons.cur_y>28)
							{
								cons_putchar(&cons, ' ', 0);
								cons.cur_y-=16;
								//cons.cur_x=480;
							}
						}
					} else if (i == 10 + 256) {
						cons_putchar(&cons, ' ', 0);
						cons_newline(&cons);
					} else 	{
						/* 堦斒暥帤 */
							/* 堦暥帤昞帵偟偰偐傜丄僇乕僜儖傪1偮恑傔傞 */
							cons_putchar(&cons, i - 256, 1);
					}
				}
				if (cons.sht != 0) {
					if (cons.cur_c >= 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
							cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
					}
					sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
					/* 僇乕僜儖嵞昞帵 */
					if (cons.sht != 0) {
						if (cons.cur_c >= 0) {
							boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
								cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
						}
						sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
					}
				}
			}
			else if(cons.flag==2)
			{
				i = fifo32_get(&task->fifo);
				io_sti();
				if (i <= 1 && cons.sht != 0) { /* 僇乕僜儖梡僞僀儅 */
					if (i != 0) {
						timer_init(cons.timer, &task->fifo, 0); /* 師偼0傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_FFFFFF;
						}
					} else {
						timer_init(cons.timer, &task->fifo, 1); /* 師偼1傪 */
						if (cons.cur_c >= 0) {
							cons.cur_c = COL8_000000;
						}
					}
					timer_settime(cons.timer, cursor_speed);
				}
				if (i == 2) {	/* 僇乕僜儖ON */
					cons.cur_c = COL8_FFFFFF;
				}
				if (i == 3) {	/* 僇乕僜儖OFF */
					if (cons.sht != 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000,
						cons.cur_x, cons.cur_y+14, cons.cur_x + 7, cons.cur_y + 15);
					}
					cons.cur_c = -1;
				}
				if (i == 4) {	/* 僐儞僜乕儖偺乽亊乿儃僞儞僋儕僢僋 */
					cmd_exit(&cons, fat);
				}
				if (256 <= i && i <= 511) { /* 僉乕儃乕僪僨乕僞乮僞僗僋A宱桼乯 */
				}
				if (cons.sht != 0) {
					if (cons.cur_c >= 0) {
						boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
							cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
					}
					sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
					/* 僇乕僜儖嵞昞帵 */
					if (cons.sht != 0) {
						if (cons.cur_c >= 0) {
							boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, 
								cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y + 15);
						}
						sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y + 16);
					}
				}
			}
			/* 僇乕僜儖嵞昞帵 */
			
		}
	}
}

 

void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
	char s[2];
	struct TASK *task=task_now();
	int i;
	s[0] = chr;
	s[1] = 0;
	if (s[0] == 0x09) {	/* 僞僽 */
		for (;;) {
			if (cons->sht != 0) {
				putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF, COL8_000000, " ", 1);
			}
			cons->cur_x += 8;
			if (cons->cur_x == 8 + 600-8-8) {
				cons_newline(cons);
			}
			if (((cons->cur_x - 8) & 0x1f) == 0) {
				break;	/* 32偱妱傝愗傟偨傜break */
			}
		}
	} else if (s[0] == 0x0a) {	/* 夵峴 */
		cons_newline(cons);
	} else if (s[0] == 0x07) { /* 响铃 */
		
	} else if (s[0] == 0x0d) {	/* 暅婣 */
		/* 偲傝偁偊偢側偵傕偟側偄 */
	} else {	/* 晛捠偺暥帤 */
		if (cons->sht != 0) {
			putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF, COL8_000000, s, 1);
		}
		if (move != 0) {
			/* move偑0偺偲偒偼僇乕僜儖傪恑傔側偄 */
			cons->cur_x += 8;
			if (cons->cur_x == 8 + 600-8-8) {
				cons_newline(cons);
			}
		}
	}
	return;
}

void cons_newline(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	struct TASK *task = task_now();
	if (cons->cur_y < 28 + 450 - 37 - 16 - 16) {
		cons->cur_y += 16; /* 師偺峴傊 */
	} else {
		/* 僗僋儘乕儖 */
		if (sheet != 0) {
			if(cons->scroll)
			{
				for (y = 28; y < 28 + 450-37; y++) {
					for (x = 8; x < 8 + 600-8-8; x++) {
						sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
					}
				}
				for (y = 28 + 450-37-16; y < 28 + 450-37; y++) {
					for (x = 8; x < 8 + 600-8-8; x++) {
						sheet->buf[x + y * sheet->bxsize] = COL8_000000;
					}
				}
				sheet_refresh(sheet, 8, 28, 8 + 600-8-8, 28 + 450-37);
			}
			else
			{
				cons->cur_y=28;
			}
		}
	}
	cons->cur_x = 8;
	if (task->langmode == 3 && task->langbyte1 != 0) {
		cons->cur_x = 16;
	}
}

void cons_putstr0(struct CONSOLE *cons, char *s)
{
	for (; *s != 0; s++) {
		cons_putchar(cons, *s, 1);
	}
	return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l)
{
	int i;
	for (i = 0; i < l; i++) {
		cons_putchar(cons, s[i], 1);
	}
	return;
}

void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned long int memtotal)
{
	struct TASK *task;
	task=task_now();
	//cons_putstr0(cons,cmdline);
	if (strcmp(cmdline, "mem") == 0 && cons->sht != 0) {
		cmd_mem(cons, memtotal);
	} else if (strcmp(cmdline, "cls") == 0 && cons->sht != 0) {
		cmd_cls(cons);
	} else if (strcmp(cmdline, "dir") == 0 && cons->sht != 0) {
		cmd_dir(cons);
	} else if (strcmp(cmdline, "exit") == 0) {
		cmd_exit(cons, fat);
	} else if (strcmp(cmdline, "scroll") == 0) {
		cmd_scroll(cons);
	} else if (strncmp(cmdline, "start ", 6) == 0) {
		cmd_start(cons, cmdline, memtotal);
	} else if (strncmp(cmdline, "ncst ", 5) == 0) {
		cmd_ncst(cons, cmdline, memtotal);
	} else if (strncmp(cmdline, "langmode ", 9) == 0) {
		cmd_langmode(cons, cmdline);
	} else if (strcmp(cmdline, "shutdown") ==0 ){
		if(task->langmode==3)
		{
			cons_putstr0(cons,"正在关机......\n");	
		}
		else
		{
			cons_putstr0(cons,"Shutting Down...\n");	
		}
		acpi_shutdown(); 
		if(task->langmode==3)
		{
			cons_putstr0(cons,"无法关机，请检查你的电源是否为ATX电源(AT电源不支持关机)，或者开启了ACPI\n");
		} 
		else
		{
			cons_putstr0(cons,"Unable to shut down, please check if your power supply is ATX power (AT power does not support shutdown), or if you have enabled ACPI.\n");
		}
	} else if (strcmp(cmdline, "reboot") ==0 ){
		if(task->langmode==3)
		{
			cons_putstr0(cons,"正在重启......\n");	
		}
		else
		{
			cons_putstr0(cons,"Rebooting...\n");
		}
		io_out8(0x64,0xfe); 
		if(task->langmode==3)
		{
			cons_putstr0(cons,"无法重启，请检查你是否开启了ACPI\n");	
		} 
		else
		{
			cons_putstr0(cons,"Unable to reboot, please check if you have enabled ACPI.\n");
		}
	} else if (strcmp(cmdline, "colortest") == 0) {
		if(task->langmode==3)
		{
			cons_putstr0(cons,"256色测试\n");	
		}
		else
		{
			cons_putstr0(cons,"256 colors test\n");
		}
		colortest();
	} else if (strcmp(cmdline, "test") == 0) {
		update_mouse_status(2);
	} else if (strcmp(cmdline,"about")==0){
		put_w24(cons);
		put_71gn(cons);
		if(task->langmode==3)
		{
			cons_putstr0(cons,"关于Neumann Operating System\n");
			cons_putstr0(cons,"Neumann Operating System v0.61[Beta 4]\n");
			cons_putstr0(cons,"\n基于HariboteOS操作系统，单内核操作系统\n");
			cons_putstr0(cons,"WRH编写，版权归W24工作室和71GN Deep Space所有\n");
			cons_putstr0(cons,"此系统属于自由软件，你可以使用、编译、再次分发此软件\n");
			cons_putstr0(cons,"\n请尊重W24工作室和71GN Deep Space的所有权利\n");
			cons_putstr0(cons,"GitHub开源地址:https://github.com/wrhmade\n");	
		}
		else
		{
			cons_putstr0(cons,"About Neumann Operating System\n");
			cons_putstr0(cons,"Neumann Operating System v0.6[Beta 4]\n");
			cons_putstr0(cons,"\nBuilt on HariboteOS,Single kernel operating system\n");
			cons_putstr0(cons,"Written by WRH, copyrighted by W24 Studio and 71GN Deep Space\n");
			cons_putstr0(cons,"This system belongs to free software, and you can use, compile, and redistribute this software.\n");
			cons_putstr0(cons,"\nPlease respect all rights of W24 Studio and 71GN Deep Space\n");
			cons_putstr0(cons,"GitHub open source address:https://github.com/wrhmade\n");
		}
	} else if (strncmp(cmdline, "curspeed ", 9) == 0) {
		cmd_curspeed(cons, cmdline);
	} else if (strncmp(cmdline, "beep ", 5) == 0) {
		cmd_beep(cons, cmdline);
	} else if (strncmp(cmdline, "echo ", 5) == 0) {
		cmd_echo(cons, cmdline);
	} else if (strncmp(cmdline, "ccmos ", 6) == 0) {
		cmd_update_cmostime(cons, cmdline);
	} else if (strncmp(cmdline, "finfo ", 6) == 0) {
		cmd_finfo(cons, cmdline);
	} else if (strcmp(cmdline, "memtest") == 0) {
		cmd_memtest(cons);
	} else if (strcmp(cmdline, "flag1") == 0) {
		cons->flag=1;
	} else if (strcmp(cmdline, "flag2") == 0) {
		cons->flag=2;
	} else if (strcmp(cmdline, "bootinfo") == 0) {
		cmd_bootinfo(cons);
	} else if (strcmp(cmdline, "help") == 0) {
		cmd_help(cons);
	} else if (strcmp(cmdline, "panic") == 0) {
		syspanic("System Panic Test");
	} else if (cmdline[0] != 0) {
		if (cmd_app(cons, fat, cmdline, memtotal) == 0) {
			/* 僐儅儞僪偱偼側偔丄傾僾儕偱傕側偔丄偝傜偵嬻峴偱傕側偄 */
			if(task->langmode==3)
			{
				cons_putstr0(cons, "无效命令.\n\n");	
			}
			else
			{
				cons_putstr0(cons, "Invalid command.\n\n");
			} 
		}
	}
	return;
}

void cmd_help(struct CONSOLE *cons)
{
	struct TASK *task=task_now();
	if(task->langmode==3)
	{
		cons_putstr0(cons,"命令列表:\n");
		cons_putstr0(cons,"help\t命令列表\n");
		cons_putstr0(cons,"mem\t内存使用情况\n");
		cons_putstr0(cons,"dir\t显示磁盘中的文件\n");
		cons_putstr0(cons,"cls\t清屏\n");
		cons_putstr0(cons,"exit\t退出控制台\n");
		cons_putstr0(cons,"scroll\t启用\\禁用滚动\n");
		cons_putstr0(cons,"start <char*:命令行>\t启动另一个控制台实例并运行命令\n");
		cons_putstr0(cons,"ncst <char*:命令行>\t启动控制台任务运行命令\n");
		cons_putstr0(cons,"langmode <unsigned int:语言模式>\t更改语言模式\n");
		cons_putstr0(cons,"shutdown\t关闭计算机\n");
		cons_putstr0(cons,"reboot\t重新启动\n");
		cons_putstr0(cons,"colortest\t256色测试\n");
		cons_putstr0(cons,"about\t关于\n");
		cons_putstr0(cons,"curspeed <unsigned int:间隔>\t光标闪烁速度\n");
		cons_putstr0(cons,"beep<unsigned int:频率>\t蜂鸣\n");
		cons_putstr0(cons,"echo <char*:文本>\t显示文本\n");
		cons_putstr0(cons,"ccmos <int:误差>\t修正CMOS时间\n");
		cons_putstr0(cons,"bootinfo\t启动信息\n");
	}
	else
	{
		cons_putstr0(cons,"Command List:\n");
		cons_putstr0(cons,"help\tCommand list\n");
		cons_putstr0(cons,"mem\tMemory usage\n");
		cons_putstr0(cons,"dir\tFiles on disk\n");
		cons_putstr0(cons,"cls\tClear screen\n");
		cons_putstr0(cons,"exit\tExit console\n");
		cons_putstr0(cons,"scroll\tEnable\\Disable Scroll\n");
		cons_putstr0(cons,"start <char*:Command>\tStart another console instance and run the command\n");
		cons_putstr0(cons,"ncst <char*:Command>\tLaunch console tasks and run commands\n");
		cons_putstr0(cons,"langmode <unsigned int:Language mode>\tChange language mode\n");
		cons_putstr0(cons,"shutdown\tShut down\n");
		cons_putstr0(cons,"reboot\tReboot\n");
		cons_putstr0(cons,"colortest\t256 colors test\n");
		cons_putstr0(cons,"about\tAbout\n");
		cons_putstr0(cons,"curspeed <unsigned int:Time>\tCursor blink rate\n");
		cons_putstr0(cons,"beep<unsigned int:frequency>\tBeep\n");
		cons_putstr0(cons,"echo <char*:Text>\tDisplay Text\n");
		cons_putstr0(cons,"ccmos <int:Correct>\tFix CMOS time\n");
		cons_putstr0(cons,"bootinfo\tBoot infomation\n");
	}
}

void cmd_bootinfo(struct CONSOLE *cons)
{
	struct TASK *task;
	task=task_now();
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[60]; 
	if(task->langmode==3)
	{
		cons_putstr0(cons,"启动信息\n");
		sprintf(s,"分辨率:%dx%d\n",binfo->scrnx,binfo->scrny);
		cons_putstr0(cons,s);
		sprintf(s,"宽高比:%d:%d\n",binfo->scrnx/(get_GCD(binfo->scrnx,binfo->scrny)),binfo->scrny/(get_GCD(binfo->scrnx,binfo->scrny)));
		cons_putstr0(cons,s);
		sprintf(s,"系统所占柱面数:%d\n",binfo->cyls);
		cons_putstr0(cons,s);
		sprintf(s,"色深:%d位\n",binfo->vmode);
		cons_putstr0(cons,s);
		sprintf(s,"显存起始地址:%8x\n",binfo->vram);
		cons_putstr0(cons,s);
	}
	else
	{
		cons_putstr0(cons,"Boot Information\n");
		sprintf(s,"Display resolution:%dx%d\n",binfo->scrnx,binfo->scrny);
		cons_putstr0(cons,s);
		sprintf(s,"Aspect ratio:%d to %d\n",binfo->scrnx/(get_GCD(binfo->scrnx,binfo->scrny)),binfo->scrny/(get_GCD(binfo->scrnx,binfo->scrny)));
		cons_putstr0(cons,s);
		sprintf(s,"Number of cylinders occupied by the system:%d\n",binfo->cyls);
		cons_putstr0(cons,s);
		sprintf(s,"Color Depth:%dbit(s)\n",binfo->vmode);
		cons_putstr0(cons,s);
		sprintf(s,"Starting address of graphics memory:%8x\n",binfo->vram);
		cons_putstr0(cons,s);
	}
	return;
}

void cmd_scroll(struct CONSOLE *cons)
{
	cons->scroll=!cons->scroll;
	struct TASK *task;
	task=task_now();
	if(task->langmode==3)
	{
		if(cons->scroll==0)
		{
			cons_putstr0(cons,"屏幕滚动已禁用\n");
		}
		else
		{
			cons_putstr0(cons,"屏幕滚动已启用\n");
		}
	}
	else
	{
		if(cons->scroll==0)
		{
			cons_putstr0(cons,"Screen scrolling disabled\n");
		}
		else
		{
			cons_putstr0(cons,"Screen scrolling enabled\n");
		
		}
	}
}

void cmd_finfo(struct CONSOLE *cons,char* cmdline)
{
	int i=0;
	char s[30];
	/*for(i=0;i<30;i++)
	{
		s[i]='\0';
	}*/
	i=6;
	while(!(cmdline[i]==' ' || cmdline=='\0'))
	{
		s[i-6]=cmdline[i];
		i++;
	}
	cons_putstr0(cons,s);
	cons_putchar(cons,0xAA,0);
}

void cmd_memtest(struct CONSOLE *cons)
{
	unsigned long int total_memory;
	struct TASK *task;
	task=task_now();
	if(task->langmode==3)
	{
		cons_putstr0(cons,"\n测试内存中......");
	}
	else
	{
		cons_putstr0(cons,"\nTesting memory......");
	}
	total_memory=memtest(0x00400000, 0xbfffffff)/1024;
	char s[40];
	if(task->langmode==3)
	{
		sprintf(s,"%ldKB 正常\n\n",total_memory);
	}
	else
	{
		sprintf(s,"%ldKB OK\n\n",total_memory);
	}
	cons_putstr0(cons,s);
}

void cmd_echo(struct CONSOLE *cons,char *cmdline)
{
	int i=5;
	while(cmdline[i]!='\0')
	{
		cons_putchar(cons,cmdline[i],1);
		i++;
	}
	cons_putchar(cons,0x0A,0);
}

void cmd_update_cmostime(struct CONSOLE *cons,char *cmdline)
{
	struct TASK *task;
	task=task_now();
	int i=6;
	int n=0;
	int flag=0;
	while(cmdline[i]!='\0')
	{
		if(cmdline[i]=='-')
		{
			flag=1;
		}
		else
		{
			n=n*10+(cmdline[i]-48);	
		}
		i++;
	}
	if(flag)
	{
		n=-n;
	}
	update_cmostime(cons,n); 
}

void cmd_beep(struct CONSOLE *cons,char *cmdline)
{
	struct TASK *task;
	task=task_now();
	int i=5;
	int n=0;
	while(cmdline[i]!='\0')
	{
		n=n*10+(cmdline[i]-48);
		i++;
	}
	beep(n);
}

void cmd_curspeed(struct CONSOLE *cons,char *cmdline)
{
	struct TASK *task;
	task=task_now();
	int i=9;
	int n=0;
	while(cmdline[i]!='\0')
	{
		n=n*10+(cmdline[i]-48);
		i++;
	}
	cursor_speed=n;
	char a[20];
	if(task->langmode==3)
	{
		sprintf(a,"光标速度已设置为%d\n",n);	
	}
	else
	{
		sprintf(a,"Cursor speed has been set to %d\n",n);
	} 
	cons_putstr0(cons, a); 
}

void cmd_mem(struct CONSOLE *cons, unsigned long int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task;
	task=task_now();
	char s[60];
	if(task->langmode==3)
	{
		cons_putstr0(cons,"内存使用报告\n");
		sprintf(s, "%ldKB(%ldMB)内存\n占用了%ldKB\n%ldKB可用\n\n", memtotal / 1024, memtotal / (1024*1024), memtotal / 1024-memman_total(memman) / 1024,memman_total(memman) / 1024);	
	}
	else
	{
		cons_putstr0(cons,"Memory usage report\n");
		sprintf(s, "%ldKB(%ldMB) memory \nOccupying %ldKB\n%ldKB free\n\n", memtotal / 1024, memtotal / (1024*1024), memtotal / 1024-memman_total(memman) / 1024,memman_total(memman) / 1024);	
	}
	cons_putstr0(cons, s);
	return;
}

void cmd_cls(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	for (y = 28; y < 28 + 450-37; y++) {
		for (x = 8; x < 8 + 600-8-8; x++) {
			sheet->buf[x + y * sheet->bxsize] = COL8_000000;
		}
	}
	sheet_refresh(sheet, 8, 28, 8 + 600-8-8, 28 + 450-37);
	cons->cur_y = 28;
	return;
}

void cmd_dir(struct CONSOLE *cons)
{
	struct TASK *task;
	task=task_now();
	if(task->langmode==3)
	{
		cons_putstr0(cons,"磁盘上的文件:\n");	
	}
	else
	{
		cons_putstr0(cons,"Files on disk:\n");
	}
	struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);
	int i, j;
	char s[30];
	for (i = 0; i < 224; i++) {
		if (finfo[i].name[0] == 0x00) {
			break;
		}
		if (finfo[i].name[0] != 0xe5) {
			if ((finfo[i].type & 0x18) == 0) {
				sprintf(s, "filename.ext   %7d", finfo[i].size);
				for (j = 0; j < 8; j++) {
					s[j] = finfo[i].name[j];
				}
				s[ 9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				cons_putstr0(cons, s);
				if((i+1)%2==0)
				{
					cons_newline(cons);
				}
				else
				{
					cons_putstr0(cons,"   ");
				}
			}
		}
	}
	cons_newline(cons);
	return;
}

void cmd_exit(struct CONSOLE *cons, int *fat)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task = task_now();
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0x0fe4);
	struct FIFO32 *fifo = (struct FIFO32 *) *((int *) 0x0fec);
	if (cons->sht != 0) {
		timer_cancel(cons->timer);
	}
	memman_free_4k(memman, (int) fat, 4 * 2880);
	io_cli();
	if (cons->sht != 0) {
		fifo32_put(fifo, cons->sht - shtctl->sheets0 + 768);	/* 768乣1023 */
	} else {
		fifo32_put(fifo, task - taskctl->tasks0 + 1024);	/* 1024乣2023 */
	}
	io_sti();
	for (;;) {
		task_sleep(task);
	}
}

void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal)
{
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0x0fe4);
	struct TASK *task;
	struct SHEET *sht;
	task=task_now();
	if(task->langmode==3)
	{
		sht = open_console(shtctl, memtotal,"控制台");	
	}
	else
	{
		sht = open_console(shtctl, memtotal,"Console");
	}
	struct FIFO32 *fifo = &sht->task->fifo;
	int i;
	sheet_slide(sht, 32, 4);
	sheet_updown(sht, shtctl->top);
	/* 僐儅儞僪儔僀儞偵擖椡偝傟偨暥帤楍傪丄堦暥帤偢偮怴偟偄僐儞僜乕儖偵擖椡 */
	for (i = 6; cmdline[i] != 0; i++) {
		fifo32_put(fifo, cmdline[i] + 256);
	}
	fifo32_put(fifo, 10 + 256);	/* Enter */
	cons_newline(cons);
	return;
}

void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal)
{
	struct TASK *task = open_constask(0, memtotal);
	struct FIFO32 *fifo = &task->fifo;
	int i;
	/* 僐儅儞僪儔僀儞偵擖椡偝傟偨暥帤楍傪丄堦暥帤偢偮怴偟偄僐儞僜乕儖偵擖椡 */
	for (i = 5; cmdline[i] != 0; i++) {
		fifo32_put(fifo, cmdline[i] + 256);
	}
	fifo32_put(fifo, 10 + 256);	/* Enter */
	cons_newline(cons);
	return;
}

void cmd_langmode(struct CONSOLE *cons, char *cmdline)
{
	struct TASK *task = task_now();
	unsigned char mode = cmdline[9] - '0';
	if (mode <= 3) {
		task->langmode = mode;
	} else {
		if(task->langmode==3)
		{
			cons_putstr0(cons, "模式号错误.\n");
		}
		else
		{
			cons_putstr0(cons, "mode number error.\n");	
		}
	}
	if(task->langmode==0)
	{
		cons_putstr0(cons,"ASCII mode.\n");
	
	}
	else if(task->langmode==3)
	{
		cons_putstr0(cons,"ASCII+HZK16 Chinese mode.\n");
	}
	cons_newline(cons);
	return;
}

int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline,unsigned int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo;;
	char name[18], *p, *q;
	struct TASK *task = task_now();
	int i, segsiz, datsiz, esp, dathrb;
	struct SHTCTL *shtctl;
	struct SHEET *sht;
	char command[80];

	/* 僐儅儞僪儔僀儞偐傜僼傽僀儖柤傪惗惉 */
	for (i = 0; i < 13; i++) {
		if (cmdline[i] <= ' ') {
			break;
		}
		name[i] = cmdline[i];
	}
	name[i] = 0; /* 偲傝偁偊偢僼傽僀儖柤偺屻傠傪0偵偡傞 */

	/* 僼傽僀儖傪扵偡 */
	finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0 && name[i - 1] != '.') {
		/* 尒偮偐傜側偐偭偨偺偱屻傠偵".HRB"傪偮偗偰傕偆堦搙扵偟偰傒傞 */
		name[i    ] = '.';
		name[i + 1] = 'P';
		name[i + 2] = 'R';
		name[i + 3] = 'G';
		name[i + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	}
	
	if (finfo != 0) {
		/* 僼傽僀儖偑尒偮偐偭偨応崌 */
		p = (char *) memman_alloc_4k(memman, finfo->size);
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
		if (finfo->size >= 36 && strncmp(p + 4, "WPRG", 4) == 0 && *p == 0x00) {
			segsiz = *((int *) (p + 0x0000));
			esp    = *((int *) (p + 0x000c));
			datsiz = *((int *) (p + 0x0010));
			dathrb = *((int *) (p + 0x0014));
			q = (char *) memman_alloc_4k(memman, segsiz);
			task->ds_base = (int) q;
			set_segmdesc(task->ldt + 0, finfo->size - 1, (int) p, AR_CODE32_ER + 0x60);
			set_segmdesc(task->ldt + 1, segsiz - 1,      (int) q, AR_DATA32_RW + 0x60);
			for (i = 0; i < datsiz; i++) {
				q[esp + i] = p[dathrb + i];
			}
			start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(task->tss.esp0));
			shtctl = (struct SHTCTL *) *((int *) 0x0fe4);
			for (i = 0; i < MAX_SHEETS; i++) {
				sht = &(shtctl->sheets0[i]);
				if ((sht->flags & 0x11) == 0x11 && sht->task == task) {
					/* 傾僾儕偑奐偒偭傁側偟偵偟偨壓偠偒傪敪尒 */
					sheet_free(sht);	/* 暵偠傞 */
				}
			}
			for (i = 0; i < 8; i++) {	/* 僋儘乕僘偟偰側偄僼傽僀儖傪僋儘乕僘 */
				if (task->fhandle[i].buf != 0) {
					memman_free_4k(memman, (int) task->fhandle[i].buf, task->fhandle[i].size);
					task->fhandle[i].buf = 0;
				}
			}
			timer_cancelall(&task->fifo);
			memman_free_4k(memman, (int) q, segsiz);
			task->langbyte1 = 0;
		} else {
			if(task->langmode==3)
			{
				cons_putstr0(cons, "这不是系统可执行的程序.\n");	
			}
			else
			{
				cons_putstr0(cons, "This is not a system executable program.\n");
			}
		}
		memman_free_4k(memman, (int) p, finfo->size);
		cons_newline(cons);
		return 1;
	}
	else
	{
		name[i    ] = '.';
		name[i + 1] = 'L';
		name[i + 2] = 'N';
		name[i + 3] = 'K';
		name[i + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
		if (finfo != 0) {
			struct FILEHANDLE *fh;
			fh->size = finfo->size;
			fh->pos = 0;
			fh->buf = file_loadfile2(finfo->clustno, &fh->size, task->fat);
			struct TASK *new_task = open_constask(0, memtotal);
			struct FIFO32 *fifo2=&new_task->fifo;
			int c;
			for(c=0;fh->buf[c]!=0;c++)
			{
				fifo32_put(fifo2, fh->buf[c] + 256);
			}
			fifo32_put(fifo2, 10 + 256);
			return 1;
		}
	}
	/* 僼傽僀儖偑尒偮偐傜側偐偭偨応崌 */
	return 0;
}

int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct TASK *task = task_now();
	int ds_base = task->ds_base;
	struct CONSOLE *cons = task->cons;
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0x0fe4);
	struct SHEET *sht;
	struct FIFO32 *sys_fifo = (struct FIFO32 *) *((int *) 0x0fec);
	int *reg = &eax + 1;	/* eax偺師偺斣抧 */
		/* 曐懚偺偨傔偺PUSHAD傪嫮堷偵彂偒姺偊傞 */
		/* reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP */
		/* reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX */
	int i;
	struct FILEINFO *finfo;
	struct FILEHANDLE *fh;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

	if (edx == 1) {
		cons_putchar(cons, eax & 0xff, 1);
	} else if (edx == 2) {
		cons_putstr0(cons, (char *) ebx + ds_base);
	} else if (edx == 3) {
		cons_putstr1(cons, (char *) ebx + ds_base, ecx);
	} else if (edx == 4) {
		return &(task->tss.esp0);
	} else if (edx == 5) {
		sht = sheet_alloc(shtctl);
		sht->task = task;
		sht->flags |= 0x10;
		sheet_setbuf(sht, (char *) ebx + ds_base, esi, edi, eax);
		make_window8((char *) ebx + ds_base, esi, edi, (char *) ecx + ds_base, 0);
		sheet_slide(sht, ((shtctl->xsize - esi) / 2) & ~3, (shtctl->ysize - edi) / 2);
		sheet_updown(sht, shtctl->top); /* 崱偺儅僂僗偲摨偠崅偝偵側傞傛偆偵巜掕丗 儅僂僗偼偙偺忋偵側傞 */
		reg[7] = (int) sht;
	} else if (edx == 6) {
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *) ebp + ds_base);
		if ((ebx & 1) == 0) {
			sheet_refresh(sht, esi, edi, esi + ecx * 8, edi + 16);
		}
	} else if (edx == 7) {
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
		if ((ebx & 1) == 0) {
			sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
		}
	} else if (edx == 8) {
		memman_init((struct MEMMAN *) (ebx + ds_base));
		ecx &= 0xfffffff0;	/* 16僶僀僩扨埵偵 */
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if (edx == 9) {
		ecx = (ecx + 0x0f) & 0xfffffff0; /* 16僶僀僩扨埵偵愗傝忋偘 */
		reg[7] = memman_alloc((struct MEMMAN *) (ebx + ds_base), ecx);
	} else if (edx == 10) {
		ecx = (ecx + 0x0f) & 0xfffffff0; /* 16僶僀僩扨埵偵愗傝忋偘 */
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if (edx == 11) {
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		sht->buf[sht->bxsize * edi + esi] = eax;
		if ((ebx & 1) == 0) {
			sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
		}
	} else if (edx == 12) {
		sht = (struct SHEET *) ebx;
		sheet_refresh(sht, eax, ecx, esi, edi);
	} else if (edx == 13) {
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		hrb_api_linewin(sht, eax, ecx, esi, edi, ebp);
		if ((ebx & 1) == 0) {
			if (eax > esi) {
				i = eax;
				eax = esi;
				esi = i;
			}
			if (ecx > edi) {
				i = ecx;
				ecx = edi;
				edi = i;
			}
			sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
		}
	} else if (edx == 14) {
		sheet_free((struct SHEET *) ebx);
	} else if (edx == 15) {
		for (;;) {
			io_cli();
			if (fifo32_status(&task->fifo) == 0) {
				if (eax != 0) {
					task_sleep(task);	/* FIFO偑嬻側偺偱怮偰懸偮 */
				} else {
					io_sti();
					reg[7] = -1;
					return 0;
				}
			}
			i = fifo32_get(&task->fifo);
			io_sti();
			if (i <= 1 && cons->sht != 0) { /* 僇乕僜儖梡僞僀儅 */
				/* 傾僾儕幚峴拞偼僇乕僜儖偑弌側偄偺偱丄偄偮傕師偼昞帵梡偺1傪拲暥偟偰偍偔 */
				timer_init(cons->timer, &task->fifo, 1); /* 師偼1傪 */
				timer_settime(cons->timer, 50);
			}
			if (i == 2) {	/* 僇乕僜儖ON */
				cons->cur_c = COL8_FFFFFF;
			}
			if (i == 3) {	/* 僇乕僜儖OFF */
				cons->cur_c = -1;
			}
			if (i == 4) {	/* 僐儞僜乕儖偩偗傪暵偠傞 */
				timer_cancel(cons->timer);
				io_cli();
				fifo32_put(sys_fifo, cons->sht - shtctl->sheets0 + 2024);	/* 2024乣2279 */
				cons->sht = 0;
				io_sti();
			}
			if (i >= 256) { /* 僉乕儃乕僪僨乕僞乮僞僗僋A宱桼乯側偳 */
				reg[7] = i - 256;
				return 0;
			}
		}
	} else if (edx == 16) {
		reg[7] = (int) timer_alloc();
		((struct TIMER *) reg[7])->flags2 = 1;	/* 帺摦僉儍儞僙儖桳岠 */
	} else if (edx == 17) {
		timer_init((struct TIMER *) ebx, &task->fifo, eax + 256);
	} else if (edx == 18) {
		timer_settime((struct TIMER *) ebx, eax);
	} else if (edx == 19) {
		timer_free((struct TIMER *) ebx);
	} else if (edx == 20) {
		if (eax == 0) {
			i = io_in8(0x61);
			io_out8(0x61, i & 0x0d);
		} else {
			i = 1193180000 / eax;
			io_out8(0x43, 0xb6);
			io_out8(0x42, i & 0xff);
			io_out8(0x42, i >> 8);
			i = io_in8(0x61);
			io_out8(0x61, (i | 0x03) & 0x0f);
		}
	} else if (edx == 21) {
		for (i = 0; i < 8; i++) {
			if (task->fhandle[i].buf == 0) {
				break;
			}
		}
		fh = &task->fhandle[i];
		reg[7] = 0;
		if (i < 8) {
			finfo = file_search((char *) ebx + ds_base,
					(struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
			if (finfo != 0) {
				reg[7] = (int) fh;
				fh->size = finfo->size;
				fh->pos = 0;
				fh->buf = file_loadfile2(finfo->clustno, &fh->size, task->fat);
			}
		}
	} else if (edx == 22) {
		fh = (struct FILEHANDLE *) eax;
		memman_free_4k(memman, (int) fh->buf, fh->size);
		fh->buf = 0;
	} else if (edx == 23) {
		fh = (struct FILEHANDLE *) eax;
		if (ecx == 0) {
			fh->pos = ebx;
		} else if (ecx == 1) {
			fh->pos += ebx;
		} else if (ecx == 2) {
			fh->pos = fh->size + ebx;
		}
		if (fh->pos < 0) {
			fh->pos = 0;
		}
		if (fh->pos > fh->size) {
			fh->pos = fh->size;
		}
	} else if (edx == 24) {
		fh = (struct FILEHANDLE *) eax;
		if (ecx == 0) {
			reg[7] = fh->size;
		} else if (ecx == 1) {
			reg[7] = fh->pos;
		} else if (ecx == 2) {
			reg[7] = fh->pos - fh->size;
		}
	} else if (edx == 25) {
		fh = (struct FILEHANDLE *) eax;
		for (i = 0; i < ecx; i++) {
			if (fh->pos == fh->size) {
				break;
			}
			*((char *) ebx + ds_base + i) = fh->buf[fh->pos];
			fh->pos++;
		}
		reg[7] = i;
	} else if (edx == 26) {
		i = 0;
		for (;;) {
			*((char *) ebx + ds_base + i) =  task->cmdline[i];
			if (task->cmdline[i] == 0) {
				break;
			}
			if (i >= ecx) {
				break;
			}
			i++;
		}
		reg[7] = i;
	} else if (edx == 27) {
		reg[7] = task->langmode;
	} else if (edx == 28) {
		update_mouse_status(eax);
	} else if (edx == 29) {
		reg[7] = getmousex();
	} else if (edx == 30) {
		reg[7] = getmousey();
	} else if (edx == 31) {
		reg[7] = getmousechick(1);
	} else if (edx == 32) {
		reg[7] = getmousechick(2);
	} else if (edx == 33) {
		reg[7] = cons->cur_x;
	} else if (edx == 34) {
		reg[7] = cons->cur_y;
	} else if (edx == 35) {
		cons_putchar(cons," ",-1); 
	}
	return 0;
}

int *inthandler0c(int *esp)
{
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	char s[60];
	if(task->langmode==3)
	{
		cons_putstr0(cons, "\n出了点问题:\nINT 0C :\n 堆栈异常.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);	
	}
	else
	{
		cons_putstr0(cons, "\nThere is a problem:\nINT 0C :\n Stack Exception.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);
	}
	return &(task->tss.esp0);	/* 堎忢廔椆偝偣傞 */
}

int *inthandler00(int *esp)
{
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	char s[60];
	if(task->langmode==3)
	{
		cons_putstr0(cons, "\n出了点问题:\nINT 00 :\n 除法错.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);	
	}
	else
	{
		cons_putstr0(cons, "\nThere is a problem:\nINT 00 :\nDivide Error.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);
	}
	return &(task->tss.esp0);	/* 堎忢廔椆偝偣傞 */
}

int *inthandler06(int *esp)
{
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	char s[60];
	if(task->langmode==3)
	{
		cons_putstr0(cons, "\n出了点问题:\nINT 06 :\n 无效指令集.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);	
	}
	else
	{
		cons_putstr0(cons, "\nThere is a problem:\nINT 06 :\nInvalid Opcode.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);
	}
	return &(task->tss.esp0);	/* 堎忢廔椆偝偣傞 */
}


int *inthandler0d(int *esp)
{
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	char s[60];
	if(task->langmode==3)
	{
		cons_putstr0(cons, "\n出了点问题:\nINT 0D :\n 一般保护异常.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);	
	}
	else
	{
		cons_putstr0(cons, "\nThere is a problem:\nINT 0D :\n General Protection Exception.\n");
		sprintf(s, "EIP = %08X\n", esp[11]);
		cons_putstr0(cons, s);
	}
	return &(task->tss.esp0);	/* 堎忢廔椆偝偣傞 */
}

void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col)
{
	int i, x, y, len, dx, dy;

	dx = x1 - x0;
	dy = y1 - y0;
	x = x0 << 10;
	y = y0 << 10;
	if (dx < 0) {
		dx = - dx;
	}
	if (dy < 0) {
		dy = - dy;
	}
	if (dx >= dy) {
		len = dx + 1;
		if (x0 > x1) {
			dx = -1024;
		} else {
			dx =  1024;
		}
		if (y0 <= y1) {
			dy = ((y1 - y0 + 1) << 10) / len;
		} else {
			dy = ((y1 - y0 - 1) << 10) / len;
		}
	} else {
		len = dy + 1;
		if (y0 > y1) {
			dy = -1024;
		} else {
			dy =  1024;
		}
		if (x0 <= x1) {
			dx = ((x1 - x0 + 1) << 10) / len;
		} else {
			dx = ((x1 - x0 - 1) << 10) / len;
		}
	}

	for (i = 0; i < len; i++) {
		sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
		x += dx;
		y += dy;
	}

	return;
}
