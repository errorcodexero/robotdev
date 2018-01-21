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
TARGETAPPLIBS=$(addprefix $(TARGETDIR)/,$(APPLIBS))

#
# The top level make target, makes the directory an library
#
all: mkdirs shared $(REALTARGET)

include ../../../makefiles/test.mk

ifdef COMMON
shared:
	(cd ../../common ; make CONFIG=$(CONFIG))
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
$(REALTARGET): $(OBJS) $(TARGETAPPLIBS) $(COMMONLIBSFULL)
	$(CROSSCXX) -o $@ $(OBJS) $(CXXFLAGS) $(TARGETAPPLIBS) $(ADDLIBS) $(COMMONLIBSFULL)

#
# Create the directories needed
#
mkdirs::
	-mkdir -p $(TARGETDIR)
	-mkdir -p $(OBJDIR)

#
# Rule to make object files
#
$(OBJDIR)/%.o : %.cpp
	$(CROSSCXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

#
# Deploy the software to the robot
#

ifndef TEAM_NUMBER
TEAM_NUMBER=1425
endif

deploy:: $(REALTARGET)
	ssh admin@roboRIO-$(TEAM_NUMBER)-FRC.local "rm -f /home/lvuser/FRCUserProgram"
	scp $(REALTARGET) admin@roboRIO-$(TEAM_NUMBER)-FRC.local:/home/lvuser/FRCUserProgram
	ssh admin@roboRIO-$(TEAM_NUMBER)-FRC.local ". /etc/profile.d/natinst-path.sh; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"


