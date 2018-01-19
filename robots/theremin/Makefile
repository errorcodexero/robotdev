#
# include the validation stuff
#

include ../../makefiles/valid.mk

SUBDIRS = \
	control\
	input\
	executive\
	util\
	roborio

all:: $(SUBDIRS)

$(SUBDIRS)::
	(cd $@ ; $(MAKE) CONFIG=$(CONFIG))

.PHONY: all $(SUBDIRS)
