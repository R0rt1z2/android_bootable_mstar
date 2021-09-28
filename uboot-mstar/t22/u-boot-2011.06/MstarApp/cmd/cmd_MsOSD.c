/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include <common.h>
#include <command.h>
#include <MsOSD.h>
#if (ENABLE_MODULE_DISPLAY_OSD==1)
U_BOOT_CMD(
    osd_create,    3,   1,  do_create_osdlayer,
    "osd_create - create osd layer \n",
    "create osd layer \n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
);

U_BOOT_CMD(
    osd_destroy,    1,   1,  do_destroy_osdlayer,
    "osd_destroy - destroy osd layer \n",
    "destroy osd layer \n"
);

U_BOOT_CMD(
    osd_flush,    1,   1,  do_flush2screen_osdlayer,
    "osd_flush - flush canvas to screen \n",
    "flush canvas to screen \n"
);

U_BOOT_CMD(
    draw_rect,    6,   1,  do_draw_rect,
    "draw_rect - draw rect with color \n",
    "draw rect \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_pixel,    4,   1,  do_draw_pixel,
    "draw_pixel - draw a pixel with color \n",
    "draw a pixel \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_string,    100,   1,  do_draw_string,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - string: in char\n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - attrib: align attrib, 0: left, 1: middle, 2: right\n"
);

U_BOOT_CMD(
    draw_progress,    5,   1,  do_draw_Progress,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - per: percent in Dec\n"
);

U_BOOT_CMD(
    draw_jpg, CONFIG_SYS_MAXARGS, 1,    do_draw_jpg,
    "blt_jpg   - Blt JPG\n",
    NULL
);
#endif
