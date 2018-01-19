#
# include the validation stuff
#

include ../../makefiles/valid.mk
include ../../makefiles/doxygen.mk

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
