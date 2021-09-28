#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
# Do not use Android variables in this file
# By using unique bootloader variables we allow the ufbl
# build to work side-by-side with Android


# This function stolen from gettop in Android build/envsetup.sh
function gettop-ufbl
{
    local TOPFILE=bootable/bootloader/ufbl-features/build/envsetup.sh
    if [ -n "$TOP" -a -f "$TOP/$TOPFILE" ] ; then
        echo $TOP
    else
        if [ -f $TOPFILE ] ; then
            # The following circumlocution (repeated below as well) ensures
            # that we record the true directory name and not one that is
            # faked up with symlink names.
            PWD= /bin/pwd
        else
            local HERE=$PWD
            T=
            while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
                \cd ..
                T=`PWD= /bin/pwd -P`
            done
            \cd $HERE
            if [ -f "$T/$TOPFILE" ]; then
                echo $T
            fi
        fi
    fi
}
# This function taken from Android build/envsetup.sh.
# It will replace the normal make invocation, with prettier output
# This should occasionally synced with the original code, in case
# anything changed
MAKE_UTIL=(`which make`)
function make()
{
    local start_time=$(date +"%s")
    $MAKE_UTIL $@
    local ret=$?
    local end_time=$(date +"%s")
    local tdiff=$(($end_time-$start_time))
    local hours=$(($tdiff / 3600 ))
    local mins=$((($tdiff % 3600) / 60))
    local secs=$(($tdiff % 60))
    echo
    if [ $ret -eq 0 ] ; then
        echo -n -e "\e[0;32m#### make completed successfully "
    else
        echo -n -e "\e[0;31m#### make failed to build some targets "
    fi
    if [ $hours -gt 0 ] ; then
        printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
    elif [ $mins -gt 0 ] ; then
        printf "(%02g:%02g (mm:ss))" $mins $secs
    elif [ $secs -gt 0 ] ; then
        printf "(%s seconds)" $secs
    fi
    echo -e " ####\e[00m"
    echo
    return $ret
}

function bcroot()
{
    T=$TOPB
    if [ "$T" ]; then
        \cd $T
    else
        echo "Couldn't locate the ufbl root.  Try setting TOPB."
    fi
}

function bcout()
{
    T=$OUTB
    if [ "$T" ]; then
        \cd $T
    else
        echo "Couldn't locate the bootloader out.  Try setting OUTB."
    fi
}

# Strip out old paths, if they were set
if [ -n "$BOOTLOADER_BUILD_PATHS" ] ; then
    export PATH=${PATH/$BOOTLOADER_BUILD_PATHS/}
    # strip leading ':', if any
    export PATH=${PATH/:%/}
fi

case $(uname -s) in
    Darwin)
        UFBL_HOST=darwin-x86
        # Remove the below line when darwin prebuilts are checked in
        echo "Unsupported target: $(uname -s)"
        return
        ;;
    Linux)
        UFBL_HOST=linux-x86
        ;;
    *)
        echo "Unsupported target: $(uname -s)"
        return
        ;;
esac
# This bootloader support is for arm (32-bit) only.  64-bit arm or other architectures
# will have to add dynamic support.  Copy Android's build/envsetup.sh
export BOOTLOADER_TOOLCHAIN=$(gettop-ufbl)/prebuilts/gcc/$UFBL_HOST/arm/arm-eabi-4.8/bin
export BOOTLOADER_SUPPORT_BIN=$(gettop-ufbl)/bootable/bootloader/ufbl-features/bin/$UFBL_HOST

# Two unnecessary directories are added because signing scripts like to
# dump output into $(BOOTLOADER_OUT)/../../
# When the signing scripts are fixed, these directories can be removed.
export BOOTLOADER_OUT=$(gettop-ufbl)/out/ufbl/unnecessary1/unnecessary2

# Helpers for the user and for bcroot/bcout
export OUTB=$BOOTLOADER_OUT
export TOPB=$(gettop-ufbl)/bootable/bootloader/ufbl-features

# Set the new paths
export BOOTLOADER_BUILD_PATHS=$BOOTLOADER_TOOLCHAIN:$BOOTLOADER_SUPPORT_BIN:
export PATH=$BOOTLOADER_BUILD_PATHS$PATH
