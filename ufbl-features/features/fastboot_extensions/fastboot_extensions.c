/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#include <malloc.h>
#include <mmc.h>
#include <partition_parser.h>
#include <stdlib.h>
#include <string.h>

#include <bootimg.h>
#include "sparse_format.h"
#include "fastboot.h"

#if defined(UFBL_FEATURE_SECURE_BOOT)
#include <amzn_secure_boot.h>
#endif

#if defined(UFBL_FEATURE_UNLOCK)
#include <amzn_unlock.h>
#endif

#define ROUND_TO_PAGE(x,y) (((x) + (y)) & (~(y)))

#if !defined(MBR_START_ADDRESS_BYTE)
#define MBR_START_ADDRESS_BYTE (0)
#endif

extern int usb_write(void * buf, unsigned len);

/* Dump one parition via fastboot. */
#define DUMP_CHUNK_SIZE 4096
void cmd_dump_ufbl(const char *arg, void *data, unsigned sz)
{
	char* ptn_name;
	int ptn_index;
	unsigned long long ptn_size;
	unsigned long long ptn_offset;
	unsigned char* ptn_buf;
	unsigned long long i;
	char msg[128] = { 0 };

	/*if (is_bootloader_locked()) {
		fastboot_fail("'dump' command requires unlocked bootloader");
		return;
	}*/

	/* Response to "dump:partition_name" command. */
	ptn_name = strtok((char*)arg, " ");

	ptn_index = partition_get_index(ptn_name);
	if (ptn_index == INVALID_PTN) {
		snprintf(msg, sizeof(msg), "Partition %s not found.", ptn_name);
		fastboot_fail(msg);
		return;
	}

	ptn_size = partition_get_size(ptn_index);
	ptn_offset = partition_get_offset(ptn_index);

	/* Ready to send the partition content. */
	snprintf(msg, sizeof(msg), "Dumping partition %s, size=%lld",
		ptn_name, ptn_size);
	fastboot_okay(msg);

	/* Write the partition size first. */
	usb_write(&ptn_size, sizeof(ptn_size));

	/* Write to the host with 4KB block data. */
	ptn_buf = (unsigned char*)malloc(DUMP_CHUNK_SIZE);
	for (i = 0; i < ptn_size / DUMP_CHUNK_SIZE; ++i) {
		// The following is a super hack that accounts for MTK adding "MBR_START_ADDRESS_BYTE" to their
		// emmc_read AND their returned offset, thereby breaking everything
		#ifdef MTK_MMC_HACK
		mmc_read(ptn_offset + i * DUMP_CHUNK_SIZE - MBR_START_ADDRESS_BYTE, (unsigned int*)ptn_buf, DUMP_CHUNK_SIZE);
		#else
		mmc_read(ptn_offset + i * DUMP_CHUNK_SIZE, (unsigned int*)ptn_buf, DUMP_CHUNK_SIZE);
		#endif
		usb_write(ptn_buf, DUMP_CHUNK_SIZE);
	}

	/* Write the rest of the data if there is any. */
	if (ptn_size - i * DUMP_CHUNK_SIZE > 0) {
		#ifdef MTK_MMC_HACK
		mmc_read(ptn_offset + i * DUMP_CHUNK_SIZE - MBR_START_ADDRESS_BYTE, (unsigned int*)ptn_buf,
			ptn_size - i * DUMP_CHUNK_SIZE);
		#else
		mmc_read(ptn_offset + i * DUMP_CHUNK_SIZE, (unsigned int*)ptn_buf, ptn_size - i * DUMP_CHUNK_SIZE);
		#endif
		usb_write(ptn_buf, ptn_size - i * DUMP_CHUNK_SIZE);
	}

	snprintf(msg, sizeof(msg), "Dumping partition %s done.", ptn_name);
	free(ptn_buf);
}


