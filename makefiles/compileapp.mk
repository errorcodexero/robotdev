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

deploy:
	ssh admin@roboRIO-$team-FRC.local rm /home/lvuser/FRCUserProgram
	scp $(REALTARGET) admin@roboRIO-$team-FRC.local:/home/lvuser/FRCUserProgram
	ssh admin@roboRIO-$team-FRC.local ". /etc/profile.d/natinst-path.sh; chmod a+x /home/lvuser/FRCUserProgram; /usr/local/frc/bin/frcKillRobot.sh -t -r"

