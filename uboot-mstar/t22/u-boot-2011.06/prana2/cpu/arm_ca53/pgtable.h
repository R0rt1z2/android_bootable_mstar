/*
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Level 3 descriptor (PTE).
 */
#define PTE_TYPE_MASK		( 3 << 0)
#define PTE_TYPE_FAULT		( 0 << 0)
#define PTE_TYPE_PAGE		( 3 << 0)
#define PTE_USER		( 1 << 6)		/* AP[1] */
#define PTE_RDONLY		( 1 << 7)		/* AP[2] */
#define PTE_SHARED		( 3 << 8)		/* SH[1:0], inner shareable */
#define PTE_AF			( 1 << 10)	/* Access Flag */
#define PTE_NG			( 1 << 11)	/* nG */
#define PTE_PXN			( 1 << 53)	/* Privileged XN */
#define PTE_UXN			( 1 << 54)	/* User XN */

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PTE_ATTRINDX(t)		(t << 2)
#define PTE_ATTRINDX_MASK	(7 << 2)
#define MAIR(attr, mt)  ((attr) << ((mt) * 8))


/*
 * 40-bit physical address supported.
 */
#define VA_BITS                 (39)
#define PHYS_MASK_SHIFT		(40)
#define PHYS_MASK		((1 << PHYS_MASK_SHIFT) - 1)

/*
 * TCR flags.
 */
#define TCR_TxSZ(x)		(((64 - (x)) << 16) | ((64 - (x)) << 0))
#define TCR_IRGN_NC		((0 << 8) | (0 << 24))
#define TCR_IRGN_WBWA		((1 << 8) | (1 << 24))
#define TCR_IRGN_WT		((2 << 8) | (2 << 24))
#define TCR_IRGN_WBnWA		((3 << 8) | (3 << 24))
#define TCR_IRGN_MASK		((3 << 8) | (3 << 24))
#define TCR_ORGN_NC		((0 << 10) | (0 << 26))
#define TCR_ORGN_WBWA		((1 << 10) | (1 << 26))
#define TCR_ORGN_WT		((2 << 10) | (2 << 26))
#define TCR_ORGN_WBnWA		((3 << 10) | (3 << 26))
#define TCR_ORGN_MASK		((3 << 10) | (3 << 26))
#define TCR_SHARED		((3 << 12) | (3 << 28))
#define TCR_TG0_64K		(1 << 14)
#define TCR_TG1_64K		(1 << 30)
#define TCR_IPS_40BIT		(2 << 32)
#define TCR_ASID16		(1 << 36)

/* 
 * PTWs cacheable, inner/outer WBWA not shareable 
 */
#define TCR_FLAGS	TCR_IRGN_WBWA | TCR_ORGN_WBWA

/*
 * Memory types available.
 */
#define MT_DEVICE_nGnRnE	0
#define MT_DEVICE_nGnRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4

/*
 * Memory map attributes.
 */
#define PTE_FLAGS	        PTE_TYPE_PAGE | PTE_AF | PTE_SHARED
#define NORM_MMUFLAGS	        PTE_ATTRINDX(MT_NORMAL) | PTE_FLAGS
#define DEV_MMUFLAGS	        PTE_ATTRINDX(MT_DEVICE_nGnRnE) | PTE_FLAGS

/*
 * With LPAE and 64KB pages, there are 2 levels of page tables. Each level has
 * 8192 entries of 8 bytes each, occupying a 64KB page. Levels 0 and 1 are not
 * used. The 2nd level table (PGD for Linux) can cover a range of 4TB, each
 * entry representing 512MB. The user and kernel address spaces are limited to
 * 512GB and therefore we only use 1024 entries in the PGD.
 */
#define PTRS_PER_PTE		8192
#define PTRS_PER_PGD		1024

/*
 * PGDIR_SHIFT determines the size a top-level page table entry can map.
 */
#define PGDIR_SHIFT		29

/*
 * Page table size
 */
#define PAGE_SHIFT              16
#define PAGE_SIZE               (1 << PAGE_SHIFT)
#define BLOCK_SHIFT             PAGE_SHIFT
#define BLOCK_SIZE              PAGE_SIZE
#define TTB1_SIZE               (PTRS_PER_PGD * 8)
#define TTB0_SIZE	        (PTRS_PER_PTE * 8)

