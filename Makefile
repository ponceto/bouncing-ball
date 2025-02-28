#
# Makefile - Copyright (c) 2024-2025 - Olivier Poncet
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# ----------------------------------------------------------------------------
# global environment
# ----------------------------------------------------------------------------

TOPDIR   = $(CURDIR)
OPTLEVEL = -O2 -g
WARNINGS = -Wall
EXTRAS   = -pthread
CC       = gcc
CFLAGS   = -std=c99 $(OPTLEVEL) $(WARNINGS) $(EXTRAS)
CXX      = g++
CXXFLAGS = -std=c++14 $(OPTLEVEL) $(WARNINGS) $(EXTRAS)
CPP      = cpp
CPPFLAGS = -I. -I$(TOPDIR)/src -D_DEFAULT_SOURCE -D_FORTIFY_SOURCE=2
LD       = g++
LDFLAGS  = -L.
CP       = cp
CPFLAGS  = -f
RM       = rm
RMFLAGS  = -f

# ----------------------------------------------------------------------------
# default rules
# ----------------------------------------------------------------------------

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

# ----------------------------------------------------------------------------
# global targets
# ----------------------------------------------------------------------------

all: build

build: build_bouncing_ball
	@echo "=== $@ ok ==="

clean: clean_bouncing_ball
	@echo "=== $@ ok ==="

# ----------------------------------------------------------------------------
# bouncing_ball files
# ----------------------------------------------------------------------------

bouncing_ball_PROGRAM = bouncing-ball.bin

bouncing_ball_SOURCES = \
	src/globals.cc \
	src/program.cc \
	src/geometry.cc \
	src/canvas.cc \
	src/objects.cc \
	src/application.cc \
	src/bouncing-ball.cc \
	$(NULL)

bouncing_ball_HEADERS = \
	src/globals.h \
	src/program.h \
	src/geometry.h \
	src/canvas.h \
	src/objects.h \
	src/application.h \
	src/bouncing-ball.h \
	$(NULL)

bouncing_ball_OBJECTS = \
	src/globals.o \
	src/program.o \
	src/geometry.o \
	src/canvas.o \
	src/objects.o \
	src/application.o \
	src/bouncing-ball.o \
	$(NULL)

bouncing_ball_LDFLAGS = \
	$(NULL)

bouncing_ball_LDADD = \
	-lSDL2_image \
	-lSDL2 \
	-lpthread \
	-lm \
	$(NULL)

bouncing_ball_CLEANFILES = \
	bouncing-ball.bin \
	bouncing-ball.data \
	bouncing-ball.html \
	bouncing-ball.wasm \
	bouncing-ball.js \
	$(NULL)

# ----------------------------------------------------------------------------
# build bouncing_ball
# ----------------------------------------------------------------------------

build_bouncing_ball: $(bouncing_ball_PROGRAM)

$(bouncing_ball_PROGRAM): $(bouncing_ball_OBJECTS)
	$(LD) $(LDFLAGS) $(bouncing_ball_LDFLAGS) -o $(bouncing_ball_PROGRAM) $(bouncing_ball_OBJECTS) $(bouncing_ball_LDADD)

# ----------------------------------------------------------------------------
# clean bouncing_ball
# ----------------------------------------------------------------------------

clean_bouncing_ball:
	$(RM) $(RMFLAGS) $(bouncing_ball_OBJECTS) $(bouncing_ball_PROGRAM) $(bouncing_ball_CLEANFILES)

# ----------------------------------------------------------------------------
# End-Of-File
# ----------------------------------------------------------------------------
