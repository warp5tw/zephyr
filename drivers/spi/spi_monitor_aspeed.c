/*
 * Copyright (c) 2021 Chin-Ting Kuo <chin-ting_kuo@aspeedtech.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT aspeed_spi_monitor_controller

#include <zephyr/types.h>
#include <stddef.h>
#include <device.h>
#include <drivers/clock_control.h>
#include <errno.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(spim_aspeed, CONFIG_SPI_LOG_LEVEL);
#include "spi_context.h"
#include <sys/sys_io.h>
#include <sys/__assert.h>
#include <drivers/misc/aspeed/pfr_aspeed.h>


#define CMD_TABLE_VALUE(G, W, R, M, DAT_MODE, DUMMY, PROG_SZ, ADDR_LEN, ADDR_MODE, CMD) \
	(G << 29 | W << 28 | R << 27 | M << 26 | DAT_MODE << 24 | DUMMY << 16 | PROG_SZ << 13 | \
	ADDR_LEN << 10 | ADDR_MODE << 8 | CMD)

struct cmd_table_info cmds_array[] = {
	{.cmd = CMD_READ_1_1_1_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 1, 0, 0, 3, 1, CMD_READ_1_1_1_3B)},
	{.cmd = CMD_READ_1_1_1_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 1, 0, 0, 4, 1, CMD_READ_1_1_1_4B)},
	{.cmd = CMD_FREAD_1_1_1_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 1, 8, 0, 3, 1, CMD_FREAD_1_1_1_3B)},
	{.cmd = CMD_FREAD_1_1_1_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 1, 8, 0, 4, 1, CMD_FREAD_1_1_1_4B)},
	{.cmd = CMD_READ_1_1_2_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 2, 8, 0, 3, 1, CMD_READ_1_1_2_3B)},
	{.cmd = CMD_READ_1_1_2_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 2, 8, 0, 4, 1, CMD_READ_1_1_2_4B)},
	{.cmd = CMD_READ_1_1_4_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 3, 8, 0, 3, 1, CMD_READ_1_1_4_3B)},
	{.cmd = CMD_READ_1_1_4_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 1, 3, 8, 0, 4, 1, CMD_READ_1_1_4_4B)},
	{.cmd = CMD_PP_1_1_1_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 1, 0, 1, 3, 1, CMD_PP_1_1_1_3B)},
	{.cmd = CMD_PP_1_1_1_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 1, 0, 1, 4, 1, CMD_PP_1_1_1_4B)},
	{.cmd = CMD_PP_1_1_4_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 3, 0, 1, 3, 1, CMD_PP_1_1_4_3B)},
	{.cmd = CMD_PP_1_1_4_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 3, 0, 1, 4, 1, CMD_PP_1_1_4_4B)},
	{.cmd = CMD_SE_1_1_0_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 0, 0, 1, 3, 1, CMD_SE_1_1_0_3B)},
	{.cmd = CMD_SE_1_1_0_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 0, 0, 1, 4, 1, CMD_SE_1_1_0_4B)},
	{.cmd = CMD_SE_1_1_0_64_3B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 0, 0, 5, 3, 1, CMD_SE_1_1_0_64_3B)},
	{.cmd = CMD_SE_1_1_0_64_4B,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 1, 0, 0, 5, 4, 1, CMD_SE_1_1_0_64_4B)},
	{.cmd = CMD_WREN,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 0, 0, 0, 0, 0, 0, 0, CMD_WREN)},
	{.cmd = CMD_WRDIS,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 0, 0, 0, 0, 0, 0, 0, CMD_WRDIS)},
	{.cmd = CMD_RDSR,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 0, 1, 0, 0, 0, 0, CMD_RDSR)},
	{.cmd = CMD_RDSR2,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 0, 1, 0, 0, 0, 0, CMD_RDSR2)},
	{.cmd = CMD_WRSR,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 0, 1, 0, 0, 0, 0, CMD_WRSR)},
	{.cmd = CMD_WRSR2,
		.cmd_table_val = CMD_TABLE_VALUE(1, 1, 0, 0, 1, 0, 0, 0, 0, CMD_WRSR2)},
	{.cmd = CMD_RDCR,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 0, 1, 0, 0, 0, 0, CMD_RDCR)},
	{.cmd = CMD_EN4B,
		.cmd_table_val = CMD_TABLE_VALUE(0, 0, 0, 0, 0, 0, 0, 0, 0, CMD_EN4B)},
	{.cmd = CMD_EX4B,
		.cmd_table_val = CMD_TABLE_VALUE(0, 0, 0, 0, 0, 0, 0, 0, 0, CMD_EX4B)},
	{.cmd = CMD_SFDP,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 0, 1, 8, 0, 3, 1, CMD_SFDP)},
	{.cmd = CMD_RDID,
		.cmd_table_val = CMD_TABLE_VALUE(1, 0, 1, 0, 1, 0, 0, 0, 0, CMD_RDID)},
};

/* control register */
#define SPIM_CTRL				(0x0000)
#define SPIM_STATUS				(0x0004)
#define SPIM_EAR				(0x0008)
#define SPIM_FIFO				(0x000C)
#define SPIM_LOG_BASE			(0x0010)
#define SPIM_LOG_SZ				(0x0014)
#define SPIM_LOG_PTR			(0x0018)
#define SPIM_LOCK_REG			(0x007C)
#define SPIM_VALID_LIST_BASE	(0x0080)
#define SPIM_VALID_ADDR_FTR		(0x0100)

