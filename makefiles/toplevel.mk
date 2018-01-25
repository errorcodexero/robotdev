#
#
#

include ../../makefiles/valid.mk

all:: robotsw

full:: tests robotsw

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

deploy:: robotsw
	(cd $(TOPDIR) ; make CONFIG=$(CONFIG) deploy)

deployall:: robotsw
	(cd $(TOPDIR) ; make CONFIG=$(CONFIG) deployall)


.PHONY: all $(SUBDIRS)
