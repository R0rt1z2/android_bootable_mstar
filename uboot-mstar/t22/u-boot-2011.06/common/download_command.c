#include "download_command.h"
#include <fastboot.h>
#include <sparse_format.h>
#include <common.h>
#include <command.h>
#include <jffs2/jffs2.h>
#include <mmc.h>
#include <bootimg.h>
#include <image-sparse.h>
#include <fb_mmc.h>
#include <MsMmc.h>
#include <MsRawIO.h>
#if defined(CONFIG_ANDROID_AVB_ENABLE)
#include "avb_ops_uboot.h"
#endif

#ifdef CONFIG_USB_FASTBOOT_ENABLE

extern void *download_base;
extern unsigned download_max;
extern unsigned download_size;
extern unsigned fastboot_state;
extern struct fastboot_var *varlist;

static int critical_lock = 1;

static const unsigned char wipedata_bin[] = {
    0x62, 0x6f, 0x6f, 0x74, 0x2d, 0x72, 0x65, 0x63, 0x6f, 0x76, 0x65, 0x72, 
    0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x72, 0x65, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 
    0x0a, 0x2d, 0x2d, 0x77, 0x69, 0x70, 0x65, 0x5f, 0x64, 0x61, 0x74, 0x61, 
    0x0a
};

static unsigned hex2unsigned(const char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

extern char *get_script_next_line(char **line_buf_ptr);

static int util_run_script(char* buffer, unsigned buffer_len)
{
    char* script_buf = 0;
    char* next_line = 0;
    int BeAbort = 1;
    char* tmp = 0;

    // Set commmand abort or not when cmd error.
    tmp = getenv ("CmdAbort");
    if (tmp != NULL)
    {
        BeAbort = (int)simple_strtol(tmp, NULL, 10);
        printf("BeAbort=%d\n",BeAbort);
    }

    script_buf = buffer;
    script_buf[buffer_len]='\n'; script_buf[buffer_len+1]='%';
    while ((next_line = get_script_next_line(&script_buf)) != 0)
    {
        printf("\n>> %s \n", next_line);
        if(-1 == run_command(next_line, 0))
        {
            printf("command error!\n");
            if(BeAbort == 1)
            {
               return -1;
            }
        }
    }
    return 0;
}

#if defined(CONFIG_MMC)
static int get_size_of_partition(const char* partition, uint64_t* out_size_in_bytes)
{
    struct mtd_device *dev;
    struct part_info *partInfo;
    uint64_t* partSize = 0;
    uint64_t* partOffset = 0;
    uint8_t partNum;
    int ret = 0;

    if (get_mmc_part_size_and_offset(partition, (unsigned int*)partSize, (unsigned int*)partOffset) == 0)
    {
        *out_size_in_bytes = (*partSize)*512;
        ret = 0;
    }
    else
    {
        printf("ERROR in get_size_of_partition\n");
        ret = -1;
    }
    return ret;
}
#else
static int get_size_of_partition(const char* partition, uint64_t* out_size_in_bytes)
{
    return -1;
}
#endif

#ifdef CONFIG_ANDROID_SECURE_BOOT
static int is_device_unlocked()
{
    // Assume default state is lock state
    uint16_t devicestate = 1;

    read_device_state(&devicestate);

    if( devicestate == 1 )
        return 0;
    else if( devicestate == 0 )
        return 1;
    else
    {
        // unknown device state, return lock state
        printf("unknown device state: %d\n", devicestate);
        return 0;
    }
}
#endif

void download_value_init(void)
{
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
	download_base = CONFIG_SYS_MALLOC_START + CONFIG_SYS_MALLOC_LEN;
	#else
    download_base = CONFIG_UPGRADE_BUFFER_ADDR+0x10000;
	#endif

	download_size = 0;
	return;
}

static int download_standard(u32 data_length)
{
	int read_length;

	printf("USB Transferring ... buffer[0x%08x], len: %d Byte.\n", download_base, data_length);

	read_length = usb_read(download_base, data_length);
	if ((read_length < 0) || ((unsigned) read_length != data_length))
	{
		printf("Read USB error, read_length = %d Byte, data_length = %d Byte.\n", read_length, data_length);
		fastboot_state = STATE_ERROR;
		return -1;
	}

	download_size = data_length;

	printf("read OK. data_length: 0x%x\n", data_length);
	return 0;
}

void do_resetBootloader(const char *arg, const char *data, unsigned int data_length)
{
	printf("rebooting the device to bootloader\n");
    run_command("setenv reboot-bootloader 1", 0);
    run_command("saveenv", 0);
	fastboot_okay("");
 	do_reset(NULL,NULL,NULL,NULL);
}

void do_continue(const char *arg, const char *data, unsigned int data_length)
{
	printf("continue booting the device ...\n");
	fastboot_okay("");
	fastboot_state = STATE_ERROR;
}

void do_reboot(const char *arg, const char *data, unsigned int data_length)
{
	char boot_msg[64];
	memset(boot_msg, 0, 64);

	printf("Rebooting the device ...\n");
//JH need fix this part
#if 0
	extern int emmc_write_to_partition(unsigned char* buf, char* pname, u64 offset, unsigned int size);
	emmc_write_to_partition((unsigned char*)boot_msg, "misc", 512, sizeof("fastboot"));
#endif
	fastboot_okay("");
	do_reset(NULL,NULL,NULL,NULL);
}
static int file_counter = 0;
int do_download(const char *arg, const char *data, unsigned int data_length )
{
	char response[64];
	unsigned len = hex2unsigned(data);
	printf("do_download begin, len = %d Byte.\n", len);

#ifdef MSTAR_USB_UDC_GLUE
	unsigned char checksum;
	unsigned i;
#endif

	if(len == 0)
	{
		fastboot_okay("download OK");
		return 0;
	}

	sprintf(response, "DATA%08x", len);
	if (usb_write(response, strlen(response)) < 0)
	{
	    printf("cmd_download -- usb write fail\n");
		return -1;
	}

	download_standard(len);
	fastboot_okay("download OK");

#ifdef MSTAR_USB_UDC_GLUE
	/* calculate checksum */
	checksum = 0;
    for (i = 0; i < len; i++) {
		checksum = checksum ^ *((U8 *)download_base+i);
    }
    printf("data checksum: %x\n", checksum);
#endif

    return 0;
}

#ifdef CC_SUPPORT_AVB
#define CHUNK_TYPE_FILL_BUFFER_SIZE (2*1024*1024) //2MB.
#else
#define CHUNK_TYPE_FILL_BUFFER_SIZE (64*1024)     //64KB.
#endif

#define CMD_FLASH_TO_EMMC(data,offset,sz)														\
do {																							\
    char *local_args[7];																		\
    char p0[64], p1[32], p2[32], p3[32];														\
	local_args[0] = "mmc";																		\
    local_args[1] = "write.b";																	\
    sprintf(p0, "0x%lx", CONFIG_SYS_MMC_ENV_DEV);												\
    local_args[2] = p0;																			\
    sprintf(p1, "0x%lx", data); 																\
    local_args[3] = p1;																			\
    sprintf(p2, "0x%llx", offset); 																\
    local_args[4] = p2;																			\
    sprintf(p3, "0x%lx", sz);																	\
    local_args[5] = p3;																			\
    if (do_mmcops(NULL, 0, 6, local_args)) {	fastboot_fail("mmc cops fail");	 return -1;	}	\											
} while(0)

int cmd_flash_emmc_sparse_img(char * data, unsigned long long offset,unsigned int sz)
{
#if 0
	unsigned int chunk;
	unsigned int chunk_data_sz;
	sparse_header_t *sparse_header;
	chunk_header_t *chunk_header;
	unsigned int total_blocks = 0;
    char *local_args[7];
    char p0[64], p1[32], p2[32], p3[32];
	printf( "Enter cmd_flash_sparse_img()\n");


	/* Read and skip over sparse image header */
	sparse_header = (sparse_header_t *) data;
	data += sparse_header->file_hdr_sz;
	if(sparse_header->file_hdr_sz > sizeof(sparse_header_t))
	{
		/* Skip the remaining bytes in a header that is longer than we expected. */
		data += (sparse_header->file_hdr_sz - sizeof(sparse_header_t));
	}

	printf ("=== Sparse Image Header ===\n");
	printf ("magic: 0x%x\n", sparse_header->magic);
	printf ("major_version: 0x%x\n", sparse_header->major_version);
	printf ("minor_version: 0x%x\n", sparse_header->minor_version);
	printf ("file_hdr_sz: %d\n", sparse_header->file_hdr_sz);
	printf ("chunk_hdr_sz: %d\n", sparse_header->chunk_hdr_sz);
	printf ("blk_sz: %d\n", sparse_header->blk_sz);
	printf ("total_blks: %d\n", sparse_header->total_blks);
	printf ("total_chunks: %d\n", sparse_header->total_chunks);

	printf("\nWriting Flash ...\n");
	/* Start processing chunks */
	for (chunk=0; chunk<sparse_header->total_chunks; chunk++)
	{
		/* Read and skip over chunk header */
		chunk_header = (chunk_header_t *) data;
		data += sizeof(chunk_header_t);

		printf ("\n=== Chunk Header ===\n");
		printf ("chunk_type:    0x%x\n", chunk_header->chunk_type);
		printf ("chunk_data_sz: 0x%08x\n", chunk_header->chunk_sz);
		printf ("total_size:    0x%08x\n", chunk_header->total_sz);

		if(sparse_header->chunk_hdr_sz > sizeof(chunk_header_t))
		{
			/* Skip the remaining bytes in a header that is longer than we expected. */
			data += (sparse_header->chunk_hdr_sz - sizeof(chunk_header_t));
		}

		switch (chunk_header->chunk_type)
		{
		case CHUNK_TYPE_RAW:
			chunk_data_sz = chunk_header->chunk_sz * sparse_header->blk_sz;
			if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz))
			{
				fastboot_fail("Error chunk size for chunk type Raw.");
				return -1;
			}

			CMD_FLASH_TO_EMMC(data,(offset + ((uint64_t)total_blocks*sparse_header->blk_sz)),chunk_data_sz);

			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

        case CHUNK_TYPE_FILL:
            chunk_data_sz = 4;
            if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz))
            {
                fastboot_fail("Error chunk size for chunk type Fill.");
                return -1;
            }
            else
            {
                static uint8_t *fill_data_buffer = NULL;
                uint32_t fill_data_size = chunk_header->chunk_sz * sparse_header->blk_sz;
                uint32_t fill_data = *(uint32_t *)data;
                uint32_t data_size_integer = fill_data_size/CHUNK_TYPE_FILL_BUFFER_SIZE;
                uint32_t data_size_remainder = fill_data_size%CHUNK_TYPE_FILL_BUFFER_SIZE;
                int i = 0;

                if(fill_data_buffer == NULL)
                {
                    fill_data_buffer = malloc(CHUNK_TYPE_FILL_BUFFER_SIZE);
                }
                // Fill buffer
                for(i=0; i<(CHUNK_TYPE_FILL_BUFFER_SIZE)/sizeof(uint32_t); i++)
                {
                    *(uint32_t *)fill_data_buffer = fill_data;
                }
                // Write buffer to emmc
                for(i=0; i<data_size_integer; i++)
                {
                    CMD_FLASH_TO_EMMC(fill_data_buffer,(offset + ((uint64_t)total_blocks*sparse_header->blk_sz) + i*CHUNK_TYPE_FILL_BUFFER_SIZE), CHUNK_TYPE_FILL_BUFFER_SIZE);
                }
                if(data_size_remainder != 0)
                {
                    CMD_FLASH_TO_EMMC(fill_data_buffer,(offset + ((uint64_t)total_blocks*sparse_header->blk_sz) + data_size_integer*CHUNK_TYPE_FILL_BUFFER_SIZE), data_size_remainder);
                }
            }
            total_blocks += chunk_header->chunk_sz;
            data += chunk_data_sz;
            break;

		case CHUNK_TYPE_DONT_CARE:
			chunk_data_sz = 0;
			if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz))
			{
				fastboot_fail("Error chunk size for chunk type Dont Care.");
				return -1;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		case CHUNK_TYPE_CRC32:
			chunk_data_sz = 4;
			if(chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz))
			{
				fastboot_fail("Error chunk size for chunk type CRC.");
				return -1;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		default:
			fastboot_fail("Unknown chunk type");
			return -1;
		}
	}

	printf("\nWrote %d blocks, expected to write %d blocks\n",total_blocks, sparse_header->total_blks);

	if(total_blocks != sparse_header->total_blks)
	{
		fastboot_fail("sparse image write failure");
		return -1;
	}

	fastboot_okay("Write Flash OK");