/* valid command table */
#define SPIM_CMD_TABLE_NUM				32
#define SPIM_CMD_TABLE_VALID_MASK		GENMASK(31, 30)
#define SPIM_CMD_TABLE_VALID_ONCE_BIT	BIT(31)
#define SPIM_CMD_TABLE_VALID_BIT		BIT(30)
#define SPIM_CMD_TABLE_IS_GENERIC_CMD	BIT(29)
#define SPIM_CMD_TABLE_IS_WRITE_CMD		BIT(28)
#define SPIM_CMD_TABLE_IS_READ_CMD		BIT(27)
#define SPIM_CMD_TABLE_IS_MEM_CMD		BIT(26)
#define SPIM_CMD_TABLE_DATA_MODE_MASK	GENMASK(25, 24)
#define SPIM_CMD_TABLE_LOCK_BIT			BIT(23)
#define SPIM_CMD_TABLE_DUMMY_MASK		GENMASK(21, 16)
#define SPIM_CMD_TABLE_PROGRAM_SZ_MASK	GENMASK(15, 13)
#define SPIM_CMD_TABLE_ADDR_LEN_MASK	GENMASK(12, 10)
#define SPIM_CMD_TABLE_ADDR_MODE_MASK	GENMASK(9, 8)
#define SPIM_CMD_TABLE_CMD_MASK			GENMASK(7, 0)

/* valid address region configuration */
#define SPIM_PRIV_WRITE_SELECT	0x57000000
#define SPIM_PRIV_READ_SELECT	0x52000000
#define SPIM_ADDR_PRIV_REG_NUN	512
#define SPIM_ADDR_PRIV_BIT_NUN	(512 * 32)

/* PFR related control */
#define SPIM_MODE_SCU_CTRL		(0x00f0)

struct aspeed_spim_data {
	struct k_sem sem;
	uint8_t valid_cmd_list[SPIM_CMD_TABLE_NUM];
	uint32_t valid_cmd_num;
	uint32_t read_forbidden_regions[32];
	uint32_t read_forbidden_region_num;
	uint32_t write_forbidden_regions[32];
	uint32_t write_forbidden_region_num;
};

struct aspeed_spim_config {
	mm_reg_t ctrl_base;
	uint32_t irq_num;
	uint32_t irq_priority;
	uint32_t log_ram_addr;
	uint32_t log_max_len;
	uint32_t ctrl_idx;
	const struct device *parent;
	bool multi_passthrough;
};

struct aspeed_spim_common_config {
	mm_reg_t scu_base;
};

struct aspeed_spim_common_data {
	struct k_spinlock lock;
};

static void acquire_device(const struct device *dev)
{
	if (IS_ENABLED(CONFIG_MULTITHREADING)) {
		struct aspeed_spim_data *const data = dev->data;
		k_sem_take(&data->sem, K_FOREVER);
	}
}

static void release_device(const struct device *dev)
{
	if (IS_ENABLED(CONFIG_MULTITHREADING)) {
		struct aspeed_spim_data *const data = dev->data;
		k_sem_give(&data->sem);
	}
}

