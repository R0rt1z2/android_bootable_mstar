/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include "ftl.h"

// ===================================
// ftl.c - the code connected to system 
//         (e.g. uboot, linux, eCos, uCOS)
// ===================================

#if MSFTL_UBOOT
    #include "./src/ftl_cfg_uboot.c"
	
#elif MSFTL_LINUX
    #include "./src/ftl_cfg_linux.c"

#elif MSFTL_eCos
    #include "./src/ftl_cfg_eCos.c"
	
#else
    #error "[ftl.c] Err: no platform selected for MStar FTL."
#endif