#endif
	return 0;;
}

/* Ported by Zhendong Wang starts for fastboot supporting packed image */

/* Note: This function must accordate with the the u32_to_u8 function in
 * image_pack.c. Otherwise, the parsed u32 is incorrect
 */
void u8_array_to_u32( const u8 * array, u32 * num ) {
    u32 i = 0;
    *num = 0;
    for( i = 0; i < 4; i++ ) {
        *num <<= 8;
        *num += array[i];
    }
} 

int do_update_packed_images(void)
{
#if 0
    int i;
    u8 *pTmp, *name;
    img_hdr *images_header;
    sparse_header_t *sparse_header;
    u32 size, partition_num, num_images, images_size, header_size;
    u64 partition_size, partition_offset;

    images_header = download_base;
    pTmp = download_base;

    u8_array_to_u32(&images_header->num_images, &num_images);
    u8_array_to_u32(&images_header->images_size, &images_size);
    u8_array_to_u32(&images_header->header_size, &header_size);
    if (num_images < 1) {
	printf("This packed images contain no image.\n");
	fastboot_fail("The packed images contain no image");
	return MULTI_IMAGE_ERROR;
    }

    pTmp += header_size;
    struct {
	u8 name[PARTITION_NAME_SIZE];
	u32 size;
    } *img_info = images_header->img_info;

    for (i = 0; i < num_images; i++) {
	name = img_info[i].name;
	u8_array_to_u32(&img_info[i].size, &size);

	if (find_partition(name, &partition_num,
		    &partition_size, &partition_offset)) {
	    printf("The partition %s is not defined.\n", name);
	    fastboot_fail("The partition is not defined");
	    return MULTI_IMAGE_ERROR;
	}
	if (size > partition_size) {
	    printf("Image size is larger than the partition size.\n");
	    fastboot_fail("Image size is larger than the partition size");
            return MULTI_IMAGE_ERROR;
	}

	sparse_header = pTmp;
	printf("Write partition %s.\n", name);

	if (sparse_header->magic != SPARSE_HEADER_MAGIC) {
	    CMD_FLASH_TO_EMMC(pTmp, partition_offset, size);
	    fastboot_okay("finished");
	} else {
	    if (cmd_flash_emmc_sparse_img(pTmp,
			partition_offset, size)) {
		fastboot_fail("mmc cops fail");
		return  MULTI_IMAGE_ERROR;
	    }
	    fastboot_okay("finished");
	}
	pTmp += size;
    }
#endif
    return MULTI_IMAGE_WRITE_SUCCESS;
}