void spim_scu_ctrl_set(const struct device *dev, uint32_t mask, uint32_t val)
{
	const struct aspeed_spim_common_config *config = dev->config;
	struct aspeed_spim_common_data *const data = dev->data;
	mm_reg_t spim_scu_ctrl = config->scu_base + SPIM_MODE_SCU_CTRL;
	uint32_t reg_val;
	/* Avoid SCU0F0 being accessed by more than a thread */
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	reg_val = sys_read32(spim_scu_ctrl);
	reg_val &= ~(mask);
	reg_val |= val;
	sys_write32(reg_val, spim_scu_ctrl);

	k_spin_unlock(&data->lock, key);
}

void spim_scu_ctrl_clear(const struct device *dev, uint32_t clear_bits)
{
	const struct aspeed_spim_common_config *config = dev->config;
	struct aspeed_spim_common_data *const data = dev->data;
	mm_reg_t spim_scu_ctrl = config->scu_base + SPIM_MODE_SCU_CTRL;
	uint32_t reg_val;
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	reg_val = sys_read32(spim_scu_ctrl);
	reg_val &= ~(clear_bits);
	sys_write32(reg_val, spim_scu_ctrl);

	k_spin_unlock(&data->lock, key);
}

void spim_scu_passthrough_mode(const struct device *dev,
	uint32_t passthrough_mode, bool passthrough_en)
{
	const struct aspeed_spim_config *config = dev->config;

	if (passthrough_en) {
		spim_scu_ctrl_set(config->parent, BIT(config->ctrl_idx - 1) << 4,
			BIT(config->ctrl_idx - 1) << 4);
	} else {
		spim_scu_ctrl_clear(config->parent, BIT(config->ctrl_idx - 1) << 4);
	}

	ARG_UNUSED(passthrough_mode);
}

void spim_ctrl_passthrough_mode(const struct device *dev,
	uint32_t passthrough_mode, bool passthrough_en)
{
	const struct aspeed_spim_config *config = dev->config;
	uint32_t ctrl_reg_val;

	acquire_device(dev);

	ctrl_reg_val = sys_read32(config->ctrl_base);

	ctrl_reg_val &= ~0x00000003;
	if (passthrough_en) {
		if (passthrough_mode == SPIM_MULTI_PASSTHROUGH)
			ctrl_reg_val |= BIT(1);
		else
			ctrl_reg_val |= BIT(0);
	}

	sys_write32(ctrl_reg_val, config->ctrl_base);

	release_device(dev);
}

/* dump command information recored in valid command table */
void spim_dump_valid_command_table(const struct device *dev)
{
	uint32_t i;
	const struct aspeed_spim_config *config = dev->config;
	uint32_t reg_val;
	uint32_t addr_len, addr_mode;
	uint32_t dummy_cyc;
	uint32_t data_mode;
	uint8_t cmd;
	uint32_t prog_sz;

	acquire_device(dev);

	for (i = 0; i < SPIM_CMD_TABLE_NUM; i++) {
		reg_val = sys_read32(config->ctrl_base + SPIM_VALID_LIST_BASE + i * 4);
		if (reg_val == 0)
			continue;
		printk("[%s]idx %02d: 0x%08x\n", dev->name, i, reg_val);
	}

	printk("\ncmd info:\n");
	for (i = 0; i < SPIM_CMD_TABLE_NUM; i++) {
		reg_val = sys_read32(config->ctrl_base + SPIM_VALID_LIST_BASE + i * 4);
		if (reg_val == 0)
			continue;

		cmd = reg_val & SPIM_CMD_TABLE_CMD_MASK;
		addr_mode = (reg_val & SPIM_CMD_TABLE_ADDR_MODE_MASK) >> 8;
		addr_len = (reg_val & SPIM_CMD_TABLE_ADDR_LEN_MASK) >> 10;
		dummy_cyc = (reg_val & SPIM_CMD_TABLE_DUMMY_MASK) >> 16;
		data_mode = (reg_val & SPIM_CMD_TABLE_DATA_MODE_MASK) >> 24;
		prog_sz = (reg_val & SPIM_CMD_TABLE_PROGRAM_SZ_MASK) >> 13;

		printk("cmd: %02x, addr: len(%d)/", cmd, addr_len);
		switch (addr_mode) {
		case 1:
			printk("mode(single),");
			break;
		case 2:
			printk("mode(dual)  ,");
			break;
		case 3:
			printk("mode(quad)  ,");
			break;
		default:
			printk("mode(no)    ,");
		}

		printk(" dummy: %d, data_mode:", dummy_cyc);

		switch (data_mode) {
		case 1:
			printk(" single,");
			break;
		case 2:
			printk(" dual  ,");
			break;
		case 3:
			printk(" quad  ,");
			break;
		default:
			printk(" no    ,");
		}

		printk(" prog_sz: %03ldKB", prog_sz == 0 ? 0 : BIT(prog_sz + 1));
		(reg_val & SPIM_CMD_TABLE_IS_MEM_CMD) != 0 ?
			printk(", mem_op") : printk(",%*c", 7, ' ');
		(reg_val & SPIM_CMD_TABLE_IS_READ_CMD) != 0 ?
			printk(", read") : printk(",%*c", 5, ' ');
		(reg_val & SPIM_CMD_TABLE_IS_WRITE_CMD) != 0 ?
			printk(", write") : printk(",%*c", 6, ' ');
		(reg_val & SPIM_CMD_TABLE_IS_GENERIC_CMD) != 0 ?
			printk(", generic") : printk(",%*c", 8, ' ');
		(reg_val & SPIM_CMD_TABLE_VALID_BIT) != 0 ?
			printk(", valid") : printk(",%*c", 6, ' ');
		(reg_val & SPIM_CMD_TABLE_VALID_ONCE_BIT) != 0 ?
			printk(", valid once") : printk(",%*c", 11, ' ');
		(reg_val & SPIM_CMD_TABLE_LOCK_BIT) != 0 ?
			printk(", locked|") : printk(",%*c|", 7, ' ');
		printk("\n");
	}

	release_device(dev);
}

