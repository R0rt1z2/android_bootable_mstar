VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0
EXTRAVERSION =
NAME = Unnamed
CUSTOMER = MSTAR
PROJECT="TM14"
TOOL_PATH="/tools/script/ck"
PROJDIR=`pwd`
BLOCK_SIZE=2097152
MAKE = make
MAKE += -j1
MAKAE += SHELL=/bin/bash

ROOTDIR := $(abspath $(TOP))
TOOLSCHAINPATH := $(ROOTDIR)/prebuilts/gcc/linux-x86/arm/arm_eabi-2011.03/bin/

# *DOCUMENTATION*
# To see a list of typical targets execute "make help"
# More info can be located in ./README
# Comments in this file are targeted only to the developer, do not
# expect to learn how to build the kernel reading this file.

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

# We are using a recursive build, so we need to do a little thinking
# to get the ordering right.
#
# Most importantly: sub-Makefiles should only ever modify files in
# their own directory. If in some directory we have a dependency on
# a file in another dir (which doesn't happen often, but it's often
# unavoidable when linking the built-in.o targets which finally
# turn into sboot), we will call a sub make in that other dir, and
# after that we are sure that everything which is in that other dir
# is now up to date.
#
# The only cases where we need to modify files which have global
# effects are thus separated out and done before the recursive
# descending is started. They are now explicitly listed as the
# prepare rule.

# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifdef V
  ifeq ("$(origin V)", "command line")
    KBUILD_VERBOSE = $(V)
  endif
endif
ifndef KBUILD_VERBOSE
  KBUILD_VERBOSE = 0
endif

# Call sparse as part of compilation of C files
# Use 'make C=1' to enable sparse checking

ifdef C
  ifeq ("$(origin C)", "command line")
    KBUILD_CHECKSRC = $(C)
  endif
endif
ifndef KBUILD_CHECKSRC
  KBUILD_CHECKSRC = 0
endif

# Use make M=dir to specify directory of external module to build
# Old syntax make ... SUBDIRS=$PWD is still supported
# Setting the environment variable KBUILD_EXTMOD take precedence
ifdef SUBDIRS
  KBUILD_EXTMOD ?= $(SUBDIRS)
endif
ifdef M
  ifeq ("$(origin M)", "command line")
    KBUILD_EXTMOD := $(M)
  endif
endif


# kbuild supports saving output files in a separate directory.
# To locate output files in a separate directory two syntaxes are supported.
# In both cases the working directory must be the root of the kernel src.
# 1) O=
# Use "make O=dir/to/store/output/files/"
#
# 2) Set KBUILD_OUTPUT
# Set the environment variable KBUILD_OUTPUT to point to the directory
# where the output files shall be placed.
# export KBUILD_OUTPUT=dir/to/store/output/files/
# make
#
# The O= assignment takes precedence over the KBUILD_OUTPUT environment
# variable.


# KBUILD_SRC is set on invocation of make in OBJ directory
# KBUILD_SRC is not intended to be used by the regular user (for now)
ifeq ($(KBUILD_SRC),)

# OK, Make called in directory where kernel src resides
# Do we want to locate output files in a separate directory?
ifdef O
  ifeq ("$(origin O)", "command line")
    KBUILD_OUTPUT := $(O)
  endif
endif

# That's our default target when none is given on the command line
PHONY := _all
_all:
ifneq ($(KBUILD_OUTPUT),)
# Invoke a second make in the output directory, passing relevant variables
# check that the output directory actually exists
saved-output := $(KBUILD_OUTPUT)
KBUILD_OUTPUT := $(shell cd $(KBUILD_OUTPUT) && /bin/pwd)
$(if $(KBUILD_OUTPUT),, \
     $(error output directory "$(saved-output)" does not exist))

PHONY += $(MAKECMDGOALS)

ifeq ($(MAKECMDGOALS),oldconfig)
$(filter-out _all,$(MAKECMDGOALS)) _all:
	$(if $(KBUILD_VERBOSE:1=),@)$(MAKE) -C $(KBUILD_OUTPUT) \
	KBUILD_SRC=$(CURDIR) \
	KBUILD_EXTMOD="$(KBUILD_EXTMOD)" -f $(CURDIR)/Makefile $@

# Leave processing to above invocation of make
skip-makefile := 1
endif

ifeq ($(MAKECMDGOALS),.kernelrelease)
$(filter-out _all,$(MAKECMDGOALS)) _all:
	$(if $(KBUILD_VERBOSE:1=),@)$(MAKE) -C $(KBUILD_OUTPUT) \
	KBUILD_SRC=$(CURDIR) \
	KBUILD_EXTMOD="$(KBUILD_EXTMOD)" -f $(CURDIR)/Makefile $@

# Leave processing to above invocation of make
skip-makefile := 1
endif


endif # ifneq ($(KBUILD_OUTPUT),)
endif # ifeq ($(KBUILD_SRC),)

# We process the rest of the Makefile if this is the final invocation of make
ifeq ($(skip-makefile),)

# If building an external module we do not care about the all: rule
# but instead _all depend on modules
PHONY += all
ifeq ($(KBUILD_EXTMOD),)
_all: all
else
_all: modules
endif

srctree		:= $(if $(KBUILD_SRC),$(KBUILD_SRC),$(CURDIR))
TOPDIR		:= $(srctree)
# FIXME - TOPDIR is obsolete, use srctree/objtree
objtree		:= $(CURDIR)
src		:= $(srctree)
obj		:= $(objtree)

VPATH		:= $(srctree)$(if $(KBUILD_EXTMOD),:$(KBUILD_EXTMOD))

export srctree objtree VPATH TOPDIR PRODUCT_OUT

# Cross compiling and selecting different set of gcc/bin-utils
# ---------------------------------------------------------------------------
#
# When performing cross compilation for other architectures ARCH shall be set
# to the target architecture. (See arch/* for the possibilities).
# ARCH can be set during invocation of make:
# make ARCH=ia64
# Another way is to have ARCH set in the environment.
# The default ARCH is the host where make is executed.

# CROSS_COMPILE specify the prefix used for all executables used
# during compilation. Only gcc and related bin-utils executables
# are prefixed with $(CROSS_COMPILE).
# CROSS_COMPILE can be set on the command line
# make CROSS_COMPILE=ia64-linux-
# Alternatively CROSS_COMPILE can be set in the environment.
# Default value for CROSS_COMPILE is not to prefix executables
# Note: Some architectures assign CROSS_COMPILE in their arch/*/Makefile

CROSS_COMPILE ?=
UBOOT_CROSS_COMPILE ?=

# bbox: we may have CONFIG_CROSS_COMPILER_PREFIX in .config,
# and it has not been included yet... thus using an awkward syntax.
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE_PREFIX := $(shell grep ^CONFIG_SBOOT_CROSS_COMPILER_PREFIX .config 2>/dev/null)
CROSS_COMPILE_PREFIX := $(subst CONFIG_SBOOT_CROSS_COMPILER_PREFIX=,,$(CROSS_COMPILE_PREFIX))
CROSS_COMPILE_PREFIX := $(subst ",,$(CROSS_COMPILE_PREFIX))
#")
CROSS_COMPILE := $(TOOLSCHAINPATH)$(CROSS_COMPILE_PREFIX)

UBOOT_CROSS_COMPILE_PREFIX := $(shell grep ^CONFIG_CROSS_COMPILER_PREFIX .config 2>/dev/null)
UBOOT_CROSS_COMPILE_PREFIX := $(subst CONFIG_CROSS_COMPILER_PREFIX=,,$(UBOOT_CROSS_COMPILE_PREFIX))
UBOOT_CROSS_COMPILE_PREFIX := $(subst ",,$(UBOOT_CROSS_COMPILE_PREFIX))
#")
UBOOT_CROSS_COMPILE := $(TOOLSCHAINPATH)$(UBOOT_CROSS_COMPILE_PREFIX)
endif

export UBOOT_CROSS_COMPILE

# SUBARCH tells the usermode build what the underlying arch is.  That is set
# first, and if a usermode build is happening, the "ARCH=um" on the command
# line overrides the setting of ARCH below.  If a native build is happening,
# then ARCH is assigned, getting whatever value it gets normally, and
# SUBARCH is subsequently ignored.

ifneq ($(CROSS_COMPILE_PREFIX),)
SUBARCH := $(shell echo $(CROSS_COMPILE_PREFIX) | cut -d- -f1)
else
SUBARCH := $(shell uname -m)
endif
SUBARCH := $(shell echo $(SUBARCH) | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ \
					 -e s/arm.*/arm/ -e s/sa110/arm/ \
					 -e s/s390x/s390/ -e s/parisc64/parisc/ \
					 -e s/ppc.*/powerpc/ -e s/mips.*/mips/ )

ARCH ?= $(SUBARCH)

# Architecture as present in compile.h
UTS_MACHINE := $(ARCH)

# SHELL used by kbuild
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

# 	Decide whether to build built-in, modular, or both.
#	Normally, just do built-in.

KBUILD_MODULES :=
KBUILD_BUILTIN := 1

#	If we have only "make modules", don't compile built-in objects.
#	When we're building modules with modversions, we need to consider
#	the built-in objects during the descend as well, in order to
#	make sure the checksums are uptodate before we record them.

ifeq ($(MAKECMDGOALS),modules)
  KBUILD_BUILTIN := $(if $(CONFIG_MODVERSIONS),1)
endif

#	If we have "make <whatever> modules", compile modules
#	in addition to whatever we do anyway.
#	Just "make" or "make all" shall build modules as well

ifneq ($(filter all _all modules,$(MAKECMDGOALS)),)
  KBUILD_MODULES := 1
endif

ifeq ($(MAKECMDGOALS),)
  KBUILD_MODULES := 1
endif

export KBUILD_MODULES KBUILD_BUILTIN
export KBUILD_CHECKSRC KBUILD_SRC KBUILD_EXTMOD

# Beautify output
# ---------------------------------------------------------------------------
#
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing wil be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If KBUILD_VERBOSE equals 0 then the above command will be hidden.
# If KBUILD_VERBOSE equals 1 then the above command is displayed.

ifeq ($(KBUILD_VERBOSE),1)
  quiet =
  Q =
else
  quiet=quiet_
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(MAKEFLAGS)),)
  quiet=silent_
endif

export quiet Q KBUILD_VERBOSE


# Look for make include files relative to root of kernel src
MAKEFLAGS += --include-dir=$(srctree)

HOSTCC  	= gcc
HOSTCXX  	= g++
HOSTCFLAGS	:=
HOSTCXXFLAGS	:=
# We need some generic definitions
include $(srctree)/scripts/Kbuild.include

HOSTCFLAGS	+= $(call hostcc-option,-Wall -Wstrict-prototypes -O2 -fomit-frame-pointer,)
HOSTCXXFLAGS	+= -O2

# For maximum performance (+ possibly random breakage, uncomment
# the following)

MAKEFLAGS += -rR

# Add flag for specific compiler version
ifeq ($(CPU),mips)
SBOOT_TOOLCHAIN_VERSION = -V 4.3.2
else
SBOOT_TOOLCHAIN_VERSION =
endif

# Make variables (CC, etc...)

AS		= $(CROSS_COMPILE)as
ifeq ($(PARA),1)
CC		= cpptestscan --cpptestscanProjectName=MBoot $(CROSS_COMPILE)gcc $(SBOOT_TOOLCHAIN_VERSION)
else
CC		= $(CROSS_COMPILE)gcc $(SBOOT_TOOLCHAIN_VERSION)
endif
LD		= $(CC) -nostdlib
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
AWK		= awk
GENKSYMS	= scripts/genksyms/genksyms
DEPMOD		= /sbin/depmod
KALLSYMS	= scripts/kallsyms
PERL		= perl
CHECK		= sparse
CAT 		=cat
GREP		=grep
CHECKFLAGS     := -D__linux__ -Dlinux -D__STDC__ -Dunix -D__unix__ -Wbitwise $(CF)
MODFLAGS	= -DMODULE
CFLAGS_MODULE   = $(MODFLAGS)
AFLAGS_MODULE   = $(MODFLAGS)
LDFLAGS_MODULE  = -r
CFLAGS_KERNEL	=
AFLAGS_KERNEL	=
ifeq ($(CPU),aeonR2)
LDFLAGS +=-maeonR2_elfl -EBinst
endif

# Use LINUXINCLUDE when you must reference the include/ directory.
# Needed to be compatible with the O= option
CFLAGS		:= $(CFLAGS)
# Added only to final link stage of sboot binary
CFLAGS_sboot	:= $(CFLAGS_sboot)
CPPFLAGS	:= $(CPPFLAGS)
AFLAGS		:= $(AFLAGS)
LDFLAGS		:= $(LDFLAGS)
LDLIBS		:=

# Read KERNELRELEASE from .kernelrelease (if it exists)
KERNELRELEASE = $(shell cat .kernelrelease 2> /dev/null)
KERNELVERSION = $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION)

export	VERSION PATCHLEVEL SUBLEVEL KERNELRELEASE KERNELVERSION \
	ARCH CONFIG_SHELL HOSTCC HOSTCFLAGS CROSS_COMPILE AS LD CC \
	CPP AR NM STRIP OBJCOPY OBJDUMP MAKE AWK GENKSYMS PERL UTS_MACHINE \
	HOSTCXX HOSTCXXFLAGS LDFLAGS_MODULE CHECK CHECKFLAGS

export CPPFLAGS NOSTDINC_FLAGS LINUXINCLUDE OBJCOPYFLAGS LDFLAGS
export CFLAGS CFLAGS_KERNEL CFLAGS_MODULE
export AFLAGS AFLAGS_KERNEL AFLAGS_MODULE
export FLTFLAGS

# When compiling out-of-tree modules, put MODVERDIR in the module
# tree rather than in the kernel tree. The kernel tree might
# even be read-only.
export MODVERDIR := $(if $(KBUILD_EXTMOD),$(firstword $(KBUILD_EXTMOD))/).tmp_versions

# Files to ignore in find ... statements

RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o -name .svn -o -name CVS -o -name .pc -o -name .hg -o -name .git \) -prune -o
export RCS_TAR_IGNORE := --exclude SCCS --exclude BitKeeper --exclude .svn --exclude CVS --exclude .pc --exclude .hg --exclude .git

# ===========================================================================
# Rules shared between *config targets and build targets

# Basic helpers built in scripts/

-include .config
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic

PROJ=sboot
_PROJ=u-boot-2011.06

ifeq ($(CONFIG_SBOOT_TO_KERNEL), y)
all: $(O)/out/sboot2knl.bin doc
endif

# To avoid any implicit rule to kick in, define an empty command.
scripts/basic/%: scripts_basic ;

