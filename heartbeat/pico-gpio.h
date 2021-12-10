/*  pico-gpio.h
 *
 *  Copyright David Haworth
 *
 *  This file is part of the pico playground.
 *
 *  The pico playground is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The pico playground is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the pico playground.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PICO_GPIO_H
#define PICO_GPIO_H

/* The name of this file is becoming increasingly incorrect ;-)
*/

#include "pico.h"

/* The peripherals have "mirror" addresses that allow atomic access
*/
#define	DV_OFFSET_XOR	0x1000		/* XOR (i.e. toggle) */
#define	DV_OFFSET_W1S	0x2000		/* Write 1 to set */
#define	DV_OFFSET_W1C	0x3000		/* Write 1 to clear */

/* The resets registers control the power to the peripherals.
 * A peripheral whose reset bit is 1 is held in reset.
*/
#define DV_RESETS_BASE	0x4000c000

typedef struct dv_pico_resets_s
{
	volatile dv_u32_t	reset;
	volatile dv_u32_t	wdsel;
	volatile dv_u32_t	done;
} dv_pico_resets_t;

#define dv_pico_resets		(((dv_pico_resets_t *)DV_RESETS_BASE)[0])
#define dv_pico_resets_xor	(((dv_pico_resets_t *)(DV_RESETS_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_resets_w1s	(((dv_pico_resets_t *)(DV_RESETS_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_resets_w1c	(((dv_pico_resets_t *)(DV_RESETS_BASE+DV_OFFSET_W1C))[0])

/* Bits in the RESETS registers
*/
#define DV_RESETS_usbctrl		0x01000000
#define DV_RESETS_uart1			0x00800000
#define DV_RESETS_uart0			0x00400000
#define DV_RESETS_timer			0x00200000
#define DV_RESETS_tbman			0x00100000
#define DV_RESETS_sysinfo		0x00080000
#define DV_RESETS_syscfg		0x00040000
#define DV_RESETS_spi1			0x00020000
#define DV_RESETS_spi0			0x00010000
#define DV_RESETS_rtc			0x00008000
#define DV_RESETS_pwm			0x00004000
#define DV_RESETS_pll_usb		0x00002000
#define DV_RESETS_pll_sys		0x00001000
#define DV_RESETS_pio1			0x00000800
#define DV_RESETS_pio0			0x00000400
#define DV_RESETS_pads_qspi		0x00000200
#define DV_RESETS_pads_bank0	0x00000100
#define DV_RESETS_jtag			0x00000080
#define DV_RESETS_io_qspi		0x00000040
#define DV_RESETS_io_bank0		0x00000020
#define DV_RESETS_i2c1			0x00000010
#define DV_RESETS_i2c0			0x00000008
#define DV_RESETS_dma			0x00000004
#define DV_RESETS_busctrl		0x00000002
#define DV_RESETS_adc			0x00000001

/* I/O user bank 0
*/
#define DV_IOBANK0_BASE	0x40014000

typedef struct dv_pico_gpio_sc_s
{
	volatile dv_u32_t	status;
	volatile dv_u32_t	ctrl;
} dv_pico_gpio_sc_t;

typedef struct dv_pico_io_intctl_s
{
	volatile dv_u32_t	inte[4];		/* 0x000 - interrupt enable */
	volatile dv_u32_t	intf[4];		/* 0x010 - interrupt force */
	volatile dv_u32_t	ints[4];		/* 0x020 - interrupt cooked status */
} dv_pico_io_intctl_t;

typedef struct dv_pico_iobank0_s
{
	dv_pico_gpio_sc_t	gpio[30];			/* 0x000 - GPIO 0..29 control and status */
	dv_pico_gpio_sc_t	gap[2];				/* Special space where GPIO 30..31 were left out ;-) */
	volatile dv_u32_t	intr[4];			/* 0x0f0 - raw interrupt status */
	dv_pico_io_intctl_t	proc_intctl[2];		/* 0x100 - interrupt control for each core */
	dv_pico_io_intctl_t	dormant_wake;		/* 0x160 - interrupt control for dormant wake */
} dv_pico_iobank0_t;

#define dv_pico_iobank0		(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE))[0])
#define dv_pico_iobank0_xor	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_iobank0_w1s	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_iobank0_w1c	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1C))[0])

/* Clocks
*/
#define DV_CLOCKS_BASE	0x40008000

typedef struct dv_pico_clkblk_s
{
	volatile dv_u32_t ctrl;
	volatile dv_u32_t div;
	volatile dv_u32_t selected;
} dv_pico_clkblk_t;