uint32_t spim_get_cmd_table_val(uint8_t cmd)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(cmds_array); i++) {
		if (cmds_array[i].cmd == cmd)
			return cmds_array[i].cmd_table_val;
	}

	LOG_ERR("Error: Cannot get item in command table cmd(%02x)\n", cmd);
	return 0;
}

void spim_valid_cmd_table_init(const struct device *dev,
	const uint8_t cmd_list[], uint32_t cmd_num, uint32_t flag)
{
	const struct aspeed_spim_config *config = dev->config;
	mm_reg_t table_base = config->ctrl_base + SPIM_VALID_LIST_BASE;
	uint32_t i;
	uint32_t reg_val;
	uint32_t idx = 3;

	acquire_device(dev);

	for (i = 0; i < cmd_num; i++) {
		reg_val = spim_get_cmd_table_val(cmd_list[i]);
		LOG_DBG("cmd %02x, val %08x", cmd_list[i], reg_val);
		if (reg_val == 0) {
			LOG_ERR("cmd is not recorded in cmds_array array");
			LOG_ERR("please edit it in spi_monitor_aspeed.c");
			continue;
		}

		if (flag & FLAG_CMD_TABLE_VALID_ONCE)
			reg_val |= SPIM_CMD_TABLE_VALID_ONCE_BIT;
		else
			reg_val |= SPIM_CMD_TABLE_VALID_BIT;

		switch (cmd_list[i]) {
		case CMD_EN4B:
			sys_write32(reg_val, table_base);
			continue;

		case CMD_EX4B:
			sys_write32(reg_val, table_base + 4);
			continue;
		default:
			idx++;
		}

		sys_write32(reg_val, table_base + idx * 4);
	}

	release_device(dev);
}

static int spim_get_empty_valid_cmd_slot(const struct device *dev)
{
	const struct aspeed_spim_config *config = dev->config;
	int idx;
	uint32_t reg_val;

	for (idx = 4; idx < SPIM_CMD_TABLE_NUM; idx++) {
		reg_val = sys_read32(config->ctrl_base + SPIM_VALID_LIST_BASE + idx * 4);
		if (reg_val == 0)
			return idx;
	}

	return -ENOSR;
}

static int spim_get_valid_cmd_slot(const struct device *dev,
	uint8_t cmd, uint32_t start_off)
{
	const struct aspeed_spim_config *config = dev->config;
	int idx;
	uint32_t reg_val;

	for (idx = start_off; idx < SPIM_CMD_TABLE_NUM; idx++) {
		reg_val = sys_read32(config->ctrl_base + SPIM_VALID_LIST_BASE + idx * 4);
		if ((reg_val & SPIM_CMD_TABLE_CMD_MASK) == cmd)
			return idx;
	}

	return -ENOSR;
}

