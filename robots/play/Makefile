#
# include the validation stuff
#

include ../../makefiles/valid.mk

SUBDIRS = \
	xerolib\
	play

all:: $(SUBDIRS)

$(SUBDIRS)::
	(cd $@ ; $(MAKE) CONFIG=$(CONFIG))

.PHONY: all $(SUBDIRS)
