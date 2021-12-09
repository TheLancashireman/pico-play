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
	dv_pico_gpio_sc_t	gap[2];				/* Space where GPIO 30..31 should be ;-) */
	volatile dv_u32_t	intr[4];			/* 0x0f0 - raw interrupt status */
	dv_pico_io_intctl_t	proc_intctl[2];		/* 0x100 - interrupt control for each core */
	dv_pico_io_intctl_t	dormant_wake;		/* 0x160 - interrupt control for dormant wake */
} dv_pico_iobank0_t;

#define dv_pico_iobank0		(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE))[0])
#define dv_pico_iobank0_xor	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_XOR))[0])
#define dv_pico_iobank0_w1s	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1S))[0])
#define dv_pico_iobank0_w1c	(((dv_pico_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1C))[0])


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