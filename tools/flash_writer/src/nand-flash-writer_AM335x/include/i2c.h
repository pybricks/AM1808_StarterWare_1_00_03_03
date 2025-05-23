/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _I2C_H_
#define _I2C_H_

#define	I2C_BASE1	0x44E0B000
#define	I2C_BASE2	0x4802A000
#define I2C_BUS_MAX	2

#define I2C_DEFAULT_BASE	I2C_BASE1

#define I2C_IP_CLK		48000000
#define I2C_INTERNAL_SAMLPING_CLK	12000000

#define I2C_IP_CLK			48000000
#define I2C_INTERNAL_SAMLPING_CLK	12000000

#define I2C_REV		(I2C_DEFAULT_BASE + 0x00)
#define I2C_SYSC	(I2C_DEFAULT_BASE + 0x10)
#define I2C_IE		(I2C_DEFAULT_BASE + 0x2C)
#define I2C_STAT	(I2C_DEFAULT_BASE + 0x28)
#define I2C_SYSS	(I2C_DEFAULT_BASE + 0x90)
#define I2C_BUF		(I2C_DEFAULT_BASE + 0x94)
#define I2C_CNT		(I2C_DEFAULT_BASE + 0x98)
#define I2C_DATA	(I2C_DEFAULT_BASE + 0x9c)
#define I2C_CON		(I2C_DEFAULT_BASE + 0xA4)
#define I2C_OA		(I2C_DEFAULT_BASE + 0xA8)
#define I2C_SA		(I2C_DEFAULT_BASE + 0xAC)
#define I2C_PSC		(I2C_DEFAULT_BASE + 0xB0)
#define I2C_SCLL	(I2C_DEFAULT_BASE + 0xB4)
#define I2C_SCLH	(I2C_DEFAULT_BASE + 0xB8)
#define I2C_SYSTEST	(I2C_DEFAULT_BASE + 0xBc)

/* I2C masks */
/* I2C System Conf Register (I2C_SYST)*/
#define I2C_SYST_NOIDLE     (1 << 3)
#define I2C_SYST_RESET      (1 << 2)
#define I2C_SYST_AUTOIDLE   (1 << 0)

/* I2C Interrupt Enable Register (I2C_IE): */
#define I2C_IE_GC_IE	(1 << 5)
#define I2C_IE_XRDY_IE	(1 << 4) /* Transmit data ready interrupt enable */
#define I2C_IE_RRDY_IE	(1 << 3) /* Receive data ready interrupt enable */
#define I2C_IE_ARDY_IE	(1 << 2) /* Register access ready interrupt enable */
#define I2C_IE_NACK_IE	(1 << 1) /* No acknowledgment interrupt enable */
#define I2C_IE_AL_IE	(1 << 0) /* Arbitration lost interrupt enable */

/* I2C Status Register (I2C_STAT): */

#define I2C_STAT_SBD	(1 << 15) /* Single byte data */
#define I2C_STAT_BB	(1 << 12) /* Bus busy */
#define I2C_STAT_ROVR	(1 << 11) /* Receive overrun */
#define I2C_STAT_XUDF	(1 << 10) /* Transmit underflow */
#define I2C_STAT_AAS	(1 << 9)  /* Address as slave */
#define I2C_STAT_GC	(1 << 5)
#define I2C_STAT_XRDY	(1 << 4)  /* Transmit data ready */
#define I2C_STAT_RRDY	(1 << 3)  /* Receive data ready */
#define I2C_STAT_ARDY	(1 << 2)  /* Register access ready */
#define I2C_STAT_NACK	(1 << 1)  /* No acknowledgment interrupt enable */
#define I2C_STAT_AL	(1 << 0)  /* Arbitration lost interrupt enable */

/* I2C Interrupt Code Register (I2C_INTCODE): */

#define I2C_INTCODE_MASK	7
#define I2C_INTCODE_NONE	0
#define I2C_INTCODE_AL		1	/* Arbitration lost */
#define I2C_INTCODE_NAK		2	/* No acknowledgement/general call */
#define I2C_INTCODE_ARDY	3	/* Register access ready */
#define I2C_INTCODE_RRDY	4	/* Rcv data ready */
#define I2C_INTCODE_XRDY	5	/* Xmit data ready */

/* I2C System Status Register (I2C_SYST)*/
#define I2C_SYST_RESETDONE   (1 << 0)

/* I2C Buffer Configuration Register (I2C_BUF): */

#define I2C_BUF_RDMA_EN		(1 << 15) /* Receive DMA channel enable */
#define I2C_BUF_XDMA_EN		(1 << 7)  /* Transmit DMA channel enable */
#define I2C_TXFIFO_CLEAR        (1 << 6)
#define I2C_RXFIFO_CLEAR        (1 << 14)

/* I2C Configuration Register (I2C_CON): */

#define I2C_CON_EN	(1 << 15)  /* I2C module enable */
#define I2C_CON_BE	(1 << 14)  /* Big endian mode */
#define I2C_CON_STB	(1 << 11)  /* Start byte mode (master mode only) */
#define I2C_CON_MST	(1 << 10)  /* Master/slave mode */
#define I2C_CON_TRX	(1 << 9)   /* Transmitter/receiver mode */
				   /* (master mode only) */
