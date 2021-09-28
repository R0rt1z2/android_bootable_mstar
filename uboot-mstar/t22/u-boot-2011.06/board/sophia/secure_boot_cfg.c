#include <stdio.h>
#include <amzn_secure_boot.h>
#include <amzn_unlock.h>
#include <amzn_secure_flashing.h>
#include <drvGPIO.h>
#include <drvSYS.h>
#include <common.h>

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#include "onetime_unlock_key.h"
#endif

const char *amzn_target_device_name(void)
{
#if defined(CONFIG_MSTAR_BD_M7322_MB170)|| defined(CONFIG_MSTAR_BD_M7322_MB175)
  return "sophia";
#elif defined(CONFIG_MSTAR_BD_M7322_G22)
  return "greta";
#elif defined(CONFIG_MSTAR_BD_M7322_G22_abc123)
  return "abc123";
#elif defined(CONFIG_MSTAR_BD_M7322_abc123)
  return "abc123";
#else
  #error "config TARGET DEVICE properly"
  return "invalid";
#endif
}

#define DEVICE_TYPE_GPIO PAD_LD_SPI_CS

#define SOPHIA_PROTO "00850001"
#define SOPHIA_HVT   "00850002"
#define GRETA_PROTO  "00860001"
#define GRETA_HVT    "00860002"

int gpio_is_production()
{
  char board_id[18] = {0};

  /* prod pin is not ready on Sophia Proto/HVT and Greta HVT */
  idme_get_var_external("board_id", board_id, sizeof(board_id)-1);
  if ( !strncmp(board_id, SOPHIA_PROTO, strlen(SOPHIA_PROTO)) ||
       !strncmp(board_id, SOPHIA_HVT, strlen(SOPHIA_HVT)) ||
       !strncmp(board_id, GRETA_PROTO, strlen(GRETA_PROTO)) ||
       !strncmp(board_id, GRETA_HVT, strlen(GRETA_HVT)) ) {
          return 0;  /* engineering gpio */
  }

  return (!mdrv_gpio_get_level(DEVICE_TYPE_GPIO));
}

int amzn_target_device_type(void)
{
  /* only check anti-rollback status, not device type gpio */
  if (anti_rb_enabled())
    return AMZN_PRODUCTION_DEVICE;
  else
    return AMZN_ENGINEERING_DEVICE;
}

int amzn_get_unlock_code(unsigned char *code, unsigned int *len)
{
  MS_U16 efuse_id[8];

  if (!code || !len || *len < (16 + 1))
    return -1;

  if (!MDrv_SYS_Init())
  {
    printf("MDrv_SYS_Init fail\n");
    return -1;
  }

  if (!MDrv_SYS_GetEfuseDid(efuse_id)) // 64 bits, but real value is 48 bits
  {
    printf("MDrv_SYS_GetEfuseDid fail\n");
    return -1;
  }

  *len = sprintf(code, "%04x%04x%04x%04x",efuse_id[3], efuse_id[2], efuse_id[1], efuse_id[0]);
  // printf("EFUSE ID %16s, len %d\n", code, *len);

  return 0;
}

const unsigned char *amzn_get_unlock_key(unsigned int *key_len)
{
  static const unsigned char unlock_key[] =
    "\x30\x82\x01\x22\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01"
    "\x01\x05\x00\x03\x82\x01\x0f\x00\x30\x82\x01\x0a\x02\x82\x01\x01"
    "\x00\xe3\x89\x0b\x06\xa5\x23\xb3\xf7\x63\x4b\xc8\x64\x81\xab\x27"
    "\x94\xa6\xf7\xbe\x7c\x1e\x22\x03\x3b\xd7\x92\x0c\x0c\x69\x08\x29"
    "\x8d\x62\x91\xed\xbe\x28\xe4\x00\x19\x63\x2e\xa0\x52\x18\xa0\x98"
    "\xdd\x25\xb2\xad\x95\x45\x87\xc5\x2e\x17\x13\xd6\x77\xd2\x37\xca"
    "\x61\xfb\xdd\xf9\x67\x3f\x46\xe0\xb5\xb3\xdf\x8d\x45\x78\x75\xd3"
    "\xaf\xa5\x9d\xc2\x14\xdc\x73\x73\xcb\x39\x40\x95\xe3\xbb\xa7\xd7"
    "\x21\xbb\xa0\xa0\xda\x8e\x21\x43\x00\x3f\xda\x48\x91\xc9\xa2\x94"
    "\x5f\x81\x8a\x9f\xa2\x5b\x39\x16\xce\x0f\x24\x36\xcf\x22\x19\x82"
    "\x3c\x44\xa1\x94\xb0\x62\xb0\xe2\x18\xcd\x59\x01\x46\x8f\x87\xd3"
    "\x56\x1b\x90\xf5\x98\x53\xb8\x79\x97\xe6\xdb\x3d\x7c\x55\xfe\x90"
    "\x86\x6e\xb1\xd5\x70\xca\x83\xb0\x1c\x17\x03\xba\xc6\x70\x90\xb9"
    "\x78\x7a\x27\xba\xad\xbc\xe6\xaf\x4d\xca\xc0\xcd\x18\x02\x02\xb1"
    "\x47\x87\x08\x99\xa9\x2f\x76\x12\x91\x87\x76\x53\x6c\x8e\xae\xd8"
    "\x53\xa0\x3f\x4d\x5d\x5e\x20\xd4\xcf\xef\x41\xc9\x00\xf3\x57\xc8"
    "\xc8\xa1\xbe\xd4\x97\xcd\x7a\x3f\xca\x91\x32\xf3\x40\x87\x16\xa3"
    "\xee\x8c\x3e\xe1\xc2\x95\x91\x30\x09\x41\xd3\xa7\x84\xdd\xb3\xcc"
    "\x03\x02\x03\x01\x00\x01"
    ;

  const int unlock_key_size = sizeof(unlock_key);

  if (!key_len)
    return NULL;

  *key_len = unlock_key_size;
  return unlock_key;
}

