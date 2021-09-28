#ifndef OPENSSL_SYS_LK
#include <linux/string.h>
#else
/* FIXME: Remove below once we took out OpenSSL support */
#ifndef __STRING_H__
#define __STRING_H__

#if !defined(_SIZE_T)
#define _SIZE_T
typedef long size_t;
#endif

int strcmp(const char * cs,const char * ct);
int strncmp(const char * cs,const char * ct, size_t count);

#endif

#endif
