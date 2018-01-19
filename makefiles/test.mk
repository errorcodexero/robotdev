#
# This runs the old style test code in the robot classes
#

TESTTARGETDIR=$(TARGETDIR)/tests
TESTNAMES=$(basename $(TESTS))
TESTTARGETS=$(addprefix $(TESTTARGETDIR)/,$(TESTNAMES))

tests: mktestdir $(TESTTARGETS) runtests

mktestdir:
	mkdir -p $(TESTTARGETDIR)

runtests:
	echo running tests

$(TESTTARGETS):
	g++ $(CXXFLAGS) -o $@ $(filename $@.cpp)


