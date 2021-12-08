/*  pico-uart.c
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
#if 0
#include "pico.h"
#include "pico-uart.h"
#include "pico-gpio.h"

/* dv_uart1_getc() - wait until there's a character available then return it.
*/
int dv_uart1_getc(void)
{
	while ( !dv_uart1_isrx() )
	{
	}

	return dv_uart1.dr;
}

/* dv_uart1_putc() - wait until there's room in the tx buffer, then put a character into it.
*/
void dv_uart1_putc(int c)
{
	while ( !dv_uart1_istx() )
	{
	}

	dv_uart1.dr = (dv_u32_t)c;
}

/* dv_uart1_init() - initialise uart1 for normal async use. Return 0 if OK.
 *
 * Returns nonzero if the parameters aren't supported.
 *
 * Assumes CPU clock of 72 MHz
 *
 * TODO: perhaps a table lookup would be better than a load of if/else.
*/

#define FRAC_0_5	0x8
#define FRAC_0_25	0x4
#define FRAC_0_125	0x2
#define FRAC_0_0625	0x1

#define MAKE_FRAC(w,f)	((dv_u32_t)((w)*16+(f)))

int dv_uart1_init(unsigned baud, char *fmt)
{
	dv_u32_t div;
	char bits;

	/* See table 192 in STM ref manual
	*/
	switch ( baud )
	{
	case 115200:		/* 39.0625 */
		div = MAKE_FRAC(39, FRAC_0_0625);
		break;

	/* Todo: other baud rates */
		
	default:
		return 1;
	}

	if ( fmt[0] == '7' || fmt[0] == '8' || fmt[0] == '9' )
	{
		bits = fmt[0] - '0';
	}
	else
		return 2;

	if ( fmt[1] == 'N' )
	{
		/* OK */
	}
	else
	if ( fmt[1] == 'E' || fmt[1] == 'O' )
	{
		bits++;
	}
	else
		return 3;

	if ( fmt[2] == '1' || fmt[2] == '2' )
	{
		/* OK */
	}
	else
		return 4;

	if ( bits < 8 || bits > 9 )
		return 5;

	/* Parameters are OK.
	*/

	/* Turn on GPIO A and USART1
	*/
	dv_rcc.apb2en |= (DV_RCC_IOPA | DV_RCC_USART1);

	/* Turn on GPIO A and enable alt functions. Assumes no remapping i.e. Tx on PA9, Rx on PA10
	*/

	/* Select alt output/open drain/50 MHz on PA9
	*/
	int cr = 9 / 8;
	int shift = (9 % 8) * 4;
	dv_u32_t mask = 0xf << shift;
	dv_u32_t val = DV_GPIO_ALT_PP_50 << shift;
	dv_gpio_a.cr[cr] = (dv_gpio_a.cr[cr] & ~mask) | val;

	/* Select input/pullup on PA10
	*/
	cr = 10 / 8;
	shift = (10 % 8) * 4;
	mask = 0xf << shift;
	val = DV_GPIO_IN_PUD << shift;
	dv_gpio_a.cr[cr] = (dv_gpio_a.cr[cr] & ~mask) | val;
	dv_gpio_a.odr |= (0x1<<10);

	


	/* Enable the UART but keep the transmitter and receiver off.
	*/
	dv_uart1.cr[0] = DV_UART_UE;

	/* Turn off all other modes (LIN, flow control, DMA etc.).
	*/
	dv_uart1.cr[1] = 0;
	dv_uart1.cr[2] = 0;
	dv_uart1.gtpr = 0;

	/* Program the baud rate
	*/
	dv_uart1.brr = div;

	/* If 9 bits needed, set the M flag
	*/
	if ( bits == 9 )
		dv_uart1.cr[0] |= DV_UART_M;

	/* If parity needed, set the PCE flag, and the PS flag for odd parity
	*/
	if ( fmt[1] == 'E' )
		dv_uart1.cr[0] |= DV_UART_PCE;
	else
	if ( fmt[1] == 'O' )
		dv_uart1.cr[0] |= (DV_UART_PCE | DV_UART_PS);

	/* If 2 stop bits needed, select it. 1 stop bit is the 0 value already written.
	*/
	if ( fmt[2] == '2' )
		dv_uart1.cr[1] |= DV_UART_STOP_2;

	/* Finally, enable the transmitter and receiver
	*/
	dv_uart1.cr[0] |= (DV_UART_TE | DV_UART_RE);

	return 0;
}
#else
extern volatile unsigned char floom;
#endif
