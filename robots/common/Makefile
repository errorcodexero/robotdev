#
# include the validation stuff
#

include ../../makefiles/valid.mk

SUBDIRS = \
	util

all:: $(SUBDIRS)

$(SUBDIRS)::
	(cd $@ ; $(MAKE) CONFIG=$(CONFIG))

.PHONY: all $(SUBDIRS)