unsigned int
mmc_verify(unsigned long long data_addr, unsigned char *verify_buf, unsigned int data_len) {

	unsigned int read_size = 1<<17;

	unsigned char *b = (unsigned char*) malloc( read_size );

	unsigned int bytes_left = data_len;
	unsigned int read_len = 0;
	unsigned int verify_offset = 0;

	while(b && bytes_left) {
		memset(b,0,read_size);
		read_len = MIN(bytes_left, read_size);
		//BUGBUG: 511, should use blocksize?
		if(mmc_read(data_addr + verify_offset, (uint32_t*)b, ROUND_TO_PAGE(read_len, 511)))
		{
			dprintf(CRITICAL, "mmc read failure at %llu\n", data_addr);
			free(b);
			return 1;
		}
		else if (memcmp(b,verify_buf+verify_offset,read_len)) {
			dprintf(CRITICAL, "verification failed within offset range (%d, %d)\n", verify_offset, verify_offset+read_len);

			if(DEBUGLEVEL>=SPEW) {
				unsigned char* v = verify_buf + verify_offset;
				for(unsigned int i=0;i<read_len-4;i++) {
					if(b[i]!=v[i]) {
						dprintf(SPEW, "stream mismatch at %d:\n", verify_offset+i);

						dprintf(SPEW, "Read:   %02x %02x %02x %02x\n", v[i], v[i+1], v[i+2], v[i+3] );
						dprintf(SPEW, "Expect: %02x %02x %02x %02x\n", b[i], b[i+1], b[i+2], b[i+3] );
						break;
					}
				}
			}

			dprintf(INFO, "Verify failed. Images do not match");
			free(b);
			return 2;
		}
		else {
			dprintf(SPEW, "verify: bits match within offset range (%d, %d)\n", verify_offset, verify_offset+read_len);

			bytes_left-=read_len;
			verify_offset+=read_len;
		}
	}

	free(b);
	return 0;
}

