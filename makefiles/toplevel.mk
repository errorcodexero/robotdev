#
#
#

include ../../makefiles/valid.mk

all:: tests robotsw

tests::
	@for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG) tests ; \
		if [ $$? -ne 0 ]; then \
			break ; \
		fi ; \
		cd .. ; \
	done

robotsw::
	@for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG)  ; \
		if [ $$? -ne 0 ]; then \
			break ; \
		fi ; \
		cd .. ; \
	done


clean::
	@for program in $(LIBSUBDIRS) $(TOPDIR) ; do \
		cd $$program ; \
		make CONFIG=$(CONFIG) clean ; \
		cd .. ; \
	done

deploy::
	(cd $(TOPDIR) ; make CONFIG=$(CONFIG) deploy)


.PHONY: all $(SUBDIRS)
