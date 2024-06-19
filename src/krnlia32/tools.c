#include "bootpack.h"

int string_len(char* s)
{
	int i=0;
	while(s[i]!='\0')
	{
		i++;
	}
	return i;
}


void beep(int tone)
{
	int i;
	if (tone == 0) {
			i = io_in8(0x61);
			io_out8(0x61, i & 0x0d);
	} else {
			i = 1193180000 / tone;
			io_out8(0x43, 0xb6);
			io_out8(0x42, i & 0xff);
			io_out8(0x42, i >> 8);
			i = io_in8(0x61);
			io_out8(0x61, (i | 0x03) & 0x0f);
	}
} 

int leap_year(int year)
{
	if(year%100==0)
	{
		if(year%400==0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if(year%4==0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}


int get_maxiday(int month,int year)
{
	int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};//�ֱ��Ӧһ���е�ʮ�����·�
	if(leap_year(year))//���괦�� 
	{
		days[1]=29;//�����¸�Ϊ29�� 
	}
	return days[month-1]; 
}
void update_date_time(struct SHEET *sht_back,int correct)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	int _y,_m,_d,y,m,d,h;
  	y=get_year();m=get_mon_hex();d=get_day_of_month();h=get_hour_hex()+correct; 
  	
  	//ʱ�䳬����Χ����
	//��λ
	int i;
	if(h>=24)//Сʱ����24Сʱ��һ�죩
 	{
		while(!(h>=0 && h<24))
		{
			h-=24;//����
			d+=1; 
			if(d>get_maxiday(m,y))//������һ����
			{
				m+=1;//����
				d-=get_maxiday(m,y);
				if(m>12)//������һ��
				{
					y+=1;//����
					m-=12; 
				} 
			}
		}
	}
	//��λ 
	if(h<0)//ʱ�������� 
	{
		while(!(h>=0 && h<24))
		{
			h+=24;//��һ�յ�24Сʱ
			d-=1;
			if(d<1)//����������
			{
				m-=1;//��һ���µ���ʮ����
				if(m<1)//����������
				{
					y-=1;//��һ�µ�365��
					m+=12;
				}
				d+=get_maxiday(m,y);
				
			} 
		}
	}
  	
	char result[6],s[30];
	
	_y=y;_m=m;_d=d;
	if(_m==1||_m==2) {
  	    _m+=12;
 	    _y--;
 	}
  	int iWeek=(_d+2*_m+3*(_m+1)/5+_y+_y/4-_y/100+_y/400)%7+1;
    
  	switch(iWeek)
  	{
  		case 1:
  	  		strcpy(result,"Mon.");
  	  		break;
    	case 2:
  	  		strcpy(result,"Tues.");
 	  			break;
   		case 3:
   			strcpy(result,"Wed.");
   			break;
   		case 4:
    		strcpy(result,"Thur.");
   			break;
   		case 5:
   			strcpy(result,"Fri.");
   			break;
    	case 6:
			strcpy(result,"Sat.");
    		break;
    	case 7:
   			strcpy(result,"Sun.");
    		break;
	}		
		
		//?������
		sprintf(s, "%04d/%02d/%02d %s", y, m, d,result);
		putfonts8_asc_sht(sht_back, binfo->scrnx - 190, binfo->scrny -20, 0, BACKC, s, 15);
		//?��??
		sprintf(s, "%02d:%02d:%02d", h, get_min_hex(),get_sec_hex());
		putfonts8_asc_sht(sht_back, binfo->scrnx - 69, binfo->scrny -20, 0, BACKC, s, 8);
}

void put_71gn(struct CONSOLE *cons)
{
	cons_putstr0(cons,"\n _______     ___     _______   ____     _\n|____   |   /   |   / ______| |    \\   | |\n     / /   /_/| |  | |   ___  | |\\  \\  | |\n    / /       | |  | |  |_    | | \\  \\ | |\n   / /        | |  | |____| | | |  \\  \\| |\n  /_/         |_|  |________| |_|   \\____|    Deep Space\n\n");
}

void put_w24(struct CONSOLE *cons)
{
	cons_putstr0(cons,"\n _    _   _____   _   _\n| |  | | |___  | | | | |\n| |  | |  ___| | | |_| |_\n| |  | | |  ___| |___   _| \n| |/\\| | | |___      | |\n \\_/\\_/  |_____|     |_|Studio\n\n");
}

void execute_program(char* programname,unsigned long int memtotal,char* mouse_cursor)
{
	init_mouse_cursor8(mouse_cursor,99,2);
	struct TASK *task = open_constask(0, memtotal);
	struct FIFO32 *fifo2=&task->fifo;
	int c;
	for(c=0;c<11;c++)
	{
		fifo32_put(fifo2, programname[c] + 256);
	}
	fifo32_put(fifo2, 10 + 256);
	init_mouse_cursor8(mouse_cursor,99,1);
}

unsigned char rgb2pal8(int r, int g, int b, int x, int y)
{
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1; /* ��������� */
	y &= 1;
	i = table[x + y * 2];	/* ���ԐF����邽�߂̒萔 */
	r = (r * 21) / 256;	/* ����� 0�`20 �ɂȂ� */
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;	/* ����� 0�`5 �ɂȂ� */
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}

int get_GCD(int a,int b)
{
	int t1=a;
	int t2=b;
	int tmp;
	while(t1%t2!=0)
	{
		tmp=t1%t2;
		t1=t2;
		t2=tmp;
	}
	return t2;
} 

int get_LCM(int a,int b)
{
	return a*b/get_GCD(a,b);
}


