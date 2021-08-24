#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

#VERBOSE = 1

SRCDIRS= src

PROJECT_NAME := VinyGo

GITREV=\"$(shell git rev-parse --short -q HEAD 2> /dev/null)\"
CPPFLAGS += -DGITREV=$(GITREV)

include $(IDF_PATH)/make/project.mk

