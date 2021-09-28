# Copyright (c) 2016-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved

#
# Common list of objects files we want to include
#
LIBTOMMATH_OBJECTS += \
    src/bncore.o \
    src/bn_error.o \
    src/bn_fast_mp_invmod.o \
    src/bn_fast_mp_montgomery_reduce.o \
    src/bn_fast_s_mp_mul_digs.o \
    src/bn_fast_s_mp_mul_high_digs.o \
    src/bn_fast_s_mp_sqr.o \
    src/bn_mp_2expt.o \
    src/bn_mp_abs.o \
    src/bn_mp_add.o \
    src/bn_mp_add_d.o \
    src/bn_mp_addmod.o \
    src/bn_mp_and.o \
    src/bn_mp_clamp.o \
    src/bn_mp_clear.o \
    src/bn_mp_clear_multi.o \
    src/bn_mp_cmp.o \
    src/bn_mp_cmp_d.o \
    src/bn_mp_cmp_mag.o \
    src/bn_mp_cnt_lsb.o \
    src/bn_mp_copy.o \
    src/bn_mp_count_bits.o \
    src/bn_mp_div_2.o \
    src/bn_mp_div_2d.o \
    src/bn_mp_div_3.o \
    src/bn_mp_div.o \
    src/bn_mp_div_d.o \
    src/bn_mp_dr_is_modulus.o \
    src/bn_mp_dr_reduce.o \
    src/bn_mp_dr_setup.o \
    src/bn_mp_exch.o \
    src/bn_mp_export.o \
    src/bn_mp_expt_d.o \
    src/bn_mp_expt_d_ex.o \
    src/bn_mp_exptmod.o \
    src/bn_mp_exptmod_fast.o \
    src/bn_mp_exteuclid.o \
    src/bn_mp_gcd.o \
    src/bn_mp_get_int.o \
    src/bn_mp_get_long.o \
    src/bn_mp_get_long_long.o \
    src/bn_mp_grow.o \
    src/bn_mp_import.o \
    src/bn_mp_init.o \
    src/bn_mp_init_copy.o \
    src/bn_mp_init_multi.o \
    src/bn_mp_init_set.o \
    src/bn_mp_init_set_int.o \
    src/bn_mp_init_size.o \
    src/bn_mp_invmod.o \
    src/bn_mp_invmod_slow.o \
    src/bn_mp_is_square.o \
    src/bn_mp_jacobi.o \
    src/bn_mp_karatsuba_mul.o \
    src/bn_mp_karatsuba_sqr.o \
    src/bn_mp_lcm.o \
    src/bn_mp_lshd.o \
    src/bn_mp_mod_2d.o \
    src/bn_mp_mod.o \
    src/bn_mp_mod_d.o \
    src/bn_mp_montgomery_calc_normalization.o \
    src/bn_mp_montgomery_reduce.o \
    src/bn_mp_montgomery_setup.o \
    src/bn_mp_mul_2.o \
    src/bn_mp_mul_2d.o \
    src/bn_mp_mul.o \
    src/bn_mp_mul_d.o \
    src/bn_mp_mulmod.o \
    src/bn_mp_neg.o \
    src/bn_mp_n_root.o \
    src/bn_mp_n_root_ex.o \
    src/bn_mp_or.o \
    src/bn_mp_prime_fermat.o \
    src/bn_mp_prime_is_divisible.o \
    src/bn_mp_prime_is_prime.o \
    src/bn_mp_prime_miller_rabin.o \
    src/bn_mp_prime_next_prime.o \
    src/bn_mp_prime_rabin_miller_trials.o \
    src/bn_mp_prime_random_ex.o \
    src/bn_mp_radix_size.o \
    src/bn_mp_radix_smap.o \
    src/bn_mp_rand.o \
    src/bn_mp_read_radix.o \
    src/bn_mp_read_signed_bin.o \
    src/bn_mp_read_unsigned_bin.o \
    src/bn_mp_reduce_2k.o \
    src/bn_mp_reduce_2k_l.o \
    src/bn_mp_reduce_2k_setup.o \
    src/bn_mp_reduce_2k_setup_l.o \
    src/bn_mp_reduce.o \
    src/bn_mp_reduce_is_2k.o \
    src/bn_mp_reduce_is_2k_l.o \
    src/bn_mp_reduce_setup.o \
    src/bn_mp_rshd.o \
    src/bn_mp_set.o \
    src/bn_mp_set_int.o \
    src/bn_mp_set_long.o \
    src/bn_mp_set_long_long.o \
    src/bn_mp_shrink.o \
    src/bn_mp_signed_bin_size.o \
    src/bn_mp_sqr.o \
    src/bn_mp_sqrmod.o \
    src/bn_mp_sqrt.o \
    src/bn_mp_sub.o \
    src/bn_mp_sub_d.o \
    src/bn_mp_submod.o \
    src/bn_mp_toom_mul.o \
    src/bn_mp_toom_sqr.o \
    src/bn_mp_toradix.o \
    src/bn_mp_toradix_n.o \
    src/bn_mp_to_signed_bin.o \
    src/bn_mp_to_signed_bin_n.o \
    src/bn_mp_to_unsigned_bin.o \
    src/bn_mp_to_unsigned_bin_n.o \
    src/bn_mp_unsigned_bin_size.o \
    src/bn_mp_xor.o \
    src/bn_mp_zero.o \
    src/bn_prime_tab.o \
    src/bn_reverse.o \
    src/bn_s_mp_add.o \
    src/bn_s_mp_exptmod.o \
    src/bn_s_mp_mul_digs.o \
    src/bn_s_mp_mul_high_digs.o \
    src/bn_s_mp_sqr.o \
    src/bn_s_mp_sub.o

LIBTOMMATH_CFLAGS += \
    -Os -DLTM_NO_FILE \
    -ffunction-sections -fdata-sections

LIBTOMMATH_HEADERS += \
    -I$(UFBL_PATH)/features/libtommath/src \
    -I$(UFBL_PATH)/features/libtommath

# Begin rules for kbuild U-Boot
ifeq (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))

ccflags-y += $(LIBTOMMATH_CFLAGS) $(LIBTOMMATH_HEADERS)
obj-y += $(LIBTOMMATH_OBJECTS)

else # default rules

include $(TOPDIR)/config.mk

LIBTOMMATH_LIB = $(obj)libtommath.o

CFLAGS += \
    $(LIBTOMMATH_CFLAGS) \
    $(LIBTOMMATH_HEADERS)

COBJS  := $(LIBTOMMATH_OBJECTS)
SRCS   := $(COBJS:.o=.c)
OBJS   := $(addprefix $(obj),$(COBJS))

$(LIBTOMMATH_LIB): $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

endif # (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))