/* Ported by Zhendong Wang starts for fastboot supporting packed image ends */

int do_update(const char *arg, const char *data, unsigned int data_length)
{
    /* part modified by David Li starts for fastboot supporting packed image */
    if (strncmp((const char *)download_base, "BOOTLDR!", 8) == 0) {
        return do_update_packed_images();
    }
    /* part modified by David Li ends */

    char tmpCmd[128] = {0};
    char partition_name[128] = {0};
    sparse_header_t *sparse_header;
    u8 part_num;
    uint64_t part_size = 0;
    uint32_t sz = download_size;
    uint32_t partition_size;
    printf("do_update begin, update size = %d Byte.\n", sz);

    strncpy(partition_name, data, data_length);

#ifdef CONFIG_ANDROID_SECURE_BOOT
    {
        int dev_is_unlock = is_device_unlocked();
        // dev_is_unlock, 1 is unlock, 0 is lock
        if( dev_is_unlock == 0 )
        {
            printf("device locked, forbid to write flash.\n");
            fastboot_fail("device locked, forbid to write flash.");
            return -1;
        }
    }

    if (critical_lock == 1 && (strncmp(partition_name, "rom_emmc_boot", 13) == 0 || strncmp(partition_name, "MBOOT", 5) == 0))
    {
        printf("critical locked, forbid to write flash.\n");
        fastboot_fail("critical locked, forbid to write flash.");
        return -1;
    }
#endif//CONFIG_ANDROID_SECURE_BOOT

    data = (const char *)download_base;
    sparse_header = (sparse_header_t *)data;

    // rom_emmc_boot needs to use additional flash cmd.
    if (strncmp(partition_name, "rom_emmc_boot", 13) == 0)
    {
        sprintf(tmpCmd, "mmc write.boot 1 0x%08x 0 0x%08x",
                data, sz);
        printf("\n%s\n", tmpCmd);
        if(run_command(tmpCmd, 0) != 0)
        {
            printf("%s fail\n", tmpCmd);
            fastboot_fail("failed to flash rom_emmc_boot\n");
            return -1;
        }

        fastboot_okay("Write rom_emmc_boot OK");
        return 0;
    }
    else if(strncmp(partition_name, "mscript", 7) == 0)
    {
        if(util_run_script((char *)data, sz) != 0) {
            printf("mscript failed!");
            fastboot_fail("mscript failed");
        }

        fastboot_okay("mscript OK");
        return 0;
    }

    if (get_size_of_partition(partition_name, &part_size)!=0)
    {
        printf("the %s partition not defined at mtdparts!\n", partition_name);
        fastboot_fail("can not find the partition");
        return -1;
    }

    partition_size = (uint32_t)part_size;

    if (sz > partition_size)
    {
        printf("update abort! file size 0x%x is larger than partition size 0x%x.\n", sz, partition_size);
        fastboot_fail("file size is larger than partition size");
        return -1;
    }
	
    fb_mmc_flash_write(partition_name, data, sz);
    fastboot_okay("Write Flash OK");

    printf("do_update Done!!\n");
    return 0;
}

