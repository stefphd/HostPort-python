
#------------------------------------------------------------------#
# Makefile for compilation of C/C++ library for python using boost #
#------------------------------------------------------------------#

# Target name
TARGET  	:= hostport

# Directories
SRCDIR      := ./src
INCDIR      := ./include
BUILDDIR    := ./.build

# Python
PY			:= python3
PYVER		:= 9
TESTPY		:= testing.py

# Compiler
CC 			:= g++

# Flags, Libraries and Includes
# edit these for different version of python and/or different path
CFLAGS      := -fpic
LIB			:= -lboost_python3$(PYVER)
INC         := -I$(INCDIR) -I/usr/include/ -I/usr/include/python3.$(PYVER)/

# Extensions
SRCEXT      := cpp
OBJEXT      := o
SOEXT       := so

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

# Find all sources in SRCDIR
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

# One object file for each source file in BUILDDIR
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# Link objects files to shared lib
$(TARGET): $(OBJECTS)
	$(CC) -shared $(CFLAGS) $(LIB) -o $(TARGET).$(SOEXT) $^

# Compile object files
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<

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