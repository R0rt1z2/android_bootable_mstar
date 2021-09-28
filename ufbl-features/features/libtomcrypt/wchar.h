/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#ifndef __WCHAR_H
#define __WCHAR_H

#ifndef FILE
#define FILE void
#endif

/*
 * We define wchar_t to same as libtomcrypt expects.
 */
typedef ulong32 wchar_t;

#endif