int amzn_device_is_unlocked(void)
{
#if defined(UFBL_FEATURE_UNLOCK)
  unsigned char signed_code[SIGNED_UNLOCK_CODE_LEN] = { 0 };

  if ( !idme_get_var_external("unlock_code", signed_code, sizeof(signed_code)) &&
       !amzn_verify_unlock((void*)signed_code, sizeof(signed_code)) ) {
    return 1;
  }
#endif
#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
  if ( amzn_target_is_onetime_unlocked() ) {
    return 1;
  }
#endif
  return 0;
}

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
int amzn_get_one_tu_code(unsigned char *code, unsigned int *len)
{
  static unsigned char code_generated = 0;
  static unsigned char one_tu_code[ONETIME_UNLOCK_CODE_LEN + 1] = {0};

  if (!code || !len || *len < ONETIME_UNLOCK_CODE_LEN)
    return -1;

  if (!code_generated) {
/**
 * Different SoC/Product may have different scheme to add entropy into PRNG.
 * For sophia, take get_timer (8 bytes) and unlock code as entropy
 */
#define ENTROPY_LEN (UNLOCK_CODE_LEN + 8)
    static unsigned char entropy[ENTROPY_LEN] = {0};
    unsigned int unlock_code_len = UNLOCK_CODE_LEN;

    sprintf(entropy, "%08x", get_timer(0));
    if (amzn_get_unlock_code(&entropy[strlen(entropy)], &unlock_code_len)) {
      return -1;
    }
/**
 * amzn_get_onetime_random_number will return a binary string which is not readable.
 * The binary string cannot be returned via fastboot so using base64 encode it.
 */
// compute how many random bytes do we need so that the converted size is the target length
#define RANDOM_BYTES_SIZE (ONETIME_UNLOCK_CODE_LEN + 3) / 4 * 3
    uint8_t random_bytes[RANDOM_BYTES_SIZE] = {0};
    unsigned int out_len = sizeof(one_tu_code);

    if (amzn_get_onetime_random_number(entropy, strlen(entropy),
                                       random_bytes, sizeof(random_bytes)))
      return -1;

    if (amzn_onetime_unlock_b64_encode(random_bytes, sizeof(random_bytes),
                                       one_tu_code, &out_len)) {
      return -1;
    }
    code_generated = 1;
  }
  memcpy(code, one_tu_code, ONETIME_UNLOCK_CODE_LEN);
  *len = ONETIME_UNLOCK_CODE_LEN;
  return 0;
}


int amzn_get_onetime_unlock_root_pubkey(const unsigned char **key, unsigned int *key_len)
{
  static const unsigned char onetime_unlock_key[] = ONETIME_UNLOCK_KEY;
  const int onetime_unlock_key_size = sizeof(onetime_unlock_key);

  if (!key || !key_len)
    return -1;

  *key_len = onetime_unlock_key_size;
  *key = onetime_unlock_key;
  return 0;
}
#endif