PHONY += outputmakefile
# outputmakefile generates a Makefile in the output directory, if using a
# separate output directory. This allows convenient use of make in the
# output directory.
outputmakefile:
ifneq ($(KBUILD_SRC),)
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/mkmakefile \
	    $(srctree) $(objtree) $(VERSION) $(PATCHLEVEL)
endif

# To make sure we do not include .config for any of the *config targets
# catch them early, and hand them over to scripts/kconfig/Makefile
# It is allowed to specify more targets when calling make, including
# mixing *config targets and build targets.
# For example 'make oldconfig all'.
# Detect when mixed targets is specified, and make a second invocation
# of make so .config is not included in this case either (for *config).

no-dot-config-targets := clean mrproper distclean \
			 cscope TAGS tags help %docs
#bbox# check% is removed from above

config-targets := 0
mixed-targets  := 0
dot-config     := 1

ifneq ($(filter $(no-dot-config-targets), $(MAKECMDGOALS)),)
	ifeq ($(filter-out $(no-dot-config-targets), $(MAKECMDGOALS)),)
		dot-config := 0
	endif
endif

ifeq ($(KBUILD_EXTMOD),)
        ifneq ($(filter config %config,$(MAKECMDGOALS)),)
                config-targets := 1
                ifneq ($(filter-out config %config,$(MAKECMDGOALS)),)
                        mixed-targets := 1
                endif
        endif
endif

ifeq ($(mixed-targets),1)
# ===========================================================================
# We're called with mixed targets (*config and build targets).
# Handle them one by one.

%:: FORCE
	$(Q)$(MAKE) -C $(srctree) KBUILD_SRC= $@

else
ifeq ($(config-targets),1)
# ===========================================================================
# *config targets only - make sure prerequisites are updated, and descend
# in scripts/kconfig to make the *config target

# Read arch specific Makefile to set KBUILD_DEFCONFIG as needed.
# KBUILD_DEFCONFIG may point out an alternative default configuration
# used for 'make defconfig'
-include $(srctree)/arch/$(ARCH)/Makefile
export KBUILD_DEFCONFIG

config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include
	$(Q)$(MAKE) $(build)=scripts/kconfig $@
	$(Q)$(MAKE) -C $(srctree) KBUILD_SRC= .kernelrelease

%config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include
	$(Q)$(MAKE) $(build)=scripts/kconfig $@
	$(Q)$(MAKE) -C $(srctree) KBUILD_SRC= .kernelrelease

else
# ===========================================================================
# Build targets only - this includes sboot, arch specific targets, clean
# targets and others. In general all targets except *config targets.

ifeq ($(KBUILD_EXTMOD),)
# Additional helpers built in scripts/
# Carefully list dependencies so we do not try to build scripts twice
# in parrallel
PHONY += scripts
scripts: scripts_basic $(O)/include/config/MARKER
	$(Q)$(MAKE) $(build)=$(@)

scripts_basic: include/autoconf.h

# Objects we will link into sboot / subdirs we need to visit
core-y		:= \

