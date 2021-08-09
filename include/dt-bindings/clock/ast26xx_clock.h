/*
 * Copyright (c) 2020 ASPEED
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_AST26XX_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_AST26XX_H_

#define ASPEED_CLK_GRP_0_OFFSET		(0)
#define ASPEED_CLK_GRP_1_OFFSET		(32)
#define ASPEED_CLK_GRP_2_OFFSET		(64)

#define ASPEED_CLK_GATE_MCLK		(ASPEED_CLK_GRP_0_OFFSET + 0)
#define ASPEED_CLK_GATE_ECLK		(ASPEED_CLK_GRP_0_OFFSET + 1)
#define ASPEED_CLK_GATE_GCLK		(ASPEED_CLK_GRP_0_OFFSET + 2)
#define ASPEED_CLK_GATE_VCLK		(ASPEED_CLK_GRP_0_OFFSET + 3)
#define ASPEED_CLK_GATE_BCLK		(ASPEED_CLK_GRP_0_OFFSET + 4)
#define ASPEED_CLK_GATE_DCLK		(ASPEED_CLK_GRP_0_OFFSET + 5)
#define ASPEED_CLK_GATE_REF0CLK		(ASPEED_CLK_GRP_0_OFFSET + 6)
#define ASPEED_CLK_GATE_USBPORT2CLK	(ASPEED_CLK_GRP_0_OFFSET + 7)
/* bit[8] is reserved */
#define ASPEED_CLK_GATE_USBUHCICLK	(ASPEED_CLK_GRP_0_OFFSET + 9)
#define ASPEED_CLK_GATE_D1CLK		(ASPEED_CLK_GRP_0_OFFSET + 10)
/* bit[12:11] are reserved */
#define ASPEED_CLK_GATE_YCLK		(ASPEED_CLK_GRP_0_OFFSET + 13)
#define ASPEED_CLK_GATE_USBPORT1CLK	(ASPEED_CLK_GRP_0_OFFSET + 14)
#define ASPEED_CLK_GATE_UART5CLK	(ASPEED_CLK_GRP_0_OFFSET + 15)
/* bit[19:16] are reserved */
#define ASPEED_CLK_GATE_MAC1CLK		(ASPEED_CLK_GRP_0_OFFSET + 20)
#define ASPEED_CLK_GATE_MAC2CLK		(ASPEED_CLK_GRP_0_OFFSET + 21)
/* bit[23:22] are reserved */
#define ASPEED_CLK_GATE_RSACLK		(ASPEED_CLK_GRP_0_OFFSET + 24)
#define ASPEED_CLK_GATE_RVASCLK		(ASPEED_CLK_GRP_0_OFFSET + 25)
/* bit[26] is reserved */
#define ASPEED_CLK_GATE_EMMCCLK		(ASPEED_CLK_GRP_0_OFFSET + 27)
/* bit[31:28] are reserved */

#define ASPEED_CLK_GATE_LCLK		(ASPEED_CLK_GRP_1_OFFSET + 0)
#define ASPEED_CLK_GATE_ESPICLK		(ASPEED_CLK_GRP_1_OFFSET + 1)
#define ASPEED_CLK_GATE_REF1CLK		(ASPEED_CLK_GRP_1_OFFSET + 2)
/* bit[3] is reserved */
#define ASPEED_CLK_GATE_SDCLK		(ASPEED_CLK_GRP_1_OFFSET + 4)
#define ASPEED_CLK_GATE_LHCCLK		(ASPEED_CLK_GRP_1_OFFSET + 5)
/* bit[7:6] are reserved */
#define ASPEED_CLK_GATE_I3C0CLK		(ASPEED_CLK_GRP_1_OFFSET + 8)
#define ASPEED_CLK_GATE_I3C1CLK		(ASPEED_CLK_GRP_1_OFFSET + 9)
#define ASPEED_CLK_GATE_I3C2CLK		(ASPEED_CLK_GRP_1_OFFSET + 10)
#define ASPEED_CLK_GATE_I3C3CLK		(ASPEED_CLK_GRP_1_OFFSET + 11)
#define ASPEED_CLK_GATE_I3C4CLK		(ASPEED_CLK_GRP_1_OFFSET + 12)
#define ASPEED_CLK_GATE_I3C5CLK		(ASPEED_CLK_GRP_1_OFFSET + 13)
/* bit[15:14] are reserved */
#define ASPEED_CLK_GATE_UART1CLK	(ASPEED_CLK_GRP_1_OFFSET + 16)
#define ASPEED_CLK_GATE_UART2CLK	(ASPEED_CLK_GRP_1_OFFSET + 17)
#define ASPEED_CLK_GATE_UART3CLK	(ASPEED_CLK_GRP_1_OFFSET + 18)
#define ASPEED_CLK_GATE_UART4CLK	(ASPEED_CLK_GRP_1_OFFSET + 19)
#define ASPEED_CLK_GATE_MAC3CLK		(ASPEED_CLK_GRP_1_OFFSET + 20)
#define ASPEED_CLK_GATE_MAC4CLK		(ASPEED_CLK_GRP_1_OFFSET + 21)
#define ASPEED_CLK_GATE_UART6CLK	(ASPEED_CLK_GRP_1_OFFSET + 22)
#define ASPEED_CLK_GATE_UART7CLK	(ASPEED_CLK_GRP_1_OFFSET + 23)
#define ASPEED_CLK_GATE_UART8CLK	(ASPEED_CLK_GRP_1_OFFSET + 24)
#define ASPEED_CLK_GATE_UART9CLK	(ASPEED_CLK_GRP_1_OFFSET + 25)
#define ASPEED_CLK_GATE_UART10CLK	(ASPEED_CLK_GRP_1_OFFSET + 26)
#define ASPEED_CLK_GATE_UART11CLK	(ASPEED_CLK_GRP_1_OFFSET + 27)
#define ASPEED_CLK_GATE_UART12CLK	(ASPEED_CLK_GRP_1_OFFSET + 28)
#define ASPEED_CLK_GATE_UART13CLK	(ASPEED_CLK_GRP_1_OFFSET + 29)
#define ASPEED_CLK_GATE_FSICLK		(ASPEED_CLK_GRP_1_OFFSET + 30)

/* clock ID without gating control numbered as 64+ */
#define ASPEED_CLK_PCLK			(ASPEED_CLK_GRP_2_OFFSET + 0)
#define ASPEED_CLK_HCLK			(ASPEED_CLK_GRP_2_OFFSET + 1)
#define ASPEED_CLK_UART5		(ASPEED_CLK_GRP_2_OFFSET + 2)
#define ASPEED_CLK_APB1			(ASPEED_CLK_GRP_2_OFFSET + 3)
#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_AST26XX_H_ */