int do_getvar(const char *arg, const char *data, unsigned int data_length )
{
	struct fastboot_var *var;
	char response[64];
	printf("do_getvar, var[%s].\n", data);

	if(!strcmp(data, "all")){
		for (var = varlist; var; var = var->next){
			if(var->handle)
			{
				var->handle(var->name,data,data_length);
			}
			else
			{
				sprintf(response, "\t%s: %s", var->name, var->value);
				fastboot_info(response);
			}
		}
		fastboot_okay("Done!!");
		return 0;
	}

	for (var = varlist; var; var = var->next) {
		if (!strncmp(var->name, data,strlen(var->name))) {
			if(var->handle)
			{
				var->handle(var->name,data + strlen(var->name),data_length - strlen(var->name));
			}
			else
			{
				fastboot_okay(var->value);
			}
			return 0;
		}
	}

	printf("invalid var[%s].\n", data);
	fastboot_fail("invalid var");
	return -1;
}

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B
static void TrimLeft(char *pDest, const char *pSrc, int len)
{
    int i = 0;
    const char *t = pSrc;
    while (i < len && (*(t++) == ' '));
    strcpy(pDest, t - 1);
}

static void EraseUserData(void)
    {
    int ret1 = 0;
    unsigned int sz = sizeof(wipedata_bin) / sizeof(wipedata_bin[0]);

    raw_io_config_push();
    ret1 = raw_io_config(FLASH_RAW_DATA_TARGET, "misc", UNNECESSARY_VOLUME);

    if(ret1 != 0)
    {
        printf("raw_io_config setting fail!\n");
    }

    printf("begin erase userdata.\n");
    ret1 = raw_write((const char *)wipedata_bin, 0, sz);
    if(ret1 != 0)
    {
        printf("raw_read misc fail!\n");
    }

    raw_io_config_pop();
    run_command("mmc erase.p userdata", 0);
    printf("end erase userdata.\n");
        }
