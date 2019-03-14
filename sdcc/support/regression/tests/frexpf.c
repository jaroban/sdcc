/* Test frexpf(), which has a habit of breaking */


#include <testfwk.h>
#include <math.h>

void testfrexpf(void)
{
#if !defined(__SDCC_pdk14) // Lack of memory
  int e;
  int *p = &e;

  ASSERT (frexpf (0.75, p) == 0.75);

  ASSERT (frexpf (0.75 * 4, p) == 0.75);

  ASSERT (frexpf (0.75 * 16, p) == 0.75);
#endif
}

