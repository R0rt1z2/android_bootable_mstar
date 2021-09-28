
//#define DEBUG

#include <common.h>
#include <malloc.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

/* GD25xx-specific commands */
#define CMD_GD25XX_WREN		0x06	/* Write Enable */
#define CMD_GD25XX_WRDI		0x04	/* Write Disable */
#define CMD_GD25XX_RDSR		0x05	/* Read Status Register */
#define CMD_GD25XX_WRSR		0x01	/* Write Status Register */
#define CMD_GD25XX_READ		0x03	/* Read Data Bytes */
#define CMD_GD25XX_FAST_READ	0x0b	/* Read Data Bytes at Higher Speed */
#define CMD_GD25XX_PP		0x02	/* Page Program */
#define CMD_GD25XX_SE		0x20	/* Sector Erase */
#define CMD_GD25XX_BE		0xD8	/* Block Erase */
#define CMD_GD25XX_CE		0xc7	/* Chip Erase */
#define CMD_GD25XX_DP		0xb9	/* Deep Power-down */
#define CMD_GD25XX_RES		0xab	/* Release from DP, and Read Signature */

#define SRP1				(1<<8)	/* Extend bit, Don't care */
#define SRP0				(1<<7)
#define SR_BP4				(1<<6)
#define SR_BP3				(1<<5)
#define SR_BP2				(1<<4)
#define SR_BP1				(1<<3)
#define SR_BP0				(1<<2)

struct gigadevice_spi_flash_params {
	u16 idcode;
	u16 page_size;
	u16 pages_per_sector;
	u16 sectors_per_block;
	u16 nr_blocks;
	const char *name;
};

struct gigadevice_spi_flash {
	struct spi_flash flash;
	const struct gigadevice_spi_flash_params *params;
};

static inline struct gigadevice_spi_flash *to_gigadevice_spi_flash(struct spi_flash
							       *flash)
{
	return container_of(flash, struct gigadevice_spi_flash, flash);
}

static const struct gigadevice_spi_flash_params gigadevice_spi_flash_table[] = {
	{
		.idcode = 0x4012,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 4,
		.name = "GD25Q20",
	},
	{
		.idcode = 0x4013,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 8,
		.name = "GD25Q40",
	},
	{
		.idcode = 0x4014,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 16,
		.name = "GD25Q80",
	},
	{
		.idcode = 0x4015,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 32,
		.name = "GD25Q16",
	},
	{
		.idcode = 0x4016,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 64,
		.name = "GD25Q32",
	},
	{
		.idcode = 0x4017,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 128,
		.name = "GD25Q64",
	},
	{
		.idcode = 0x4018,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 256,
		.name = "GD25Q128",
	},
	{
		.idcode = 0x4019,
		.page_size = 256,
		.pages_per_sector = 16,
		.sectors_per_block = 16,
		.nr_blocks = 512,
		.name = "GD25Q256",
	},
};

static int gigadevice_write(struct spi_flash *flash,
			  u32 offset, size_t len, const void *buf)
{
	struct gigadevice_spi_flash *gd = to_gigadevice_spi_flash(flash);
	unsigned long page_addr;
	unsigned long byte_addr;
	unsigned long page_size;
	size_t chunk_len;
	size_t actual;
	int ret;
	u8 cmd[4];

	page_size = gd->params->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: Unable to claim SPI bus\n");
		return ret;
	}

	ret = 0;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = min(len - actual, page_size - byte_addr);

		cmd[0] = CMD_GD25XX_PP;
		cmd[1] = page_addr >> 8;
		cmd[2] = page_addr;
		cmd[3] = byte_addr;

		debug
		    ("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x } chunk_len = %d\n",
		     buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], chunk_len);

		ret = spi_flash_cmd(flash->spi, CMD_GD25XX_WREN, NULL, 0);
		if (ret < 0) {
			debug("SF: Enabling Write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, 4,
					  buf + actual, chunk_len);
		if (ret < 0) {
			debug("SF: gigadevice Page Program failed\n");
			break;
		}

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret)
			break;

		page_addr++;
		byte_addr = 0;
	}

	debug("SF: gigadevice: Successfully programmed %u bytes @ 0x%x\n",
	      len, offset);

	spi_release_bus(flash->spi);
	return ret;
}

static int gigadevice_erase(struct spi_flash *flash, u32 offset, size_t len)
{
	return spi_flash_cmd_erase(flash, CMD_GD25XX_BE, offset, len);
}

static int gigadevice_protect(struct spi_flash *flash, char *option)
{
	int ret;
	u8 status;
	u8 cmd, prot_mask;
	
	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: Unable to claim SPI bus\n");
		return ret;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_GD25XX_RDSR, &status, 1);
	if (ret < 0) {
		debug("SF: read status register failed\n");
		return ret;
	}
	
	prot_mask = (SRP0 | SR_BP4 | SR_BP3 | SR_BP2 | SR_BP1 | SR_BP0);
	if (strcmp(option, "status") == 0) {
		if ((status & prot_mask) == prot_mask) {
			printf("SF : Flash Lock\n");
		} else if ((status & prot_mask) == 0) {
			printf("SF : Flash unlock\n");
		} else {
			printf("SF : Flash status unkown\n");
		}
		return 0;
	}
	
	// lock/unlock all block
	if (strcmp(option, "lock") == 0) {
		status |= prot_mask;
	} else if (strcmp(option, "unlock") == 0) {
		status &= (~ prot_mask);
	} else {
		printf("unkown option\n");
		return -1;
	}
	
	ret = spi_flash_cmd(flash->spi, CMD_GD25XX_WREN, NULL, 0);
	if (ret < 0) {
		debug("SF: Enabling Write failed\n");
		return ret;
	}

	cmd = CMD_GD25XX_WRSR;
	ret = spi_flash_cmd_write(flash->spi, &cmd, 1, &status, 1);
	if (ret < 0) {
		debug("SF: write status register failed\n");
		return ret;
	}
	
	ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
	if (ret) {
		debug("SF : %s flash fail !\n", (is_lock ? "lock" : "unlock"));
		return ret;
	}
	
	spi_release_bus(flash->spi);
	return ret;
}


struct spi_flash *spi_flash_probe_gigadevice(struct spi_slave *spi, u8 *idcode)
{
	const struct gigadevice_spi_flash_params *params;
	struct gigadevice_spi_flash *gd;
	unsigned int i;
	u16 id = idcode[2] | idcode[1] << 8;

	for (i = 0; i < ARRAY_SIZE(gigadevice_spi_flash_table); i++) {
		params = &gigadevice_spi_flash_table[i];
		if (params->idcode == id)
			break;
	}

	if (i == ARRAY_SIZE(gigadevice_spi_flash_table)) {
		debug("SF: Unsupported gigadevice ID %04x\n", id);
		return NULL;
	}

	gd = malloc(sizeof(*gd));
	if (!gd) {
		debug("SF: Failed to allocate memory\n");
		return NULL;
	}
	memset(gd, 0, sizeof(*gd));

	gd->params = params;
	gd->flash.spi = spi;
	gd->flash.name = params->name;

	gd->flash.write = gigadevice_write;
	gd->flash.erase = gigadevice_erase;
	gd->flash.read = spi_flash_cmd_read_fast;
	gd->flash.prot = gigadevice_protect;
	gd->flash.sector_size = params->page_size * params->pages_per_sector
		* params->sectors_per_block;
	gd->flash.size = gd->flash.sector_size * params->nr_blocks;

	return &gd->flash;
}