#endif//CONFIG_ANDROID_SECURE_BOOT_CLASS_B

int do_erase_emmc(const char *arg, const char *data, unsigned int data_length )
        {
    char tmpCmd[128] = {0};

    char partition_name[128] = {0};
    sparse_header_t *sparse_header;
    u8 part_num;
    uint64_t part_size = 0;
    uint32_t sz = download_size;
    uint32_t partition_size;

    strncpy(partition_name, data, data_length);

    // check the partition is existed or not
    if (get_size_of_partition(partition_name, &part_size)!=0)
        {
        printf("the %s partition not defined at mtdparts!\n", partition_name);
        fastboot_fail("can not find the partition");
            return -1;
        }

    memset(tmpCmd, 0, 128);
    sprintf(tmpCmd, "mmc erase.p %s", partition_name);

#ifdef CONFIG_ANDROID_SECURE_BOOT
        {
        int dev_is_unlock = is_device_unlocked();
        // dev_is_unlock, 1 is unlock, 0 is lock
        if( dev_is_unlock == 0 )
    {
            // if device state is lock, only allow to erase userdata partition.
            if (!strcmp("userdata", partition_name)) {
            printf("begin to erase userdata, please waiting ...\n");
                if(run_command(tmpCmd, 0) != 0)
                {
                    printf("%s fail\n", tmpCmd);
                    return -1;
                }
                EraseUserData();
            printf("MMC erase userdata done.\n");
            fastboot_okay("Done!!");
            return 0;
        }
        else
        {
            printf("devicestate is lock, did not support to erase partition: %s\n",data);
            fastboot_fail("only support erase userdata partition.");
            return -1;
        }
    }
        else
        {
            // unlock case, still cant erase rom_emmc_boot and mboot partition
            if(strcmp("MBOOT", partition_name) == 0 || strcmp(partition_name, "rom_emmc_boot") == 0)
            {
                printf("devicestate is unlock, did not support to erase partition: %s\n",data);
                fastboot_fail("did not support to erase uboot partition.");
                return -1;
}

            printf("begin to erase %s, please waiting ...\n",partition_name);
            if(run_command(tmpCmd, 0) != 0)
{
                printf("%s fail\n", tmpCmd);
                return -1;
            }
            return 0;
        }
}
#else//CONFIG_ANDROID_SECURE_BOOT
    printf("begin to erase %s, please waiting ...\n",partition_name);
    if(run_command(tmpCmd, 0) != 0)
    {
        printf("%s fail\n", tmpCmd);
        fastboot_fail("failed to erase\n");
        return -1;
    }
	
    fastboot_okay("Done!!");
    return 0;
#endif//CONFIG_ANDROID_SECURE_BOOT
}

