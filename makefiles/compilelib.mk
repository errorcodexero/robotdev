
#
# Include the compiler stuff
#
include ../../../makefiles/compile.mk

#
# Define the full name of the real target
#
REALTARGET=$(TARGETDIR)/$(TARGET)

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
# Make the library
#
$(REALTARGET): $(OBJS)
	$(AR) r $@ $(OBJS)

#
# Create the directories needed
#
mkdirs::
	-mkdir -p $(TARGETDIR)
	-mkdir -p $(OBJDIR)
ifneq "$(TARGETMKDIRS)" ""
	-mkdir -p $(TARGETMKDIRS)
endif

#
# Rule to make object files
#
$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