/* - If the command already exists in valid command table and
 *   it is disabled, it will be enabled by spim_add_valid_command.
 * - If the command already exists in valid command table and
 *   it is lock, it will not be enabled and an error code will
 *   be returned.
 * - If the command doesn't exist in valid command table, an
 *   empty slot will be found and the command info will be
 *   filled into.
 */

int spim_add_valid_command(const struct device *dev,
	uint8_t cmd, uint32_t flag)
{
	int ret = 0;
	const struct aspeed_spim_config *config = dev->config;
	mm_reg_t table_base = config->ctrl_base + SPIM_VALID_LIST_BASE;
	int idx;
	uint32_t off;
	uint32_t reg_val;
	bool found = false;

	acquire_device(dev);

	/* check whether the command is already recorded in valid cmd table */
	for (off = 0; off < SPIM_CMD_TABLE_NUM; off++) {
		idx = spim_get_valid_cmd_slot(dev, cmd, off);
		if (idx >= 0) {
			found = true;
			reg_val = sys_read32(table_base + idx * 4);
			if ((reg_val & SPIM_CMD_TABLE_LOCK_BIT) != 0) {
				LOG_WRN("cmd %02x cannot be enabled in valid cmd table(%d)", cmd, idx);
				off = idx + 1;
				/* search for the next slot with the same command */
				continue;
			} else {
				reg_val &= ~SPIM_CMD_TABLE_VALID_MASK;
				if (flag & FLAG_CMD_TABLE_VALID_ONCE)
					reg_val |= SPIM_CMD_TABLE_VALID_ONCE_BIT;
				else
					reg_val |= SPIM_CMD_TABLE_VALID_BIT;

				sys_write32(reg_val, table_base + idx * 4);
				found = true;
				goto end;
			}
		}
	}

	/* If the cmd already exists in the valid command table and
	 * the register is locked, the same command should not be added again.
	 */
	if (found) {
		LOG_WRN("cmd %02x should not be added in the valid command table again", cmd);
		goto end;
	}

	reg_val = spim_get_cmd_table_val(cmd);
	/* cmd info is not found in valid table array */
	if (reg_val == 0) {
		LOG_ERR("cmd is not recorded in \"cmds_array\" array");
		LOG_ERR("please edit it in spi_monitor_aspeed.c");
		ret = -EINVAL;
		goto end;
	}

	if (flag & FLAG_CMD_TABLE_VALID_ONCE)
		reg_val |= SPIM_CMD_TABLE_VALID_ONCE_BIT;
	else
		reg_val |= SPIM_CMD_TABLE_VALID_BIT;

	switch (cmd) {
	case CMD_EN4B:
		sys_write32(reg_val, table_base);
		goto end;

	case CMD_EX4B:
		sys_write32(reg_val, table_base + 4);
		goto end;

	default:
		break;
	}

	idx = spim_get_empty_valid_cmd_slot(dev);
	if (idx < 0) {
		LOG_ERR("No more space for new command");
		ret = -ENOSR;
		goto end;
	}
	sys_write32(reg_val, table_base + idx * 4);

end:
	release_device(dev);

	return ret;
}

/* All command table slot which command is equal to "cmd"
 * parameter will be removed.
 */
int spim_remove_valid_command(const struct device *dev, uint8_t cmd)
{
	int ret = 0;
	const struct aspeed_spim_config *config = dev->config;
	mm_reg_t table_base = config->ctrl_base + SPIM_VALID_LIST_BASE;
	int idx;
	uint32_t off;
	uint32_t reg_val;
	bool found = false;

	acquire_device(dev);

	/* check whether the command is already recorded in valid cmd table */
	for (off = 0; off < SPIM_CMD_TABLE_NUM; off++) {
		idx = spim_get_valid_cmd_slot(dev, cmd, off);
		if (idx >= 0) {
			found = true;
			reg_val = sys_read32(table_base + idx * 4);
			if ((reg_val & SPIM_CMD_TABLE_LOCK_BIT) != 0 &&
				(reg_val & SPIM_CMD_TABLE_VALID_MASK) != 0) {
				LOG_ERR("cmd %02x is locked and cannot be removed or disabled. (%d)",
					cmd, idx);
				ret = -EINVAL;
				goto end;
			} else if ((reg_val & SPIM_CMD_TABLE_LOCK_BIT) == 0) {
				sys_write32(0, table_base + idx * 4);
			} else {
				LOG_INF("cmd %02x is locked and cannot be removed. (%d)",
					cmd, idx);
			}

			off = idx + 1;
			continue;
		}
	}

	if (!found) {
		LOG_ERR("cmd %02x is not found in valid command table", cmd);
		ret = -EINVAL;
		goto end;
	}

end:
	release_device(dev);

	return ret;
}