libs-y		:= \
		$(sort $(dir $(wildcard src/*))) \

endif # KBUILD_EXTMOD

ifeq ($(dot-config),1)
# In this section, we need .config

# Read in dependencies to all Kconfig* files, make sure to run
# oldconfig if changes are detected.
-include .kconfig.d

-include .config

# If .config needs to be updated, it will be done via the dependency
# that autoconf has on .config.
# To avoid any implicit rule to kick in, define an empty command
.config .kconfig.d: ;

# Now we can define CFLAGS etc according to .config
include $(srctree)/Makefile.flags

# If .config is newer than include/autoconf.h, someone tinkered
# with it and forgot to run make oldconfig.
# If kconfig.d is missing then we are probarly in a cleaned tree so
# we execute the config step to be sure to catch updated Kconfig files
include/autoconf.h: .kconfig.d .config
	$(Q)$(MAKE) -f $(srctree)/Makefile silentoldconfig

else
# Dummy target needed, because used as prerequisite
include $(srctree)/Makefile.flags
include/autoconf.h: ;
endif

ifeq ($(ARCH),mips)
  GCCVERSION := $(shell expr `$(UBOOT_CROSS_COMPILE)gcc -dumpversion | cut -f1-3 -d.` \)
  ifeq ($(strip $(TOOLCHAIN_VERSION)),)
    ifneq ($(GCCVERSION),4.8.3)
      $(error MIPS-GCCVERSION is $(GCCVERSION), it needs to use version 4.8.3)
    endif
  else
    ifneq ($(GCCVERSION),$(TOOLCHAIN_VERSION))
      $(error MIPS-GCCVERSION is $(GCCVERSION), it needs to use version $(TOOLCHAIN_VERSION))
    endif
  endif
endif

# The all: target is the default when no target is given on the
# command line.
# This allow a user to issue only 'make' to build a kernel including modules
# Defaults sboot but it is usually overridden in the arch makefile

ifeq ($(origin TOOLCHAIN_VERSION),undefined)
all: PROTECT_FLAG_CONFLICT_CHECK MBOOT_SIZE_CHECK LINT_RESULT doc
else
all: check_build_toolver PROTECT_FLAG_CONFLICT_CHECK MBOOT_SIZE_CHECK LINT_RESULT doc
endif

	$(Q)rm -f ../$(_PROJ)/u-boot.bin
	$(Q)rm -f ../$(_PROJ)/u-boot.map
	$(Q)rm src/$(CHIP_FOLDER)/_*.o
	$(Q)mv hash.bin $(O)/out

-include $(srctree)/arch/$(ARCH)/Makefile

# arch Makefile may override CC so keep this after arch Makefile is included
#bbox# NOSTDINC_FLAGS += -nostdinc -isystem $(shell $(CC) -print-file-name=include)
CHECKFLAGS += $(NOSTDINC_FLAGS)

# Default kernel image to build when no specific target is given.
# KBUILD_IMAGE may be overruled on the commandline or
# set in the environment
# Also any assignments in arch/$(ARCH)/Makefile take precedence over
# this default value
export KBUILD_IMAGE ?= sboot

#
# INSTALL_PATH specifies where to place the updated kernel and system map
# images. Default is /boot, but you can set it to other values
export	INSTALL_PATH ?= /boot

#
# INSTALL_MOD_PATH specifies a prefix to MODLIB for module directory
# relocations required by build roots.  This is not defined in the
# makefile but the arguement can be passed to make if needed.
#

MODLIB	= $(INSTALL_MOD_PATH)/lib/modules/$(KERNELRELEASE)
export MODLIB

libs-y +=  $(sort $(dir $(wildcard src/$(CHIP_FOLDER)/))) \

ifeq ($(KBUILD_EXTMOD),)
sboot-dirs	:= $(patsubst %/,%,$(filter %/, $(core-y) $(core-m) $(libs-y) $(libs-m)))

sboot-alldirs	:= $(sort $(sboot-dirs) $(patsubst %/,%,$(filter %/, \
		     $(core-n) $(core-) $(libs-n) $(libs-) \
		)))

ifeq ($(CONFIG_MSTAR_EDEN_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSYS.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib

#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvHWI2C.lib     \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvVE.lib        \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWBLE.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvBDMA.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIRQ.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMMIO.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSEM.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvUART.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMBX.lib      \

core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else ifeq ($(CONFIG_MSTAR_MAYA_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib


core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else ifeq ($(CONFIG_MSTAR_WHISKY_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIPAUTH.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib

core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else ifeq ($(CONFIG_MSTAR_MELODY_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIPAUTH.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib

core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else ifeq ($(CONFIG_MSTAR_EULER_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSYS.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib

#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvHWI2C.lib     \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvVE.lib        \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWBLE.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvBDMA.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIRQ.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMMIO.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSEM.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvUART.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMBX.lib      \

core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else ifeq ($(CONFIG_MSTAR_NASA_PM),y)
UTOPIA_LIB = ./src/$(CHIP_FOLDER)/utopia/lib/libapiCEC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiSWI2C.lib     \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvCPU.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvDDC2BI.lib    \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvGPIO.lib      \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIR.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPM.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvPWM.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvRTC.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSAR.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSERFLASH.lib  \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSYS.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWDT.lib       \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libapiXC.lib        \
						 ./src/$(CHIP_FOLDER)/utopia/lib/libnos.lib

#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvHWI2C.lib     \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvVE.lib        \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvWBLE.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvBDMA.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvIRQ.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMMIO.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvSEM.lib       \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvUART.lib      \
#						 ./src/$(CHIP_FOLDER)/utopia/lib/libdrvMBX.lib      \

core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(patsubst %/, %/lib.a, $(libs-y))
libs-y2		:= $(patsubst %/, %/built-in.o, $(libs-y))
libs-y		:= ./src/$(CHIP_FOLDER)/lib.a $(libs-y2) $(UTOPIA_LIB)
else
core-y		:= $(patsubst %/, %/built-in.o, $(core-y))
libs-y1		:= $(foreach n,$(patsubst %/, %/lib.a, $(libs-y)),$(O)/$(n))
libs-y2		:= $(foreach n,$(patsubst %/, %/built-in.o, $(libs-y)),$(O)/$(n))
libs-y		:= $(libs-y1) $(libs-y2)
endif

# Build sboot
# ---------------------------------------------------------------------------
# sboot is build from the objects selected by $(sboot-init) and
# $(sboot-main). Most are built-in.o files from top-level directories
# in the kernel tree, others are specified in arch/$(ARCH)Makefile.
# Ordering when linking is important, and $(sboot-init) must be first.
#
# sboot
#   ^
#   |
#   +-< $(sboot-init)
#   |   +--< init/version.o + more
#   |
#   +--< $(sboot-main)
#   |    +--< driver/built-in.o mm/built-in.o + more
#   |
#   +-< kallsyms.o (see description in CONFIG_KALLSYMS section)
#
# sboot version (uname -v) cannot be updated during normal
# descending-into-subdirs phase since we do not yet know if we need to
# update sboot.
# Therefore this step is delayed until just before final link of sboot -
# except in the kallsyms case where it is done just before adding the
# symbols to the kernel.
#
# System.map is generated to document addresses of all kernel symbols

BOOT_OUTPUT_PATH= $(O)/out

BOOT1_INC       = $(BOOT_OUTPUT_PATH)/boot1.inc
SBOOT_BIN       = $(BOOT_OUTPUT_PATH)/sboot.bin
UBOOT_BIN       = $(BOOT_OUTPUT_PATH)/$(CONFIG_APP_AS_2ND_LOADER_BIN_PATH)
UBOOT_MAP       = $(BOOT_OUTPUT_PATH)/$(CONFIG_APP_AS_2ND_LOADER_MAP_PATH)
CHUNK_HEADER    = $(BOOT_OUTPUT_PATH)/chunk_header.bin
SECURITY_BIN    = $(BOOT_OUTPUT_PATH)/secure_info.bin
SECURITY_INC    = $(BOOT_OUTPUT_PATH)/security.inc

ifeq ($(CONFIG_HASH0_PATH), y)
HASH0_BIN       = secure/hash/$(CHIP_FOLDER)/$(CONFIG_HASH0_PATH_STRING)/hash0.bin
else
HASH0_BIN       = secure/hash/$(CHIP_FOLDER)/hash0.bin
endif
ifeq ($(CONFIG_MTK_LOADER),y)
MTK_LOADER_SRC_BIN       = bin/mtk_loader/$(CHIP_FOLDER)/bootloader.bin
MTK_LOADER_BIN       = $(BOOT_OUTPUT_PATH)/bootloader.bin
endif
HASH0_BIN_SZ    = secure/hash/$(CHIP_FOLDER)/SZ/hash0.bin
AUT_BIN         = secure/AUT_info.bin
HASH0_BIN_AMZN  = secure/hash/$(CHIP_FOLDER)/HASH0_CID40_SWAU.bin

HASH1_BIN       = $(BOOT_OUTPUT_PATH)/hash1.bin
HASH2_BIN       = $(BOOT_OUTPUT_PATH)/hash2.bin
RSAUPGRADE_BIN  = $(SECUREDIR)/RSAupgrade_pub.bin
RSAEA_BIN       = $(BOOT_OUTPUT_PATH)/RSAEA_pub.bin

ifeq ($(CONFIG_DOUBLE_MBOOT), y)
HASH3_BIN       = $(BOOT_OUTPUT_PATH)/hash3.bin
endif

HASH1_MIU_BIN   = $(BOOT_OUTPUT_PATH)/hash1_miu.bin
HASH1_STR_BIN   = $(BOOT_OUTPUT_PATH)/hash1_str.bin
HASH1_EXT_BIN   = $(BOOT_OUTPUT_PATH)/hash1_ext.bin
HASH1_DESKEW_BIN= $(BOOT_OUTPUT_PATH)/hash1_deskew.bin

EMMC_BOOT_BIN   = $(BOOT_OUTPUT_PATH)/rom_emmc_boot.bin
ROM_SPI_NOR_BOOT_BIN = $(BOOT_OUTPUT_PATH)/rom_spi_nor_boot.bin

HASH1_SIG       = $(BOOT_OUTPUT_PATH)/hash1.bin.sig.bin
HASH2_SIG       = $(BOOT_OUTPUT_PATH)/hash2.bin.sig.bin
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
HASH3_SIG       = $(BOOT_OUTPUT_PATH)/hash3.bin.sig.bin
endif

HASH1_STR_SIG   = $(BOOT_OUTPUT_PATH)/hash1_str.bin.sig.bin
HASH1_MIU_SIG   = $(BOOT_OUTPUT_PATH)/hash1_miu.bin.sig.bin
HASH1_EXT_SIG   = $(BOOT_OUTPUT_PATH)/hash1_ext.bin.sig.bin
HASH1_DESKEW_SIG= $(BOOT_OUTPUT_PATH)/hash1_deskew.bin.sig.bin

# gen signature for AllTBL.bin
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
ALLTBL_BIN       = $(BOOT_OUTPUT_PATH)/AllTbl.bin
endif

ifeq ($(CONFIG_TEE_LOADER), y)
HASH2_H_BIN      = $(BOOT_OUTPUT_PATH)/hash2_header.bin
HASH2_H_SIG      = $(BOOT_OUTPUT_PATH)/hash2_header.bin.sig.bin
TEELOADER_PREFIX =
TEELOADER_SUFFIX =
ifeq ($(CONFIG_R2TEE_TEELOADER), y)
TEELOADER_PREFIX = R2
endif
ifeq ($(CONFIG_ENCRYPTED_TEE_LOADER), y)
TEELOADER_SUFFIX = .aes
endif
ifeq ($(CONFIG_ENCRYPTED_CUSTOMER_KEYBANK), y)
CUSTOMER_KEYBANK_SUFFIX = .aes
endif
endif
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
TEELOADER_BIN    = $(BOOT_OUTPUT_PATH)/teeloader.bin
TEELOADER_SIG    = $(BOOT_OUTPUT_PATH)/teeloader.bin.sig.bin
endif


ifeq ($(CONFIG_MSTAR_MILAN),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3000
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xA300

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_M5321),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3000
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xA300

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MARLON),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x3C00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x4000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xBF00

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MAINZ),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x5400
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x5800
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xEB00
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif
CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MASERATI),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH1_EXT_OFFSET_ADDRESS= 0x7000
CONFIG_BOOT_HASH1_EXT_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_EXT_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_EXT_SIG_ADDRESS   = 0x8B00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x9000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xA300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0xA400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x5800
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x5C00
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xFF00
else
ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_BOOT_HASH2_HEADER_SIZE       = 0x100
CONFIG_BOOT_HASH2_HEADER_SIG_ADDRESS= 0x9100
endif
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xF700
endif
CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000

ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_OFFSET_ADDRESS    = 0x10000
CONFIG_TEE_LOADER_IMAGE_SIZE        = 0x6900
CONFIG_TEE_LOADER_AREA_SIZE         = 0x6C00
CONFIG_TEE_LOADER_SIG_ADDRESS       = 0x16900
endif
endif

ifeq ($(CONFIG_MSTAR_M7821),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xB700
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif
CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_M7221),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xF300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0xF400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0x17300
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x16000
endif

ifeq ($(CONFIG_MSTAR_M7322),y)
CHIP_ID=M7322
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_OFFSET_ADDRESS    = 0xe000
CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x17000
else
CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
endif
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xF300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0xF400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0x17300
else
ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_IMAGE_SIZE        = 0x8C00
CONFIG_TEE_LOADER_AREA_SIZE         = 0x9000
CONFIG_TEE_LOADER_SIG_ADDRESS       = 0x16F00
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x8C00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x9000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x1ff00
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
ifeq ($(CONFIG_TEE_LOADER), y)
SBOOT_SIZE = 0x20000
else
SBOOT_SIZE = 0x16000
endif
endif

ifeq ($(CONFIG_MSTAR_MOONEY),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xFB00
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif
CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MAXIM),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1A00
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1A00
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x5C00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x6000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xF300
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_M7621),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4000
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x4400
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xD700
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x16000
endif

ifeq ($(CONFIG_MSTAR_M7622),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4000
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x4400
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xD700
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x16000
endif

ifeq ($(CONFIG_MSTAR_M5621),y)
CHIP_ID=M5621
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_OFFSET_ADDRESS    = 0xe000
CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x16000
else
CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
endif
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4000
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x4400
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xD700
else
ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_IMAGE_SIZE        = 0x7C00
CONFIG_TEE_LOADER_AREA_SIZE         = 0x8000
CONFIG_TEE_LOADER_SIG_ADDRESS       = 0x15F00
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x1df00
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x16000
endif

ifeq ($(CONFIG_MSTAR_MUSTANG),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xB700
else
ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_BOOT_HASH2_HEADER_SIZE       = 0x100
CONFIG_BOOT_HASH2_HEADER_SIG_ADDRESS= 0x8100
endif
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700

ifeq ($(CONFIG_TEE_LOADER), y)
CONFIG_TEE_LOADER_OFFSET_ADDRESS    = 0x10000
CONFIG_TEE_LOADER_IMAGE_SIZE        = 0x4D00
CONFIG_TEE_LOADER_AREA_SIZE          = 0x5000
CONFIG_TEE_LOADER_SIG_ADDRESS       = 0x14D00
endif
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MACAN),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4C00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x5000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xE300
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_MAZDA),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4C00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x5000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xE300
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x6400
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x6800
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xE700
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_M3822),y)
ifeq ($(CONFIG_MSTAR_CHIP_REVISION_U01), y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0xe000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0xde00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0xe000
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0xdf00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0xe000
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x1000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x1400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x9300

CONFIG_BOOT_HASH3_OFFSET_ADDRESS    = 0x9400
CONFIG_BOOT_HASH3_IMAGE_SIZE        = 0x4C00
CONFIG_BOOT_HASH3_AREA_SIZE         = 0x5000
CONFIG_BOOT_HASH3_SIG_ADDRESS       = 0xE300
else
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7c00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0x15f00
endif

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x16000
else

ifeq ($(CONFIG_HASH0_PATH), y)
HASH0_BIN       = secure/hash/$(CHIP_FOLDER)/$(CONFIG_HASH0_PATH_STRING)/hash0_u02.bin
else
HASH0_BIN       = secure/hash/$(CHIP_FOLDER)/hash0_u02.bin
endif

CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x2000

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x5e00
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x5f00
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x6000

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x6000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x7e00
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xdf00
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x8000

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif
endif


ifeq ($(CONFIG_MSTAR_MUNICH),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xA300

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_CELTICS),y)
CONFIG_BOOT_HASH0_IMAGE_SIZE        = 0x3800

CONFIG_BOOT_HASH1_OFFSET_ADDRESS    = 0x0000
CONFIG_BOOT_HASH1_IMAGE_SIZE        = 0x3600
CONFIG_BOOT_HASH1_AREA_SIZE         = 0x3800
CONFIG_BOOT_HASH1_SIG_ADDRESS       = 0x3700

CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS= 0x3800
CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_MIU_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS   = 0x5300

CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS= 0x5400
CONFIG_BOOT_HASH1_STR_IMAGE_SIZE    = 0x1800
CONFIG_BOOT_HASH1_STR_AREA_SIZE     = 0x1C00
CONFIG_BOOT_HASH1_STR_SIG_ADDRESS   = 0x6F00

CONFIG_BOOT_HASH2_OFFSET_ADDRESS    = 0x8000
CONFIG_BOOT_HASH2_IMAGE_SIZE        = 0x2000
CONFIG_BOOT_HASH2_AREA_SIZE         = 0x2400
CONFIG_BOOT_HASH2_SIG_ADDRESS       = 0xA300

CONFIG_BOOT_SIG_IMAGE_SIZE          = 0x0100
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_KRATOS),y)
SBOOT_SIZE = 0x10000
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	CONFIG_ROM_BOOTING_ENABLE = yes
	TEELOADER_BOOTMODE = bfe
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH), y)
	CONFIG_ROM_BOOTING_ENABLE = yes
	TEELOADER_BOOTMODE = bfn
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_SPI_NOR_FLASH), y)
	CONFIG_ROM_BOOTING_ENABLE = yes
	TEELOADER_BOOTMODE = bfs
else
	CONFIG_ROM_BOOTING_ENABLE = no
	TEELOADER_BOOTMODE =
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
HASH1_CODE_SIZE = 0x2400
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
HASH2_CODE_SIZE = $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
HASH3_CODE_SIZE = $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
HASH3_SIZE = $(CONFIG_BOOT_HASH3_IMAGE_SIZE)+0x200
else
  ifeq ($(CONFIG_BOOTRAM_STORE_IN_MBOOT), y)
    HASH2_CODE_SIZE = $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
    HASH2_SIZE = $(CONFIG_BOOT_HASH2_IMAGE_SIZE)+0x200
  else
    HASH2_CODE_SIZE = 0x8000
  endif
endif
endif

STATIC_SBOOT_PATH=../u-boot-2011.06/MstarCustomer/$(CUSTOMER)/sboot/bin/sboot
PM_SBOOT_PATH=../u-boot-2011.06/MstarCustomer/$(CUSTOMER)/sboot/bin/pm

ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_CPU), y)
	ifeq ($(CONFIG_MSTAR_STR_ENABLE), y)
		PM_BIN	   	= $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_cpu/STR_PM.bin
	else
		PM_BIN	    = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_cpu/PM.bin
	endif
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH), y)
	ifeq ($(CONFIG_MSTAR_STR_ENABLE), y)
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/STR_PM.bin
	else
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/PM.bin
	endif
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	ifeq ($(CONFIG_MSTAR_STR_ENABLE), y)
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/STR_PM.bin
	else
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/PM.bin
	endif
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_SPI_NOR_FLASH), y)
	ifeq ($(CONFIG_MSTAR_STR_ENABLE), y)
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/STR_PM.bin
	else
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/PM.bin
	endif
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_FOR_BUILD_PM_BIN), y)
	ifeq ($(CONFIG_MSTAR_STR_ENABLE), y)
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/STR_PM.bin
	else
	    PM_BIN      = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/rom_with_cpu/PM.bin
	endif
else ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51), y)
		PM_BIN		= $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/PM.bin
else ifeq ($(CONFIG_BOOTING_FROM_OTP_WITH_PM51), y)
		PM_BIN		= $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/otp_with_51/PM.bin
else
		error "Unknown Booting mode!!"
endif
ifeq ($(CONFIG_MSTAR_RT_PM_IN_SPI),y)
    RT_PM_BIN           = $(PM_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_cpu/RT_PM.bin
endif
AUTOCONF_H  = include/autoconf.h
UBOOT_MODULE_CONFIG_H = $(O)/include/configs/uboot_module_config.h
UBOOT_BOARD_CONFIG_H = $(O)/include/configs/uboot_board_config.h
UBOOT_PANEL_CONFIG_H = $(O)/include/configs/uboot_panel_config.h


ifdef CONFIG_STATIC_SBOOT
static-sboot:
		$(Q) echo "== STATIC SBOOT =="
ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_CPU), y)
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_cpu/$(BOARD_NAME)/sboot.* ./$(O)/out
else ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51),y)
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/sboot.* ./$(O)/out
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH),y)
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_emmc_boot.bin ./$(O)/out
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_emmc_boot.bin ./$(O)/out
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH),y)
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_emmc_boot.bin ./$(O)/out
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_emmc_boot.bin ./$(O)/out
else ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_SPI_NOR_FLASH),y)
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_spi_nor_boot.bin ./$(O)/out
	$(Q)cp $(STATIC_SBOOT_PATH)/$(CHIP_FOLDER)/ext_spi_with_51/$(BOARD_NAME)/rom_spi_nor_boot.bin ./$(O)/out
else
        $(Q)echo "wait to implement!!"
endif
endif

sboot-all  := $(core-y) $(libs-y)

# Rule to link sboot - also used during CONFIG_KALLSYMS
# May be overridden by arch/$(ARCH)/Makefile

ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_CPU),y)
  ifeq ($(findstring $(CONFIG_MSTAR_MACAN)$(CONFIG_MSTAR_M3822)$(CONFIG_MSTAR_MAZDA), y),y)
  TRYLINK = trylink_armgcc
  else
  TRYLINK = trylink
  endif
else
  TRYLINK = trylink
endif

quiet_cmd_sboot__ ?= LINK    $@
	cmd_sboot__ ?= $(srctree)/scripts/$(TRYLINK) \
	"$@" \
	"$(CC)" \
	"$(CFLAGS) $(CFLAGS_sboot)" \
	"$(LDFLAGS) $(EXTRA_LDFLAGS)" \
	"$(core-y)" \
	"$(libs-y)" \
	"$(LDLIBS)" \
	"$(O)"

$(O)/out:
ifeq ($(CONFIG_TEE_LOADER), y)
ifneq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
ifneq ($(CONFIG_CUSCID), 0x00)
ifeq ($(CONFIG_ENCRYPTED_TEE_LOADER), y)
ifneq ("$(wildcard src/$(CHIP_FOLDER)/HWKey2Bin.ini)","")
	$(Q)echo "CID: $(shell printf "%.2x" $(CONFIG_CUSCID))"
	$(Q)sh /tools/script/ck/HWKey2bin.sh src/$(CHIP_FOLDER)/HWKey2Bin.ini $(CHIP_ID) $(shell printf "%.2x" $(CONFIG_CUSCID)) 0
else
	error "Not support HWKey2bin"
endif
	$(Q)python src/tee_loader/utility/BintoDat.py src/$(CHIP_FOLDER)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX).cusid src/$(CHIP_FOLDER)/tee_loader.dat
	$(Q)python src/tee_loader/utility/BintoDat.py src/$(CHIP_FOLDER)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin.sig.bin$(TEELOADER_SUFFIX).cusid src/$(CHIP_FOLDER)/tee_loader_sig.dat
else
	error "CONFIG_CUSCID must be combined with CONFIG_ENCRYPTED_TEE_LOADER"
endif
else
	$(Q)python src/tee_loader/utility/BintoDat.py src/$(CHIP_FOLDER)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX) src/$(CHIP_FOLDER)/tee_loader.dat
	$(Q)python src/tee_loader/utility/BintoDat.py src/$(CHIP_FOLDER)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin.sig.bin$(TEELOADER_SUFFIX) src/$(CHIP_FOLDER)/tee_loader_sig.dat
endif
else
ifeq ($(CONFIG_R2TEE_TEELOADER), y)
	$(Q)python src/tee_loader/utility/BintoDat.py $(RSA_IMAGE_PUBLIC_KEY_BIN) TL_A_TI_KEY.dat
else
	$(Q)python src/tee_loader/utility/BintoDat.py $(TEE_RSA_PUBLIC_KEY_BIN) TL_A_TI_KEY.dat
endif
	$(Q)python src/tee_loader/utility/BintoDat.py $(EMBEDDED_REE_RSA_PUBLIC_KEY_BIN) TL_A_REE_KEY.dat
	$(Q)python src/tee_loader/utility/BintoDat.py $(TEE_AES_KEY) TL_D_TI_KEY.dat
	$(Q)python src/tee_loader/utility/BintoDat.py $(EMBEDDED_REE_AESKEY) TL_D_REE_KEY.dat
endif
endif
	$(Q)[ -d @ ] || mkdir -p $@

$(O)/u-boot.bin:
	$(Q)$(CC) -v 2>/dev/null
	$(Q)echo "  GEN     uboot_module_config.h"
	$(Q)mkdir -p $(O)/include/configs
	$(Q)echo "/* Automatically generated module config: don't edit */" > $(UBOOT_MODULE_CONFIG_H)
	$(Q)$(AWK) -f scripts/uboot-module-config.awk $(AUTOCONF_H) >> $(UBOOT_MODULE_CONFIG_H)
	$(Q)echo "  GEN     uboot_board_config.h"
	$(Q)echo "/* Automatically generated board config: don't edit */" > $(UBOOT_BOARD_CONFIG_H)
	$(Q)$(AWK) -f scripts/uboot-board-config.awk $(AUTOCONF_H) >> $(UBOOT_BOARD_CONFIG_H)
ifneq ($(CONFIG_MSTAR_MANUAL), y)
ifeq ($(CONFIG_CPU_216MHZ), y)
	$(Q)echo "#define CPU_CLOCK 216000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_240MHZ), y)
	$(Q)echo "#define CPU_CLOCK 240000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_264MHZ), y)
	$(Q)echo "#define CPU_CLOCK 264000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_288MHZ), y)
	$(Q)echo "#define CPU_CLOCK 288000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_312MHZ), y)
	$(Q)echo "#define CPU_CLOCK 312000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_336MHZ), y)
	$(Q)echo "#define CPU_CLOCK 336000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_360MHZ), y)
	$(Q)echo "#define CPU_CLOCK 360000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_384MHZ), y)
	$(Q)echo "#define CPU_CLOCK 384000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_408MHZ), y)
	$(Q)echo "#define CPU_CLOCK 408000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_432MHZ), y)
	$(Q)echo "#define CPU_CLOCK 432000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_456MHZ), y)
	$(Q)echo "#define CPU_CLOCK 456000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_480MHZ), y)
	$(Q)echo "#define CPU_CLOCK 480000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_504MHZ), y)
	$(Q)echo "#define CPU_CLOCK 504000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_528MHZ), y)
	$(Q)echo "#define CPU_CLOCK 528000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_552MHZ), y)
	$(Q)echo "#define CPU_CLOCK 552000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_576MHZ), y)
	$(Q)echo "#define CPU_CLOCK 576000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_600MHZ), y)
	$(Q)echo "#define CPU_CLOCK 600000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_624MHZ), y)
	$(Q)echo "#define CPU_CLOCK 624000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_672MHZ), y)
	$(Q)echo "#define CPU_CLOCK 672000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_684MHZ), y)
	$(Q)echo "#define CPU_CLOCK 684000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_696MHZ), y)
	$(Q)echo "#define CPU_CLOCK 696000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_708MHZ), y)
	$(Q)echo "#define CPU_CLOCK 708000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_720MHZ), y)
	$(Q)echo "#define CPU_CLOCK 720000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_756MHZ), y)
	$(Q)echo "#define CPU_CLOCK 756000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_780MHZ), y)
	$(Q)echo "#define CPU_CLOCK 780000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_792MHZ), y)
	$(Q)echo "#define CPU_CLOCK 792000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_800MHZ), y)
	$(Q)echo "#define CPU_CLOCK 800000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_804MHZ), y)
	$(Q)echo "#define CPU_CLOCK 804000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_816MHZ), y)
	$(Q)echo "#define CPU_CLOCK 816000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_828MHZ), y)
	$(Q)echo "#define CPU_CLOCK 828000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_840MHZ), y)
	$(Q)echo "#define CPU_CLOCK 840000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_850MHZ), y)
	$(Q)echo "#define CPU_CLOCK 850000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_852MHZ), y)
	$(Q)echo "#define CPU_CLOCK 852000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_864MHZ), y)
	$(Q)echo "#define CPU_CLOCK 864000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_876MHZ), y)
	$(Q)echo "#define CPU_CLOCK 876000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_888MHZ), y)
	$(Q)echo "#define CPU_CLOCK 888000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_900MHZ), y)
	$(Q)echo "#define CPU_CLOCK 900000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_936MHZ), y)
	$(Q)echo "#define CPU_CLOCK 936000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_948MHZ), y)
	$(Q)echo "#define CPU_CLOCK 948000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_950MHZ), y)
	$(Q)echo "#define CPU_CLOCK 950000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_984MHZ), y)
	$(Q)echo "#define CPU_CLOCK 984000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1008MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1008000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1044MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1044000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1050MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1050000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1100MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1100000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1152MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1152000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1400MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1400000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1450MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1450000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
ifeq ($(CONFIG_CPU_1700MHZ), y)
	$(Q)echo "#define CPU_CLOCK 1700000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
else
ifeq ($(CONFIG_MSTAR_MASERATI),y)
	$(Q)echo "#define CPU_CLOCK $(CONFIG_MSTAR_OTHERS_LIT)000000UL" >> $(UBOOT_BOARD_CONFIG_H)
else ifeq ($(CONFIG_MSTAR_M7821),y)
	$(Q)echo "#define CPU_CLOCK $(CONFIG_MSTAR_OTHERS_LIT)000000UL" >> $(UBOOT_BOARD_CONFIG_H)
else ifeq ($(CONFIG_MSTAR_M7221),y)
	$(Q)echo "#define CPU_CLOCK $(CONFIG_MSTAR_OTHERS_LIT)000000UL" >> $(UBOOT_BOARD_CONFIG_H)
else
	$(Q)echo "#define CPU_CLOCK $(CONFIG_MSTAR_OTHERS)000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif
endif

ifeq ($(CONFIG_MSTAR_GPU_MANUAL), y)
	$(Q)echo "#define GPU_CLOCK $(CONFIG_MSTAR_GPU_OTHERS)000000UL" >> $(UBOOT_BOARD_CONFIG_H)
endif

ifeq ($(CONFIG_EARLY_DISABLE_UART), y)
	$(Q)echo "#define EARLY_DISABLE_UART 1" >> $(UBOOT_BOARD_CONFIG_H)
else
	$(Q)echo "#define EARLY_DISABLE_UART 0" >> $(UBOOT_BOARD_CONFIG_H)
endif
	$(Q)echo "  GEN     uboot_panel_config.h"
	$(Q)echo "/* Automatically generated panel type config: don't edit */" > $(UBOOT_PANEL_CONFIG_H)
	$(Q)$(AWK) -f scripts/uboot-panel-config.awk $(AUTOCONF_H) >> $(UBOOT_PANEL_CONFIG_H)
	$(Q)$(AWK) -f scripts/uboot-chip-config.awk $(AUTOCONF_H)

ifeq ($(CONFIG_UBOOT_AS_2ND_LOADER), y)
$(UBOOT_BIN): $(O)/u-boot.bin
else
$(UBOOT_BIN): $(subst ", ,$(CONFIG_APP_AS_2ND_LOADER_BIN_PATH))
endif
	$(Q)echo "  COPY    $@"
	$(Q)cp $< $@
ifeq ($(CONFIG_COMPRESSED_UBOOT), y)
	$(Q)echo "  COMPRESS $(CONFIG_APP_AS_2ND_LOADER_BIN_PATH)   "
	$(Q)mv $@ $@.nocompress
ifeq ($(CONFIG_MSCOMPRESS7),y)
	$(Q)$(TOOLDIR)/mscompress7 e 0 $< $@
else
	$(Q)$(TOOLDIR)/mscompress -c -u 0 $< > $@
endif
endif

ifeq ($(CONFIG_MTK_LOADER),y)
	$(Q)echo "  COPY MTK LOADER.."
	$(Q)echo "  COPY $(MTK_LOADER_BIN)"
	$(Q)cp ../$(PROJ)/bin/mtk_loader/$(CHIP_FOLDER)/bootloader.bin $(MTK_LOADER_BIN)
endif

ifeq ($(CONFIG_UBOOT_AS_2ND_LOADER), y)
$(UBOOT_MAP): $(O)/u-boot.map
else
$(UBOOT_MAP): $(subst ", ,$(CONFIG_APP_AS_2ND_LOADER_MAP_PATH))
endif
	$(Q)echo "  COPY    $@"
	$(Q)cp $< $@

ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	@make OneBIN
	@echo "RUN OneBIN"
endif

ifeq ($(CONFIG_NON_OS), y)
$(BOOT1_INC):
	$(Q)echo "  GEN     $@"
	$(Q)echo "#This file is auto created. Do not modify!" > $(BOOT1_INC)
ifneq ($(CONFIG_MSTAR_MANUAL), y)
ifeq ($(CONFIG_CPU_216MHZ), y)
	$(Q)echo "CPU_CLOCK = 216" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_240MHZ), y)
	$(Q)echo "CPU_CLOCK = 240" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_264MHZ), y)
	$(Q)echo "CPU_CLOCK = 264" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_288MHZ), y)
	$(Q)echo "CPU_CLOCK = 288" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_312MHZ), y)
	$(Q)echo "CPU_CLOCK = 312" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_336MHZ), y)
	$(Q)echo "CPU_CLOCK = 336" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_360MHZ), y)
	$(Q)echo "CPU_CLOCK = 360" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_384MHZ), y)
	$(Q)echo "CPU_CLOCK = 384" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_408MHZ), y)
	$(Q)echo "CPU_CLOCK = 408" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_432MHZ), y)
	$(Q)echo "CPU_CLOCK = 432" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_456MHZ), y)
	$(Q)echo "CPU_CLOCK = 456" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_480MHZ), y)
	$(Q)echo "CPU_CLOCK = 480" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_504MHZ), y)
	$(Q)echo "CPU_CLOCK = 504" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_528MHZ), y)
	$(Q)echo "CPU_CLOCK = 528" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_552MHZ), y)
	$(Q)echo "CPU_CLOCK = 552" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_576MHZ), y)
	$(Q)echo "CPU_CLOCK = 576" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_600MHZ), y)
	$(Q)echo "CPU_CLOCK = 600" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_624MHZ), y)
	$(Q)echo "CPU_CLOCK = 624" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_672MHZ), y)
	$(Q)echo "CPU_CLOCK = 672" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_684MHZ), y)
	$(Q)echo "CPU_CLOCK = 684" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_696MHZ), y)
	$(Q)echo "CPU_CLOCK = 696" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_708MHZ), y)
	$(Q)echo "CPU_CLOCK = 708" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_720MHZ), y)
	$(Q)echo "CPU_CLOCK = 720" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_756MHZ), y)
	$(Q)echo "CPU_CLOCK = 756" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_780MHZ), y)
	$(Q)echo "CPU_CLOCK = 780" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_792MHZ), y)
	$(Q)echo "CPU_CLOCK = 792" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_800MHZ), y)
	$(Q)echo "CPU_CLOCK = 800" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_804MHZ), y)
	$(Q)echo "CPU_CLOCK = 804" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_816MHZ), y)
	$(Q)echo "CPU_CLOCK = 816" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_828MHZ), y)
	$(Q)echo "CPU_CLOCK = 828" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_840MHZ), y)
	$(Q)echo "CPU_CLOCK = 840" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_850MHZ), y)
	$(Q)echo "CPU_CLOCK = 850" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_852MHZ), y)
	$(Q)echo "CPU_CLOCK = 852" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_864MHZ), y)
	$(Q)echo "CPU_CLOCK = 864" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_876MHZ), y)
	$(Q)echo "CPU_CLOCK = 876" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_888MHZ), y)
	$(Q)echo "CPU_CLOCK = 888" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_900MHZ), y)
	$(Q)echo "CPU_CLOCK = 900" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_936MHZ), y)
	$(Q)echo "CPU_CLOCK = 936" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_948MHZ), y)
	$(Q)echo "CPU_CLOCK = 948" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_950MHZ), y)
	$(Q)echo "CPU_CLOCK = 950" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_984MHZ), y)
	$(Q)echo "CPU_CLOCK = 984" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1008MHZ), y)
	$(Q)echo "CPU_CLOCK = 1008" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1044MHZ), y)
	$(Q)echo "CPU_CLOCK = 1044" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1050MHZ), y)
	$(Q)echo "CPU_CLOCK = 1050" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1100MHZ), y)
	$(Q)echo "CPU_CLOCK = 1100" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1152MHZ), y)
	$(Q)echo "CPU_CLOCK = 1152" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1400MHZ), y)
	$(Q)echo "CPU_CLOCK = 1400" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1450MHZ), y)
	$(Q)echo "CPU_CLOCK = 1450" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1700MHZ), y)
	$(Q)echo "CPU_CLOCK = 1700" >> $(BOOT1_INC)
