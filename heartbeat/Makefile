#  Makefile
#
#  Copyright David Haworth
#
#  This file is part of the pico playground.
#
#  The pico playground is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  The pico playground is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the pico playground.  If not, see <http://www.gnu.org/licenses/>.

# Use the installed ARM gcc
DV_GNU_D	?= /usr

DV_GCC		?= $(DV_GNU_D)/bin/arm-none-eabi-gcc
DV_LD		?= $(DV_GNU_D)/bin/arm-none-eabi-ld
DV_OBJCOPY	?= $(DV_GNU_D)/bin/arm-none-eabi-objcopy

DV_ELF2UF2	?=	elf2uf2

DV_BIN_D	= bin
DV_OBJ_D	= obj

DV_ENTRY	?= dv_boot
DV_LDSCRIPT	?= pico.ldscript

CC_OPT		+= -Wall
CC_OPT		+= -fno-common
CC_OPT		+= -mcpu=cortex-m0plus
CC_OPT		+= -mthumb

CC_OPT		+= -O2
CC_OPT		+= -g

LD_OPT		+= --thumb-entry=$(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+= -L /data1/projects/OtherPeople/libaeabi-cortexm0
LD_OPT		+= -laeabi-cortexm0

VPATH		+= .

# The demo code
DV_LD_OBJS	+= $(DV_OBJ_D)/boot.o
DV_LD_OBJS	+= $(DV_OBJ_D)/vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/play.o
DV_LD_OBJS	+= $(DV_OBJ_D)/leds.o
DV_LD_OBJS	+= $(DV_OBJ_D)/systick.o
DV_LD_OBJS	+= $(DV_OBJ_D)/pico-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/nvic.o

.PHONY:		default all help clean install srec binary

default:	uf2

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D)

all:		$(DV_OBJ_D) $(DV_BIN_D) elf

elf:		$(DV_BIN_D)/play.elf

srec:		$(DV_BIN_D)/play.srec

binary:		$(DV_BIN_D)/play.bin

uf2:		$(DV_BIN_D)/play.uf2

$(DV_BIN_D)/play.elf:	$(DV_LD_OBJS) $(DV_LDSCRIPT)
	$(DV_LD) -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)

$(DV_OBJ_D)/%.o:	%.c
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_OBJ_D)/%.o:	%.S
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p bin

$(DV_OBJ_D):
	mkdir -p obj

install:	all
	$(DV_OBJCOPY) $(DV_BIN_D)/play.elf -O binary $(INSTALL_DIR)/kernel.img

$(DV_BIN_D)/play.srec:	all
	$(DV_OBJCOPY) $(DV_BIN_D)/play.elf -O srec --srec-forceS3 /dev/stdout | dos2unix | \
		egrep -v '^S3..........00*..$$' > $(DV_BIN_D)/play.srec

$(DV_BIN_D)/play.bin:	all
	$(DV_OBJCOPY) $(DV_BIN_D)/play.elf -O binary $(DV_BIN_D)/play.bin

$(DV_BIN_D)/play.uf2:	all
	$(DV_ELF2UF2) -v $(DV_BIN_D)/play.elf $(DV_BIN_D)/play.uf2