/* - The overall valid command table will be locked when
 *   flag is FLAG_CMD_TABLE_LOCK_ALL.
 * - All command table slot which command is equal to "cmd"
 *   parameter will be locked.
 */
int spim_lock_valid_command_table(const struct device *dev,
	uint8_t cmd, uint32_t flag)
{
	int ret = 0;
	const struct aspeed_spim_config *config = dev->config;
	mm_reg_t table_base = config->ctrl_base + SPIM_VALID_LIST_BASE;
	int idx;
	uint32_t off;
	uint32_t reg_val;
	bool found = false;

	acquire_device(dev);

	if ((flag & FLAG_CMD_TABLE_LOCK_ALL) != 0) {
		for (idx = 0; idx < SPIM_CMD_TABLE_NUM; idx++) {
			reg_val = sys_read32(table_base + idx * 4);
			reg_val |= SPIM_CMD_TABLE_LOCK_BIT;
			sys_write32(reg_val, table_base + idx * 4);
		}
		goto end;
	}

	for (off = 0; off < SPIM_CMD_TABLE_NUM; off++) {
		idx = spim_get_valid_cmd_slot(dev, cmd, off);
		if (idx >= 0) {
			found = true;
			reg_val = sys_read32(table_base + idx * 4);
			if ((reg_val & SPIM_CMD_TABLE_LOCK_BIT) != 0) {
				LOG_INF("cmd %02x is already locked (%d)", cmd, idx);
			} else {
				reg_val |= SPIM_CMD_TABLE_LOCK_BIT;
				sys_write32(reg_val, table_base + idx * 4);
			}

			off = idx + 1;
			continue;
		}
	}

	if (!found) {
		LOG_ERR("cmd %02x is not found in valid command table", cmd);
		ret = -EINVAL;
		goto end;
	}

end:
	release_device(dev);
	return ret;
}

void spim_rw_perm_init(const struct device *dev)
{
	const struct aspeed_spim_config *config = dev->config;
	uint32_t reg_val;
	uint32_t i;

	acquire_device(dev);

	/* select write privilege */
	reg_val = sys_read32(config->ctrl_base);
	reg_val = (reg_val & 0x00ffffff) | SPIM_PRIV_WRITE_SELECT;
	sys_write32(reg_val, config->ctrl_base);
	for (i = 0; i < SPIM_ADDR_PRIV_REG_NUN; i++)
		sys_write32(0xffffffff, config->ctrl_base + SPIM_VALID_ADDR_FTR + i * 4);

	/* select read privilege */
	reg_val = sys_read32(config->ctrl_base);
	reg_val = (reg_val & 0x00ffffff) | SPIM_PRIV_READ_SELECT;
	sys_write32(reg_val, config->ctrl_base);
	for (i = 0; i < SPIM_ADDR_PRIV_REG_NUN; i++)
		sys_write32(0xffffffff, config->ctrl_base + SPIM_VALID_ADDR_FTR + i * 4);

	release_device(dev);
}

void spim_scu_monitor_config(const struct device *dev, bool enable)
{
	const struct aspeed_spim_config *config = dev->config;

	if (enable) {
		spim_scu_ctrl_set(config->parent, BIT(config->ctrl_idx - 1) << 8,
			BIT(config->ctrl_idx - 1) << 8);
	} else {
		spim_scu_ctrl_clear(config->parent, BIT(config->ctrl_idx - 1) << 8);
	}
}

void spim_ctrl_monitor_config(const struct device *dev, bool enable)
{
	const struct aspeed_spim_config *config = dev->config;
	uint32_t reg_val;

	acquire_device(dev);

	reg_val = sys_read32(config->ctrl_base);
	if (enable)
		reg_val |= BIT(2);
	else
		reg_val &= ~(BIT(2));

	sys_write32(reg_val, config->ctrl_base);

	release_device(dev);
}

void spim_monitor_enable(const struct device *dev, bool enable)
{
	spim_ctrl_monitor_config(dev, enable);
}

