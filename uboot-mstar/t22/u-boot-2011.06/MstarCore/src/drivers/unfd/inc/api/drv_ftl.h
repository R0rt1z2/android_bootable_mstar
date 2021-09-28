/*
 * Copyright 2008,2010 Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based (loosely) on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _MSTAR_FTL_H_
#define _MSTAR_FTL_H_

#include <linux/list.h>

#define FTL_DATA_READ		1
#define FTL_DATA_WRITE		2

struct ftl {
	struct list_head link;
	char name[32];
	void *priv;
	uint has_init;
	uint clock;
	char part_config;
	char part_num;
	uint read_bl_len;
	uint write_bl_len;
	u64 capacity;
	block_dev_desc_t block_dev;
};

int ftl_register(struct ftl *ftl);
int ftl_initialize(void);
struct ftl *find_ftl_device(int dev_num);
int ftl_set_dev(int dev_num);
void print_ftl_devices(char separator);
int get_ftl_num(void);

#endif /* _MLC_H_ */
