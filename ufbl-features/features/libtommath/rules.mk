#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_PATH := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_PATH) \
		-I$(LOCAL_PATH)/src/

# Default CFLAGS
CFLAGS += -Os -funroll-loops \
		-fomit-frame-pointer \
		-DLTM_NO_FILE

UNUSED_OBJS += \
	$(LOCAL_PATH)/src/bn_mp_fread.o \
	$(LOCAL_PATH)/src/bn_mp_fwrite.o \
	
OBJS += \
	$(LOCAL_PATH)/src/bncore.o \
	$(LOCAL_PATH)/src/bn_error.o \
	$(LOCAL_PATH)/src/bn_fast_mp_invmod.o \
	$(LOCAL_PATH)/src/bn_fast_mp_montgomery_reduce.o \
	$(LOCAL_PATH)/src/bn_fast_s_mp_mul_digs.o \
	$(LOCAL_PATH)/src/bn_fast_s_mp_mul_high_digs.o \
	$(LOCAL_PATH)/src/bn_fast_s_mp_sqr.o \
	$(LOCAL_PATH)/src/bn_mp_2expt.o \
	$(LOCAL_PATH)/src/bn_mp_abs.o \
	$(LOCAL_PATH)/src/bn_mp_add.o \
	$(LOCAL_PATH)/src/bn_mp_add_d.o \
	$(LOCAL_PATH)/src/bn_mp_addmod.o \
	$(LOCAL_PATH)/src/bn_mp_and.o \
	$(LOCAL_PATH)/src/bn_mp_clamp.o \
	$(LOCAL_PATH)/src/bn_mp_clear.o \
	$(LOCAL_PATH)/src/bn_mp_clear_multi.o \
	$(LOCAL_PATH)/src/bn_mp_cmp.o \
	$(LOCAL_PATH)/src/bn_mp_cmp_d.o \
	$(LOCAL_PATH)/src/bn_mp_cmp_mag.o \
	$(LOCAL_PATH)/src/bn_mp_cnt_lsb.o \
	$(LOCAL_PATH)/src/bn_mp_copy.o \
	$(LOCAL_PATH)/src/bn_mp_count_bits.o \
	$(LOCAL_PATH)/src/bn_mp_div_2.o \
	$(LOCAL_PATH)/src/bn_mp_div_2d.o \
	$(LOCAL_PATH)/src/bn_mp_div_3.o \
	$(LOCAL_PATH)/src/bn_mp_div.o \
	$(LOCAL_PATH)/src/bn_mp_div_d.o \
	$(LOCAL_PATH)/src/bn_mp_dr_is_modulus.o \
	$(LOCAL_PATH)/src/bn_mp_dr_reduce.o \
	$(LOCAL_PATH)/src/bn_mp_dr_setup.o \
	$(LOCAL_PATH)/src/bn_mp_exch.o \
	$(LOCAL_PATH)/src/bn_mp_export.o \
	$(LOCAL_PATH)/src/bn_mp_expt_d.o \
	$(LOCAL_PATH)/src/bn_mp_expt_d_ex.o \
	$(LOCAL_PATH)/src/bn_mp_exptmod.o \
	$(LOCAL_PATH)/src/bn_mp_exptmod_fast.o \
	$(LOCAL_PATH)/src/bn_mp_exteuclid.o \
	$(LOCAL_PATH)/src/bn_mp_gcd.o \
	$(LOCAL_PATH)/src/bn_mp_get_int.o \
	$(LOCAL_PATH)/src/bn_mp_get_long.o \
	$(LOCAL_PATH)/src/bn_mp_get_long_long.o \
	$(LOCAL_PATH)/src/bn_mp_grow.o \
	$(LOCAL_PATH)/src/bn_mp_import.o \
	$(LOCAL_PATH)/src/bn_mp_init.o \
	$(LOCAL_PATH)/src/bn_mp_init_copy.o \
	$(LOCAL_PATH)/src/bn_mp_init_multi.o \
	$(LOCAL_PATH)/src/bn_mp_init_set.o \
	$(LOCAL_PATH)/src/bn_mp_init_set_int.o \
	$(LOCAL_PATH)/src/bn_mp_init_size.o \
	$(LOCAL_PATH)/src/bn_mp_invmod.o \
	$(LOCAL_PATH)/src/bn_mp_invmod_slow.o \
	$(LOCAL_PATH)/src/bn_mp_is_square.o \
	$(LOCAL_PATH)/src/bn_mp_jacobi.o \
	$(LOCAL_PATH)/src/bn_mp_karatsuba_mul.o \
	$(LOCAL_PATH)/src/bn_mp_karatsuba_sqr.o \
	$(LOCAL_PATH)/src/bn_mp_lcm.o \
	$(LOCAL_PATH)/src/bn_mp_lshd.o \
	$(LOCAL_PATH)/src/bn_mp_mod_2d.o \
	$(LOCAL_PATH)/src/bn_mp_mod.o \
	$(LOCAL_PATH)/src/bn_mp_mod_d.o \
	$(LOCAL_PATH)/src/bn_mp_montgomery_calc_normalization.o \
	$(LOCAL_PATH)/src/bn_mp_montgomery_reduce.o \
	$(LOCAL_PATH)/src/bn_mp_montgomery_setup.o \
	$(LOCAL_PATH)/src/bn_mp_mul_2.o \
	$(LOCAL_PATH)/src/bn_mp_mul_2d.o \
	$(LOCAL_PATH)/src/bn_mp_mul.o \
	$(LOCAL_PATH)/src/bn_mp_mul_d.o \
	$(LOCAL_PATH)/src/bn_mp_mulmod.o \
	$(LOCAL_PATH)/src/bn_mp_neg.o \
	$(LOCAL_PATH)/src/bn_mp_n_root.o \
	$(LOCAL_PATH)/src/bn_mp_n_root_ex.o \
	$(LOCAL_PATH)/src/bn_mp_or.o \
	$(LOCAL_PATH)/src/bn_mp_prime_fermat.o \
	$(LOCAL_PATH)/src/bn_mp_prime_is_divisible.o \
	$(LOCAL_PATH)/src/bn_mp_prime_is_prime.o \
	$(LOCAL_PATH)/src/bn_mp_prime_miller_rabin.o \
	$(LOCAL_PATH)/src/bn_mp_prime_next_prime.o \
	$(LOCAL_PATH)/src/bn_mp_prime_rabin_miller_trials.o \
	$(LOCAL_PATH)/src/bn_mp_prime_random_ex.o \
	$(LOCAL_PATH)/src/bn_mp_radix_size.o \
	$(LOCAL_PATH)/src/bn_mp_radix_smap.o \
	$(LOCAL_PATH)/src/bn_mp_rand.o \
	$(LOCAL_PATH)/src/bn_mp_read_radix.o \
	$(LOCAL_PATH)/src/bn_mp_read_signed_bin.o \
	$(LOCAL_PATH)/src/bn_mp_read_unsigned_bin.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_2k.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_2k_l.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_2k_setup.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_2k_setup_l.o \
	$(LOCAL_PATH)/src/bn_mp_reduce.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_is_2k.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_is_2k_l.o \
	$(LOCAL_PATH)/src/bn_mp_reduce_setup.o \
	$(LOCAL_PATH)/src/bn_mp_rshd.o \
	$(LOCAL_PATH)/src/bn_mp_set.o \
	$(LOCAL_PATH)/src/bn_mp_set_int.o \
	$(LOCAL_PATH)/src/bn_mp_set_long.o \
	$(LOCAL_PATH)/src/bn_mp_set_long_long.o \
	$(LOCAL_PATH)/src/bn_mp_shrink.o \
	$(LOCAL_PATH)/src/bn_mp_signed_bin_size.o \
	$(LOCAL_PATH)/src/bn_mp_sqr.o \
	$(LOCAL_PATH)/src/bn_mp_sqrmod.o \
	$(LOCAL_PATH)/src/bn_mp_sqrt.o \
	$(LOCAL_PATH)/src/bn_mp_sub.o \
	$(LOCAL_PATH)/src/bn_mp_sub_d.o \
	$(LOCAL_PATH)/src/bn_mp_submod.o \
	$(LOCAL_PATH)/src/bn_mp_toom_mul.o \
	$(LOCAL_PATH)/src/bn_mp_toom_sqr.o \
	$(LOCAL_PATH)/src/bn_mp_toradix.o \
	$(LOCAL_PATH)/src/bn_mp_toradix_n.o \
	$(LOCAL_PATH)/src/bn_mp_to_signed_bin.o \
	$(LOCAL_PATH)/src/bn_mp_to_signed_bin_n.o \
	$(LOCAL_PATH)/src/bn_mp_to_unsigned_bin.o \
	$(LOCAL_PATH)/src/bn_mp_to_unsigned_bin_n.o \
	$(LOCAL_PATH)/src/bn_mp_unsigned_bin_size.o \
	$(LOCAL_PATH)/src/bn_mp_xor.o \
	$(LOCAL_PATH)/src/bn_mp_zero.o \
	$(LOCAL_PATH)/src/bn_prime_tab.o \
	$(LOCAL_PATH)/src/bn_reverse.o \
	$(LOCAL_PATH)/src/bn_s_mp_add.o \
	$(LOCAL_PATH)/src/bn_s_mp_exptmod.o \
	$(LOCAL_PATH)/src/bn_s_mp_mul_digs.o \
	$(LOCAL_PATH)/src/bn_s_mp_mul_high_digs.o \
	$(LOCAL_PATH)/src/bn_s_mp_sqr.o \
	$(LOCAL_PATH)/src/bn_s_mp_sub.o