static int aspeed_spi_monitor_init(const struct device *dev)
{
	const struct aspeed_spim_config *config = dev->config;
	struct aspeed_spim_data *const data = dev->data;

	if (IS_ENABLED(CONFIG_MULTITHREADING))
		k_sem_init(&data->sem, 1, 1);

	/* always enable internal passthrough configuration */
	spim_scu_passthrough_mode(dev, 0, true);

	if (config->multi_passthrough)
		spim_ctrl_passthrough_mode(dev, SPIM_MULTI_PASSTHROUGH, true);
	else
		spim_ctrl_passthrough_mode(dev, SPIM_SINGLE_PASSTHROUGH, true);

	spim_valid_cmd_table_init(dev, data->valid_cmd_list, data->valid_cmd_num, 0);

	spim_rw_perm_init(dev);

	/* enable filter */
	spim_scu_monitor_config(dev, true);
	spim_ctrl_monitor_config(dev, true);

	return 0;
}

static int aspeed_spi_monitor_common_init(const struct device *dev)
{
	/* nothing to do currently */
	return 0;
}

#define SPIM_ENUM(node_id) node_id,

#define ASPEED_SPIM_DEV_CFG(node_id) {	\
		.ctrl_base = DT_REG_ADDR(DT_PARENT(node_id)) + 0x1000 * (DT_REG_ADDR(node_id) - 1),	\
		.irq_num = DT_IRQN(node_id),		\
		.irq_priority = DT_IRQ(node_id, priority),	\
		.log_ram_addr = DT_PROP_BY_IDX(node_id, log_ram_info, 0), \
		.log_max_len = DT_PROP_BY_IDX(node_id, log_ram_info, 1),	\
		.ctrl_idx = DT_REG_ADDR(node_id),	\
		.parent = DEVICE_DT_GET(DT_PARENT(node_id)),	\
		.multi_passthrough = DT_PROP(node_id, multi_passthrough),	\
},

#define ASPEED_SPIM_DEV_DATA(node_id) {	\
		.valid_cmd_list = DT_PROP(node_id, valid_cmds), \
		.valid_cmd_num = DT_PROP_LEN(node_id, valid_cmds), \
		.read_forbidden_regions = DT_PROP(node_id, read_forbidden_regions),	\
		.read_forbidden_region_num = DT_PROP_LEN(node_id, read_forbidden_regions),	\
		.write_forbidden_regions = DT_PROP(node_id, write_forbidden_regions),	\
		.write_forbidden_region_num = DT_PROP_LEN(node_id, write_forbidden_regions),	\
},

/* child node define */
#define ASPEED_SPIM_DT_DEFINE(node_id)	\
	DEVICE_DT_DEFINE(node_id, aspeed_spi_monitor_init,	\
					NULL,	\
					&aspeed_spim_data[node_id],		\
					&aspeed_spim_config[node_id],	\
					POST_KERNEL, 71, NULL);

/* common node define */
#define ASPEED_SPI_MONITOR_COMMON_INIT(n)	\
	static struct aspeed_spim_common_config aspeed_spim_common_config_##n = { \
		.scu_base = DT_REG_ADDR_BY_IDX(DT_INST_PHANDLE_BY_IDX(n, aspeed_scu, 0), 0), \
	};								\
	static struct aspeed_spim_common_data aspeed_spim_common_data_##n;	\
		\
	DEVICE_DT_INST_DEFINE(n, &aspeed_spi_monitor_common_init,			\
			    NULL,					\
			    &aspeed_spim_common_data_##n,			\
			    &aspeed_spim_common_config_##n, POST_KERNEL,	\
			    70,		\
			    NULL);		\
	/* handle child node */	\
	static const struct aspeed_spim_config aspeed_spim_config[] = {	\
		DT_FOREACH_CHILD(DT_DRV_INST(n), ASPEED_SPIM_DEV_CFG)};		\
	static struct aspeed_spim_data aspeed_spim_data[] = {			\
		DT_FOREACH_CHILD(DT_DRV_INST(n), ASPEED_SPIM_DEV_DATA)};	\
		\
	enum {DT_FOREACH_CHILD(DT_DRV_INST(n), SPIM_ENUM)};	\
	DT_FOREACH_CHILD(DT_DRV_INST(n), ASPEED_SPIM_DT_DEFINE)


DT_INST_FOREACH_STATUS_OKAY(ASPEED_SPI_MONITOR_COMMON_INIT)
