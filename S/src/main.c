#include <stdio.h>
#include <../include/S.h>

int main()
{
	init_set();
    clear();
	setbkcolor(0, 0, 0);
	setcolor(255, 255, 255);
	dot(400, 400);
	line(200, 200, 600, 600);
	rect(200, 200, 600, 600);
	circle(600, 600, 50, 0);
	circle(200, 200, 50, 1);
	print_text(350 , 380, "Hello World!");
	move_circle(200, 600, 600, 200, 20, 1);
    return 0;
}