endif
else
ifeq ($(CONFIG_MSTAR_MASERATI),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else ifeq ($(CONFIG_MSTAR_M7821),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else ifeq ($(CONFIG_MSTAR_M7221),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else
	$(Q)echo "CPU_CLOCK = $(CONFIG_MSTAR_OTHERS)" >> $(BOOT1_INC)
endif
endif

ifeq ($(CONFIG_SBOOT_TO_KERNEL), y)
	$(Q)$(AWK) -f scripts/find-kernel-entry.awk $(SYSTEM_MAP) >> $(BOOT1_INC)
	$(Q)echo "SBOOT_TO_KERNEL = 1" >> $(BOOT1_INC)
	$(Q)printf "SIZE_KERNEL = 0x%08X\n" `ls -Gl $(KERNEL_BIN) | $(AWK) '{ print $$4 }'` >> $(BOOT1_INC)
else
	$(Q)echo "SBOOT_TO_KERNEL = 0" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_MSTAR_TITANIA_BD_FPGA), y)
	$(Q)echo "FPGA_BOARD = 1" >> $(BOOT1_INC)
else
	$(Q)echo "FPGA_BOARD = 0" >> $(BOOT1_INC)
endif
else
######################################################
# Mboot related Makefile rules
######################################################

# mboot defination
include mboot.mk

# create chunk header
include Makefile.chunkheader

# create mboot image (cat image together)
include Makefile.mboot

# create customer key bank
include Makefile.keybank

PHONY += gen_upgrade_loader
gen_upgrade_loader:
ifeq ($(CONFIG_USB_DISK_UPGRADE_LOADER),y)
	python scripts/upgrade_loader/gen_tv_loader.py TV_CHIP disk ../../$(HASH0_BIN) ../../secure ../../out/
else ifeq ($(CONFIG_USB_PC_UPGRADE_LOADER),y)
	python scripts/upgrade_loader/gen_tv_loader.py TV_CHIP pc ../../$(HASH0_BIN) ../../secure ../../out/
endif

ifeq ($(CONFIG_ONEBIN_ENABLE),y)
OneBIN	:
#M7221/M7322 add new flow========
ifeq ($(findstring $(CONFIG_MSTAR_M7221)$(CONFIG_MSTAR_M7322), y),y)
ifeq ($(CONFIG_TEE_LOADER), y)
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	@sh Onebin_v2.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)+$(CONFIG_TEE_LOADER_AREA_SIZE)))) AUTHEN_ENABLE $(CHIP_FOLDER)
else
	@sh Onebin_v2.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)+$(CONFIG_TEE_LOADER_AREA_SIZE)))) AUTHEN_DISABLE $(CHIP_FOLDER)
endif
else
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	@sh Onebin_v2.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)))) AUTHEN_ENABLE $(CHIP_FOLDER)
else
	@sh Onebin_v2.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)))) AUTHEN_DISABLE $(CHIP_FOLDER)
endif
endif
#Maserati and Maxim
else
ifeq ($(CONFIG_TEE_LOADER), y)
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	@sh Onebin.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)+$(CONFIG_TEE_LOADER_AREA_SIZE)))) AUTHEN_ENABLE $(CHIP_FOLDER)
else
	@sh Onebin.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)+$(CONFIG_TEE_LOADER_AREA_SIZE)))) AUTHEN_DISABLE $(CHIP_FOLDER)
