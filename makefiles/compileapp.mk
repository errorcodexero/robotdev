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
all: mkdirs $(REALTARGET)

#
# The clean target
#
clean::
	rm -f $(REALTARGET) $(OBJS)

#
# Make the application
#
$(REALTARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS) $(TARGETAPPLIBS) $(ADDLIBS)

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
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

