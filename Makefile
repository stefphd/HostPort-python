
#------------------------------------------------------------------#
# Makefile for compilation of C/C++ library for python using boost #
#------------------------------------------------------------------#

# OS selection
UNAME_S := $(shell uname -s)
ifeq ($(findstring MINGW64_NT, $(UNAME_S)),MINGW64_NT)
    UNAME_S := mingw64
else ifeq ($(UNAME_S), Linux)
	UNAME_S := linux
else
	$(error OS not Supported)
endif

# Target name
TARGET  	:= hostport

# Directories
SRCDIR      := ./src
INCDIR      := ./include
BUILDDIR    := ./.build

# Python
PY			:= python3
PYVER		:= 10
TESTPY		:= testing.py

# Compiler
CC 			:= g++

# Flags, Libraries and Includes
# edit these for different version of python and/or different path
CFLAGS      := -fpic
ifeq ($(UNAME_S), linux)
LIB			:= -lboost_$(PY)$(PYVER) -l$(PY).$(PYVER)
INC         := -I$(INCDIR) -I/usr/include -I/usr/include/$(PY).$(PYVER)
else
LIB			:= -lboost_$(PY)$(PYVER)-mt -l$(PY).$(PYVER)
INC         := -I$(INCDIR) -I/usr/include -I/mingw64/include/$(PY).$(PYVER)
endif

# Extensions
SRCEXT      := cpp
OBJEXT      := o
SOEXT		:= pyd

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

# Find all sources in SRCDIR
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

# One object file for each source file in BUILDDIR
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# Link objects files to shared lib
$(TARGET): $(OBJECTS)
	$(CC) -shared $(CFLAGS) -o $(TARGET).$(SOEXT) $^ $(LIB)

# Compile object files
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(LIB)

# Clean
clean:
	@$(RM) -rf $(BUILDDIR)
	@$(RM) -rf $(TARGET).$(SOEXT)

# Remake (clean + make)
remake: clean $(TARGET)

# All (make + test)
all: $(TARGET) test

# Test the library
test:
	@$(PY) $(TESTPY)

.PHONY: $(TARGET) clean remake test all