endif
else
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	@sh Onebin.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)))) AUTHEN_ENABLE $(CHIP_FOLDER)
else
	@sh Onebin.sh $(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE)))) AUTHEN_DISABLE $(CHIP_FOLDER)
endif
endif
endif
endif

$(BOOT1_INC): $(UBOOT_BIN) $(UBOOT_MAP) $(SECURITY_BIN)
	$(TOOLDIR)/align $(UBOOT_BIN) 10;
	$(Q)cat $(SECUREDIR)/padding.bin >> $(UBOOT_BIN)
ifeq ($(CONFIG_MSTAR_NS_UBOOT), y)
	$(Q)python src/tee_loader/utility/BintoDat.py $(EMBEDDED_TEE_RSA_PUBLIC_KEY_BIN) $(O)/out/RSAcus_pub.dat
	$(Q)python src/tee_loader/utility/BintoDat.py $(EMBEDDED_REE_RSA_PUBLIC_KEY_BIN) $(O)/out/RSAree_pub.dat
	$(Q)python src/tee_loader/utility/BintoDat.py $(EMBEDDED_REE_AESKEY) $(O)/out/AES_REE_Uniform.dat
endif
	$(Q)echo "  GEN     $@"
	$(Q)echo "#This file is auto created. Do not modify!" > $(BOOT1_INC)
ifeq ($(CHIP_FOLDER), janus)
	$(Q)$(AWK) '$$2 == "_start" { print "UBOOT_ENTRY_ADDR = "$$1 }' $(UBOOT_MAP) >> $(BOOT1_INC)
endif
ifeq ($(CHIP_FOLDER), maria10)
	$(Q)$(AWK) '$$2 == "_start" { print "UBOOT_ENTRY_ADDR = "$$1 }' $(UBOOT_MAP) >> $(BOOT1_INC)
endif
ifeq ($(CHIP_FOLDER), macaw12)
	$(Q)$(AWK) '$$2 == "_start" { print "UBOOT_ENTRY_ADDR = "$$1 }' $(UBOOT_MAP) >> $(BOOT1_INC)
endif
ifneq ($(CONFIG_MSTAR_MANUAL), y)
ifeq ($(CONFIG_CPU_216MHZ), y)
	$(Q)echo "CPU_CLOCK = 216" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_240MHZ), y)
	$(Q)echo "CPU_CLOCK = 240" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_264MHZ), y)
	$(Q)echo "CPU_CLOCK = 264" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_288MHZ), y)
	$(Q)echo "CPU_CLOCK = 288" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_312MHZ), y)
	$(Q)echo "CPU_CLOCK = 312" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_336MHZ), y)
	$(Q)echo "CPU_CLOCK = 336" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_360MHZ), y)
	$(Q)echo "CPU_CLOCK = 360" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_384MHZ), y)
	$(Q)echo "CPU_CLOCK = 384" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_408MHZ), y)
	$(Q)echo "CPU_CLOCK = 408" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_432MHZ), y)
	$(Q)echo "CPU_CLOCK = 432" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_456MHZ), y)
	$(Q)echo "CPU_CLOCK = 456" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_480MHZ), y)
	$(Q)echo "CPU_CLOCK = 480" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_504MHZ), y)
	$(Q)echo "CPU_CLOCK = 504" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_528MHZ), y)
	$(Q)echo "CPU_CLOCK = 528" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_552MHZ), y)
	$(Q)echo "CPU_CLOCK = 552" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_576MHZ), y)
	$(Q)echo "CPU_CLOCK = 576" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_600MHZ), y)
	$(Q)echo "CPU_CLOCK = 600" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_624MHZ), y)
	$(Q)echo "CPU_CLOCK = 624" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_672MHZ), y)
	$(Q)echo "CPU_CLOCK = 672" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_684MHZ), y)
	$(Q)echo "CPU_CLOCK = 684" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_696MHZ), y)
	$(Q)echo "CPU_CLOCK = 696" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_708MHZ), y)
	$(Q)echo "CPU_CLOCK = 708" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_720MHZ), y)
	$(Q)echo "CPU_CLOCK = 720" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_756MHZ), y)
	$(Q)echo "CPU_CLOCK = 756" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_780MHZ), y)
	$(Q)echo "CPU_CLOCK = 780" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_792MHZ), y)
	$(Q)echo "CPU_CLOCK = 792" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_800MHZ), y)
	$(Q)echo "CPU_CLOCK = 800" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_804MHZ), y)
	$(Q)echo "CPU_CLOCK = 804" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_816MHZ), y)
	$(Q)echo "CPU_CLOCK = 816" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_828MHZ), y)
	$(Q)echo "CPU_CLOCK = 828" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_840MHZ), y)
	$(Q)echo "CPU_CLOCK = 840" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_850MHZ), y)
	$(Q)echo "CPU_CLOCK = 850" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_852MHZ), y)
	$(Q)echo "CPU_CLOCK = 852" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_864MHZ), y)
	$(Q)echo "CPU_CLOCK = 864" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_876MHZ), y)
	$(Q)echo "CPU_CLOCK = 876" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_888MHZ), y)
	$(Q)echo "CPU_CLOCK = 888" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_900MHZ), y)
	$(Q)echo "CPU_CLOCK = 900" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_936MHZ), y)
	$(Q)echo "CPU_CLOCK = 936" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_948MHZ), y)
	$(Q)echo "CPU_CLOCK = 948" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_950MHZ), y)
	$(Q)echo "CPU_CLOCK = 950" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_984MHZ), y)
	$(Q)echo "CPU_CLOCK = 984" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1008MHZ), y)
	$(Q)echo "CPU_CLOCK = 1008" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1044MHZ), y)
	$(Q)echo "CPU_CLOCK = 1044" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1050MHZ), y)
	$(Q)echo "CPU_CLOCK = 1050" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1100MHZ), y)
	$(Q)echo "CPU_CLOCK = 1100" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1152MHZ), y)
	$(Q)echo "CPU_CLOCK = 1152" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1400MHZ), y)
	$(Q)echo "CPU_CLOCK = 1400" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1450MHZ), y)
	$(Q)echo "CPU_CLOCK = 1450" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_CPU_1700MHZ), y)
	$(Q)echo "CPU_CLOCK = 1700" >> $(BOOT1_INC)
endif
else
ifeq ($(CONFIG_MSTAR_MASERATI),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else ifeq ($(CONFIG_MSTAR_M7821),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else ifeq ($(CONFIG_MSTAR_M7221),y)
	$(Q)echo "CPU_CLOCK_BIG = $(CONFIG_MSTAR_OTHERS_BIG)" >> $(BOOT1_INC)
	$(Q)echo "CPU_CLOCK_LIT = $(CONFIG_MSTAR_OTHERS_LIT)" >> $(BOOT1_INC)
else
	$(Q)echo "CPU_CLOCK = $(CONFIG_MSTAR_OTHERS)" >> $(BOOT1_INC)
endif
endif

ifeq ($(CONFIG_MSTAR_GPU_MANUAL), y)
	$(Q)echo "GPU_CLOCK = $(CONFIG_MSTAR_GPU_OTHERS)" >> $(BOOT1_INC)
endif


ifeq ($(CONFIG_EMMC_BOOT), y)
	$(Q)echo "EMMC_BOOT = 1" >> $(BOOT1_INC)
else
	$(Q)echo "EMMC_BOOT = 0" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_SBOOT_TO_KERNEL), y)
	$(Q)$(AWK) -f scripts/find-kernel-entry.awk $(SYSTEM_MAP) >> $(BOOT1_INC)
	$(Q)echo "SBOOT_TO_KERNEL = 1" >> $(BOOT1_INC)
	$(Q)printf "SIZE_KERNEL = 0x%08X\n" `ls -Gl $(KERNEL_BIN) | $(AWK) '{ print $$4 }'` >> $(BOOT1_INC)
else
	$(Q)echo "SBOOT_TO_KERNEL = 0" >> $(BOOT1_INC)
endif
ifeq ($(CONFIG_MSTAR_TITANIA_BD_FPGA), y)
	$(Q)echo "FPGA_BOARD = 1" >> $(BOOT1_INC)
else
	$(Q)echo "FPGA_BOARD = 0" >> $(BOOT1_INC)
endif

ifeq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51), y)
	$(Q)echo "BOOTING_FROM_EXT_SPI_WITH_PM51 = 1" >> $(BOOT1_INC)
else
	$(Q)echo "BOOTING_FROM_EXT_SPI_WITH_PM51 = 0" >> $(BOOT1_INC)
endif
	$(Q)printf "SIZE_UBOOT = 0x%08X\n" `ls -Gl $(UBOOT_BIN) | $(AWK) '{ print $$4 }'` >> $(BOOT1_INC)
	$(Q)echo "  GEN     out/security.inc"
	$(Q)echo "#This file is auto created. Do not modify!" > $(SECURITY_INC)
	$(Q)$(AWK) '$$2 == "ENABLE_SECURITY_BOOT" { print "ENABLE_MODULE_SECURITY_BOOT = " $$3 }' $(TOPDIR)/$(AUTOCONF_H) >> $(SECURITY_INC)
endif
ifeq ($(CONFIG_MSTAR_M7221),y)
	@ar -xv $(shell $(CC) -print-sysroot)/../lib/gcc/arm-none-eabi/`$(CC) -dumpversion`/libgcc.a _udivsi3.o _dvmd_tls.o
	@mv _udivsi3.o _dvmd_tls.o src/$(CHIP_FOLDER)
endif

ifeq ($(CONFIG_MSTAR_M7322),y)
	@ar -xv $(shell $(CC) -print-sysroot)/../lib/gcc/arm-none-eabi/`$(CC) -dumpversion`/libgcc.a _udivsi3.o _dvmd_tls.o
	@mkdir -p $(O)/src/$(CHIP_FOLDER)
	@cp _udivsi3.o _dvmd_tls.o src/$(CHIP_FOLDER)
	@mv _udivsi3.o _dvmd_tls.o $(O)/src/$(CHIP_FOLDER)
endif
symbolic_link:

# Generate System.map
quiet_cmd_sysmap = SYSMAP
      cmd_sysmap = $(CONFIG_SHELL) $(srctree)/scripts/mksysmap

# Link of sboot
# If CONFIG_KALLSYMS is set .version is already updated
# Generate System.map and verify that the content is consistent
# Use + in front of the sboot_version rule to silent warning with make -j2
# First command is ':' to allow us to use + in front of the rule
define rule_sboot__
	:
	$(call cmd,sboot__)
	$(Q)echo 'cmd_$@ := $(cmd_sboot__)' > $(@D)/.$(@F).cmd
endef


ifdef CONFIG_KALLSYMS
# Generate section listing all symbols and add it into sboot $(kallsyms.o)
# It's a three stage process:
# o .tmp_sboot1 has all symbols and sections, but __kallsyms is
#   empty
#   Running kallsyms on that gives us .tmp_kallsyms1.o with
#   the right size - sboot version (uname -v) is updated during this step
# o .tmp_sboot2 now has a __kallsyms section of the right size,
#   but due to the added section, some addresses have shifted.
#   From here, we generate a correct .tmp_kallsyms2.o
# o The correct .tmp_kallsyms2.o is linked into the final sboot.
# o Verify that the System.map from sboot matches the map from
#   .tmp_sboot2, just in case we did not generate kallsyms correctly.
# o If CONFIG_KALLSYMS_EXTRA_PASS is set, do an extra pass using
#   .tmp_sboot3 and .tmp_kallsyms3.o.  This is only meant as a
#   temporary bypass to allow the kernel to be built while the
#   maintainers work out what went wrong with kallsyms.

ifdef CONFIG_KALLSYMS_EXTRA_PASS
last_kallsyms := 3
else
last_kallsyms := 2
endif

kallsyms.o := .tmp_kallsyms$(last_kallsyms).o

define verify_kallsyms
	$(Q)$(if $($(quiet)cmd_sysmap),                       \
	  echo '  $($(quiet)cmd_sysmap) .tmp_System.map' &&)  \
	  $(cmd_sysmap) .tmp_sboot$(last_kallsyms) .tmp_System.map
	$(Q)cmp -s System.map .tmp_System.map ||              \
		(echo Inconsistent kallsyms data;             \
		 echo Try setting CONFIG_KALLSYMS_EXTRA_PASS; \
		 rm .tmp_kallsyms* ; /bin/false )
endef

# Update sboot version before link
# Use + in front of this rule to silent warning about make -j1
# First command is ':' to allow us to use + in front of this rule
cmd_ksym_ld = $(cmd_sboot__)
define rule_ksym_ld
	:
	+$(call cmd,sboot_version)
	$(call cmd,sboot__)
	$(Q)echo 'cmd_$@ := $(cmd_sboot__)' > $(@D)/.$(@F).cmd
endef

# Generate .S file with all kernel symbols
quiet_cmd_kallsyms = KSYM    $@
      cmd_kallsyms = $(NM) -n $< | $(KALLSYMS) \
                     $(if $(CONFIG_KALLSYMS_ALL),--all-symbols) > $@

.tmp_kallsyms1.o .tmp_kallsyms2.o .tmp_kallsyms3.o: %.o: %.S scripts FORCE
	$(call if_changed_dep,as_o_S)

.tmp_kallsyms%.S: .tmp_sboot% $(KALLSYMS)
	$(call cmd,kallsyms)

# .tmp_sboot1 must be complete except kallsyms, so update sboot version
.tmp_sboot1: $(sboot-lds) $(sboot-all) FORCE
	$(call if_changed_rule,ksym_ld)

.tmp_sboot2: $(sboot-lds) $(sboot-all) .tmp_kallsyms1.o FORCE
	$(call if_changed,sboot__)

.tmp_sboot3: $(sboot-lds) $(sboot-all) .tmp_kallsyms2.o FORCE
	$(call if_changed,sboot__)

# Needs to visit scripts/ before $(KALLSYMS) can be used.
$(KALLSYMS): scripts ;

# Generate some data for debugging strange kallsyms problems
debug_kallsyms: .tmp_map$(last_kallsyms)

.tmp_map%: .tmp_sboot% FORCE
	($(OBJDUMP) -h $< | $(AWK) '/^ +[0-9]/{print $$4 " 0 " $$2}'; $(NM) $<) | sort > $@

.tmp_map3: .tmp_map2

.tmp_map2: .tmp_map1

endif # ifdef CONFIG_KALLSYMS

