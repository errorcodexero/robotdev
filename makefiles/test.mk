#
# This runs the old style test code in the robot classes
#

TARGETBASE=$(basename $(TARGET))
TESTTARGETDIR=$(TARGETDIR)/tests/$(TARGETBASE)
TESTNAMES=$(basename $(TESTS))
TESTTARGETS=$(addprefix $(TESTTARGETDIR)/,$(TESTNAMES))

ifeq ($(OS),Windows_NT)
TESTTARGETNAMES=$(addsuffix .exe,$(TESTTARGETS))
COMPLEXTESTTARGETNAMES=$(addprefix $(TESTTARGETDIR)/, $(addsuffix .exe,$(COMPLEXTESTS)))
RUNPREFIX=
else
TESTTARGETNAMES=$(TESTTARGETS)
COMPLEXTESTTARGETNAMES=$(addprefix $(TESTTARGETDIR)/, $(COMPLEXTESTS))
RUNPREFIX=valgrind -v --error-exitcode=1
endif

tests: cleantests buildmsg mktestdir $(TESTTARGETNAMES) $(COMPLEXTESTTARGETNAMES) runmsg runtests

cleantests::
	@rm -rf $(TESTTARGETDIR)

buildmsg::
	@echo
	@echo "Building tests for module " $(TARGETBASE)
	@echo "===================================================="

runmsg::
	@echo
	@echo "Running tests for module " $(TARGETBASE)
	@echo "===================================================="

mktestdir:
	@mkdir -p $(TESTTARGETDIR)

runtests:
	@for program in $(TESTTARGETNAMES) $(COMPLEXTESTTARGETNAMES) ; do \
		pname=$${program##*/} ; \
		echo -n Running $$pname " ... " ; \
		$(RUNPREFIX) $$program > $(TESTTARGETDIR)//$$pname.out 2>&1 ;\
		if [ $$? -eq 0 ]; then \
			echo success ; \
		else \
			echo failure ;\
		fi ; \
	done

$(TESTTARGETNAMES):
	@echo -n Building test $(basename $(notdir $@)) " ... "
	@$(CXX) $(CXXFLAGS) -o $@ $(basename $(notdir $@)).cpp -D $(shell echo $(basename $(notdir $@)) | tr a-z A-Z)_TEST
	@echo done

$(TESTTARGETDIR)/%$(EXEEXT):
	@echo -n Building test $(basename $(notdir $@)) " ... "
	@$(CXX) $(CXXFLAGS) -o $@ $? -D $(shell echo $(basename $(notdir $@)) | tr a-z A-Z)_TEST
	@echo done
