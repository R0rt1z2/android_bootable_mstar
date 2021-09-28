#!/bin/bash
#  MstarCore
rm -rf ../u-boot-2011.06/lib/mstar
rm -rf ../u-boot-2011.06/include/mstar/core
rm -rf ../u-boot-2011.06/drivers/mstar
rm -rf ../u-boot-2011.06/board/mstar
rm -rf ../u-boot-2011.06/include/configs/*
rm -rf ../u-boot-2011.06/arch/arm/lib/mstar
echo "-- remove symbolic link OK --"
