/*  vectors.c - vector table for Cortex-M0 devices
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
extern unsigned dv_stacktop;
extern void dv_reset(void);
extern void dv_nmi(void);
extern void dv_hardfault(void);
extern void dv_svctrap(void);
extern void dv_pendsvtrap(void);
extern void dv_systickirq(void);
extern void dv_irq(void);
extern void dv_unknowntrap(void);

extern void dv_irq_ext0(void);

/* The vector table is an array of addresses.
 * This table has 48 vectors:
 *	- 16 are the armv7m exception/reset vectors (including reset SP)
 *	- 32 are for the NVIC interrupts. They are all mappped to dv_irq except interrupt 0
 *
 * The pico's boot sequence means that the reset vector will never be used. However, it is set up here
 * "just in case".
 * The RP2040's VTOR has the lower 8 bits nailed to 0, so the vector table must be aligned to 256 bytes.
 * The remaining bits are all implemented, so the vector table can be anywhere in the address space.
 * It isn't possible to put this vector table at the load address because the UF2 loader jumps there after
 * loading.
*/
const unsigned vectors[] =
{	(unsigned)&dv_stacktop,
	(unsigned)&dv_reset,
	(unsigned)&dv_nmi,
	(unsigned)&dv_hardfault,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_svctrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_unknowntrap,
	(unsigned)&dv_pendsvtrap,
	(unsigned)&dv_systickirq,
	(unsigned)&dv_irq_ext0,	/* 00 */
	(unsigned)&dv_irq,	/* 01 */
	(unsigned)&dv_irq,	/* 02 */
	(unsigned)&dv_irq,	/* 03 */
	(unsigned)&dv_irq,	/* 04 */
	(unsigned)&dv_irq,	/* 05 */
	(unsigned)&dv_irq,	/* 06 */
	(unsigned)&dv_irq,	/* 07 */
	(unsigned)&dv_irq,	/* 08 */
	(unsigned)&dv_irq,	/* 09 */
	(unsigned)&dv_irq,	/* 10 */
	(unsigned)&dv_irq,	/* 11 */
	(unsigned)&dv_irq,	/* 12 */
	(unsigned)&dv_irq,	/* 13 */
	(unsigned)&dv_irq,	/* 14 */
	(unsigned)&dv_irq,	/* 15 */
	(unsigned)&dv_irq,	/* 16 */
	(unsigned)&dv_irq,	/* 17 */
	(unsigned)&dv_irq,	/* 18 */
	(unsigned)&dv_irq,	/* 19 */
	(unsigned)&dv_irq,	/* 20 */
	(unsigned)&dv_irq,	/* 21 */
	(unsigned)&dv_irq,	/* 22 */
	(unsigned)&dv_irq,	/* 23 */
	(unsigned)&dv_irq,	/* 24 */
	(unsigned)&dv_irq,	/* 25 */
	(unsigned)&dv_irq,	/* 26 */
	(unsigned)&dv_irq,	/* 27 */
	(unsigned)&dv_irq,	/* 28 */
	(unsigned)&dv_irq,	/* 29 */
	(unsigned)&dv_irq,	/* 30 */
	(unsigned)&dv_irq,	/* 31 */
};
