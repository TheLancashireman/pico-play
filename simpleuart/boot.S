/*	boot.S
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
	.extern		dv_stacktop
	.extern		notmain
	.globl		dv_boot
	.globl		PUT32
	.globl		GET32

dv_boot:
    ldr		r0,=dv_stacktop
    mov		sp,r0
    bl		notmain
    b .

PUT32:
    str r1,[r0]
    bx lr

GET32:
    ldr r0,[r0]
    bx lr

