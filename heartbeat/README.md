# TheLancashireman's pico playground.

## What's this?

This subdirectory contains some code that I hacked together to learn about the RP2040 processor
and the Raspberry Pi Pico development board.

The aim of the game is to prepare the way for porting davroska to the pico.

Just the basic stuff one might need to get started on a simple bare-metal project:

* Load code directly into RAM via the onboard UF2 loader.
* Set the GPIO to blink the onboard LED.
* Set the PLL to run at the full 133 MHz.
* Use the timer - cortex M SysTick - to measure time reasonably accurately.
* Use the uart to communicate with the outside world.
* Basic interrupt handling using a software trigger.

Getting the code out of the flash and into the RAM is an exercise for later, as is the execute-in-place
mode.

# WARNING

The enclosed shell script, to-pico.sh, copies the UF2 file to the pico. To do that, it tries to
mount /dev/sdc1 at /media/pico. It requires supervisor rights (sudo) in order to do that.
You may have to enter your password to obtain those rights.

Ensure that you read and understand the script before using it.

# Development history

## Initial version

Copied from STM32 playground and bigly hacked. Heartbeat is a double pulse with a frequency of
nominally 75 beats per minute. No PLL initialisation, uses software timing loopsfor delays.
The loop in the delay() function is six instructions. The delay factor of 800 means that 4800 instructions
are executed every millisecond - which suggests a CPU clock frequency of about 5 MHz, which is
compatible with the range of frequencies given for the ring oscillator (see RP2040 refman 2.15.2.1)



# License, disclaimer etc.

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

