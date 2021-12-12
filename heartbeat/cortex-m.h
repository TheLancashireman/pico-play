/*  cortexm.h
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
#ifndef CORTEXM_H
#define CORTEXM_H

/* This file covers all Cortex M variants, including the ARMv6-M-based M0 and M0+.
 *
 * Registers that are not present on the ARMv6-M variants are marked "not v6m".
 * SysTick is optional on ARMv6-M.
*/

#include "pico.h"

/* Aux control registers, including the SysTick timer
*/
typedef struct dv_cortexmacr_s dv_cortexmacr_t;

struct dv_cortexmacr_s
{
	dv_reg32_t unknown;		/* E000: What's this */
	dv_reg32_t ictr;		/* E004: Interrupt controller type - not v6m */
	dv_reg32_t actlr;		/* E008: Aux control register */
	dv_reg32_t gap;
	dv_reg32_t stcsr;		/* E010: SysTick control/status */
	dv_reg32_t strvr;		/* E014: SysTick reload */
	dv_reg32_t stcvr;		/* E018: SysTick current value */
	dv_reg32_t stcr;		/* E01C: SysTick calibration value */
};

#define DV_CORTEXMACR_BASE		0xe000e000
#define dv_macr					((dv_cortexmacr_t *)DV_CORTEXMACR_BASE)[0]

/* System control registers
*/
typedef struct dv_cortexmscr_s dv_cortexmscr_t;

struct dv_cortexmscr_s
{
	dv_reg32_t cpuid;		/* ED00: RO - CPU identification */
	dv_reg32_t icsr;		/* ED04: Interrupt control/state */
	dv_reg32_t vtor;		/* ED08: Vector table offset */
	dv_reg32_t aircr;		/* ED0C: Application interrupt and reset control */
	dv_reg32_t scr;			/* ED10: System control - not v6m */
	dv_reg32_t ccr;			/* ED14: Configuration and control */
	dv_reg32_t shpr1;		/* ED18: System handler priority - not v6m */
	dv_reg32_t shpr2;		/* ED1C: System handler priority */
	dv_reg32_t shpr3;		/* ED20: System handler priority */
	dv_reg32_t shcsr;		/* ED24: System handler control and state */
	dv_reg32_t cfsr;		/* ED28: Configurable fault status - not v6m */
	dv_reg32_t hfsr;		/* ED2C: Hardfault status - not v6m */
	dv_reg32_t dfsr;		/* ED30: Debug fault status */
	dv_reg32_t mmfar;		/* ED34: MemManage fault address - not v6m */
	dv_reg32_t bfar;		/* ED38: Bus fault address - not v6m */
	dv_reg32_t afsr;		/* ED3C: Auxiliary fault status - not v6m */
	dv_reg32_t id_pfr0;		/* ED40: RO - not v6m */
	dv_reg32_t id_pfr1;		/* ED44: RO - not v6m */
	dv_reg32_t id_dfr0;		/* ED48: RO - not v6m */
	dv_reg32_t id_afr0;		/* ED4C: RO - not v6m */
	dv_reg32_t id_mmfr0;	/* ED50: RO - not v6m */
	dv_reg32_t id_mmfr1;	/* ED54: RO - not v6m */
	dv_reg32_t id_mmfr2;	/* ED58: RO - not v6m */
	dv_reg32_t id_mmfr3;	/* ED5C: RO - not v6m */
	dv_reg32_t id_isar0;	/* ED60: RO - not v6m */
	dv_reg32_t id_isar1;	/* ED64: RO - not v6m */
	dv_reg32_t id_isar2;	/* ED68: RO - not v6m */
	dv_reg32_t id_isar3;	/* ED6C: RO - not v6m */
	dv_reg32_t id_isar4;	/* ED70: RO - not v6m */
	dv_reg32_t id_isar5;	/* ED74: RO - not v6m */
	dv_reg32_t id_isar6;	/* ED78: RO - not v6m */
	dv_reg32_t id_isar7;	/* ED7C: RO - not v6m */
};

#define DV_CORTEXMSCR_BASE		0xe000ed00
#define dv_mscr					((dv_cortexmscr_t *)DV_CORTEXMSCR_BASE)[0]

/* CPACR and STIR are well separated from the SCR
*/
#define DV_CORTEXM_CPACR		0xe000ed88
#define DV_mcpacr				((dv_reg32_t *)DV_CORTEXM_CPACR)[0]		/* Not v6m */

#define DV_CORTEXM_STIR			0xe000ef00
#define DV_mstir				((dv_reg32_t *)DV_CORTEXM_STIR)[0]		/* Not v6m */

#define DV_SYST_COUNTFLAG		0x00010000		/* 1 ==> counter has counted to zero (read-only, clear by reading) */
#define DV_SYST_CLKSRC			0x00000004		/* 1 = CPU, 0 = ext. ref */
#define DV_SYST_TICKINT			0x00000002		/* 1 = trigger exception when timer reaches 0 */
#define DV_SYST_ENABLE			0x00000001		/* 1 = counter is enabled */

#define DV_SYST_MASK			0x00ffffff		/* Max value mask */

extern void dv_init_systick(void);
extern dv_u32_t dv_read_systick(void);

#endif
