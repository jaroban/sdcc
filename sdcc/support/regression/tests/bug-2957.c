/*
   bug-2957.c
 */

#include <testfwk.h>

char bug2957(char *p, int i, char c)
{
    p[0] = c;
    p[i] = 8;
    return p[0]; // Always returns c.
}

void testBug(void)
{
	char c;
	ASSERT(bug2957(&c, 0, 0) == 8);
}

