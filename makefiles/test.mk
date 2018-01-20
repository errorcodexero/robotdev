#
# This runs the old style test code in the robot classes
#
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))


TARGETBASE=$(basename $(TARGET))
TESTTARGETDIR=$(TARGETDIR)/tests/$(TARGETBASE)
TESTNAMES=$(basename $(TESTS))
TESTTARGETS=$(addprefix $(TESTTARGETDIR)/,$(TESTNAMES))

ifeq ($(OS),Windows_NT)
TESTTARGETNAMES=$(addsuffix .exe,$(TESTTARGETS))
COMPLEXTESTTARGETNAMES=$(addprefix $(TESTTARGETDIR)/, $(addsuffix .exe,$(COMPLEXTESTS)))
else
TESTTARGETNAMES=$(TESTTARGETS)
COMPLEXTESTTARGETNAMES=$(addprefix $(TESTTARGETDIR)/, $(COMPLEXTESTS))
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
		$$program > $(TESTTARGETDIR)//$$pname.out 2>&1 ;\
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

