#
# Setup the flags based on debug versus
#
ifndef CONFIG
$(error missing the CONFIG variable on the make command line, e.g. make CONFIG=debug)
else
ifeq ($(CONFIG), debug)
CXXFLAGS += -g -D_DEBUG
else
ifeq ($(CONFIG), release)
CXXFLAGS += -O3 -DRELEASE
else
$(error CONFIG must be set to either 'debug' or 'release')
endif
endif
endif

#
# We rely on the bash shell for pushd/popd
#
SHELL=bash

#
# The directory for external libraries
#
EXTERNALSW = ../../../external


#
# The compiler to use to build the robot code
#
ifeq ($(OS),Windows_NT)

ifndef YEAR
FRCYEAR=2018
else
FRCYEAR=$(YEAR)
endif

ifeq ($(FRCYEAR),2018)
CROSSCXX = $(EXTERNALSW)/frc/bin/arm-frc-linux-gnueabi-g++
AR = $(EXTERNALSW)/frc/bin/arm-frc-linux-gnueabi-ar
endif

ifeq ($(FRCYEAR),2017)
CROSSCXX = $(EXTERNALSW)/frc2017/bin/arm-frc-linux-gnueabi-g++
AR = $(EXTERNALSW)/frc2017/bin/arm-frc-linux-gnueabi-ar
endif


EXEEXT=.exe
else
#
# For other years, the user must make sure the
# right version of the compiler is in the path
#
CROSSCXX = arm-frc-linux-gnueabi-g++
AR = arm-frc-linux-gnueabi-ar
EXEEXT=
endif

#
# The local compiler for compiling tests
#
CXX=g++



#
# The flags to apply to the C++ compilation
#
CXXFLAGS += -std=c++14 -Wall

#
# The preprocessor flags to apply
#
CPPFLAGS =

#
# The build directory based on the CONFIG
#
TARGETDIR=../builddir/$(CONFIG)

#
# The set of directories to make in the target
#
TARGETMKDIRS=$(addprefix $(OBJDIR)/,$(MKDIRS))

#
# Target name
#
TARGETNAME=$(basename $(TARGET))

#
# The directory for the object files
#
OBJDIR=$(TARGETDIR)/obj/$(TARGETNAME)

#
# Generate the list of object files desired
#
OBJS=$(addprefix $(OBJDIR)/,$(patsubst %.cpp,%.o,$(SRC)))

#
# Include the common directories
#
ifdef COMMON
COMMONHDRSFULL=$(addprefix -I../../common/,$(COMMON))
CXXFLAGS += $(COMMONHDRSFULL)
COMMONLIBSFULL=$(addprefix ../../common/builddir/$(CONFIG)/,$(addsuffix .a,$(COMMON)))
COMMONDIRS=$(addprefix ../../common/,$(COMMON))
endif


#
# Check to see if we need the NAVX libraries
#
ifdef NAVX
ifeq ($(NAVX), true)
CXXFLAGS += -I$(EXTERNALSW)/navx$(YEAR)/include
ADDLIBS += -L$(EXTERNALSW)/navx$(YEAR)/lib -lnavx_frc_cpp
endif
endif

#
# Check to see if we need the CTRE libraries
#
ifdef CTRE
ifeq ($(CTRE), true)

ifndef YEAR
CTREYEAR=2018
else
CTREYEAR=$(YEAR)
endif

ifeq ($(CTREYEAR),2018)
CXXFLAGS += -I$(EXTERNALSW)/CTRE_FRCLibs/cpp/include
ADDLIBS += -L$(EXTERNALSW)/CTRE_FRCLibs/cpp/lib -lCTRE_Phoenix -lCTRE_PhoenixCCI
endif

ifeq ($(CTREYEAR),2017)
CXXFLAGS += -I$(EXTERNALSW)/CTRE_FRCLibs$(CTREYEAR)/cpp/include
ADDLIBS += -L$(EXTERNALSW)/CTRE_FRCLibs$(CTREYEAR)/cpp/lib -lCTRLib
endif

endif
endif

#
# Check to see if we need the WPILIB
#
ifdef WPILIB
ifeq ($(WPILIB), true)

ifndef YEAR
WPIYEAR=2018
else
WPIYEAR=$(YEAR)
endif

ifeq ($(WPIYEAR), 2018)
CXXFLAGS += -I$(EXTERNALSW)/wpilib/cpp/current/include
ADDLIBS += -L$(EXTERNALSW)/wpilib/cpp/current/reflib/linux/athena/shared
ADDLIBS += -L$(EXTERNALSW)/wpilib/common/current/lib/linux/athena/shared
ADDLIBS += -lwpi -lpthread
endif

ifeq ($(WPIYEAR), 2017)
CXXFLAGS += -I$(EXTERNALSW)/wpilib2017/cpp/current/include
ADDLIBS += -L$(EXTERNALSW)/wpilib2017/cpp/current/lib
ADDLIBS += -lwpi -lpthread
endif

endif
endif

#
# Check to see if we need the boost libraries
#
ifdef BOOST
ifeq ($(BOOST), true)
CXXFLAGS += -I$(EXTERNALSW)/boost/include
ADDLIBS += $(EXTERNALSW)/boost/lib/libboost_system.a
endif
endif

ifdef ADDCXXFLAGS
CXXFLAGS += $(ADDCXXFLAGS)
endif


#
# Rule to make object files
#
ifeq ($(VERBOSE),1)
$(OBJDIR)/%.o : %.cpp
	$(CROSSCXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
else
$(OBJDIR)/%.o : %.cpp
	@echo -n "Compiling file '"$<"' ... "
	@$(CROSSCXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
	@echo done
endif
