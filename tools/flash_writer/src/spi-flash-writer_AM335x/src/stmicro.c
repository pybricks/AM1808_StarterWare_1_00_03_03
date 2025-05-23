/*
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright 2008, Network Appliance Inc.
 * Jason McMullan <mcmullan@netapp.com>
 *
 * Copyright (C) 2004-2007 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/omap3_spi.h"


/* M25Pxx-specific commands */
#define CMD_M25PXX_WREN		0x06	/* Write Enable */
#define CMD_M25PXX_WRDI		0x04	/* Write Disable */
#define CMD_M25PXX_RDSR		0x05	/* Read Status Register */
#define CMD_M25PXX_WRSR		0x01	/* Write Status Register */
#define CMD_M25PXX_READ		0x03	/* Read Data Bytes */
#define CMD_M25PXX_FAST_READ	0x0b	/* Read Data Bytes at Higher Speed */
#define CMD_M25PXX_PP		0x02	/* Page Program */
#define CMD_M25PXX_SE		0xd8	/* Sector Erase */
#define CMD_M25PXX_BE		0xc7	/* Bulk Erase */
#define CMD_M25PXX_DP		0xb9	/* Deep Power-down */
#define CMD_M25PXX_RES		0xab	/* Release from DP, and Read Signature */

#define STM_ID_M25P16		0x15
#define STM_ID_M25P20		0x12
#define STM_ID_M25P32		0x16
#define STM_ID_M25P40		0x13
#define STM_ID_M25P64		0x17
#define STM_ID_M25P80		0x14
#define STM_ID_M25P128		0x18

#define STMICRO_SR_WIP		(1 << 0)	/* Write-in-Progress */

static const struct stmicro_spi_flash_params stmicro_spi_flash_table[] = {
	{
		STM_ID_M25P16,
		256,
		256,
		32,
		"M25P16",
	},
	{
		STM_ID_M25P20,
		256,
		256,
		4,
		"M25P20",
	},
	{
		STM_ID_M25P32,
		256,
		256,
		64,
		"M25P32",
	},
	{
		STM_ID_M25P40,
		256,
		256,
		8,
		"M25P40",
	},
	{
		STM_ID_M25P64,
		256,
		256,
		128,
		"M25P64",
	},
	{
		STM_ID_M25P80,
		256,
		256,
		16,
		"M25P80",
	},
	{
		STM_ID_M25P128,
		256,
		1024,
		64,
		"M25P128",
	},
};

static int stmicro_wait_ready(struct spi_flash *flash, unsigned long timeout) {
	struct spi_slave *spi = flash->spi;
	int ret;
	u8 status;
	u8 cmd[4] = { CMD_M25PXX_RDSR, 0xff, 0xff, 0xff };

	ret = spi_xfer(spi, 32, &cmd[0], NULL, SPI_XFER_BEGIN);
	if (ret) {
		printf("SF: Failed to send command %02x: %d\n", cmd[0], ret);
		return ret;
	}

	do {
		ret = spi_xfer(spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & STMICRO_SR_WIP) == 0)
			break;

	} while (1);

	spi_xfer(spi, 0, NULL, NULL, SPI_XFER_END);

	if ((status & STMICRO_SR_WIP) == 0)
		return 0;

	/* Timed out */
	return -1;
}

static int stmicro_read_fast(struct spi_flash *flash,
			     u32 offset, size_t len, void *buf) {
	struct stmicro_spi_flash *stm = to_stmicro_spi_flash(flash);
	unsigned long page_addr;
	unsigned long page_size;
	u8 cmd[5];

	page_size = stm->params->page_size;
	page_addr = offset / page_size;

	cmd[0] = CMD_READ_ARRAY_FAST;
	cmd[1] = page_addr >> 8;
	cmd[2] = page_addr;
	cmd[3] = offset % page_size;
	cmd[4] = 0x00;

	return spi_flash_read_common(flash, cmd, sizeof(cmd), buf, len);
}