# sboot image - including updated kernel symbols
ifeq ($(CONFIG_NON_OS), y)
$(O)/out/sboot.elf: symbolic_link $(O)/out $(BOOT1_INC) $(sboot-all)  FORCE
else
ifeq ($(CONFIG_BOOTING_FROM_OTP_WITH_PM51),y)
$(O)/out/sboot.elf: symbolic_link $(O)/out $(BOOT1_INC) $(sboot-all)  FORCE
else
$(O)/out/sboot.elf: symbolic_link $(O)/out $(BOOT1_INC) $(CHUNK_HEADER) $(sboot-all) FORCE
endif
endif
	$(call if_changed_rule,sboot__)
	$(Q)rm -f .old_version
	$(Q)$(OBJDUMP) -S $@ > $(O)/out/sboot.dis
	$(Q)size $@ > $(O)/out/sboot.siz

ifndef CONFIG_STATIC_SBOOT
ifeq ($(CONFIG_MSTAR_EDEN_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_MSTAR_MAYA_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_MSTAR_WHISKY_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_MSTAR_MELODY_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_MSTAR_EULER_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_MSTAR_NASA_PM),y)
$(O)/out/PM.bin: $(O)/out/sboot.elf
else ifeq ($(CONFIG_BOOTING_FROM_OTP_WITH_PM51),y)
$(O)/out/sboot.bin: symbolic_link $(O)/out/sboot.elf lint $(CHUNK_HEADER)
else
$(O)/out/sboot.bin: $(O)/out/sboot.elf lint
endif
	$(Q)$(OBJCOPY) -O binary -I elf32-little $< $@
#sboot.bin must be padded to 64KByte due to fixed nand layout with 14KB hash0
#Please note the max CONFIG_BOOT_HASH2_AREA_SIZE including hash2_sig for these cases will be 32KB(0x8000)
ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH),y)
ifeq ($(CONFIG_BOOT_HASH0_IMAGE_SIZE), 0x3800)
	$(Q)$(HOSTCC) -o $(O)/scripts/pad_version ./scripts/pad_version.c
	$(Q)$(O)/scripts/pad_version $@ $(MBOOT_CL)
endif
endif
else
$(O)/out/sboot.bin: $(O)/out $(BOOT1_INC) $(CHUNK_HEADER) static-sboot
endif

ifeq ($(CONFIG_ROM_BOOTING_ENABLE), yes)

ifeq ($(CONFIG_MSTAR_MILAN),y)
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
endif
endif

ifeq ($(CONFIG_MSTAR_M5321),y)
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
endif
endif

ifeq ($(CONFIG_MSTAR_MARLON),y)
ifeq ($(CONFIG_MSTAR_AUTHEN_ENABLE), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
endif
endif

ifeq ($(CONFIG_MSTAR_MACAN),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_MAZDA),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_MAINZ),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_MASERATI),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_EXT_BIN) $(CONFIG_BOOT_HASH1_EXT_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_EXT_IMAGE_SIZE)
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_H_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_HEADER_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_H_BIN) $(REE_RSA_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH2_H_SIG) $(CONFIG_BOOT_HASH2_HEADER_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_HEADER_SIG_ADDRESS)
endif
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(TEELOADER_BIN) $(CONFIG_TEE_LOADER_OFFSET_ADDRESS) $(CONFIG_TEE_LOADER_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_EXT_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(REE_RSA_PRIVATE_KEY)
else
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(TEELOADER_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_EXT_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_EXT_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)scripts/append_signature.pl $(TEELOADER_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_TEE_LOADER_SIG_ADDRESS)
	$(Q)src/tee_loader/tee_loader_dat.py $(BOOT_OUTPUT_PATH)
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_M7821),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif


ifeq ($(CONFIG_MSTAR_M7221),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_MOONEY),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_MAXIM),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_M7621),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_M3822),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_SPI_NOR_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(ROM_SPI_NOR_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(ROM_SPI_NOR_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_M7622),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

	mkdir -p $(PRODUCT_OUT)/unsigned
	cp $(HASH0_BIN_AMZN) $(PRODUCT_OUT)/unsigned/hash0.bin
	cp $(SBOOT_BIN) $(PRODUCT_OUT)/unsigned/sboot.bin.unsigned

ifeq ($(CONFIG_MSTAR_M7322),y)
	$(Q)cp $(RSAUPGRADE_BIN) $(RSAEA_BIN)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_ENCRYPTED_REE_LOADER), y)
	$(Q)cp $(HASH2_BIN) $(HASH2_BIN).orig
	$(Q)$(SECUREDIR)/cbccrypt.exe 0 $(HASH2_BIN).orig $(HASH2_BIN) $(EMBEDDED_REE_AESKEY)
	$(Q)scripts/append_signature.pl $(HASH2_BIN) $(CONFIG_BOOT_HASH2_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS)
endif
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(EMBEDDED_REE_RSA_PRIVATE_KEY)
else
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(RSAEA_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(CONFIG_TEE_LOADER_OFFSET_ADDRESS) $(CONFIG_TEE_LOADER_IMAGE_SIZE)
ifeq ($(CONFIG_ENCRYPTED_TEE_LOADER), y)
	$(Q)sh /tools/script/ck/HWAescrypt.sh $(CHIP_ID) 00 0 $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX)
	$(Q)$(SECUREDIR)/rsa_sign $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)mv $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX).sig.bin $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin.sig.bin$(TEELOADER_SUFFIX)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(RSA_BOOT_PRIVATE_KEY)
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
ifeq ($(CONFIG_ONEBIN_ENABLE),y)
	KSUM=$(shell printf "0x%x" $$(($(SBOOT_SIZE)+$(CONFIG_BOOT_HASH0_IMAGE_SIZE))))
	$(Q)$(TOOLDIR)/MergeBin.exe $(ALLTBL_BIN) $(EMMC_BOOT_BIN) KSUM
endif
endif

ifeq ($(CONFIG_MSTAR_M5621),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_ENCRYPTED_REE_LOADER), y)
	$(Q)cp $(HASH2_BIN) $(HASH2_BIN).orig
	$(Q)$(SECUREDIR)/cbccrypt.exe 0 $(HASH2_BIN).orig $(HASH2_BIN) $(EMBEDDED_REE_AESKEY)
	$(Q)scripts/append_signature.pl $(HASH2_BIN) $(CONFIG_BOOT_HASH2_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS)
endif
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(EMBEDDED_REE_RSA_PRIVATE_KEY)
else
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(CONFIG_TEE_LOADER_OFFSET_ADDRESS) $(CONFIG_TEE_LOADER_IMAGE_SIZE)
ifeq ($(CONFIG_ENCRYPTED_TEE_LOADER), y)
	$(Q)sh /tools/script/ck/HWAescrypt.sh $(CHIP_ID) 00 0 $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX)
	$(Q)$(SECUREDIR)/rsa_sign $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)mv $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin$(TEELOADER_SUFFIX).sig.bin $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin.sig.bin$(TEELOADER_SUFFIX)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(BOOT_OUTPUT_PATH)/$(TEELOADER_PREFIX)teeloader_$(TEELOADER_BOOTMODE).bin $(RSA_BOOT_PRIVATE_KEY)
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_MUSTANG),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_H_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_HEADER_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_H_BIN) $(REE_RSA_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH2_H_SIG) $(CONFIG_BOOT_HASH2_HEADER_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_HEADER_SIG_ADDRESS)
endif
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH3_BIN) $(CONFIG_BOOT_HASH3_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH3_IMAGE_SIZE)
endif
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(TEELOADER_BIN) $(CONFIG_TEE_LOADER_OFFSET_ADDRESS) $(CONFIG_TEE_LOADER_IMAGE_SIZE)
endif
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
ifeq ($(CONFIG_TEE_LOADER), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(REE_RSA_PRIVATE_KEY)
else
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(TEELOADER_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
ifeq ($(CONFIG_DOUBLE_MBOOT), y)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH3_BIN) $(RSA_BOOT_PRIVATE_KEY)
endif
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	$(Q)scripts/append_signature.pl $(TEELOADER_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_TEE_LOADER_SIG_ADDRESS)
	$(Q)src/tee_loader/tee_loader_dat.py $(BOOT_OUTPUT_PATH)
endif

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_MUNICH),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

ifeq ($(CONFIG_MSTAR_CELTICS),y)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_BIN) $(CONFIG_BOOT_HASH1_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_MIU_BIN) $(CONFIG_BOOT_HASH1_MIU_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_MIU_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH1_STR_BIN) $(CONFIG_BOOT_HASH1_STR_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH1_STR_IMAGE_SIZE)
	$(Q)$(TOOLDIR)/SeparateBin.exe $(SBOOT_BIN) $(HASH2_BIN) $(CONFIG_BOOT_HASH2_OFFSET_ADDRESS) $(CONFIG_BOOT_HASH2_IMAGE_SIZE)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_STR_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH1_MIU_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)$(SECUREDIR)/rsa_sign $(HASH2_BIN) $(RSA_BOOT_PRIVATE_KEY)
	$(Q)scripts/append_signature.pl $(HASH1_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_MIU_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_MIU_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH1_STR_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH1_STR_SIG_ADDRESS)
	$(Q)scripts/append_signature.pl $(HASH2_SIG) $(CONFIG_BOOT_SIG_IMAGE_SIZE) $(SBOOT_BIN) $(CONFIG_BOOT_HASH2_SIG_ADDRESS)

ifeq ($(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH), y)
	$(Q)cp $(HASH0_BIN) $(EMMC_BOOT_BIN);
	$(Q)$(TOOLDIR)/MergeBin.exe $(SBOOT_BIN) $(EMMC_BOOT_BIN) $(CONFIG_BOOT_HASH0_IMAGE_SIZE)
endif
endif

endif

#
## ROM Layout:
#   SBOOT_BIN (boot + bootrom + bootram (nandflash+drvNAND_SLC+uart16550))
#   CHUNK_HEADER
#	UBOOT_BIN
#   DUMMY_BIN (if needed)
#	CHUNK 1, 2, ... 16
#
ifeq ($(CONFIG_SBOOT_TO_KERNEL), y)
ifeq ($(CONFIG_KERNEL_3_0_20), y)
SYSTEM_MAP =  ../../../THEALE/RedLion/3.0.20/System.map
KERNEL_BIN      = ../../../THEALE/RedLion/3.0.20/arch/arm/boot/zImage
KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/3.0.20/arch/arm/boot/zImage.lzo
else
ifeq ($(CONFIG_KERNEL_2_6_35_11), y)
SYSTEM_MAP =  ../../../THEALE/RedLion/2.6.35.11/System.map
KERNEL_BIN      = ../../../THEALE/RedLion/2.6.35.11/arch/arm/boot/zImage
KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/2.6.35.11/arch/arm/boot/zImage.lzo
else
ifeq ($(CONFIG_KERNEL_2_6_28_9), y)
SYSTEM_MAP =  ../../../THEALE/RedLion/2.6.28.9/System.map
KERNEL_BIN      = ../../../THEALE/RedLion/2.6.28.9/arch/mips/boot/vmlinux.bin
KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/2.6.28.9/arch/mips/boot/uImage.lzo
else
ifeq ($(CONFIG_KERNEL_3_10_23), y)
SYSTEM_MAP =  ../../../THEALE/RedLion/3.10.23/System.map
KERNEL_BIN      = ../../../THEALE/RedLion/3.10.23/arch/arm/boot/zImage
KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/3.10.23/arch/arm/boot/zImage.lzo
else ifeq ($(CONFIG_KERNEL_3_1_10), y)
SYSTEM_MAP =  ../../../THEALE/RedLion/3.1.10/System.map

ifeq ($(ARCH),mips)
    KERNEL_BIN = ../../../THEALE/RedLion/3.1.10/arch/mips/boot/vmlinux.bin
    KERNEL_COMPRESSED_BIN = ../../../THEALE/RedLion/3.1.10/arch/mips/boot/uImage.lzo
else ifeq ($(ARCH),arm)
    KERNEL_BIN      = ../../../THEALE/RedLion/3.1.10/arch/arm/boot/zImage
    KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/3.1.10/arch/arm/boot/zImage.lzo
else
endif

else
SYSTEM_MAP = ../../../THEALE/RedLion/2.6.32.15/kernel/System.map
KERNEL_BIN      = ../../../THEALE/RedLion/2.6.32.15/kernel/arch/mips/boot/vmlinux.bin
KERNEL_COMPRESSED_BIN      = ../../../THEALE/RedLion/2.6.32.15/kernel/arch/mips/boot/uImage.lzo
endif
endif
endif
endif

$(O)/out/sboot2knl.bin: $(O)/out/sboot.bin
ifneq ($(CONFIG_BOOTING_FROM_EXT_SPI_WITH_CPU), y)
ifneq ($(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH), y)
	cat $(PM_BIN) > $@
endif
endif
	cat $< > $@; \
	cat $(KERNEL_BIN) >> $@;
endif


sboot: sboot_unstripped
ifeq ($(SKIP_STRIP),y)
	$(Q)cp $< $@
else
	$(Q)$(STRIP) -s --remove-section=.note --remove-section=.comment \
		sboot_unstripped -o $@
# strip is confused by PIE executable and does not set exec bits
	$(Q)chmod a+x $@
endif

# The actual objects are generated when descending,
# make sure no implicit rule kicks in
$(sort $(sboot-all)): $(sboot-dirs) ;

# Handle descending into subdirectories listed in $(sboot-dirs)
# Preset locale variables to speed up the build process. Limit locale
# tweaks to this spot to avoid wrong language settings when running
# make menuconfig etc.
# Error messages still appears in the original language

PHONY += $(sboot-dirs)
$(sboot-dirs): prepare scripts
	$(Q)$(MAKE) $(build)=$@

# Build the kernel release string
# The KERNELRELEASE is stored in a file named .kernelrelease
# to be used when executing for example make install or make modules_install
#
# Take the contents of any files called localversion* and the config
# variable CONFIG_LOCALVERSION and append them to KERNELRELEASE.
# LOCALVERSION from the command line override all of this

nullstring :=
space      := $(nullstring) # end of line

___localver = $(objtree)/localversion* $(srctree)/localversion*
__localver  = $(sort $(wildcard $(___localver)))
# skip backup files (containing '~')
_localver = $(foreach f, $(__localver), $(if $(findstring ~, $(f)),,$(f)))

localver = $(subst $(space),, \
	   $(shell cat /dev/null $(_localver)) \
	   $(patsubst "%",%,$(CONFIG_LOCALVERSION)))

# If CONFIG_LOCALVERSION_AUTO is set scripts/setlocalversion is called
# and if the SCM is know a tag from the SCM is appended.
# The appended tag is determinded by the SCM used.
#
# Currently, only git is supported.
# Other SCMs can edit scripts/setlocalversion and add the appropriate
# checks as needed.
ifdef _BB_DISABLED_CONFIG_LOCALVERSION_AUTO
	_localver-auto = $(shell $(CONFIG_SHELL) \
	                  $(srctree)/scripts/setlocalversion $(srctree))
	localver-auto  = $(LOCALVERSION)$(_localver-auto)
