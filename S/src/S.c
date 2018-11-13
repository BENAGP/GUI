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
unsigned char *addr;
struct fb_var_screeninfo vinfo;
int style = 0;

void init_set()
{
	int fd;
	int screensize = 0;
	
	fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FRIOGET_VSCREENINFO, &vinfo);
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
	bzero(addr, vinfo.xres * vinfo.yres * vinfo.bit_per_pixel / 8);
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

void line(int x1, int y1, int x2, int y2)
{
	int i;
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
                    for(i = y1; i <= y2; i+=2)
                        dot((x1 + (i - y1) / k), i);
                }
            }
	}

}