typedef struct dv_pico_clocks_s
{
	dv_pico_clkblk_t	gpout[4];			/* 0x00/0x0c/0x18/0x24 */
	dv_pico_clkblk_t	ref;				/* 0x30 - */
	dv_pico_clkblk_t	sys;				/* 0x3c - */
	dv_pico_clkblk_t	peri;				/* 0x48 - no DIV in this block? */
	dv_pico_clkblk_t	usb;				/* 0x54 - */
	dv_pico_clkblk_t	adc;				/* 0x60 - */
	dv_pico_clkblk_t	rtc;				/* 0x6c - */
	volatile dv_u32_t	sys_resus_ctrl;		/* 0x74 - */
	volatile dv_u32_t	sys_resus_status;	/* 0x7c - */
	volatile dv_u32_t	fc0_ref_khz;		/* 0x80 - */
	volatile dv_u32_t	fc0_min_khz;		/* 0x84 - */
	volatile dv_u32_t	fc0_max_khz;		/* 0x88 - */
	volatile dv_u32_t	fc0_delay;			/* 0x8c - */
	volatile dv_u32_t	fc0_interval;		/* 0x90 - */
	volatile dv_u32_t	fc0_src;			/* 0x94 - */
	volatile dv_u32_t	fc0_status;			/* 0x98 - */
	volatile dv_u32_t	fc0_result;			/* 0x9c - */
	volatile dv_u32_t	wake_en[2];			/* 0xa0 - */
	volatile dv_u32_t	sleep_en[2];		/* 0xa8 - */
	volatile dv_u32_t	enabled[2];			/* 0xb0 - */
	volatile dv_u32_t	intr;				/* 0xb8 - */
	volatile dv_u32_t	inte;				/* 0xbc - */
	volatile dv_u32_t	intf;				/* 0xc0 - */
	volatile dv_u32_t	ints;				/* 0xc4 - */
} dv_pico_clocks_t;

#define dv_pico_clocks		(((dv_pico_clocks_t *)(DV_CLOCKS_BASE))[0])
#define dv_pico_clocks_xor	(((dv_pico_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_clocks_w1s	(((dv_pico_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_clocks_w1c	(((dv_pico_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_W1C))[0])

#define DV_CLKSRC_REF_ROSC		0x00
#define DV_CLKSRC_REF_AUX		0x01
#define DV_CLKSRC_REF_XOSC		0x02
#define DV_CLKSRC_SYS_REF		0x00
#define DV_CLKSRC_SYS_AUX		0x01

/* Xosc
*/
#define DV_XOSC_BASE			0x40024000

typedef struct dv_pico_xosc_s
{
	volatile dv_u32_t	ctrl;
	volatile dv_u32_t	status;
	volatile dv_u32_t	dormant;
	volatile dv_u32_t	startup;
	volatile dv_u32_t	count;
} dv_pico_xosc_t;

#define dv_pico_xosc		(((dv_pico_xosc_t *)(DV_XOSC_BASE))[0])
#define dv_pico_xosc_xor	(((dv_pico_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_xosc_w1s	(((dv_pico_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_xosc_w1c	(((dv_pico_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_W1C))[0])

#define DV_XOSC_1_15_MHZ	0x00000aa0
#define DV_XOSC_ENABLE		0x00fab000
#define DV_XOSC_DISABLE		0x00d1e000
#define DV_XOSC_STARTUP_X4	0x00100000
#define DV_XOSC_STABLE		0x80000000
#define DV_XOSC_BADWRITE	0x01000000
#define DV_XOSC_ENABLED		0x00001000

/* PLL
 * The sys PLL can be used to drive the cores. For full speed we need to wind it up to 133 MHz
 *
 * From the block diagram and description in refman 2.18:
 *	REFDIV = 1 because FREF is 12 MHz.
 *	VCO output must be in range 400..1600 MHz. The higher the frequency, the less jitter there is.
 *	FBDIV must be in the range 16..320. This is actually the multiplication factor.
 *
 * So if we choose FBDIV = 133, we have a VCO output frequeny of 1596, which is in range (just!)
 * Then we need to divide by 12 by setting POSTDIV1 to 6 and POSTDIV2 to 2
*/
#define DV_PLL_SYS_BASE		0x40028000
#define DV_PLL_USB_BASE		0x4002c000

typedef struct dv_pico_pll_s
{
	volatile dv_u32_t	cs;			/* 0x00 - control and status */
	volatile dv_u32_t	pwr;		/* 0x04 - controls power modes */
	volatile dv_u32_t	fbdiv_int;	/* 0x08 - feedback divisor */
	volatile dv_u32_t	prim;		/* 0x0c - controls PLL post dividers */
} dv_pico_pll_t;

#define dv_pico_pll			(((dv_pico_pll_t *)(DV_PLL_SYS_BASE))[0])
#define dv_pico_pll_xor		(((dv_pico_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_pll_w1s		(((dv_pico_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_pll_w1c		(((dv_pico_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_W1C))[0])

#define dv_pico_usbpll		(((dv_pico_pll_t *)(DV_PLL_USB_BASE))[0])
#define dv_pico_usbpll_xor	(((dv_pico_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_usbpll_w1s	(((dv_pico_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_usbpll_w1c	(((dv_pico_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_W1C))[0])

