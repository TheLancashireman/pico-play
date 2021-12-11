/*  leds.c
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
#include "pico-gpio.h"
#include "cortex-m.h"
#include "pico-uart.h"
#include "nvic.h"

#define USE_SYSTICK		1

extern volatile int delay_factor;

int char_count;

void init_led(void);
void led_on(void);
void led_off(void);
void delay(int);

/* Onboard LED is attached to GPIO 25
*/
#define PIN	25
#define PINMASK		(0x1 << PIN)

void init_led(void)
{
	/* Release the reset on IO_BANK0 and wait till ready.
	*/
	dv_pico_resets_w1c.reset = DV_RESETS_io_bank0;
	do {	/* Wait	*/	} while ( (dv_pico_resets.done & DV_RESETS_io_bank0) == 0 );

	/* Disable the SIO output and turn off.
	*/
	dv_pico_sio.gpio_oe.w1c = PINMASK;
	dv_pico_sio.gpio_out.w1c = PINMASK;

	/* Select SIO function for the pin.
	*/
	dv_pico_iobank0.gpio[PIN].ctrl = DV_FUNCSEL_SIO;

	/* Enable the SIO output
	*/
	dv_pico_sio.gpio_oe.w1s = PINMASK;

#if USE_SYSTICK
	dv_init_systick();
#endif
}

void led_off(void)
{
	dv_pico_sio.gpio_out.w1c = PINMASK;
}

void led_on(void)
{
	dv_pico_sio.gpio_out.w1s = PINMASK;
}

void play_putc(int c)
{
	dv_uart0_putc(c);

	if ( c == '\n' )
	{
		dv_uart0_putc('\r');
		char_count = 0;
	}
	else
	{
		char_count++;
		if ( char_count >= 64 )
		{
			dv_uart0_putc('\r');
			dv_uart0_putc('\n');
			char_count = 0;
		}
	}
}

void delay(int ms)
{
#if USE_SYSTICK
	/* Ext. clock is 1 MHz ==> 1000000 ticks per second ==> 1000 ticks per millisecond */
	dv_u32_t count = ms * 1000;
	dv_u32_t old = dv_read_systick();
	dv_u32_t diff = 0;
	dv_u32_t new = 0;

	while ( count > diff )
	{
		count -= diff;
		new = dv_read_systick();
		diff = (old - new) & DV_SYST_MASK;	/* Down counter! */
		old = new;

		if ( dv_uart0_isrx() )
		{
			int c = dv_uart0_getc();
			play_putc(c);

#if 0
			if ( c == '!' )
				dv_nvic_triggerirq(0);
#endif
		}
	}

#else	/* Timing loop version calibrated (approximately) for 8 MHz CPU clock */

	volatile int count = ms * delay_factor;

	do {
		count--;
	} while ( count > 0 );
	
#endif
}
