/** Simple test for the _ltoa and _ultoa.
  test: ultoa, ltoa
*/
//  part: 1, 2, 3, 4

#include <testfwk.h>
#include <stdlib.h>
#include <string.h>

#if !defined(PORT_HOST) && !(defined(__SDCC_mcs51) && !defined(__SDCC_STACK_AUTO) && defined(__SDCC_MODEL_SMALL) ) && !defined(__SDCC_pic14) && !defined(__SDCC_pic16) && !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Lack of memory
/* do not run tests for listed above platforms - out of memory */
#define TEST_{test} 1
#define LTOA(num,base,res) check_l(num, base, res)
#define ULTOA(num,base,res) check_ul(num, base, res)

static char
check_l (long n, int b, const char *r)
{
  static char buf[66];
  _ltoa(n, buf, b);
  return !strcmp (buf, r);
}

static char
check_ul (unsigned long n, int b, const char *r)
{
  char buf[66];
  _ultoa(n, buf, b);
  return !strcmp (buf, r);
}

#endif

void test_ltoa (void)
{
#ifdef TEST_ltoa
//#if {part} == 1
  ASSERT (LTOA (0, 2, "0"));
  ASSERT (LTOA (0, 8, "0"));
  ASSERT (LTOA (0, 10, "0"));
  ASSERT (LTOA (0, 16, "0"));
  ASSERT (LTOA (1, 2, "1"));
  ASSERT (LTOA (1, 8, "1"));
  ASSERT (LTOA (1, 10, "1"));
  ASSERT (LTOA (1, 16, "1"));
  ASSERT (LTOA (2, 2, "10"));
  ASSERT (LTOA (2, 8, "2"));
  ASSERT (LTOA (2, 10, "2"));
  ASSERT (LTOA (2, 16, "2"));
  ASSERT (LTOA (3, 2, "11"));
  ASSERT (LTOA (3, 8, "3"));
  ASSERT (LTOA (3, 10, "3"));
  ASSERT (LTOA (3, 16, "3"));
  ASSERT (LTOA (4, 2, "100"));
  ASSERT (LTOA (4, 8, "4"));
  ASSERT (LTOA (4, 10, "4"));
  ASSERT (LTOA (4, 16, "4"));
  ASSERT (LTOA (5, 2, "101"));
  ASSERT (LTOA (5, 8, "5"));
  ASSERT (LTOA (5, 10, "5"));
  ASSERT (LTOA (5, 16, "5"));
  ASSERT (LTOA (6, 2, "110"));
  ASSERT (LTOA (6, 8, "6"));
  ASSERT (LTOA (6, 10, "6"));
  ASSERT (LTOA (6, 16, "6"));
  ASSERT (LTOA (7, 2, "111"));
  ASSERT (LTOA (7, 8, "7"));
  ASSERT (LTOA (7, 10, "7"));
  ASSERT (LTOA (7, 16, "7"));
  ASSERT (LTOA (8, 2, "1000"));
  ASSERT (LTOA (8, 8, "10"));
  ASSERT (LTOA (8, 10, "8"));
  ASSERT (LTOA (8, 16, "8"));
  ASSERT (LTOA (9, 2, "1001"));
  ASSERT (LTOA (9, 8, "11"));
  ASSERT (LTOA (9, 10, "9"));
  ASSERT (LTOA (9, 16, "9"));
  ASSERT (LTOA (10, 2, "1010"));
  ASSERT (LTOA (10, 8, "12"));
  ASSERT (LTOA (10, 10, "10"));
  ASSERT (LTOA (10, 16, "A"));
  ASSERT (LTOA (11, 2, "1011"));
  ASSERT (LTOA (11, 8, "13"));
  ASSERT (LTOA (11, 10, "11"));
  ASSERT (LTOA (11, 16, "B"));
  ASSERT (LTOA (12, 2, "1100"));
  ASSERT (LTOA (12, 8, "14"));
  ASSERT (LTOA (12, 10, "12"));
  ASSERT (LTOA (12, 16, "C"));
  ASSERT (LTOA (13, 2, "1101"));
  ASSERT (LTOA (13, 8, "15"));
//#elif {part} == 2
  ASSERT (LTOA (13, 10, "13"));
  ASSERT (LTOA (13, 16, "D"));
  ASSERT (LTOA (14, 2, "1110"));
  ASSERT (LTOA (14, 8, "16"));
  ASSERT (LTOA (14, 10, "14"));
  ASSERT (LTOA (14, 16, "E"));
  ASSERT (LTOA (15, 2, "1111"));
  ASSERT (LTOA (15, 8, "17"));
  ASSERT (LTOA (15, 10, "15"));
  ASSERT (LTOA (15, 16, "F"));
  ASSERT (LTOA (16, 2, "10000"));
  ASSERT (LTOA (16, 8, "20"));
  ASSERT (LTOA (16, 10, "16"));
  ASSERT (LTOA (16, 16, "10"));
  ASSERT (LTOA (-1, 2, "11111111111111111111111111111111"));
  ASSERT (LTOA (-1, 8, "37777777777"));
  ASSERT (LTOA (-1, 10, "-1"));
  ASSERT (LTOA (-1, 16, "FFFFFFFF"));
  ASSERT (LTOA (-2, 2, "11111111111111111111111111111110"));
  ASSERT (LTOA (-2, 8, "37777777776"));
  ASSERT (LTOA (-2, 10, "-2"));
  ASSERT (LTOA (-2, 16, "FFFFFFFE"));
  ASSERT (LTOA (-3, 2, "11111111111111111111111111111101"));
  ASSERT (LTOA (-3, 8, "37777777775"));
  ASSERT (LTOA (-3, 10, "-3"));
  ASSERT (LTOA (-3, 16, "FFFFFFFD"));
  ASSERT (LTOA (-4, 2, "11111111111111111111111111111100"));
  ASSERT (LTOA (-4, 8, "37777777774"));
  ASSERT (LTOA (-4, 10, "-4"));
  ASSERT (LTOA (-4, 16, "FFFFFFFC"));
  ASSERT (LTOA (-5, 2, "11111111111111111111111111111011"));
  ASSERT (LTOA (-5, 8, "37777777773"));
  ASSERT (LTOA (-5, 10, "-5"));
  ASSERT (LTOA (-5, 16, "FFFFFFFB"));
  ASSERT (LTOA (-6, 2, "11111111111111111111111111111010"));
  ASSERT (LTOA (-6, 8, "37777777772"));
  ASSERT (LTOA (-6, 10, "-6"));
  ASSERT (LTOA (-6, 16, "FFFFFFFA"));
  ASSERT (LTOA (-7, 2, "11111111111111111111111111111001"));
  ASSERT (LTOA (-7, 8, "37777777771"));
  ASSERT (LTOA (-7, 10, "-7"));
  ASSERT (LTOA (-7, 16, "FFFFFFF9"));
  ASSERT (LTOA (-8, 2, "11111111111111111111111111111000"));
  ASSERT (LTOA (-8, 8, "37777777770"));
  ASSERT (LTOA (-8, 10, "-8"));
  ASSERT (LTOA (-8, 16, "FFFFFFF8"));
  ASSERT (LTOA (-9, 2, "11111111111111111111111111110111"));
  ASSERT (LTOA (-9, 8, "37777777767"));
  ASSERT (LTOA (-9, 10, "-9"));
  ASSERT (LTOA (-9, 16, "FFFFFFF7"));
  ASSERT (LTOA (-10, 2, "11111111111111111111111111110110"));
  ASSERT (LTOA (-10, 8, "37777777766"));
  ASSERT (LTOA (-10, 10, "-10"));
  ASSERT (LTOA (-10, 16, "FFFFFFF6"));
//#elif {part} == 3
  ASSERT (LTOA (-11, 2, "11111111111111111111111111110101"));
  ASSERT (LTOA (-11, 8, "37777777765"));
  ASSERT (LTOA (-11, 10, "-11"));
  ASSERT (LTOA (-11, 16, "FFFFFFF5"));
  ASSERT (LTOA (-12, 2, "11111111111111111111111111110100"));
  ASSERT (LTOA (-12, 8, "37777777764"));
  ASSERT (LTOA (-12, 10, "-12"));
  ASSERT (LTOA (-12, 16, "FFFFFFF4"));
  ASSERT (LTOA (-13, 2, "11111111111111111111111111110011"));
  ASSERT (LTOA (-13, 8, "37777777763"));
  ASSERT (LTOA (-13, 10, "-13"));
  ASSERT (LTOA (-13, 16, "FFFFFFF3"));
  ASSERT (LTOA (-14, 2, "11111111111111111111111111110010"));
  ASSERT (LTOA (-14, 8, "37777777762"));
  ASSERT (LTOA (-14, 10, "-14"));
  ASSERT (LTOA (-14, 16, "FFFFFFF2"));
  ASSERT (LTOA (-15, 2, "11111111111111111111111111110001"));
  ASSERT (LTOA (-15, 8, "37777777761"));
  ASSERT (LTOA (-15, 10, "-15"));
  ASSERT (LTOA (-15, 16, "FFFFFFF1"));
  ASSERT (LTOA (-16, 2, "11111111111111111111111111110000"));
  ASSERT (LTOA (-16, 8, "37777777760"));
  ASSERT (LTOA (-16, 10, "-16"));
  ASSERT (LTOA (-16, 16, "FFFFFFF0"));
  ASSERT (LTOA (127, 2, "1111111"));
  ASSERT (LTOA (127, 8, "177"));
  ASSERT (LTOA (127, 10, "127"));
  ASSERT (LTOA (127, 16, "7F"));
  ASSERT (LTOA (-127, 2, "11111111111111111111111110000001"));
  ASSERT (LTOA (-127, 8, "37777777601"));
  ASSERT (LTOA (-127, 10, "-127"));
  ASSERT (LTOA (-127, 16, "FFFFFF81"));
  ASSERT (LTOA (128, 2, "10000000"));
  ASSERT (LTOA (128, 8, "200"));
  ASSERT (LTOA (128, 10, "128"));
  ASSERT (LTOA (128, 16, "80"));
  ASSERT (LTOA (-128, 2, "11111111111111111111111110000000"));
  ASSERT (LTOA (-128, 8, "37777777600"));
  ASSERT (LTOA (-128, 10, "-128"));
  ASSERT (LTOA (-128, 16, "FFFFFF80"));
  ASSERT (LTOA (255, 2, "11111111"));
  ASSERT (LTOA (255, 8, "377"));
  ASSERT (LTOA (255, 10, "255"));
  ASSERT (LTOA (255, 16, "FF"));
  ASSERT (LTOA (-255, 2, "11111111111111111111111100000001"));
  ASSERT (LTOA (-255, 8, "37777777401"));
  ASSERT (LTOA (-255, 10, "-255"));
  ASSERT (LTOA (-255, 16, "FFFFFF01"));
  ASSERT (LTOA (256, 2, "100000000"));
  ASSERT (LTOA (256, 8, "400"));
  ASSERT (LTOA (256, 10, "256"));
  ASSERT (LTOA (256, 16, "100"));
  ASSERT (LTOA (-256, 2, "11111111111111111111111100000000"));
  ASSERT (LTOA (-256, 8, "37777777400"));
//#elif {part} == 4
  ASSERT (LTOA (-256, 10, "-256"));
  ASSERT (LTOA (-256, 16, "FFFFFF00"));
  ASSERT (LTOA (0x12345678, 2, "10010001101000101011001111000"));
  ASSERT (LTOA (0x12345678, 8, "2215053170"));
  ASSERT (LTOA (0x12345678, 10, "305419896"));
  ASSERT (LTOA (0x12345678, 16, "12345678"));
  ASSERT (LTOA (-0x12345678, 2, "11101101110010111010100110001000"));
  ASSERT (LTOA (-0x12345678, 8, "35562724610"));
  ASSERT (LTOA (-0x12345678, 10, "-305419896"));
  ASSERT (LTOA (-0x12345678, 16, "EDCBA988"));
  ASSERT (LTOA (0x70abcdef, 2, "1110000101010111100110111101111"));
  ASSERT (LTOA (0x70abcdef, 8, "16052746757"));
  ASSERT (LTOA (0x70abcdef, 10, "1890307567"));
  ASSERT (LTOA (0x70abcdef, 16, "70ABCDEF"));
  ASSERT (LTOA (-0x70abcdef, 2, "10001111010101000011001000010001"));
  ASSERT (LTOA (-0x70abcdef, 8, "21725031021"));
  ASSERT (LTOA (-0x70abcdef, 10, "-1890307567"));
  ASSERT (LTOA (-0x70abcdef, 16, "8F543211"));
  ASSERT (LTOA (01234567, 2, "1010011100101110111"));
  ASSERT (LTOA (01234567, 8, "1234567"));
  ASSERT (LTOA (01234567, 10, "342391"));
  ASSERT (LTOA (01234567, 16, "53977"));
  ASSERT (LTOA (-01234567, 2, "11111111111110101100011010001001"));
  ASSERT (LTOA (-01234567, 8, "37776543211"));
  ASSERT (LTOA (-01234567, 10, "-342391"));
  ASSERT (LTOA (-01234567, 16, "FFFAC689"));
  ASSERT (LTOA (32767, 2, "111111111111111"));
  ASSERT (LTOA (32767, 8, "77777"));
  ASSERT (LTOA (32767, 10, "32767"));
  ASSERT (LTOA (32767, 16, "7FFF"));
  ASSERT (LTOA (-32767, 2, "11111111111111111000000000000001"));
  ASSERT (LTOA (-32767, 8, "37777700001"));
  ASSERT (LTOA (-32767, 10, "-32767"));
  ASSERT (LTOA (-32767, 16, "FFFF8001"));
  ASSERT (LTOA (32768, 2, "1000000000000000"));
  ASSERT (LTOA (32768, 8, "100000"));
  ASSERT (LTOA (32768, 10, "32768"));
  ASSERT (LTOA (32768, 16, "8000"));
  ASSERT (LTOA (-32768, 2, "11111111111111111000000000000000"));
  ASSERT (LTOA (-32768, 8, "37777700000"));
  ASSERT (LTOA (-32768, 10, "-32768"));
  ASSERT (LTOA (-32768, 16, "FFFF8000"));
  ASSERT (LTOA (0x7fffffff, 2, "1111111111111111111111111111111"));
  ASSERT (LTOA (0x7fffffff, 8, "17777777777"));
  ASSERT (LTOA (0x7fffffff, 10, "2147483647"));
  ASSERT (LTOA (0x7fffffff, 16, "7FFFFFFF"));
  ASSERT (LTOA (-0x7fffffff, 2, "10000000000000000000000000000001"));
  ASSERT (LTOA (-0x7fffffff, 8, "20000000001"));
  ASSERT (LTOA (-0x7fffffff, 10, "-2147483647"));
  ASSERT (LTOA (-0x7fffffff, 16, "80000001"));
  ASSERT (LTOA (-0x80000000, 2, "10000000000000000000000000000000"));
  ASSERT (LTOA (-0x80000000, 8, "20000000000"));
  ASSERT (LTOA (-0x80000000, 10, "-2147483648"));
  ASSERT (LTOA (-0x80000000, 16, "80000000"));
//#endif /* {part} */
#undef LTOA
#endif /* TEST_ltoa */
}


