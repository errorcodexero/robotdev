#
#
#

include ../../makefiles/valid.mk

all:: tests robotsw

tests::
	for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG) tests ; \
		cd .. ; \
	done

robotsw::
	for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG)  ; \
		cd .. ; \
	done


clean::
	for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG) clean ; \
		cd .. ; \
	done

deploy::
	(cd $(TOPDIR) ; make CONFIG=$(CONFIG) deploy)


.PHONY: all $(SUBDIRS)