endif

localver-full = $(localver)$(localver-auto)

# Store (new) KERNELRELASE string in .kernelrelease
kernelrelease = $(KERNELVERSION)$(localver-full)
.kernelrelease: FORCE
	$(Q)rm -f $@
	$(Q)echo $(kernelrelease) > $(O)/$@


# Things we need to do before we recursively start building the kernel
# or the modules are listed in "prepare".
# A multi level approach is used. prepareN is processed before prepareN-1.
# archprepare is used in arch Makefiles and when processed asm symlink,
# version.h and scripts_basic is processed / created.

# Listed in dependency order
PHONY += prepare archprepare prepare0 prepare1 prepare2 prepare3

# prepare-all is deprecated, use prepare as valid replacement
PHONY += prepare-all

# prepare3 is used to check if we are building in a separate output directory,
# and if so do:
# 1) Check that make has not been executed in the kernel src $(srctree)
# 2) Create the include2 directory, used for the second asm symlink
prepare3: .kernelrelease
ifneq ($(KBUILD_SRC),)
	@echo '  Using $(srctree) as source for sboot'
	$(Q)if [ -f $(srctree)/.config ]; then \
		echo "  $(srctree) is not clean, please run 'make mrproper'";\
		echo "  in the '$(srctree)' directory.";\
		/bin/false; \
	fi;
	$(Q)if [ ! -d include2 ]; then mkdir -p include2; fi;
	$(Q)ln -fsn $(srctree)/include/asm-$(ARCH) include2/asm
endif

# prepare2 creates a makefile if using a separate output directory
prepare2: prepare3 outputmakefile

