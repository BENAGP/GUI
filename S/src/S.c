#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <linux/fb.h>
#include<fcntl.h>
#include<sys/mman.h>
#include "../include/S.h"

#define PI 3.1415926535898

int fd;
unsigned short color;
unsigned short now_color;
unsigned char *addr;
struct fb_var_screeninfo vinfo;
int style = 0;

void init_set()
{
	int fd;
	int screensize = 0;
	
	fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	printf("Initialzing screen parameters:\n");
	printf("	bits_per_pixel:%d\n",vinfo.bits_per_pixel);
	printf("	xres:%d\n",vinfo.xres);
	printf("	yres:%d\n",vinfo.yres);

        screensize = vinfo.bits_per_pixel * vinfo.xres * vinfo.yres / 8;
        printf("screensize:%d\n", screensize);
        addr = (unsigned char *)mmap(0, screensize,
                                     PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

void clear()
{
	printf("Clearing screen!\n");
	bzero(addr, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8);
}

void setcolor(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	printf("Setting color, (Red, Green, Blue) = (%d, %d, %d)", Red, Green, Blue);
	color = RGB565(Red, Green, Blue);
}

void dot(int x, int y)
{
	*(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 0) = color & 0xFF;
	*(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 1) = (color >> 8) & 0xFF;
}

void setbkcolor(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	int x, y;
	printf("Setting background color, (Red, Green, Blue) = (%d, %d, %d)", Red, Green, Blue);
	color = RGB565(Red, Green, Blue);
	for(x = 0; x < vinfo.xres; x++)
	{
		for(y = 0; y < vinfo.yres; y++)
		{
			dot(x, y);
		}
	}	
}

void setlinestyle(int sty)
{
	style = sty;
}
int ** line(int x1, int y1, int x2, int y2)
{
	int dots[400][2];
    int i, j;
    j = 0;
	float k;
	k = (y2 - y1) / (x2 -x1);
	switch(style)
	{
        case 0:
            {
                 if(x1 == x2){
                     if(y1 > y2){
                         i = y1;
                         y1 = y2;
                         y2 = i;
                     }
                     for(i = y1; i <= y2; i++)
                     {  
                         if(getcolor(x1, i) != color)
                             dot(x1, i);
                         dots[j][0] = x1;
                         dots[j][1] = i;
                         j++;
                     }          
                 } else if(abs(x2 - x1) >= abs(y2 - y1)){
                     if(x1 > x2){
                         i = x1;
                         x1 = x2;
                         x2 = i;
                         i = y1;
                         y1 = y2;
                         y2 = i;
                     }
                     for(i = x1; i <= x2; i += 1){
                         if(getcolor(i, (y1 + (i - x1) * k)) != color)
                             dot(i, (y1 + (i - x1) * k));
                         dots[j][0] = i;
                         dots[j][1] = y1 + (i - x1) * k;
                         j++;
                     }  
                 } else{
                     if(y1 >y2){
                         i = x1;
                         x1 = x2;
                         x2 = i;
                         i = y1;
                         y1 = y2;
                         y2 = i;
                     }
                     for(i = y1; i <= y2; i += 1){
                         if(getcolor((x1 + (i - y1) / k), i) != color)
                             dot((x1 + (i - y1) / k), i);
                         dots[j][0] = x1 + (i - y1) / k;
                         dots[j][1] = i;
                         j++;
                     }
                 }
            }
		case 1:
			{
				if(x1 == x2){
					if(y1 > y2){
						i = y1;
						y1 = y2;
						y2 = i;
					}
					for(i = y1; i <= y2; i++)
						dot(x1, i);
				} else if(abs(x2 - x1) >= abs(y2 - y1)){
					if(x1 >	x2){
						i = x1;
						x1 = x2;
						x2 = i;
						i = y1;
						y1 = y2;
						y2 = i;
					}
                    for(i = x1; i <= x2; i += 2)
                        dot(i, (y1 + (i - x1) * k));
                } else{
                    if(y1 >y2){
                        i = x1;
                        x1 = x2;
					    x2 = i;
					    i = y1;
					    y1 = y2;
					    y2 = i;
                    }
                    for(i = y1; i <= y2; i += 2)
                        dot((x1 + (i - y1) / k), i);
                }
            }
	}
    return dots;
}

void rect(int x1, int y1, int x2, int y2)
{
    printf("Drawing a rectangle.\n");
    line(x1, y1, x2, y1);
    line(x1, y1, x1, y2);
    line(x1, y2, x2, y2);
    line(x2, y1, x2, y2);
}

unsigned short getcolor(int x, int y)
{
    now_color = *(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 0);
    now_color = (*(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 1) << 8);
    return now_color;
}

void circle(int x, int y, int r, int circle_style)
{
    int xtemp, ytemp, i;
    switch(circle_style){
    case 0:
        for(i = 0; i < 360; i++)
        {
            xtemp = x + r * cos(i * PI / 180);
            ytemp = y + r * sin(i * PI / 180);
            if(getcolor(xtemp, ytemp) != color)
            {
                dot(xtemp, ytemp);
            }
        }
    case 1:
        for(i = 0; i < 360; i++)
        {
            xtemp = x + r * cos(i * PI / 180);
            ytemp = y + r * sin(i * PI / 180);
            line(x, y, xtemp, ytemp);
        }
    }
    printf("Drawing a circle.\n");
}

void display_bmp(char *path)
{
    int file, x, y;
    char buf[3] = { 0 };
    file = open(path, O_RDONLY, 0666);
    if(file < 0){
        perror("open");
        exit(1);
    }
    lseek(file, 54, SEEK_SET);
    for(y = 479; y >= 0; y--){
        for(x = 0; x <= 799; x++){
            read(file, buf, 3);
            color = RGB565(buf[2], buf[1], buf[0]);
            dot(x, y);
        }
    }
}
void print_text(int x, int y, char *t)
{
    register int i, j, k;
    FILE *fpa, *fph;
    char buffera[16], bufferh[32];
    unsigned char qh, wh;
    unsigned long location;

    if((fpa = fopen("../open/asc16", "rb")) == NULL)
        perror("open asc16");
    if((fph = fopen("../open/hzk16", "rb")) == NULL)
        perror("open hzk16");
    printf("Printing string:%s\n", t);
    while(*t){
        if((char)(*t) >0){
            fseek(fpa, (*t) * 16, 0);
            fread(buffera, 1, 16, fpa);
            for(i = 0; i < 16; i++)
                for(j = 0; j < 8; j++){
                    if(((buffera[i] >> (7 - j)) & 0x01) != 0)
                        dot(j + x, i + y);
                }
            t++;
            x += 8;
        } else {
            qh = *t - 0xa0;
            wh = *(t + 1) - 0xa0;
            printf("wh=%d\n", wh);
            location = (94 * (qh - 1) + (wh - 1)) * 32L;
            fseek(fph, location, SEEK_SET);
            fread(bufferh, 32, 1, fph);
            for(i = 0;  i < 16; i++)
                for(j = 0; j < 2; j++)
                    for(k = 0; k < 8; k++)
                        if(((bufferh[i * 2 + j] >> (7 - k)) & 0x01) != 0)
                            dot(x + 8 * j + k, y + i);
            t += 2;
            x += 16;
        }
    }
    fclose(fpa);
    fclose(fph);
}

void move_circle(int x1, int y1, int x2, int y2, int r, int speed)
{
    int ** dots;
    dots = line(x1, y1 ,x2 ,y2);
    int i=0;
    while(dots[i][0]){
        circle(dots[i][0], dots[i][1], r, 1);
        i += speed;
    }
}
