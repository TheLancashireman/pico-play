#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if ( argc < 3 )
	{
		fprintf(stderr, "Usage: brcalc clockspeed baudrate [baudrate ..]\n");
		exit(1);
	}

	long v = atol(argv[1]);

	if ( v <= 0 || v > 2147483647 )
	{
		fprintf(stderr, "Clock speed out of range\n");
		exit(1);
	}

	unsigned long clk = v;

	for ( int i = 2; i < argc; i++ )
	{
		v = atol(argv[i]);

		if ( v <= 0 || v > 2147483647 )
		{
			printf("%ld  Invalid!\n", v);
			continue;
		}

		unsigned long baud = (unsigned long)v;

		unsigned bdiv = (8 * clk)/baud;
		unsigned ibrd = bdiv >> 7;
		unsigned fbrd = ((bdiv & 0x7f) + 1) / 2;

		if ( ibrd == 0 || ibrd > 65535 )
		{
			printf("%8u  %5u  %2u/64  Invalid!\n", baud, ibrd, fbrd);
		}
		else
		{
			printf("%8u  %5u  %2u/64\n", baud, ibrd, fbrd);
		}
	}

	return 0;
}