static int stmicro_write(struct spi_flash *flash,
			 u32 offset, size_t len, unsigned char *buf) {
	struct stmicro_spi_flash *stm = to_stmicro_spi_flash(flash);
	unsigned long page_addr;
	unsigned long byte_addr;
	unsigned long page_size;
	size_t chunk_len;
	size_t actual;
	int ret;
	u8 cmd[4];

	page_size = stm->params->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf("SF: Unable to claim SPI bus\n");
		return ret;
	}

	ret = 0;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = ((len - actual) < (page_size - byte_addr) ?
								(len - actual) : (page_size - byte_addr));

		cmd[0] = CMD_M25PXX_PP;
		cmd[1] = page_addr >> 8;
		cmd[2] = page_addr;
		cmd[3] = byte_addr;

		printf
		    ("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x } chunk_len = %d\n",
		     buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], chunk_len);

		ret = spi_flash_cmd(flash->spi, CMD_M25PXX_WREN, NULL, 0);
		if (ret < 0) {
			printf("SF: Enabling Write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, 4,
					  buf + actual, chunk_len);
		if (ret < 0) {
			printf("SF: STMicro Page Program failed\n");
			break;
		}

		ret = stmicro_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret < 0) {
			printf("SF: STMicro page programming timed out\n");
			break;
		}

		page_addr++;
		byte_addr = 0;
	}

	printf("SF: STMicro: Successfully programmed %u bytes @ 0x%x\n",
	      len, offset);

	spi_release_bus(flash->spi);
	return ret;
}

int stmicro_erase(struct spi_flash *flash, u32 offset, size_t len) {
	struct stmicro_spi_flash *stm = to_stmicro_spi_flash(flash);
	unsigned long sector_size;
	size_t actual;
	int ret;
	u8 cmd[4];

	/*
	 * This function currently uses sector erase only.
	 * probably speed things up by using bulk erase
	 * when possible.
	 */

	sector_size = stm->params->page_size * stm->params->pages_per_sector;

	if (offset % sector_size || len % sector_size) {
		printf("SF: Erase offset/length not multiple of sector size\n");
		return -1;
	}

	len /= sector_size;
	cmd[0] = CMD_M25PXX_SE;
	cmd[2] = 0x00;
	cmd[3] = 0x00;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf("SF: Unable to claim SPI bus\n");
		return ret;
	}

	ret = 0;
	for (actual = 0; actual < len; actual++) {
		cmd[1] = (offset / sector_size) + actual;

		ret = spi_flash_cmd(flash->spi, CMD_M25PXX_WREN, NULL, 0);
		if (ret < 0) {
			printf("SF: Enabling Write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret < 0) {
			printf("SF: STMicro page erase failed\n");
			break;
		}

		/* Up to 2 seconds */
		ret = stmicro_wait_ready(flash, 2 * CFG_HZ);
		if (ret < 0) {
			printf("SF: STMicro page erase timed out\n");
			break;
		}
	}

	printf("SF: STMicro: Successfully erased %u bytes @ 0x%x\n",
	      len * sector_size, offset);

	spi_release_bus(flash->spi);
	return ret;
}

struct spi_flash *spi_flash_probe_stmicro(struct spi_slave *spi, u8 * idcode) {
	const struct stmicro_spi_flash_params *params;
	struct stmicro_spi_flash *stm;
	unsigned int i;
	int ret;
	u8 id[3];

	ret = spi_flash_cmd(spi, CMD_READ_ID, id, sizeof(id));
	if (ret)
		return NULL;

	for (i = 0; i < ARRAY_SIZE(stmicro_spi_flash_table); i++) {
		params = &stmicro_spi_flash_table[i];
		if (params->idcode1 == idcode[2]) {
			break;
		}
	}

	if (i == ARRAY_SIZE(stmicro_spi_flash_table)) {
		printf("SF: Unsupported STMicro ID %02x\n", id[1]);
		return NULL;
	}

	stm = (struct stmicro_spi_flash *) malloc(sizeof(struct stmicro_spi_flash));
	if (!stm) {
		printf("SF: Failed to allocate memory\n");
		return NULL;
	}

	stm->params = params;
	stm->flash.spi = spi;
	stm->flash.name = params->name;

	stm->flash.write = stmicro_write;
	stm->flash.erase = stmicro_erase;
	stm->flash.read = stmicro_read_fast;
	stm->flash.size = params->page_size * params->pages_per_sector
	    * params->nr_sectors;

	printf("SF: Detected %s with page size %u, total %u bytes\n",
	      params->name, params->page_size, stm->flash.size);

	return &stm->flash;
}
