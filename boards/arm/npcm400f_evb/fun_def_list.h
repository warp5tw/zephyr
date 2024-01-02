/*
 * Copyright (c) 2023 Nuvoton Technology Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <devicetree.h>

#if DT_NODE_HAS_STATUS(DT_NODELABEL(uart0), okay) && CONFIG_UART_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_uart0_default), UARTA_CR_SIN, UARTA_CR_SOUT)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(uart1), okay) && CONFIG_UART_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_uart1_default), UARTB_CR_SIN, UARTB_CR_SOUT)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c1a), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c1a_default), SCL1A, SDA1A)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c1b), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c1b_default), SCL1B, SDA1B)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c3a), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c3a_default), SCL3A, SDA3A)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c4a), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c4a_default), SCL4A, SDA4A)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c5a), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c5a_default), SCL5A, SDA5A)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c6a), okay) && CONFIG_I2C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i2c6a_default), SCL6A, SDA6A)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(peci0), okay) && CONFIG_PECI_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_peci0_default), PECI)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(spi_fiu0), okay) && CONFIG_SPI_NPCM4XX_FIU
FUN_DEFINE(DT_NODELABEL(pinctrl_shd_spi_default), SHD_CS, SHD_SCLK, SHD_DIO0, SHD_DIO1)
FUN_DEFINE(DT_NODELABEL(pinctrl_shd_spi_quad), SHD_DIO2, SHD_DIO3)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(adc0), okay) && CONFIG_ADC_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_vin7_default), VIN7)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin5_default), VIN5)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin16_default), VIN16)
FUN_DEFINE(DT_NODELABEL(pinctrl_thr16_default), THR16)
FUN_DEFINE(DT_NODELABEL(pinctrl_td2p_default), TD2P)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin15_default), VIN15)
FUN_DEFINE(DT_NODELABEL(pinctrl_thr15_default), THR15)
FUN_DEFINE(DT_NODELABEL(pinctrl_td1p_default), TD1P)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin14_default), VIN14)
FUN_DEFINE(DT_NODELABEL(pinctrl_thr14_default), THR14)
FUN_DEFINE(DT_NODELABEL(pinctrl_td0p_default), TD0P)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin1_default), VIN1)
FUN_DEFINE(DT_NODELABEL(pinctrl_thr1_default), THR1)
FUN_DEFINE(DT_NODELABEL(pinctrl_td3p_default), TD3P)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin2_default), VIN2)
FUN_DEFINE(DT_NODELABEL(pinctrl_thr2_default), THR2)
FUN_DEFINE(DT_NODELABEL(pinctrl_td4p_default), TD4P)
FUN_DEFINE(DT_NODELABEL(pinctrl_vin3_default), VIN3)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(spip1), okay) && CONFIG_SPIP_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_spip1_default), SPIP1_CS, SPIP1_SCLK, SPIP1_DIO0, SPIP1_DIO1)
FUN_DEFINE(DT_NODELABEL(pinctrl_spip1_quad), SPIP1_DIO2, SPIP1_DIO3)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i3c0), okay) && CONFIG_I3C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i3c0_default), I3C1_SCL, I3C1_SDA)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i3c1), okay) && CONFIG_I3C_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_i3c1_default), I3C2_SCL, I3C2_SDA)
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(usbd0), okay) && CONFIG_USB_DC_NPCM4XX
FUN_DEFINE(DT_NODELABEL(pinctrl_usbd_phy_iclk), USBD_PHY_ICLK)
FUN_DEFINE(DT_NODELABEL(pinctrl_usbd_phy_xclk), USBD_PHY_XCLK)
#endif