void test_ultoa (void)
{
#ifdef TEST_ultoa
//#if {part} == 1
  ASSERT (ULTOA (0, 2, "0"));
  ASSERT (ULTOA (0, 8, "0"));
  ASSERT (ULTOA (0, 10, "0"));
  ASSERT (ULTOA (0, 16, "0"));
  ASSERT (ULTOA (1, 2, "1"));
  ASSERT (ULTOA (1, 8, "1"));
  ASSERT (ULTOA (1, 10, "1"));
  ASSERT (ULTOA (1, 16, "1"));
  ASSERT (ULTOA (2, 2, "10"));
  ASSERT (ULTOA (2, 8, "2"));
  ASSERT (ULTOA (2, 10, "2"));
  ASSERT (ULTOA (2, 16, "2"));
  ASSERT (ULTOA (3, 2, "11"));
  ASSERT (ULTOA (3, 8, "3"));
  ASSERT (ULTOA (3, 10, "3"));
  ASSERT (ULTOA (3, 16, "3"));
  ASSERT (ULTOA (4, 2, "100"));
  ASSERT (ULTOA (4, 8, "4"));
  ASSERT (ULTOA (4, 10, "4"));
  ASSERT (ULTOA (4, 16, "4"));
  ASSERT (ULTOA (5, 2, "101"));
  ASSERT (ULTOA (5, 8, "5"));
  ASSERT (ULTOA (5, 10, "5"));
  ASSERT (ULTOA (5, 16, "5"));
  ASSERT (ULTOA (6, 2, "110"));
  ASSERT (ULTOA (6, 8, "6"));
  ASSERT (ULTOA (6, 10, "6"));
  ASSERT (ULTOA (6, 16, "6"));
//#elif {part} == 2
  ASSERT (ULTOA (7, 2, "111"));
  ASSERT (ULTOA (7, 8, "7"));
  ASSERT (ULTOA (7, 10, "7"));
  ASSERT (ULTOA (7, 16, "7"));
  ASSERT (ULTOA (8, 2, "1000"));
  ASSERT (ULTOA (8, 8, "10"));
  ASSERT (ULTOA (8, 10, "8"));
  ASSERT (ULTOA (8, 16, "8"));
  ASSERT (ULTOA (9, 2, "1001"));
  ASSERT (ULTOA (9, 8, "11"));
  ASSERT (ULTOA (9, 10, "9"));
  ASSERT (ULTOA (9, 16, "9"));
  ASSERT (ULTOA (10, 2, "1010"));
  ASSERT (ULTOA (10, 8, "12"));
  ASSERT (ULTOA (10, 10, "10"));
  ASSERT (ULTOA (10, 16, "A"));
  ASSERT (ULTOA (11, 2, "1011"));
  ASSERT (ULTOA (11, 8, "13"));
  ASSERT (ULTOA (11, 10, "11"));
  ASSERT (ULTOA (11, 16, "B"));
  ASSERT (ULTOA (12, 2, "1100"));
  ASSERT (ULTOA (12, 8, "14"));
  ASSERT (ULTOA (12, 10, "12"));
  ASSERT (ULTOA (12, 16, "C"));
  ASSERT (ULTOA (13, 2, "1101"));
  ASSERT (ULTOA (13, 8, "15"));
  ASSERT (ULTOA (13, 10, "13"));
  ASSERT (ULTOA (13, 16, "D"));
//#elif {part} == 3
  ASSERT (ULTOA (14, 2, "1110"));
  ASSERT (ULTOA (14, 8, "16"));
  ASSERT (ULTOA (14, 10, "14"));
  ASSERT (ULTOA (14, 16, "E"));
  ASSERT (ULTOA (15, 2, "1111"));
  ASSERT (ULTOA (15, 8, "17"));
  ASSERT (ULTOA (15, 10, "15"));
  ASSERT (ULTOA (15, 16, "F"));
  ASSERT (ULTOA (16, 2, "10000"));
  ASSERT (ULTOA (16, 8, "20"));
  ASSERT (ULTOA (16, 10, "16"));
  ASSERT (ULTOA (16, 16, "10"));
  ASSERT (ULTOA (127, 2, "1111111"));
  ASSERT (ULTOA (127, 8, "177"));
  ASSERT (ULTOA (127, 10, "127"));
  ASSERT (ULTOA (127, 16, "7F"));
  ASSERT (ULTOA (255, 2, "11111111"));
  ASSERT (ULTOA (255, 8, "377"));
  ASSERT (ULTOA (255, 10, "255"));
  ASSERT (ULTOA (255, 16, "FF"));
  ASSERT (ULTOA (65535, 2, "1111111111111111"));
  ASSERT (ULTOA (65535, 8, "177777"));
  ASSERT (ULTOA (65535, 10, "65535"));
  ASSERT (ULTOA (65535, 16, "FFFF"));
  ASSERT (ULTOA (65536, 2, "10000000000000000"));
  ASSERT (ULTOA (65536, 8, "200000"));
  ASSERT (ULTOA (65536, 10, "65536"));
  ASSERT (ULTOA (65536, 16, "10000"));
//#elif {part} == 4
  ASSERT (ULTOA (0xffffffff, 2, "11111111111111111111111111111111"));
  ASSERT (ULTOA (0xffffffff, 8, "37777777777"));
  ASSERT (ULTOA (0xffffffff, 10, "4294967295"));
  ASSERT (ULTOA (0xffffffff, 16, "FFFFFFFF"));
  ASSERT (ULTOA (0x12345678, 2, "10010001101000101011001111000"));
  ASSERT (ULTOA (0x12345678, 8, "2215053170"));
  ASSERT (ULTOA (0x12345678, 10, "305419896"));
  ASSERT (ULTOA (0x12345678, 16, "12345678"));
  ASSERT (ULTOA (0x56789abc, 2, "1010110011110001001101010111100"));
  ASSERT (ULTOA (0x56789abc, 8, "12636115274"));
  ASSERT (ULTOA (0x56789abc, 10, "1450744508"));
  ASSERT (ULTOA (0x56789abc, 16, "56789ABC"));
  ASSERT (ULTOA (0xdef01234, 2, "11011110111100000001001000110100"));
  ASSERT (ULTOA (0xdef01234, 8, "33674011064"));
  ASSERT (ULTOA (0xdef01234, 10, "3740275252"));
  ASSERT (ULTOA (0xdef01234, 16, "DEF01234"));
  ASSERT (ULTOA (01234567, 2, "1010011100101110111"));
  ASSERT (ULTOA (01234567, 8, "1234567"));
  ASSERT (ULTOA (01234567, 10, "342391"));
  ASSERT (ULTOA (01234567, 16, "53977"));
  ASSERT (ULTOA (0x7fffffff, 2, "1111111111111111111111111111111"));
  ASSERT (ULTOA (0x7fffffff, 8, "17777777777"));
  ASSERT (ULTOA (0x7fffffff, 10, "2147483647"));
  ASSERT (ULTOA (0x7fffffff, 16, "7FFFFFFF"));
  ASSERT (ULTOA (0x80000000, 2, "10000000000000000000000000000000"));
  ASSERT (ULTOA (0x80000000, 8, "20000000000"));
  ASSERT (ULTOA (0x80000000, 10, "2147483648"));
  ASSERT (ULTOA (0x80000000, 16, "80000000"));
  ASSERT (ULTOA (0xffffffff, 2, "11111111111111111111111111111111"));
  ASSERT (ULTOA (0xffffffff, 8, "37777777777"));
  ASSERT (ULTOA (0xffffffff, 10, "4294967295"));
  ASSERT (ULTOA (0xffffffff, 16, "FFFFFFFF"));
//#endif /* {part} */
#undef ULTOA
#endif /*TEST_ultoa */
}
