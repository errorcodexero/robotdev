#
# Include the compiler stuff
#
include ../../../makefiles/compile.mk


#
# Define the full name of the real target
#
REALTARGET=$(TARGETDIR)/$(TARGET)

#
# Define the target app libs
#
ifdef APPLIBS
TARGETAPPFILES=$(addsuffix .a,$(addprefix $(TARGETDIR)/lib,$(APPLIBS)))
TARGETAPPLIBS=-L$(TARGETDIR) $(addprefix -l,$(APPLIBS))
else
TARGETAPPFILES=
TARGETAPPLIBS=
endif


#
# The top level make target, makes the directory an library
#
all: mkdirs shared startmsg $(REALTARGET) endmsg

startmsg:
	@echo
	@echo Building application $(notdir $(REALTARGET))
	@echo $(LINE)

endmsg:
	@echo $(LINE)
	@echo
	@echo


include ../../../makefiles/test.mk

ifdef COMMON
shared:
	@for lib in $(COMMONDIRS) ; do \
		pushd $$lib ; \
		make CONFIG=$(CONFIG) ; \
		if [ $$? -ne 0 ]; then \
			break ; \
		fi ; \
		popd ; \
	done
else
shared:
	echo No shared libraries required
endif

#
# The clean target
#
clean::
	rm -f $(REALTARGET) $(OBJS)

#
# Make the application
#
ifeq ($(VERBOSE),1)
$(REALTARGET): $(OBJS) $(TARGETAPPFILES) $(COMMONLIBSFILES)
	$(COMPILER) -o $@ $(OBJS) $(LDFLAGS) $(TARGETAPPLIBS) $(COMMONLIBSFULL) $(ADDLIBS) $(USERLIBS)
else
$(info $(COMMONLIBSFULL))
$(REALTARGET): $(OBJS) $(TARGETAPPFILES) $(COMMONLIBSFILES)
	@echo -n Linking application $@ " ... "
	@$(COMPILER) -o $@ $(OBJS) $(LDFLAGS) $(TARGETAPPLIBS) $(COMMONLIBSFULL) $(ADDLIBS) $(USERLIBS)
	@echo complete
endif

#
# Create the directories needed
#
mkdirs::
	-mkdir -p $(TARGETDIR)
	-mkdir -p $(OBJDIR)
	-mkdir -p $(DEPDIR)

#
# Deploy the software to the robot
#

ifndef TEAM_NUMBER
TEAM_NUMBER=1425
endif

ifndef DEPLOYHOST
DEPLOYHOST=roboRIO-$(TEAM_NUMBER)-FRC.local
endif

deploy:: all
	@ssh lvuser@$(DEPLOYHOST) "rm -f /home/lvuser/FRCUserProgram"
	@for file in $(REALTARGET) ; do \
		scp $$file lvuser@$(DEPLOYHOST):/home/lvuser ; \
	done
	ssh lvuser@$(DEPLOYHOST) ". /etc/profile.d/natinst-path.sh; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"
	scp lvuser@$(DEPLOYHOST):/home/lvuser/params.txt params-latest.txt

$(DEPDIR)/%.d: ;

.PRECIOUS: $(DEPDIR)/%.d

-include $(addsuffix .Td,$(addprefix $(DEPDIR),$(basename $(SRC))))