#define DV_PLL_LOCK			0x80000000	/* CS */
#define DV_PLL_BYPASS		0x00000100	/* CS */
#define DV_PLL_REFDIV		0x0000003f	/* CS */
#define DV_PLL_VCOPD		0x00000020	/* PWR */
#define DV_PLL_POSTDIVPD	0x00000008	/* PWR */
#define DV_PLL_DSMPD		0x00000004	/* PWR */
#define DV_PLL_PD			0x00000001	/* PWR */
#define DV_PLL_FBDIV		0x00000fff	/* FBDIV_INT */
#define DV_PLL_POSTDIV1		0x00070000	/* PRIM */
#define DV_PLL_POSTDIV2		0x00007000	/* PRIM */

/* Single-cycle I/O block.
*/
#define DV_SIO_BASE		0xd0000000

typedef struct dv_pico_gpioblk_s
{
	volatile dv_u32_t	val;
	volatile dv_u32_t	w1s;
	volatile dv_u32_t	w1c;
	volatile dv_u32_t	xor;
} dv_pico_gpioblk_t;

typedef struct dv_pico_interp_s
{
	volatile dv_u32_t accum0;				/* 0x00 - */
	volatile dv_u32_t accum1;				/* 0x04 - */
	volatile dv_u32_t base0;				/* 0x08 - */
	volatile dv_u32_t base1;				/* 0x0c - */
	volatile dv_u32_t base2;				/* 0x10 - */
	volatile dv_u32_t pop_lane0;			/* 0x14 - */
	volatile dv_u32_t pop_lane1;			/* 0x18 - */
	volatile dv_u32_t pop_full;				/* 0x1c - */
	volatile dv_u32_t peek_lane0;			/* 0x20 - */
	volatile dv_u32_t peek_lane1;			/* 0x24 - */
	volatile dv_u32_t peek_full;			/* 0x28 - */
	volatile dv_u32_t ctrl_lane0;			/* 0x2c - */
	volatile dv_u32_t ctrl_lane1;			/* 0x30 - */
	volatile dv_u32_t accum0_add;			/* 0x34 - */
	volatile dv_u32_t accum1_add;			/* 0x38 - */
	volatile dv_u32_t base_1and0;			/* 0x3c - */
} dv_pico_interp_t;

typedef struct dv_pico_sio_s
{
	volatile dv_u32_t	cpuid;				/* 0x000 - Core ID register */
	volatile dv_u32_t	gpio_in;			/* 0x004 - GPIO 0..29 input state */
	volatile dv_u32_t	gpio_hi_in;			/* 0x008 - QSPI GPIO input state */
	volatile dv_u32_t	gap1;
	dv_pico_gpioblk_t	gpio_out;			/* 0x010 - GPIO 0..29 output state */
	dv_pico_gpioblk_t	gpio_oe;			/* 0x020 - GPIO 0..29 output enable (1 = output, 0 = input) */
	dv_pico_gpioblk_t	gpio_hi_out;		/* 0x030 - QSPI GPIO output state */
	dv_pico_gpioblk_t	gpio_hi_oe;			/* 0x040 - QSPI GPIO output enable (1 = output, 0 = input) */
	volatile dv_u32_t	fifo_st;			/* 0x050 - FIFO status */
	volatile dv_u32_t	fifo_wr;			/* 0x054 - FIFO write */
	volatile dv_u32_t	fifo_rd;			/* 0x058 - FIFO read */
	volatile dv_u32_t	div_udividend;		/* 0x060 - unsigned dividend */
	volatile dv_u32_t	div_udivisor;		/* 0x064 - unsigned divisor */
	volatile dv_u32_t	div_sdividend;		/* 0x068 - signed dividend */
	volatile dv_u32_t	div_sdivisor;		/* 0x06c - signed divisor */
	volatile dv_u32_t	div_quotient;		/* 0x070 - quotient */
	volatile dv_u32_t	div_remainder;		/* 0x074 - remainder */
	volatile dv_u32_t	div_csr;			/* 0x078 - divider control/status */
	volatile dv_u32_t	gap2;
	dv_pico_interp_t	interp0;			/* 0x080 - interpolator 0 */
	dv_pico_interp_t	interp1;			/* 0x0c0 - interpolator 0 */
	volatile dv_u32_t	spinlock[32];		/* 0x100 - spinlocks */
} dv_pico_sio_t;

#define dv_pico_sio		(((dv_pico_sio_t *)DV_SIO_BASE)[0])

/* Bits 0..4 of gpio[n].ctrl select the function.
 *
 * The exact meaning (which peripheral, which signal) depends on the pin.
 * See section 2.19.2 in the RP2040 ref. manual for full details.
*/
#define DV_FUNCSEL_SPI		1
#define DV_FUNCSEL_UART		2
#define DV_FUNCSEL_I2C		3
#define DV_FUNCSEL_PWM		4
#define DV_FUNCSEL_SIO		5
#define DV_FUNCSEL_PIO0		6
#define DV_FUNCSEL_PIO1		7
#define DV_FUNCSEL_CLOCK	8
#define DV_FUNCSEL_USB		9
#define DV_FUNCSEL_NULL		31

#endif
