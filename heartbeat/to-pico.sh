#!/bin/sh
set -e

if [ -z "$1" ] ; then
	echo "Usage: to-pico.sh <UF2-file>"
	exit 1
fi

sudo mount -o uid=dave /dev/sdc1 /media/pico
if [ -e /media/pico/INFO_UF2.TXT ]; then
	if diff /media/pico/INFO_UF2.TXT INFO_UF2.TXT ; then
		cp $1 /media/pico
	else
		echo "The mounter UF2 loader is not a Raspberry Pi pico"
		exit 1
	fi
else
	echo "The mounted filesystem is not a UF2 loader"
	exit 1
fi
sudo umount /media/pico
