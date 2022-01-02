# pico-playground

I bought a Raspberry Pi Pico board. These are my first simple projects for it.

## blink

"blink" is a simple LED blinker using the SDK. However, that carries the whole baggage of the SDK with it.
Among other things, it's slow to build. I want to run davroska on the board, so I need to understand the actual
hardware a bit better.

## heartbeat

A blink/uart/interrupt project that was developed iteratively.

## simpleuart

Substantially copied from the uart01 example in David Welch's pico-baremetal projects:

https://github.com/dwelch67/raspberrypi-pico

I copied it here as part of the process of finding out why the uart part of heartbeat didn't work. There
are minor modifications:
* Load directly into RAM, like heartbeat.
* Split the uart initialisation into a function that can be called from elsewhere.

## License, disclaimer etc.

With the exception of simpleuart/notmain.c, the software is released under the GPL:

Copyright David Haworth

This file is part of the pico playground.

The pico playground is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The pico playground is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the pico playground.  If not, see <http://www.gnu.org/licenses/>.

