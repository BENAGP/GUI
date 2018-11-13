#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<linux/fb.h>

#define PI 3.1415926535898   //yuanzhoulv

typedef unsigned short INT16;

#define RGB565(R, G, B) \
	(((INT16)((R) >> 3)) << 11) + (((INT16)((G) >> 2)) << 5) + ((INT16)((B) >> 3))

void init_set();
void clear();
void setcolor(unsigned char, unsigned char, unsigned char);
void setbkcolor(unsigned char, unsigned char, unsigned char);
void dot(int, int);
void line(int, int, int, int);
void dot(int, int);
