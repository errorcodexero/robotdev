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

all:: tests robotsw

tests::
	for program in $(SUBDIRS) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG) tests ; \
		cd .. ; \
	done

robotsw::
	for program in $(SUBDIRS) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG)  ; \
		cd .. ; \
	done


.PHONY: all $(SUBDIRS)
