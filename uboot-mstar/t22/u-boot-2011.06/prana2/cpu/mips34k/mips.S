##=============================================================================
##
##	mips.S
##
##	MIPS assembly code
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 2005, 2006 eCosCentric Limited
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s): 	 nickg
## Contributors: nickg, dmoseley
## Date:	1998-02-04
## Purpose:	MIPS assembly code
## Description:
##
##
##
##
######DESCRIPTIONEND####
##
##=============================================================================

##include <pkgconf/system.h>
##include <pkgconf/hal.h>
##include <pkgconf/hal_mips.h>

##ifdef CYGPKG_KERNEL
## include <pkgconf/kernel.h>
##endif

##include <cyg/hal/arch.inc>
##include <cyg/hal/hal_arch.h>

#include "mips.inc"

	.macro	mfc0_delay
	nop
	nop
	nop
	nop
	.endm

	# Clear execution hazard, MIPS32 R1
	.macro	clear_ehb
	nop
	nop
	nop
	.endm


##-----------------------------------------------------------------------------
## Interrupt handling
##
## Beware of nops in this code. They fill delay slots and avoid CP0 hazards
## that might otherwise cause following code to run in the wrong state or
## cause a resource conflict.

#ifndef CYGHWR_HAL_INTERRUPT_ENABLE_DISABLE_RESTORE_DEFINED

        .section .text.hal_disable_interrupts,"ax",@progbits
FUNC_START(hal_disable_interrupts)
        mfc0    v1,$12
        mfc0_delay
        move    v0,v1
        and     v1,v1,0XFFFFFFFE
        mtc0    v1,$12
	    clear_ehb
        and     v0,v0,0X1
        jr      ra
        nop
FUNC_END(hal_disable_interrupts)

        .section .text.hal_enable_interrupts,"ax",@progbits
FUNC_START(hal_enable_interrupts)
        mfc0    v1,$12
        mfc0_delay
        or      v1,v1,1
        mtc0    v1,$12
	    clear_ehb
        jr      ra
        nop
FUNC_END(hal_enable_interrupts)

        .section .text.hal_restore_interrupts,"ax",@progbits
FUNC_START(hal_restore_interrupts)
        mfc0    v1,$12
        mfc0_delay
        and     a0,a0,0x1
        or      v1,v1,a0
        mtc0    v1,$12
	    clear_ehb
        jr      ra
        nop
FUNC_END(hal_restore_interrupts)

        .section .text.hal_query_interrupts,"ax",@progbits
FUNC_START(hal_query_interrupts)
        mfc0    v0,$12
        mfc0_delay
        and     v0,v0,0x1
        jr      ra
        nop
FUNC_END(hal_query_interrupts)

#endif

##-----------------------------------------------------------------------------
## Interrupt mask/unmask/ack routines for the IM bits in status and
## cause registers. Mainly for the use of platform specific
## implementations that need to also work in MIPS16 mode, and
## hence cannot inline this code. These routines will only be
## included if they are called.

        .section .text.hal_interrupt_mask_im,"ax",@progbits
FUNC_START(hal_interrupt_mask_im)
        mfc0    v1,$12
        la      v0,0x00000400
        sllv    v0,v0,a0
        nor     v0,v0,z0
        and     v1,v1,v0
        mtc0    v1,$12
	    clear_ehb
        jr      ra
         nop
FUNC_END(hal_interrupt_mask_im)

        .section .text.hal_interrupt_unmask_im,"ax",@progbits
FUNC_START(hal_interrupt_unmask_im)
        mfc0    v1,$12
        la      v0,0x00000400
        sllv    v0,v0,a0
        or      v1,v1,v0
        mtc0    v1,$12
	    clear_ehb
        jr      ra
         nop
FUNC_END(hal_interrupt_unmask_im)

        .section .text.hal_interrupt_acknowledge_im,"ax",@progbits
FUNC_START(hal_interrupt_acknowledge_im)
        mfc0   v1,$13
        la     v0,0x00000400
        sllv   v0,v0,a0
        nor    v0,v0,z0
        and    v1,v1,v0
        mtc0   v1,$13
	    clear_ehb
        jr      ra
         nop
FUNC_END(hal_interrupt_acknowledge_im)

##-----------------------------------------------------------------------------
## Clock access routines

        .section .text.hal_clock_initialize,"ax",@progbits
FUNC_START(hal_clock_initialize)
        mtc0    $0,$9
	    clear_ehb
        mtc0    a0,$11
	    clear_ehb
        jr      ra
        nop
FUNC_END(hal_clock_initialize)

        .section .text.hal_clock_reset,"ax",@progbits

#if defined(CYG_HWR_HAL_MIPS_MSTAR_PLATFORM_MONTAGE) || \
    defined(CYG_HWR_HAL_MIPS_MSTAR_PLATFORM_MONTAGE2) || \
    defined(CYG_HWR_HAL_MIPS_MSTAR_PLATFORM_MONTAGE3)
FUNC_START(hal_clock_reset)
        mtc0    $0,$9
        clear_ehb
        mtc0    a0,$11
	    clear_ehb
        jr      ra
        nop
FUNC_END(hal_clock_reset)
#else
#kevinhuang20061026, keep COUNT increase continuously
FUNC_START(hal_clock_reset)
        mfc0    v0,$9
        mfc0_delay
        addu    v0,a0
        mtc0    v0,$11
        clear_ehb
        jr      ra
        nop
FUNC_END(hal_clock_reset)
#endif

        .section .text.hal_clock_read,"ax",@progbits
FUNC_START(hal_clock_read)
        mfc0    v0,$9
        jr      ra
        nop
FUNC_END(hal_clock_read)

##-----------------------------------------------------------------------------
## end of mips.S
