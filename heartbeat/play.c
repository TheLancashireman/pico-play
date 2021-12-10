/*  play.c
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
#include "pico-gpio.h"
#include "pico-uart.h"
#include "nvic.h"

#define USE_PLL		1

/* Linker script symbols
 *
 * The volatile qualifier here and in the pointers inhibits the gcc "optimisation" that replaces the
 * initialisation loop with a call to memset.
*/
extern volatile unsigned dv_start_data, dv_end_data, dv_start_bss, dv_end_bss, dv_idata;

/* Let's have something in .data to check that the linker script is working
*/
volatile unsigned char floom = 42u;

volatile int delay_factor;

void pass(void);
void fail(void);
void dv_init_data(void);
void init_led(void);
void led_on(void);
void led_off(void);
void delay(int);
void dv_init_clock();
void dv_init_pll();
void play_putc(int);
void dv_irq_ext0(void);
void putstr(char *);

/* Entry point
*/
void dv_reset(void)
{
#if 1
	dv_init_clock();
#if USE_PLL
	dv_init_pll();
#endif
#endif
	dv_init_data();

	if ( floom == 42 )
		pass();
	else
		fail();

	dv_nvic_init();

#if 0
	if ( dv_uart1_init(115200, "8N1") != 0 )
		fail();

	dv_nvic_setprio(0, 12);
	dv_nvic_enableirq(0);
#endif

	init_led();
	led_off();
#if USE_PLL
	delay_factor = 16000;
#else
	delay_factor = 800;
#endif

	for (;;)
	{
		play_putc('*');
		led_on();
		delay(20);
		led_off();
		delay(100);
		play_putc('!');
		led_on();
		delay(20);
		led_off();
		delay(700);
	}
}

void pass(void)
{
	floom = 66u;
}

void fail(void)
{
	floom = 99u;
}

/*	Initialise the .data and .bss sections
*/
void dv_init_data(void)
{
#if 0	/* Not needed - bootloader loads data directly in place */
	volatile unsigned *s = &dv_idata;
	volatile unsigned *d = &dv_start_data;

	while ( d < &dv_end_data )
	{
		*d++ = *s++;
	}
#endif

	volatile unsigned *b = &dv_start_bss;
	while ( b < &dv_end_bss )
	{
		*b++ = 0x00;
	}
}

void dv_init_clock(void)
{
	dv_pico_clocks.sys_resus_ctrl = 0x00;				/* Disable resuscitation for now */

	dv_pico_xosc.ctrl = DV_XOSC_1_15_MHZ;				/* Should be fixed according to datasheet, but is R/W */
	dv_pico_xosc.startup = ((12000000/1000)+255)/256;	/* 1 ms at 12 MHz, rounded up */
	dv_pico_xosc_w1s.ctrl = DV_XOSC_ENABLE;
	do {	/* Wait */	} while ( (dv_pico_xosc.status & DV_XOSC_STABLE) == 0 );

	dv_pico_clocks.ref.ctrl = DV_CLKSRC_REF_XOSC;		/* Select XOSC as the reference clock source */
	dv_pico_clocks.sys.ctrl = DV_CLKSRC_SYS_REF;		/* Select REF as the system clock source */
}

void putstr(char *s)
{
	while ( *s != '\0' )
	{
		play_putc(*s++);
	}
}

#define MY_REFDIV		1
#define MY_FBDIV		133
#define MY_POSTDIV1		6
#define MY_POSTDIV2		2
#define MY_POSTDIVS		((MY_POSTDIV1 << 16) | (MY_POSTDIV2 << 12))

void dv_init_pll(void)
{
	/* Check if PLL is already correctly configured and running
	*/
	if ( (dv_pico_pll.cs & DV_PLL_LOCK) != 0 &&
		 (dv_pico_pll.cs & DV_PLL_REFDIV) == MY_REFDIV &&
		 (dv_pico_pll.fbdiv_int & DV_PLL_FBDIV) == MY_FBDIV &&
		 (dv_pico_pll.prim & (DV_PLL_POSTDIV1 | DV_PLL_POSTDIV2)) == MY_POSTDIVS )
		return;

	/* Apply the reset on PLL, then remove it and wait until ready
	*/
	dv_pico_resets_w1s.reset = DV_RESETS_pll_sys;
	dv_pico_resets_w1c.reset = DV_RESETS_pll_sys;
	do {	/* Wait	*/	} while ( (dv_pico_resets.done & DV_RESETS_pll_sys) == 0 );

	/* Load the VCO-related dividers
	*/
	dv_pico_pll.cs = MY_REFDIV;
	dv_pico_pll.fbdiv_int = MY_FBDIV;

	/* Power on the PLL
	*/
	dv_pico_pll_w1c.pwr = DV_PLL_VCOPD | DV_PLL_PD;

	/* Wait for the PLL to lock
	*/
	do {	/* Wait */	} while ( (dv_pico_pll.cs & DV_PLL_LOCK) == 0 );

	/* Configure the post-dividers
	*/
	dv_pico_pll.prim = MY_POSTDIVS;

	/* Power on the post-dividers
	*/
	dv_pico_pll_w1c.pwr = DV_PLL_POSTDIVPD;

	/* Switch the sys clock to the PLL
	*/
	dv_pico_clocks.sys.ctrl = DV_CLKSRC_SYS_REF | DV_CLKSRC_SYS_AUX_PLL;	/* Should be this already */
	dv_pico_clocks.sys.ctrl = DV_CLKSRC_SYS_AUX | DV_CLKSRC_SYS_AUX_PLL;	/* Switch to the aux clock */
}

void dv_irq_ext0(void)
{
	putstr("dv_irq_ext0()\n");
}

/* Below this line are stub functions to satisfy the vector addresses
*/
void dv_nmi(void)
{
	putstr("dv_nmi()\n");
	for (;;)	{ }
}

void dv_hardfault(void)
{
	putstr("dv_hardfault()\n");
	for (;;)	{ }
}

void dv_svctrap(void)
{
	putstr("dv_svctrap()\n");
	for (;;)	{ }
}

void dv_pendsvtrap(void)
{
	putstr("dv_pendsvtrap()\n");
}

void dv_systickirq(void)
{
	putstr("dv_systickirq()\n");
	for (;;)	{ }
}

void dv_irq(void)
{
	putstr("dv_irq()\n");
	for (;;)	{ }
}

void dv_unknowntrap(void)
{
	putstr("dv_unknowntrap()\n");
	for (;;)	{ }
}

