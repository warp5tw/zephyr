/*
 * Copyright (c) 2021 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <drivers/misc/aspeed/abr_aspeed.h>
#include <drivers/misc/aspeed/pfr_aspeed.h>
#include <soc.h>

extern struct k_work rst_work;

void demo_spim_irq_init(void);
void demo_wdt(void);
void aspeed_dcscm_rst_demo(struct k_work *item);
void rst_ind_gpio_event_register(void);
void ast1060_i2c_demo_flt(void);

void main(void)
{
	printk("%s demo\n", CONFIG_BOARD);
	aspeed_print_sysrst_info();
	disable_abr_wdt();

	/* reset BMC and PCH first */
	pfr_bmc_rst_enable_ctrl(true);
	pfr_pch_rst_enable_ctrl(true);

	rst_ind_gpio_event_register();

	k_work_init(&rst_work, aspeed_dcscm_rst_demo);
	demo_spim_irq_init();

	aspeed_dcscm_rst_demo(NULL);

#if CONFIG_I2C_PFR_FILTER
	ast1060_i2c_demo_flt();
#endif
	/* the following demo part should be allocated
	 * at the final step of main function.
	 */
#if CONFIG_WDT_DEMO_ASPEED
	demo_wdt();
#endif
}