#if defined(UFBL_FEATURE_SECURE_FLASHING)
const unsigned char *amzn_get_sec_flashing_root_pubkey(unsigned int *key_len)
{
	static const unsigned char root_pubkey[] =
	"\x30\x82\x01\x22\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01"
	"\x01\x05\x00\x03\x82\x01\x0f\x00\x30\x82\x01\x0a\x02\x82\x01\x01"
	"\x00\xb4\xd2\x63\x33\x58\xff\x8d\xaf\x32\x3f\xe2\x16\xa2\x9c\x33"
	"\xcc\xd1\x01\x3c\x09\x96\xe6\xa2\x47\xa9\x20\xd3\xd2\xbc\x89\xbc"
	"\xef\x9d\x26\xf7\xa3\x49\x35\x64\x72\x21\x42\xf7\x14\x73\xf8\x64"
	"\x4d\xd4\xa8\x46\x4b\xe0\x13\x48\xa9\xe7\x50\xa8\x14\xf1\xc3\x46"
	"\x7a\xfe\xf4\x8d\x1d\xd2\xa2\x65\x81\xd5\x42\x90\xcd\x16\x11\x97"
	"\x48\xf3\x61\xbd\xb2\xd3\x66\xb1\xa4\x50\x80\x52\x70\xcf\x84\x71"
	"\x9e\x54\x12\x6d\x51\x18\x53\x34\xc7\xfa\x2c\x4d\x54\x67\xce\xa2"
	"\xf2\xa4\x43\x86\xdf\xe6\x3f\xfd\x29\xbc\x51\x56\x8e\xcf\xdb\xf0"
	"\x6d\x70\x57\x1d\xbb\x71\x5c\xd7\xcd\xb2\x23\xcf\xb1\xb2\x4f\x94"
	"\x99\xbc\x52\xa6\xd1\x82\x6a\x89\x76\xd7\x89\x50\xba\xcd\x84\x0e"
	"\x47\xec\x21\x2c\x3e\x95\x20\xab\xf0\x0c\x7e\xf7\x02\xe1\x06\xdf"
	"\x87\x74\x44\xc9\xb7\x42\x82\x9e\x8d\x94\xae\x7d\x04\xf7\x29\x72"
	"\x78\xe2\x34\xba\x78\xa5\x4b\x2c\xb3\x37\x51\xba\x4b\xd8\xa3\x07"
	"\xa1\x05\x26\xf3\x94\x5c\xb6\xe1\xc8\x33\x1e\x1d\x29\x88\x65\xd8"
	"\xf5\xac\x39\x50\x3a\xe6\x13\x7d\x6a\xb0\x01\xf8\x7c\x4c\x8e\xa6"
	"\x9c\xd5\xec\xed\x39\x12\xbc\xc4\x38\x26\x9f\x92\x61\xa2\xad\x52"
	"\x61\x02\x03\x01\x00\x01"
	;

	if (!key_len)
	  return NULL;
	*key_len = sizeof(root_pubkey);
	return root_pubkey;
}

int amzn_verify_device_meta(const unsigned char *data, unsigned int len)
{
	int i;
	const unsigned int max_hwid_num = 16;

	typedef struct type_device_meta {
		int hardware_id[max_hwid_num];
		unsigned int reserved[16];
	} device_meta;

	device_meta *meta = (device_meta *)data;

	if (!data || len != DEVICE_META_SIZE) {
		return -1;
	}

	for (i = 0; i < max_hwid_num; i++) {
		if (meta->hardware_id[i] == amzn_get_hardware_id()) {
			return 0;
		}
	}
	return -2;
}

const unsigned char * amzn_get_sec_flashing_pk_blacklist(unsigned int *len)
{
	static const unsigned char pk_blacklist[][SHA256_DIGEST_SIZE] = {
	};
	if (!len)
		return NULL;
	*len = sizeof(pk_blacklist) / sizeof(pk_blacklist[0]);
	return (const unsigned char *)pk_blacklist;
}

static unsigned int get_random_number(void)
{
    int rand = 0;
    int seed = 0;
    seed = rRand((unsigned int)get_timer(0),0);
    rand = rRand((unsigned int)get_timer(0), (unsigned int)seed);
    return rand;
}

int amzn_get_sec_flashing_code(unsigned char *code, unsigned int *len)
{
  static unsigned char sec_flashing_code[UNLOCK_CODE_LEN + 1] = {0};
  static unsigned char code_generated = 0;
  unsigned int unlock_code_len = UNLOCK_CODE_LEN;
  unsigned int rand1, rand2;

  if (!code || !len || *len < UNLOCK_CODE_LEN)
    return -1;

  if (!code_generated) {
	if(amzn_get_unlock_code(sec_flashing_code, &unlock_code_len)){
		return -1;
	}
	rand1 = get_random_number();
	rand2 = get_random_number();
	sprintf(&sec_flashing_code[16], "%08x%08x", rand1, rand2);
	code_generated = 1;
  }
  memcpy(code, sec_flashing_code, UNLOCK_CODE_LEN);
  *len = UNLOCK_CODE_LEN;
  return 0;
}
#endif