void cmd_verify_mmc_img(const char *arg, void *data, unsigned sz)
{
	unsigned long long ptn = 0;
	unsigned long long size = 0;
	int index = INVALID_PTN;
	char *token = NULL;
	char *pname = NULL;
	//char msg[128] = { 0 };
	//uint8_t lun = 0;
	//bool lun_set = false;

	token = strtok((char *)arg, ":");
	pname = token;
	token = strtok(NULL, ":");
	/*if(token)
	{
		lun = atoi(token);
		mmc_set_lun(lun);
		lun_set = true;
	}*/

	index = partition_get_index(pname);
	ptn = partition_get_offset(index);
	if(ptn == 0) {
		fastboot_fail("partition table doesn't exist");
		return;
	}

	if (!strcmp(pname, "boot") || !strcmp(pname, "recovery")) {
		if (memcmp((void *)data, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
			fastboot_fail("image is not a boot image");
			return;
		}
	}

    /*if(!lun_set)
    {
        lun = partition_get_lun(index);
        mmc_set_lun(lun);
    }*/

	size = partition_get_size(index);
	if (ROUND_TO_PAGE(sz,511) > size) {
		fastboot_fail("size too large");
		return;
	}
	else if (mmc_verify(ptn - MBR_START_ADDRESS_BYTE, (unsigned char*)data, sz)) {
		fastboot_fail("flash verify failure");
		return;
	}

	fastboot_okay("");
	return;
}

void cmd_verify_mmc_sparse_img(const char *arg, void *data, unsigned sz, bool verify)
{
	unsigned int chunk;
	unsigned int chunk_data_sz;
	uint32_t *fill_buf = NULL;
	uint32_t fill_val;
	uint32_t chunk_blk_cnt = 0;
	sparse_header_t *sparse_header;
	chunk_header_t *chunk_header;
	uint32_t total_blocks = 0;
	unsigned long long ptn = 0;
	unsigned long long size = 0;
	int index = INVALID_PTN;
	unsigned int i;
	//uint8_t lun = 0;
	//char msg[128] = { 0 };

	index = partition_get_index(arg);
	ptn = partition_get_offset(index);
	if(ptn == 0) {
		fastboot_fail("partition table doesn't exist");
		return;
	}

	size = partition_get_size(index);
	if (ROUND_TO_PAGE(sz,511) > size) {
		fastboot_fail("size too large");
		return;
	}

	/*lun = partition_get_lun(index);
	mmc_set_lun(lun);*/

	/* Read and skip over sparse image header */
	sparse_header = (sparse_header_t *) data;
	if ((sparse_header->total_blks * sparse_header->blk_sz) > size) {
		fastboot_fail("size too large");
		return;
	}

	data += sparse_header->file_hdr_sz;
	if(sparse_header->file_hdr_sz > sizeof(sparse_header_t))
	{
		/* Skip the remaining bytes in a header that is longer than
		 * we expected.
		 */
		data += (sparse_header->file_hdr_sz - sizeof(sparse_header_t));
	}

	dprintf (SPEW, "=== Sparse Image Header ===\n");
	dprintf (SPEW, "magic: 0x%x\n", sparse_header->magic);
	dprintf (SPEW, "major_version: 0x%x\n", sparse_header->major_version);
	dprintf (SPEW, "minor_version: 0x%x\n", sparse_header->minor_version);
	dprintf (SPEW, "file_hdr_sz: %d\n", sparse_header->file_hdr_sz);
	dprintf (SPEW, "chunk_hdr_sz: %d\n", sparse_header->chunk_hdr_sz);
	dprintf (SPEW, "blk_sz: %d\n", sparse_header->blk_sz);
	dprintf (SPEW, "total_blks: %d\n", sparse_header->total_blks);
	dprintf (SPEW, "total_chunks: %d\n", sparse_header->total_chunks);
	dprintf (SPEW, "size %lld, blk_sz*total_blks %d\n", size, sparse_header->blk_sz*sparse_header->total_blks);

	sz = sparse_header->blk_sz * sparse_header->total_blks;
	if (ROUND_TO_PAGE(sz, 511) > size ) {
		dprintf( CRITICAL, "unsparse image size %u too large for partition\n", sz);
		fastboot_fail("unsparse image size too large");
		return;
	}


	/* Start processing chunks */
	for (chunk=0; chunk<sparse_header->total_chunks; chunk++)
	{
		/* Read and skip over chunk header */
		chunk_header = (chunk_header_t *) data;
		data += sizeof(chunk_header_t);

		dprintf (SPEW, "=== Chunk Header ===\n");
		dprintf (SPEW, "chunk_type: 0x%x\n", chunk_header->chunk_type);
		dprintf (SPEW, "chunk_data_sz: 0x%x\n", chunk_header->chunk_sz);
		dprintf (SPEW, "total_size: 0x%x\n", chunk_header->total_sz);

		if(sparse_header->chunk_hdr_sz > sizeof(chunk_header_t))
		{
			/* Skip the remaining bytes in a header that is longer than
			 * we expected.
			 */
			data += (sparse_header->chunk_hdr_sz - sizeof(chunk_header_t));
		}

		chunk_data_sz = sparse_header->blk_sz * chunk_header->chunk_sz;
		switch (chunk_header->chunk_type)
		{
			case CHUNK_TYPE_RAW:
			if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz +
											chunk_data_sz))
			{
				fastboot_fail("Bogus chunk size for chunk type Raw");
				return;
			}

			if(mmc_verify(ptn + ((uint64_t)total_blocks*sparse_header->blk_sz) - MBR_START_ADDRESS_BYTE, (unsigned char*)data, chunk_data_sz ))
			{
				fastboot_fail("flash verify failure");
				return;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

			case CHUNK_TYPE_FILL:
			if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz +
											sizeof(uint32_t)))
			{
				fastboot_fail("Bogus chunk size for chunk type FILL");
				return;
			}

			fill_buf = (uint32_t *)memalign(CACHE_LINE, ROUNDUP(sparse_header->blk_sz, CACHE_LINE));
			if (!fill_buf)
			{
				fastboot_fail("Malloc failed for: CHUNK_TYPE_FILL");
				return;
			}

			fill_val = *(uint32_t *)data;
			data = (char *) data + sizeof(uint32_t);
			chunk_blk_cnt = chunk_data_sz / sparse_header->blk_sz;

			for (i = 0; i < (sparse_header->blk_sz / sizeof(fill_val)); i++)
			{
				fill_buf[i] = fill_val;
			}

			for (i = 0; i < chunk_blk_cnt; i++)
			{
				#ifdef MTK_MMC_HACK
				if(mmc_write(ptn + ((uint64_t)total_blocks*sparse_header->blk_sz) - MBR_START_ADDRESS_BYTE,
							sparse_header->blk_sz,
							fill_buf))
				#else
				if(mmc_write(ptn + ((uint64_t)total_blocks*sparse_header->blk_sz),
							sparse_header->blk_sz,
							fill_buf))
				#endif
				{
					fastboot_fail("flash write failure");
					free(fill_buf);
					return;
				}

				total_blocks++;
			}

			free(fill_buf);
			break;

			case CHUNK_TYPE_DONT_CARE:
			total_blocks += chunk_header->chunk_sz;
			break;

			case CHUNK_TYPE_CRC:
			if(chunk_header->total_sz != sparse_header->chunk_hdr_sz)
			{
				fastboot_fail("Bogus chunk size for chunk type Dont Care");
				return;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

			default:
			dprintf(CRITICAL, "Unkown chunk type: %x\n",chunk_header->chunk_type);
			fastboot_fail("Unknown chunk type");
			return;
		}
	}

	dprintf(INFO, "%s %d blocks, expected to %s %d blocks\n",
					verify? "Verified":"Wrote",
					total_blocks,
					verify? "verify":"write",
					sparse_header->total_blks);

	if(total_blocks != sparse_header->total_blks)
	{
		if(verify) {
			fastboot_fail("sparse image verify failure");
		}
		else {
			fastboot_fail("sparse image write failure");
		}
	}

	fastboot_okay("");
	return;
}

