/*
   920922-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#endif

unsigned long*
f(unsigned long *p)
{
  unsigned long a = (*p++) >> 24;
  return p + a;
}

void
testTortureExecute (void)
{
#if !(defined(__SDCC_mcs51) && defined(__SDCC_STACK_AUTO)) // Bug #2669
  unsigned long x = 0x80000000UL;
  if (f(&x) != &x + 0x81)
    ASSERT(0);
  return;
#endif
}

