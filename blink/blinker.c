/* blinker.c
*/
#include "pico/stdlib.h"
#include "blink.h"

#define ledpin	PICO_DEFAULT_LED_PIN

int main(int argc, char **argv)
{
	gpio_init(ledpin);
	gpio_set_dir(ledpin, GPIO_OUT);

	for (;;)
	{
		blink(ledpin);
	}
}
