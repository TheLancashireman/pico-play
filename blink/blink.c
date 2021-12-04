/* blink.c
 *
 * (c) David Haworth
*/
#include "pico/stdlib.h"
#include "blink.h"

void blink(int pin)
{
	for (int i = 0; i < 100; i++ )
	{
		gpio_put(pin, (i == 0));
		sleep_ms(20);
	}
}
