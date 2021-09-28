/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

#ifndef __UFBL_BOLT_COMPAT_H__
#define __UFBL_BOLT_COMPAT_H__

void *ufbl_calloc(size_t, size_t);
int ufbl_toupper(int);

#define XCALLOC(x,y)		ufbl_calloc(x,y)
#define XTOUPPER(x)		ufbl_toupper(x)

#endif /* __UFBL_BOLT_COMPAT_H__ */
