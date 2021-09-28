#include <common.h>

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <search.h>
#include <errno.h>
#include <MsSystem.h>

#include "ufs-mstar.h"
#include "../disk/part_ufs.h"
#if defined (CONFIG_DUAL_SYSTEM)
#include <MsBoot.h>
#endif

/* references to names in env_common.c */
extern uchar default_environment[];

char *env_name_spec = "UFS";

#ifdef ENV_IS_EMBEDDED
extern uchar environment[];
env_t *env_ptr = (env_t *)(&environment[0]);
#else /* ! ENV_IS_EMBEDDED */
env_t *env_ptr = NULL;
#endif /* ENV_IS_EMBEDDED */

/* local functions */
#if !defined(ENV_IS_EMBEDDED)
static void use_default(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

#if !defined(CONFIG_ENV_OFFSET)
#define CONFIG_ENV_OFFSET 0
#endif

uchar env_get_char_spec(int index)
{
    return *((uchar *)(gd->env_addr + index));
}

int env_init(void)
{
    /* use default */
    gd->env_addr = (ulong)&default_environment[0];
    gd->env_valid = 1;

    return 0;
}

#ifdef CONFIG_CMD_SAVEENV
int write_env(unsigned long size,
			unsigned long offset, const void *buffer)
{
	uint blk_cnt, n,i;
	int ret;
	ufs_partition_t	*mpart;
	mpart = malloc(sizeof(*mpart));
	char env_part_name[32] = {0};
	#if 0
	char command[100];
	#endif

	strcpy(env_part_name, "MPOOL");

#if defined (CONFIG_DUAL_SYSTEM)
	if (0 != read_bootflag())
	{
		strcat(env_part_name, DUAL_SYSTEM_PARTITION_SUFFIX);
	}
#endif

	blk_cnt   = ALIGN(size, 4096) / 4096;

	memset(mpart,0,sizeof(*mpart));

	for(i=1 ; i<= UFS_RESERVED_FOR_MAP ; i++)
	{
		n = ufs_block_read(0, i, 1, (void *)mpart);
		if(n != 1)
			return -1;

		if(strcmp(env_part_name, (const char *)mpart->name)==0)
		{
			break;
		}
	}

	if(i > UFS_RESERVED_FOR_MAP)
		return -1;

	#if 0
	sprintf(command,"ufs write.p.continue 0x%x %s 0x%x 0x%x",
		(unsigned int)buffer, env_part_name,
		(unsigned int)(mpart->block_count - (blk_cnt <<1) + (offset >> 12)),
		(unsigned int)blk_cnt<<12);
	ret = run_command((const char *)command, 0);
	#else
	n = ufs_block_write(0, (mpart->block_count - (blk_cnt <<1) + (offset >> 12)), blk_cnt, (void *)buffer);
	if( n!= blk_cnt)
		ret = -1;
	else
		ret = 0;
	#endif

	free(mpart);

	return (ret != -1) ? 0 : -1;
}

int saveenv(void)
{
	env_t *tmp_env1 = NULL;
	ssize_t	len;
	char *res=NULL;
	u32 offset = 0;//CONFIG_ENV_OFFSET;

	tmp_env1 = (env_t *)malloc(CONFIG_ENV_SIZE);

	if (!tmp_env1) {
		set_default_env("!malloc() failed");
		return 1;
	}

	res = (char *)tmp_env1->data;
	len = hexport_r(&env_htab, '\0', &res, ENV_SIZE);
	if (len < 0) {
		error("Cannot export environment: errno = %d\n", errno);
		free(tmp_env1);
		return 1;
	}
	tmp_env1->crc = crc32(0, tmp_env1->data, ENV_SIZE);
	printf("Writing to UFS... ");
	if (write_env(CONFIG_ENV_SIZE, offset, (u_char *)tmp_env1)) {
		puts("failed to save env\n");
		//return 1;
	}
	if (write_env(CONFIG_ENV_SIZE, (offset + CONFIG_ENV_SIZE), (u_char *)tmp_env1)) {
		puts("failed to save backup env\n");
		//return 1;
	}

	free(tmp_env1);
	puts("done\n");
	return 0;
}
#endif /* CONFIG_CMD_SAVEENV */

int read_env(unsigned long size,
			unsigned long offset, const void *buffer)
{
	uint blk_cnt, n,i;
	int ret;
	ufs_partition_t	*mpart;
	mpart = malloc(sizeof(*mpart));
	char env_part_name[32] = {0};
	#if 0
	char command[100];
	#endif

	strcpy(env_part_name, "MPOOL");

#if defined (CONFIG_DUAL_SYSTEM)
	if (0 != read_bootflag())
	{
		strcat(env_part_name, DUAL_SYSTEM_PARTITION_SUFFIX);
	}
#endif

	blk_cnt   = ALIGN(size, 4096) / 4096;

	memset(mpart,0,sizeof(*mpart));

	for(i=1 ; i<= UFS_RESERVED_FOR_MAP ; i++)
	{
		n = ufs_block_read(0, i, 1, (void *)mpart);
		if(n != 1)
			return -1;

		if(strcmp(env_part_name, (const char *)mpart->name)==0)
		{
			break;
		}
	}

	if(i > UFS_RESERVED_FOR_MAP)
		return -1;

	#if 0
	sprintf(command,"ufs read.p.continue 0x%x %s 0x%x 0x%x",
		(unsigned int)buffer, env_part_name,
		(unsigned int)(mpart->block_count - (blk_cnt <<1) + (offset >> 12)),
		(unsigned int)blk_cnt<<12);
	ret=run_command((const char *)command, 0);
	#else
	n = ufs_block_read(0, (mpart->block_count - (blk_cnt <<1) + (offset >> 12)), blk_cnt, (void *)buffer);
	if( n!= blk_cnt)
		ret = -1;
	else
		ret = 0;
	#endif

	free(mpart);

    return (ret != -1) ? 0 : -1;
}

void env_relocate_spec(void)
{
#if !defined(ENV_IS_EMBEDDED)
	char *buf=malloc(CONFIG_ENV_SIZE+CONFIG_ENV_SIZE);
	char *bufbak=NULL;
	env_t *ep, *epbak;
	u32 crc, crcbak, offset = 0;//CONFIG_ENV_OFFSET;
	int ret, retbak;
	if(!buf)
	{
		use_default();
		printf("malloc env buf fail, using default\n");
		return;
	}
	bufbak=buf+CONFIG_ENV_SIZE;

	ret = read_env(CONFIG_ENV_SIZE, offset, buf);
	retbak = read_env(CONFIG_ENV_SIZE, (offset + CONFIG_ENV_SIZE), bufbak);
	ep = (env_t *)buf;
	epbak = (env_t *)bufbak;
	crc = crc32(0, ep->data, ENV_SIZE);
	crcbak = crc32(0, epbak->data, ENV_SIZE);

	if (((crc != ep->crc) || (ret == -1)) && ((crcbak != epbak->crc) || (retbak == -1)))
	{
		use_default();
		free(buf);
		return;
	}

	if (crc != ep->crc)
	{
		if (write_env(CONFIG_ENV_SIZE, offset, bufbak)) {
			puts("failed to resave env\n");
			//return 1;
		}
	}
	else if (crcbak != epbak->crc)
	{
		if (write_env(CONFIG_ENV_SIZE, offset + CONFIG_ENV_SIZE, buf)) {
			puts("failed to resave backup env\n");
			//return 1;
		}
	}
	env_import((crc != ep->crc) ? bufbak : buf, 1);
	free(buf);
#endif
}

#if !defined(ENV_IS_EMBEDDED)
static void use_default()
{
	set_default_env(NULL);
}
#endif
