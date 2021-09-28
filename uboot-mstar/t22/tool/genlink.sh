#!/bin/bash
CHIP=$1

if [ "$1" = "" ]; then
    echo "Warning, please input the argument!"
	echo "ex: sh genlink.sh a3"
    exit 1
fi

test -d ../u-boot-2011.06/include/mstar
if [ $? != 0 ]
then
	mkdir ../u-boot-2011.06/include/mstar
	echo "mkdir ../u-boot-2011.06/include/mstar"
fi

test -d ../u-boot-2011.06/include/mstar/core
if [ $? != 0 ]
then
	mkdir ../u-boot-2011.06/include/mstar/core
	echo "mkdir ../u-boot-2011.06/include/mstar/core"
fi

test -L ../u-boot-2011.06/lib/mstar
if [ $? != 0 ]
then
# link MstarCore

echo "-- Construct symbolic link OK --"
else
echo "-- already link --"
fi
