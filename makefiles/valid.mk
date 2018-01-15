#
# validate the build environment
#

ifndef CONFIG
$(error missing the CONFIG variable on the make command line, e.g. make CONFIG=debug)
else
ifeq ($(CONFIG), debug)
else
ifeq ($(CONFIG), release)
else
$(error CONFIG must be set to either 'debug' or 'release')
endif
endif
endif
