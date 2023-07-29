/* ddconfig_win.h. This file replaces ddconfig.h on Windows */

#ifndef DDCONFIG_WIN_HEADER
#define DDCONFIG_WIN_HEADER

#define TYPE_BYTE char
#define TYPE_WORD short
#define TYPE_DWORD int
#define TYPE_QWORD long long int

#define UCSOCKET_T int
#define VERSIONSTR "0.6-pre69"

#define STATISTIC

#define STDC_HEADERS 1
#define SOCKET_AVAIL 1
#define HEADER_SOCKET <winsock2.h>

#define YY_NO_UNISTD_H 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
#if defined __BIG_ENDIAN__
#define WORDS_BIGENDIAN 1
#endif
#else
#ifndef WORDS_BIGENDIAN
#undef WORDS_BIGENDIAN
#endif
#endif

/* ucsim custom defines */
#define DD_TRUE 1
#define DD_FALSE 0
#define NIL 0

#endif /* DDCONFIG_WIN_HEADER */