void cmd_verify(const char *arg, void *data, unsigned sz)
{
	sparse_header_t *sparse_header;

#ifdef SSD_ENABLE
	/* 8 Byte Magic + 2048 Byte xml + Encrypted Data */
	unsigned int *magic_number = (unsigned int *) data;
	int              ret=0;
	uint32           major_version=0;
	uint32           minor_version=0;

	ret = scm_svc_version(&major_version,&minor_version);
	if(!ret)
	{
		if(major_version >= 2)
		{
			if( !strcmp(arg, "ssd") || !strcmp(arg, "tqs") )
			{
				ret = encrypt_scm((uint32 **) &data, &sz);
				if (ret != 0) {
					dprintf(CRITICAL, "ERROR: Encryption Failure\n");
					return;
				}

				/* Protect only for SSD */
				if (!strcmp(arg, "ssd")) {
					ret = scm_protect_keystore((uint32 *) data, sz);
					if (ret != 0) {
						dprintf(CRITICAL, "ERROR: scm_protect_keystore Failed\n");
						return;
					}
				}
			}
			else
			{
				ret = decrypt_scm_v2((uint32 **) &data, &sz);
				if(ret != 0)
				{
					dprintf(CRITICAL,"ERROR: Decryption Failure\n");
					return;
				}
			}
		}
		else
		{
			if (magic_number[0] == DECRYPT_MAGIC_0 &&
			magic_number[1] == DECRYPT_MAGIC_1)
			{
				ret = decrypt_scm((uint32 **) &data, &sz);
				if (ret != 0) {
					dprintf(CRITICAL, "ERROR: Invalid secure image\n");
					return;
				}
			}
			else if (magic_number[0] == ENCRYPT_MAGIC_0 &&
				magic_number[1] == ENCRYPT_MAGIC_1)
			{
				ret = encrypt_scm((uint32 **) &data, &sz);
				if (ret != 0) {
					dprintf(CRITICAL, "ERROR: Encryption Failure\n");
					return;
				}
			}
		}
	}
	else
	{
		dprintf(CRITICAL,"INVALID SVC Version\n");
		return;
	}
#endif /* SSD_ENABLE */

	sparse_header = (sparse_header_t *) data;
	if (sparse_header->magic != SPARSE_HEADER_MAGIC)
		cmd_verify_mmc_img(arg, data, sz);
	else
		cmd_verify_mmc_sparse_img(arg, data, sz, true);
	return;
}

