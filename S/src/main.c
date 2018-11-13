#include <stdio.h>
#include "../include/S.h"

int main()
{
        init_set();
        clear();
	setbkcolor(255, 255, 255);
	setcolor(0, 0, 0);
	dot(100, 100);
	line(200, 200, 400, 400);
        return 0;
}