int do_flashing(const char *arg, const char *data, unsigned int data_length)
{
    int ret = 0;
    char TempData[20] = {0};
    char dev_state[20] = {0};

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B
    TrimLeft(TempData, data, strlen(data));
    // dev_is_unlock, 1 is unlock, 0 is lock
    int dev_is_unlock = is_device_unlocked();

    // printf("TempData=%s\n", TempData);
    if (strcmp("get_unlock_ability", TempData) == 0)
    {
        if( dev_is_unlock == 0 )
            strcpy(dev_state, "lock");
        else
            strcpy(dev_state, "unlock");

        printf("dev_state=%s\n", dev_state);
        fastboot_info(dev_state);
    }
    else if (strcmp("lock_critical", TempData) == 0)
    {
        critical_lock = 1;
    }
    else if (strcmp("unlock_critical", TempData) == 0)
        {
        printf("flashing bootloader-related partitions has its own risk\n");
        critical_lock = 0;
    }
    else if (strcmp("lock", TempData) == 0)
    {
        // skip dev_is_unlock==0, device state not change
        if( dev_is_unlock == 1 )
        {
            printf("wipe user data\n");
            EraseUserData();
            write_device_state(1);
        }
    }
    else if (strcmp("unlock", TempData) == 0)
    {
        // skip dev_is_unlock==1, device state not change
        if( dev_is_unlock == 0 )
    {
            printf("wipe user data\n");
            EraseUserData();
            // write unlock (0)
            write_device_state(0);
    }
    }
    else
#endif//CONFIG_ANDROID_SECURE_BOOT_CLASS_B
    {
        ret = -1;
    }

    if (ret == 0)
        fastboot_okay("Done!!");
    else
        fastboot_fail("invalid command");
    return ret;
}

