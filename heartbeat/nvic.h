/*  nvic.h
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
#ifndef NVIC_H
#define NVIC_H

#include "pico.h"

#define DV_NVIC_BASE	0xe000e100

/* NVIC registers.
 *
 * The gaps between the various register sets are consisent with allowing future support for
 * up to 1024 interrupt sources although the architecture document explicitly excludes
 * the possibility.
 *
 * This structure declares placeholders for all 1024 potential sources.
 *
 * In practice, the number of sources available is defined by the microcontroller implementation.
 *
 * In the ARMv6M variants there are only 32 external interrupts, so there's exactly one
 * ISER, one ICER, one ISPR, one ICPR and eight IPRs. Only four priority levels (0x00, 0x40, 0x80 and 0xc0)
 * are supported. IABR is missing but not noted in the differences (Appendix D3)
*/

typedef struct dv_nvic_s dv_nvic_t;

struct dv_nvic_s
{
	dv_reg32_t iser[32];		/* Write 1 to a bit to enable an IRQ; Write 0 has no effect */
	dv_reg32_t icer[32];		/* Write 1 to a bit to disable an IRQ; Write 0 has no effect */
	dv_reg32_t ispr[32];		/* Write 1 to a bit to set an IRQ's pending flag; Write 0 has no effect */
	dv_reg32_t icpr[32];		/* Write 1 to a bit to clear an IRQ's pending flag; Write 0 has no effect */
	dv_reg32_t iabr[32];		/* Read-only. a bit at 1 indicates interrupt is active */
								/* Note: in the above register arrays, registers 16..31 are reserved */
	dv_u32_t reserved[32];
	dv_reg32_t	ipr[256];		/* Interrupt priorities; 8 bits per IRQ; last 124..127 are reserved */
};

#define dv_nvic		((dv_nvic_t *)DV_NVIC_BASE)[0]

/* dv_nvic_write1() - write a 1 to a bit in an array
*/
static inline void dv_nvic_write1(dv_reg32_t bank[32], int irq)
{
	bank[irq/32] = 0x1 << (irq%32);
}

/* dv_nvic_read1() - read a bit in an array
*/
static inline int dv_nvic_read1(dv_reg32_t bank[32], int irq)
{
	return (bank[irq/32] & (0x1 << (irq%32))) != 0;
}

/* dv_nvic_enableirq() - enable an interrupt source
*/
static inline void dv_nvic_enableirq(int irq)
{
	dv_nvic_write1(dv_nvic.iser, irq);
}

/* dv_nvic_disableirq() - disable an interrupt source
*/
static inline void dv_nvic_disableirq(int irq)
{
	dv_nvic_write1(dv_nvic.icer, irq);
}

/* dv_nvic_triggerirq() - trigger an interrupt (set its pending flag)
*/
static inline void dv_nvic_triggerirq(int irq)
{
	dv_nvic_write1(dv_nvic.ispr, irq);
}

/* dv_nvic_cleaarirq() - clear an interrupt source's pending flag
*/
static inline void dv_nvic_clearirq(int irq)
{
	dv_nvic_write1(dv_nvic.icpr, irq);
}

/* dv_nvic_setprio() - set the priority of an interrupt source
*/
static inline void dv_nvic_setprio(int irq, dv_u8_t p)
{
	int shift = (irq%4) * 8;
	dv_nvic.ipr[irq/4] = (dv_nvic.ipr[irq/4] & ~(0xff << shift)) | (((dv_u32_t)p) << shift);
}

extern void dv_nvic_init(void);

#endif