#define I2C_CON_XA	(1 << 8)   /* Expand address */
#define I2C_CON_STP	(1 << 1)   /* Stop condition (master mode only) */
#define I2C_CON_STT	(1 << 0)   /* Start condition (master mode only) */

/* I2C System Test Register (I2C_SYSTEST): */

#define I2C_SYSTEST_ST_EN	(1 << 15) /* System test enable */
#define I2C_SYSTEST_FREE	(1 << 14) /* Free running mode, on brkpoint) */
#define I2C_SYSTEST_TMODE_MASK	(3 << 12) /* Test mode select */
#define I2C_SYSTEST_TMODE_SHIFT	(12)	  /* Test mode select */
#define I2C_SYSTEST_SCL_I	(1 << 3)  /* SCL line sense input value */
#define I2C_SYSTEST_SCL_O	(1 << 2)  /* SCL line drive output value */
#define I2C_SYSTEST_SDA_I	(1 << 1)  /* SDA line sense input value */
#define I2C_SYSTEST_SDA_O	(1 << 0)  /* SDA line drive output value */

#define I2C_SCLL_SCLL		0
#define I2C_SCLL_SCLL_M		0xFF
#define I2C_SCLL_HSSCLL		8
#define I2C_SCLH_HSSCLL_M	0xFF
#define I2C_SCLH_SCLH		0
#define I2C_SCLH_SCLH_M		0xFF
#define I2C_SCLH_HSSCLH		8
#define I2C_SCLH_HSSCLH_M	0xFF

#define OMAP_I2C_STANDARD	100000
#define OMAP_I2C_FAST_MODE	400000
#define OMAP_I2C_HIGH_SPEED	3400000

#define OMAP_I2C_SLAVE      1

#define SYSTEM_CLOCK_12		12000000
#define SYSTEM_CLOCK_13		13000000
#define SYSTEM_CLOCK_192	19200000
#define SYSTEM_CLOCK_96		96000000

/* Use the reference value of 96MHz if not explicitly set by the board */
#ifndef I2C_IP_CLK
#define I2C_IP_CLK		SYSTEM_CLOCK_96
#endif

/*
 * The reference minimum clock for high speed is 19.2MHz.
 * The linux 2.6.30 kernel uses this value.
 * The reference minimum clock for fast mode is 9.6MHz
 * The reference minimum clock for standard mode is 4MHz
 * In TRM, the value of 12MHz is used.
 */
#ifndef I2C_INTERNAL_SAMPLING_CLK
#define I2C_INTERNAL_SAMPLING_CLK	19200000
#endif

/*
 * The equation for the low and high time is
 * tlow = scll + scll_trim = (sampling clock * tlow_duty) / speed
 * thigh = sclh + sclh_trim = (sampling clock * (1 - tlow_duty)) / speed
 *
 * If the duty cycle is 50%
 *
 * tlow = scll + scll_trim = sampling clock / (2 * speed)
 * thigh = sclh + sclh_trim = sampling clock / (2 * speed)
 *
 * In TRM
 * scll_trim = 7
 * sclh_trim = 5
 *
 * The linux 2.6.30 kernel uses
 * scll_trim = 6
 * sclh_trim = 6
 *
 * These are the trim values for standard and fast speed
 */
#ifndef I2C_FASTSPEED_SCLL_TRIM
#define I2C_FASTSPEED_SCLL_TRIM	 7	
#endif
#ifndef I2C_FASTSPEED_SCLH_TRIM
#define I2C_FASTSPEED_SCLH_TRIM	5	
#endif

/* These are the trim values for high speed */
#ifndef I2C_HIGHSPEED_PHASE_ONE_SCLL_TRIM
#define I2C_HIGHSPEED_PHASE_ONE_SCLL_TRIM	I2C_FASTSPEED_SCLL_TRIM
#endif
#ifndef I2C_HIGHSPEED_PHASE_ONE_SCLH_TRIM
#define I2C_HIGHSPEED_PHASE_ONE_SCLH_TRIM	I2C_FASTSPEED_SCLH_TRIM
#endif
#ifndef I2C_HIGHSPEED_PHASE_TWO_SCLL_TRIM
#define I2C_HIGHSPEED_PHASE_TWO_SCLL_TRIM	I2C_FASTSPEED_SCLL_TRIM
#endif
#ifndef I2C_HIGHSPEED_PHASE_TWO_SCLH_TRIM
#define I2C_HIGHSPEED_PHASE_TWO_SCLH_TRIM	I2C_FASTSPEED_SCLH_TRIM
#endif

/*
#define OMAP_I2C_STANDARD	100
#define OMAP_I2C_FAST_MODE	400
#define OMAP_I2C_HIGH_SPEED	3400

#define SYSTEM_CLOCK_12		12000
#define SYSTEM_CLOCK_13		13000
#define SYSTEM_CLOCK_192	19200
#define SYSTEM_CLOCK_96		96000

#define I2C_IP_CLK		SYSTEM_CLOCK_12

*/


#define I2C_PSC_MAX		0x0f
#define I2C_PSC_MIN		0x00

#endif /* _I2C_H_ */