prepare1: prepare2 $(O)/include/config/MARKER
ifneq ($(KBUILD_MODULES),)
	$(Q)mkdir -p $(MODVERDIR)
	$(Q)rm -f $(MODVERDIR)/*
endif

archprepare: prepare1 scripts_basic

prepare0: archprepare FORCE
	$(Q)$(MAKE) $(build)=.

# All the preparing..
prepare prepare-all: prepare0

#	Leave this as default for preprocessing sboot.lds.S, which is now
#	done in arch/$(ARCH)/kernel/Makefile

export CPPFLAGS_sboot.lds += -P -C -U$(ARCH)

# 	FIXME: The asm symlink changes when $(ARCH) changes. That's
#	hard to detect, but I suppose "make mrproper" is a good idea
#	before switching between archs anyway.

#bbox# include/asm:
#bbox# 	@echo '  SYMLINK $@ -> include/asm-$(ARCH)'
#bbox# 	$(Q)if [ ! -d include ]; then mkdir -p include; fi;
#bbox# 	@ln -fsn asm-$(ARCH) $@

# 	Split autoconf.h into include/linux/config/*
quiet_cmd_gen_bbconfigopts = GEN     include/bbconfigopts.h
      cmd_gen_bbconfigopts = $(srctree)/scripts/mkconfigs > include/bbconfigopts.h
quiet_cmd_split_autoconf   = SPLIT   include/autoconf.h -> include/config/*
      cmd_split_autoconf   = $(O)/scripts/basic/split-include $(O)/include/autoconf.h $(O)/include/config
#bbox# piggybacked generation of few .h files
$(O)/include/config/MARKER: scripts/basic/split-include include/autoconf.h
	$(call cmd,split_autoconf)
	$(call cmd,gen_bbconfigopts)
	@touch $@

# Generate some files
# ---------------------------------------------------------------------------

# KERNELRELEASE can change from a few different places, meaning version.h
# needs to be updated, so this check is forced on all builds

uts_len := 64

define filechk_version.h
	if [ `echo -n "$(KERNELRELEASE)" | wc -c ` -gt $(uts_len) ]; then \
	  echo '"$(KERNELRELEASE)" exceeds $(uts_len) characters' >&2; \
	  exit 1; \
	fi; \
	(echo \#define UTS_RELEASE \"$(KERNELRELEASE)\"; \
	  echo \#define LINUX_VERSION_CODE `expr $(VERSION) \\* 65536 + $(PATCHLEVEL) \\* 256 + $(SUBLEVEL)`; \
	 echo '#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))'; \
	)
endef

# ---------------------------------------------------------------------------

PHONY += depend dep
depend dep:
	@echo '*** Warning: make $@ is unnecessary now.'

# ---------------------------------------------------------------------------
# Modules

ifdef _BB_DISABLED_CONFIG_MODULES

# 	By default, build modules as well

all: modules

#	Build modules

PHONY += modules
modules: $(sboot-dirs) $(if $(KBUILD_BUILTIN),sboot)
	@echo '  Building modules, stage 2.';
	$(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modpost


# Target to prepare building external modules
PHONY += modules_prepare
modules_prepare: prepare scripts

# Target to install modules
PHONY += modules_install
modules_install: _modinst_ _modinst_post

PHONY += _modinst_
_modinst_:
	@if [ -z "`$(DEPMOD) -V 2>/dev/null | grep module-init-tools`" ]; then \
		echo "Warning: you may need to install module-init-tools"; \
		echo "See http://www.codemonkey.org.uk/docs/post-halloween-2.6.txt";\
		sleep 1; \
	fi
	@rm -rf $(MODLIB)/kernel
	@rm -f $(MODLIB)/source
	@mkdir -p $(MODLIB)/kernel
	@ln -s $(srctree) $(MODLIB)/source
	@if [ ! $(objtree) -ef  $(MODLIB)/build ]; then \
		rm -f $(MODLIB)/build ; \
		ln -s $(objtree) $(MODLIB)/build ; \
	fi
	$(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modinst

# If System.map exists, run depmod.  This deliberately does not have a
# dependency on System.map since that would run the dependency tree on
# sboot.  This depmod is only for convenience to give the initial
# boot a modules.dep even before / is mounted read-write.  However the
# boot script depmod is the master version.
ifeq "$(strip $(INSTALL_MOD_PATH))" ""
depmod_opts	:=
else
depmod_opts	:= -b $(INSTALL_MOD_PATH) -r
endif
PHONY += _modinst_post
_modinst_post: _modinst_
	if [ -r System.map -a -x $(DEPMOD) ]; then $(DEPMOD) -ae -F System.map $(depmod_opts) $(KERNELRELEASE); fi

else # CONFIG_MODULES

# Modules not configured
# ---------------------------------------------------------------------------

modules modules_install: FORCE
	@echo
	@echo "The present sboot configuration has modules disabled."
	@echo "Type 'make config' and enable loadable module support."
	@echo "Then build a kernel with module support enabled."
	@echo
	@exit 1

endif # CONFIG_MODULES

###
# Cleaning is done on three levels.
# make clean     Delete most generated files
#                Leave enough to build external modules
# make mrproper  Delete the current configuration, and all generated files
# make distclean Remove editor backup files, patch leftover files and the like

# Directories & files removed with 'make clean'
CLEAN_DIRS  += $(MODVERDIR) _install 0_lib $(O)/out src/compat51
CLEAN_FILES +=	sboot $(O)/out/sboot.elf* sboot.links \
                System.map .kernelrelease sboot_ldscript \
                .tmp_kallsyms* .tmp_version .tmp_sboot* .tmp_System.map

# Directories & files removed with 'make mrproper'
MRPROPER_DIRS  += include/config include2
MRPROPER_FILES += .config .config.old include/asm .version .old_version \
		  include/autoconf.h \
		  include/bbconfigopts.h \
		  include/usage_compressed.h \
		  include/applet_tables.h \
		  applets/usage \
		  .kernelrelease Module.symvers tags TAGS cscope* \
		  sboot_old

# clean - Delete most, but leave enough to build external modules
#
clean: rm-dirs  := $(CLEAN_DIRS)
clean: rm-files := $(CLEAN_FILES)
clean-dirs      := $(addprefix _clean_,$(srctree) $(sboot-alldirs))

PHONY += $(clean-dirs) clean archclean
$(clean-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _clean_%,%,$@)

clean: symbolic_link archclean $(clean-dirs) cleanteeloader
ifneq ($(CONFIG_NON_OS), y)
	$(Q)make -C ../$(_PROJ)/ distclean
endif
	$(call cmd,rmdirs)
	$(call cmd,rmfiles)
	@find . $(RCS_FIND_IGNORE) \
		\( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
		-type f -print | xargs rm -f
ifeq ($(PARA),1)
	@find ../ -name "*.bdf" -exec rm -f {} \;
endif

PHONY += doc-clean
doc-clean: rm-files := docs/sboot.pod \
		  docs/sboot.html docs/sboot.1 docs/sboot.txt
doc-clean:
	$(call cmd,rmfiles)

# mrproper - Delete all generated files, including .config
#
mrproper: rm-dirs  := $(wildcard $(MRPROPER_DIRS))
mrproper: rm-files := $(wildcard $(MRPROPER_FILES))
mrproper-dirs      := $(addprefix _mrproper_,scripts)

PHONY += $(mrproper-dirs) mrproper archmrproper
$(mrproper-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _mrproper_%,%,$@)

mrproper: clean archmrproper $(mrproper-dirs)
	$(call cmd,rmdirs)
	$(call cmd,rmfiles)

# distclean
#
PHONY += distclean

distclean: mrproper cleanteeloader
	@find $(srctree) $(RCS_FIND_IGNORE) \
		\( -name '*.orig' -o -name '*.rej' -o -name '*~' \
		-o -name '*.bak' -o -name '#*#' -o -name '.*.orig' \
		-o -name '.*.rej' -o -name '*.tmp' -o -size 0 \
		-o -name '*%' -o -name '.*.cmd' -o -name 'core' \) \
		-type f -print | xargs rm -f
	$(Q)rm -f ./UConfig.in


# Packaging of the kernel to various formats
# ---------------------------------------------------------------------------
# rpm target kept for backward compatibility
package-dir	:= $(srctree)/scripts/package

%pkg: FORCE
	$(Q)$(MAKE) $(build)=$(package-dir) $@
rpm: FORCE
	$(Q)$(MAKE) $(build)=$(package-dir) $@


# Brief documentation of the typical targets used
# ---------------------------------------------------------------------------

boards := $(wildcard $(srctree)/arch/$(ARCH)/configs/*_defconfig)
boards := $(notdir $(boards))

-include $(srctree)/Makefile.help

# Documentation targets
# ---------------------------------------------------------------------------
%docs: scripts_basic FORCE
	$(Q)$(MAKE) $(build)=Documentation/DocBook $@

else # KBUILD_EXTMOD

###
# External module support.
# When building external modules the kernel used as basis is considered
# read-only, and no consistency checks are made and the make
# system is not used on the basis kernel. If updates are required
# in the basis kernel ordinary make commands (without M=...) must
# be used.
#
# The following are the only valid targets when building external
# modules.
# make M=dir clean     Delete all automatically generated files
# make M=dir modules   Make all modules in specified dir
# make M=dir	       Same as 'make M=dir modules'
# make M=dir modules_install
#                      Install the modules build in the module directory
#                      Assumes install directory is already created

# We are always building modules
KBUILD_MODULES := 1
PHONY += crmodverdir
crmodverdir:
	$(Q)mkdir -p $(MODVERDIR)
	$(Q)rm -f $(MODVERDIR)/*

PHONY += $(objtree)/Module.symvers
$(objtree)/Module.symvers:
	@test -e $(objtree)/Module.symvers || ( \
	echo; \
	echo "  WARNING: Symbol version dump $(objtree)/Module.symvers"; \
	echo "           is missing; modules will have no dependencies and modversions."; \
	echo )

module-dirs := $(addprefix _module_,$(KBUILD_EXTMOD))
PHONY += $(module-dirs) modules
$(module-dirs): crmodverdir $(objtree)/Module.symvers
	$(Q)$(MAKE) $(build)=$(patsubst _module_%,%,$@)

modules: $(module-dirs)
	@echo '  Building modules, stage 2.';
	$(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modpost

PHONY += modules_install
modules_install: _emodinst_ _emodinst_post

install-dir := $(if $(INSTALL_MOD_DIR),$(INSTALL_MOD_DIR),extra)
PHONY += _emodinst_
_emodinst_:
	$(Q)mkdir -p $(MODLIB)/$(install-dir)
	$(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modinst

# Run depmod only is we have System.map and depmod is executable
quiet_cmd_depmod = DEPMOD  $(KERNELRELEASE)
      cmd_depmod = if [ -r System.map -a -x $(DEPMOD) ]; then \
                      $(DEPMOD) -ae -F System.map             \
                      $(if $(strip $(INSTALL_MOD_PATH)),      \
		      -b $(INSTALL_MOD_PATH) -r)              \
		      $(KERNELRELEASE);                       \
                   fi

PHONY += _emodinst_post
_emodinst_post: _emodinst_
	$(call cmd,depmod)

clean-dirs := $(addprefix _clean_,$(KBUILD_EXTMOD))

PHONY += $(clean-dirs) clean
$(clean-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _clean_%,%,$@)

clean:	rm-dirs := $(MODVERDIR)
clean: $(clean-dirs) cleanteeloader
	$(call cmd,rmdirs)
	@find $(KBUILD_EXTMOD) $(RCS_FIND_IGNORE) \
		\( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
		-type f -print | xargs rm -f

help:
	@echo  '  Building external modules.'
	@echo  '  Syntax: make -C path/to/kernel/src M=$$PWD target'
	@echo  ''
	@echo  '  modules         - default target, build the module(s)'
	@echo  '  modules_install - install the module'
	@echo  '  clean           - remove generated files in module directory only'
	@echo  ''

# Dummies...
PHONY += prepare scripts
prepare: ;
scripts: ;
endif # KBUILD_EXTMOD

PHONY += str2unicode
str2unicode:
	@cd ../u-boot-2011.06/tools;./str2unicode

# Generate tags for editors
# ---------------------------------------------------------------------------

#We want __srctree to totally vanish out when KBUILD_OUTPUT is not set
#(which is the most common case IMHO) to avoid unneeded clutter in the big tags file.
#Adding $(srctree) adds about 20M on i386 to the size of the output file!

ifeq ($(src),$(obj))
__srctree =
else
__srctree = $(srctree)/
endif

ifeq ($(ALLSOURCE_ARCHS),)
ifeq ($(ARCH),um)
ALLINCLUDE_ARCHS := $(ARCH) $(SUBARCH)
else
ALLINCLUDE_ARCHS := $(ARCH)
endif
else
#Allow user to specify only ALLSOURCE_PATHS on the command line, keeping existing behaviour.
ALLINCLUDE_ARCHS := $(ALLSOURCE_ARCHS)
endif

ALLSOURCE_ARCHS := $(ARCH)

define all-sources
	( find $(__srctree) $(RCS_FIND_IGNORE) \
	       \( -name include -o -name arch \) -prune -o \
	       -name '*.[chS]' -print; \
	  for ARCH in $(ALLSOURCE_ARCHS) ; do \
	       find $(__srctree)arch/$${ARCH} $(RCS_FIND_IGNORE) \
	            -name '*.[chS]' -print; \
	  done ; \
	  find $(__srctree)security/selinux/include $(RCS_FIND_IGNORE) \
	       -name '*.[chS]' -print; \
	  find $(__srctree)include $(RCS_FIND_IGNORE) \
	       \( -name config -o -name 'asm-*' \) -prune \
	       -o -name '*.[chS]' -print; \
	  for ARCH in $(ALLINCLUDE_ARCHS) ; do \
	       find $(__srctree)include/asm-$${ARCH} $(RCS_FIND_IGNORE) \
	            -name '*.[chS]' -print; \
	  done ; \
	  find $(__srctree)include/asm-generic $(RCS_FIND_IGNORE) \
	       -name '*.[chS]' -print )
endef

quiet_cmd_cscope-file = FILELST cscope.files
      cmd_cscope-file = (echo \-k; echo \-q; $(all-sources)) > cscope.files

quiet_cmd_cscope = MAKE    cscope.out
      cmd_cscope = cscope -b

cscope: FORCE
	$(call cmd,cscope-file)
	$(call cmd,cscope)

quiet_cmd_TAGS = MAKE   $@
define cmd_TAGS
	rm -f $@; \
	ETAGSF=`etags --version | grep -i exuberant >/dev/null &&     \
                echo "-I __initdata,__exitdata,__acquires,__releases  \
                      -I EXPORT_SYMBOL,EXPORT_SYMBOL_GPL              \
                      --extra=+f --c-kinds=+px"`;                     \
                $(all-sources) | xargs etags $$ETAGSF -a
endef

TAGS: FORCE
	$(call cmd,TAGS)


quiet_cmd_tags = MAKE   $@
define cmd_tags
	rm -f $@; \
	CTAGSF=`ctags --version | grep -i exuberant >/dev/null &&     \
                echo "-I __initdata,__exitdata,__acquires,__releases  \
                      -I EXPORT_SYMBOL,EXPORT_SYMBOL_GPL              \
                      --extra=+f --c-kinds=+px"`;                     \
                $(all-sources) | xargs ctags $$CTAGSF -a
endef

tags: FORCE
	$(call cmd,tags)


# Scripts to check various things for consistency
# ---------------------------------------------------------------------------

includecheck:
	find * $(RCS_FIND_IGNORE) \
		-name '*.[hcS]' -type f -print | sort \
		| xargs $(PERL) -w scripts/checkincludes.pl

versioncheck:
	find * $(RCS_FIND_IGNORE) \
		-name '*.[hcS]' -type f -print | sort \
		| xargs $(PERL) -w scripts/checkversion.pl

namespacecheck:
	$(PERL) $(srctree)/scripts/namespace.pl

endif #ifeq ($(config-targets),1)
endif #ifeq ($(mixed-targets),1)

PHONY += checkstack
checkstack:
	$(OBJDUMP) -d sboot $$(find . -name '*.ko') | \
	$(PERL) $(src)/scripts/checkstack.pl $(ARCH)

kernelrelease:
	$(if $(wildcard .kernelrelease), $(Q)echo $(KERNELRELEASE), \
	$(error kernelrelease not valid - run 'make *config' to update it))
kernelversion:
	@echo $(KERNELVERSION)
cleanteeloader:
ifeq ($(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT), y)
	@find ../tv_tee_loader \
		\( -name '*.[oas]' -o -name '.*.cmd' -o -name '*.ld' \) \
		-type f -print | xargs rm -f
endif

# Single targets
# ---------------------------------------------------------------------------
# Single targets are compatible with:
# - build whith mixed source and output
# - build with separate output dir 'make O=...'
# - external modules
#
#  target-dir => where to store outputfile
#  build-dir  => directory in kernel source tree to use

ifeq ($(KBUILD_EXTMOD),)
        build-dir  = $(patsubst %/,%,$(dir $@))
        target-dir = $(dir $@)
else
        zap-slash=$(filter-out .,$(patsubst %/,%,$(dir $@)))
        build-dir  = $(KBUILD_EXTMOD)$(if $(zap-slash),/$(zap-slash))
        target-dir = $(if $(KBUILD_EXTMOD),$(dir $<),$(dir $@))
endif

%.s: %.c prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
%.i: %.c prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
%.o: %.c prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
%.lst: %.c prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
%.s: %.S prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
%.o: %.S prepare scripts FORCE
	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)

# Modules
/: prepare scripts FORCE
	$(Q)$(MAKE) KBUILD_MODULES=$(if $(CONFIG_MODULES),1) \
	$(build)=$(build-dir)
%/: prepare scripts FORCE
	$(Q)$(MAKE) KBUILD_MODULES=$(if $(CONFIG_MODULES),1) \
	$(build)=$(build-dir)
%.ko: prepare scripts FORCE
	$(Q)$(MAKE) KBUILD_MODULES=$(if $(CONFIG_MODULES),1)   \
	$(build)=$(build-dir) $(@:.ko=.o)
	$(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modpost

# FIXME Should go into a make.lib or something
# ===========================================================================

quiet_cmd_rmdirs = $(if $(wildcard $(rm-dirs)),CLEAN   $(wildcard $(rm-dirs)))
      cmd_rmdirs = rm -rf $(rm-dirs)

quiet_cmd_rmfiles = $(if $(wildcard $(rm-files)),CLEAN   $(wildcard $(rm-files)))
      cmd_rmfiles = rm -f $(rm-files)


a_flags = -Wp,-MD,$(depfile) $(AFLAGS) $(AFLAGS_KERNEL) \
	  $(NOSTDINC_FLAGS) $(CPPFLAGS) \
	  $(modkern_aflags) $(EXTRA_AFLAGS) $(AFLAGS_$(*F).o)

quiet_cmd_as_o_S = AS      $@
cmd_as_o_S       = $(CC) $(a_flags) -c -o $@ $<

# read all saved command lines

targets := $(wildcard $(sort $(targets)))
cmd_files := $(wildcard .*.cmd $(foreach f,$(targets),$(dir $(f)).$(notdir $(f)).cmd))

ifneq ($(cmd_files),)
  $(cmd_files): ;	# Do not try to update included dependency files
  include $(cmd_files)
endif

# Shorthand for $(Q)$(MAKE) -f scripts/Makefile.clean obj=dir
# Usage:
# $(Q)$(MAKE) $(clean)=dir
clean := -f $(if $(KBUILD_SRC),$(srctree)/)scripts/Makefile.clean obj

endif	# skip-makefile

PHONY += FORCE
FORCE:

-include $(srctree)/Makefile.custom

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)

#LINT_SRC_C +=  $(addprefix ../${PROJ}/src/, $(sort $(notdir $(wildcard src/*.c))))
#LINT_SRC_C +=  $(addprefix ../${PROJ}/src/, $(sort $(notdir $(wildcard src/*.s))))
#LINT_SRC_C +=  $(addprefix ../${PROJ}/src/$(CHIP_FOLDER)/, $(sort $(notdir $(wildcard src/$(CHIP_FOLDER)/*.c))))
#LINT_SRC_C +=  $(addprefix ../${PROJ}/src/$(CHIP_FOLDER)/, $(sort $(notdir $(wildcard src/$(CHIP_FOLDER)/*.s))))
#LINT_INC+= $(subst -I,,$(addprefix ../${PROJ}/, $(filter -I%, $(CPPFLAGS))))
LINT_SRC_C +=  $(addprefix ${CURDIR}/src/, $(sort $(notdir $(wildcard src/*.c))))
LINT_SRC_C +=  $(addprefix ${CURDIR}/src/, $(sort $(notdir $(wildcard src/*.s))))
LINT_SRC_C +=  $(addprefix ${CURDIR}/src/$(CHIP_FOLDER)/, $(sort $(notdir $(wildcard src/$(CHIP_FOLDER)/*.c))))
LINT_SRC_C +=  $(addprefix ${CURDIR}/src/$(CHIP_FOLDER)/, $(sort $(notdir $(wildcard src/$(CHIP_FOLDER)/*.s))))
LINT_INC+= $(subst -I,,$(addprefix ${CURDIR}/, $(filter -I%, $(CPPFLAGS))))
LINT_DEF+=$(subst -D,-d,$(filter -D%,$(CPPFLAGS)))
LINT_PATH=../lint
FLINT=$(LINT_PATH)/flint
lint:
ifeq ($(ENABLE_LINT),1)
	@echo "==sboot LINT===";
	@(\
	cat ../${PROJ}/include/autoconf.h |\
	grep 'ENABLE' |\
	sed -e 's/#define \([^ ]*\) "\(..*\)"/-d"\1=(\2)"/' | \
	sed -e 's/#define \([^ ]*\) \(..*\)/-d"\1=\2"/' | \
	sed -e 's/#define /-d/' |\
	sed -e /#undef/d |\
	sed -e /USE/d |\
	sed -e /__VA_ARGS__/d >../${PROJ}/include/_autoconf.h;\
	cat ../${PROJ}/include/_autoconf.h |\
	grep '=1' |\
	sed -e 's/ENABLE/CONFIG/' >../${PROJ}/include/_conf.h;\
	cat ../${PROJ}/include/_autoconf.h > ../lint/${PROJ}.lnt;\
	cat ../${PROJ}/include/_conf.h >> ../lint/${PROJ}.lnt;\
	echo |  $(CROSS_COMPILE)cpp -dM | \
	sed -e '/LONG_LONG/d' | \
	sed -e 's/#define \([^ ]*\) "\(..*\)"/-d"\1=(\2)"/' | \
	sed -e 's/#define \([^ ]*\) \(..*\)/-d"\1=\2"/' | \
	sed -e 's/#define /-d/'; \
	#echo $(CPPFLAGS); \
	for i in $(LINT_DEF); do \
		echo $$i; \
	done; \
	for i in $(LINT_INC); do \
		echo -i\"$$i\"; \
	done;\
	for i in $(LINT_SRC_C); do \
		echo $$i; \
	done; \
	) >> ../lint/${PROJ}.lnt
	@$(FLINT) -fff $(LINT_PATH)/$(PROJ).lnt > $(LINT_PATH)/$(PROJ)LINT.txt; true
	@rm ../${PROJ}/include/_conf.h;
	@rm ../${PROJ}/include/_autoconf.h;
endif
LINT_PATH=../lint
SPROJ=sboot
UPROJ=u-boot-1.1.6
LINT_RESULT:
ifeq ($(ENABLE_LINT),1)
	@echo "====MBoot lint result====";
	@echo `grep 'Error' $(LINT_PATH)/$(SPROJ)LINT.txt | wc -l` $(SPROJ) LINT Errors. Please check $(LINT_PATH)/$(SPROJ)LINT.txt
	@echo `grep 'Error' $(LINT_PATH)/$(UPROJ)LINT.txt | wc -l` $(UPROJ) LINT Errors. Please check $(LINT_PATH)/$(UPROJ)LINT.txt
endif

PROTECT_FLAG_CONFLICT_CHECK:
ifeq ($(CONFIG_SECURE_SPI_FULL_PROTECT),y)
ifeq ($(CONFIG_ENV_IN_SERIAL),y)
	@echo " ##### Error ##### " ;
	@echo " Config Conflict"
	@echo " CONFIG_ENV_IN_SERIAL and CONFIG_SECURE_SPI_PARTIAL_PROTECT : no"
	@echo " Please re-tune CONFIG_ENV or CONFIG_SECURE_SPI_PARTIAL_PROTECT and compile again"
	@exit 99;
endif
endif

MBOOT_SIZE_CHECK: $(O)/out/mboot.bin
ifeq ($(CONFIG_SECURE_SPI_PARTIAL_PROTECT),y)
	@echo " "
	@echo " ======= Check whether or not MBoot size is over protection range  $< Start	======= "
	$(Q)if [ $(shell expr $(shell printf %lu "$(CONFIG_SECURE_SPI_PROTECT_SPACE)") \>= $(shell stat -c '%s' "$<")) == 0 ]; then \
		echo " ##### Error ##### " ; \
		echo " CONFIG_SECURE_SPI_PROTECT_SPACE < $< size " ; \
		echo " CONFIG_SECURE_SPI_PROTECT_SPACE : $(shell printf %lu "$(CONFIG_SECURE_SPI_PROTECT_SPACE)")" ; \
		echo " $< : $(shell stat -c '%s' "$<")" ; \
		echo " Please re-tune CONFIG_SECURE_SPI_PROTECT_SPACE and compile again" ; \
		rm -rf "$<"; \
		exit 99; \
	else\
		echo " ##### Success ##### " ; \
	fi;
	@echo " ======= Check done $< Finished	======= "
endif
ifeq ($(CONFIG_FORCE_TO_ASSIGN_ENV_OFFSET),y)
	@echo " "
	@echo " ======= Check whether or not MBoot size overlay env address $< Start	======= "
	$(Q)if [ $(shell expr $(shell printf %lu "$(CONFIG_ENV_VAR_OFFSET)") \>= $(shell stat -c '%s' "$<")) == 0 ]; then \
		echo " ##### Error ##### " ; \
		echo " CONFIG_ENV_VAR_OFFSET < $< size " ; \
		echo " CONFIG_ENV_VAR_OFFSET : $(shell printf %lu "$(CONFIG_ENV_VAR_OFFSET)")" ; \
		echo " $< : $(shell stat -c '%s' "$<")" ; \
		echo " Please re-tune CONFIG_ENV_VAR_OFFSET and compile again" ; \
		rm -rf "$<"; \
		exit 99; \
	else\
		echo " ##### Success ##### " ; \
	fi;
	@echo " ======= Check done $< Finished	======= "
endif

log_gccver:
	@$(CC) -v > .GCCver.log 2>&1

check_build_toolver: log_gccver
	@echo         "### Start checking workspace tool chain version against $(CROSS_COMPILE) $(TOOLCHAIN_VERSION) ###"
	@if [ '$(shell grep "gcc version $(TOOLCHAIN_VERSION)" .GCCver.log)' ] ;\
		then 	echo "### Pass checking workspace tool chain version ###" ;\
			rm .GCCver.log ;\
		else 	echo "[Error] Tool chain version should be $(CROSS_COMPILE) $(TOOLCHAIN_VERSION). please check it!!" ;\
			echo "Your workspace tool chain version:" ;\
			$(CC) -v ;\
			echo "### End checking workspace tool chain version  ###" ;\
			rm .GCCver.log ;exit 99;\
	fi