#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
int do_setActive(const char *arg, const char *data, unsigned int data_length)
{
    unsigned int slotNumber = 0;

    if(*data != 'a' && *data != 'b')
    {
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data -'a';

    printf("%s:%d:%s: fastboot set active of slot number %d.\n",__FILE__,__LINE__,__FUNCTION__, slotNumber);
    if(ab_setActiveSlot(slotNumber) == 0) //0 means set OK.
    {
        fastboot_okay("Done!!");
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotSuffixes(const char *arg, const char *data, unsigned int data_length)
{
    char slotSuffix[4] = {0};

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    if(ab_appendSlotSuffix(slotSuffix,""))
    {
        fastboot_okay(slotSuffix);
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_hasSlot(const char *arg, const char *data, unsigned int data_length)
{
    char partition_a[64] = {0};
    char partition_b[64] = {0};
    uint8_t pnum;
    struct mtd_device *dev;
    struct part_info *part;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    memcpy(partition_a,data,data_length);
    strcat(partition_a,"_a");
    memcpy(partition_b,data,data_length);
    strcat(partition_b,"_b");
    if((mtdparts_init() == 0)
        && (find_dev_and_part(partition_a, &dev, &pnum, &part) == 0)
        && (find_dev_and_part(partition_b, &dev, &pnum, &part) == 0))
    {
    	fastboot_okay("yes");
    }
    else
    {
    	fastboot_okay("no");
    }
    return 0;
}

int do_getvar_currentSlot(const char *arg, const char *data, unsigned int data_length)
{
    char str[32] = {0};
    int currentSlot = 0;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    currentSlot = ab_getCurrentSlotIndex();
    if(currentSlot >= 0)
    {
        sprintf(str, "%c", currentSlot+'a');
        fastboot_okay(str);
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotSuccessful(const char *arg, const char *data, unsigned int data_length)
{
    int slotNumber = 0, isSuccessful = -1;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    if(*data != 'a' && *data != 'b')
    {
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    isSuccessful = ab_getSlotSuccessful(slotNumber);
    if(isSuccessful >= 0)
    {
        if(isSuccessful)
            fastboot_okay("yes");
        else
            fastboot_okay("no");
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotUnbootable(const char *arg, const char *data, unsigned int data_length)
{
    int slotNumber = 0, isUnbootable = -1;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    if(*data != 'a' && *data != 'b')
    {
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    isUnbootable = ab_getSlotUnbootable(slotNumber);
    if(isUnbootable >= 0)
    {
        if(isUnbootable)
            fastboot_okay("yes");
        else
            fastboot_okay("no");
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotRetryCount(const char *arg, const char *data, unsigned int data_length)
{
    char str[32] = {0};
    int retryCount = 0;
    int slotNumber = 0;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        return 0;
    }

    if(*data != 'a' && *data != 'b')
    {
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    retryCount = ab_getSlotRetryCount(slotNumber);
    if(retryCount >= 0)
    {
        sprintf(str, "%d", retryCount);
        fastboot_okay(str);
        return 0;
    }
    else
    {
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

#endif

#endif
