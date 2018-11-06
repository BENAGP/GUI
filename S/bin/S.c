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
