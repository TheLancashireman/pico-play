/*  systick.c
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
#include "pico.h"
#include "cortex-m.h"

void dv_init_systick(void)
{
	dv_macr.stcsr = DV_SYST_ENABLE;	/* Enable systick with implementation-defined clock */
	dv_macr.strvr = 0x00ffffff;		/* At then next cycle after 0, load this value */
	dv_macr.stcvr = 0x0;				/* Writing to CVR clears the register */
}

dv_u32_t dv_read_systick(void)
{
	return dv_macr.stcvr;
}