#if defined(UFBL_FEATURE_UNLOCK)
#if defined(UFBL_PLATFORM_MTK)
void cmd_relock(const char *arg, void *data, unsigned sz)
{
#if defined(UFBL_FEATURE_IDME)
	unsigned char code[SIGNED_UNLOCK_CODE_LEN] = { 0 };

	idme_update_var_ex("unlock_code", code, sizeof(code));
	fastboot_publish("unlock_status", "false");
	fastboot_okay("");
#endif
}
#endif
#endif

void register_fastboot_extention_cmds(){
#ifdef UFBL_PLATFORM_MTK
#if defined(UFBL_FEATURE_SECURE_BOOT)
	if (amzn_target_device_type() == AMZN_PRODUCTION_DEVICE) {
#if defined(UFBL_FEATURE_UNLOCK)
		if (amzn_target_is_unlocked()) {
#else
		{
#endif
			fastboot_register("dump:", cmd_dump_ufbl, FALSE);
			fastboot_register("verify:", cmd_verify, FALSE);
		}
	} else {
		fastboot_register("dump:", cmd_dump_ufbl, FALSE);
		fastboot_register("verify:", cmd_verify, FALSE);
	}
#else
	fastboot_register("dump:", cmd_dump_ufbl, FALSE);
	fastboot_register("verify:", cmd_verify, FALSE);
#endif /* defined(UFBL_FEATURE_SECURE_BOOT) && defined(UFBL_FEATURE_UNLOCK) */
#else
	fastboot_register("dump:", cmd_dump_ufbl);
	fastboot_register("verify:", cmd_verify);
#endif

#if defined(UFBL_FEATURE_SECURE_BOOT)
#if defined(UFBL_PLATFORM_MTK)
	if (amzn_target_device_type() == AMZN_PRODUCTION_DEVICE) {
		fastboot_publish("prod", "1");
	} else {
		fastboot_publish("prod", "0");
	}
#endif
#endif

#if defined(UFBL_FEATURE_UNLOCK)
#if defined(UFBL_PLATFORM_MTK)
	unsigned char unlock_code[UNLOCK_CODE_LEN];
	unsigned int len = sizeof(unlock_code);

	if (!amzn_get_unlock_code(unlock_code, &len))
		fastboot_publish("unlock_code", strdup((char *)unlock_code));

#if defined(UFBL_FEATURE_UNLOCK_V0)
	/* Legacy unlock schemes */
	fastboot_publish("unlock_version", "0");
#else
	fastboot_publish("unlock_version", "1");

	if (amzn_target_is_unlocked()) {
		fastboot_publish("unlock_status", "true");
	} else {
		fastboot_publish("unlock_status", "false");
	}
#endif

	fastboot_register("oem relock", cmd_relock, FALSE);
#endif
#endif
}

void ufbl_update_cmdline(char *cmdline)
{
#if defined(UFBL_FEATURE_SECURE_BOOT)
#if defined(UFBL_PLATFORM_MTK)
        if (cmdline)
                sprintf(cmdline, "%s androidboot.prod=%d", cmdline,
                        (amzn_target_device_type()
                                == AMZN_PRODUCTION_DEVICE) ? 1 : 0);
#endif
#endif
}

__WEAK void ufbl_fastboot_features_init(){
	register_fastboot_extention_cmds();
} 


