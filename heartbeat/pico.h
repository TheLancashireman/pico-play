/* pico.h
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
#ifndef PICO_H
#define PICO_H

/* Some standard typedefs for a 32-bit core
*/
typedef unsigned int dv_u32_t;
typedef unsigned short dv_u16_t;
typedef unsigned char dv_u8_t;
typedef volatile dv_u32_t dv_reg32_t;

/* No. of interrupt request signals to the NVIC
 *
 * Only 0..25 are used. 26..31 are tied to 0. Q: can they be software-triggered?
*/
#define DV_NVIC_NIRQ	32

#endif
