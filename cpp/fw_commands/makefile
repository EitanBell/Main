TARGET = $(shell basename $(CURDIR))
TARGET_LIB = lib$(TARGET)
LIB_NAME = $(shell dirname $(PWD))

define newline


endef

OBJ_DIR = $(LIB_NAME)/obj/$(MAKECMDGOALS)
LIB_DIR = $(LIB_NAME)/libs/$(MAKECMDGOALS)
INCLUDE_DIR = $(LIB_NAME)/include/
LIB_DEBUG = $(LIB_NAME)/libs/debug/
LIB_RELEASE = $(LIB_NAME)/libs/release/
OBJ_DEBUG = $(LIB_NAME)/obj/debug/
OBJ_RELEASE = $(LIB_NAME)/obj/release/

CC = g++
CFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)
DEP_FLAGS = $(addprefix -l,$(DEP_LIST))

SOURCES = $(wildcard *.cpp)
HEADER = $(SOURCES:.cpp=.hpp)
VERSION_FILE := version.txt
VERSION := $(shell cat $(VERSION_FILE))
OUTPUT_FILE := $(TARGET_LIB).$(VERSION).so

.PHONY: all clean debug release version_check

all:
	$(MAKE) debug -s

version_check:
ifeq ("$(wildcard $(VERSION_FILE))","")
	@echo 1 > $(VERSION_FILE)
else
	$(eval VERSION := $(shell cat $(VERSION_FILE)))
	$(eval NEXT_VERSION := $(shell expr $(VERSION) + 1))
	@echo $(NEXT_VERSION) > $(VERSION_FILE)
	$(eval OUTPUT_FILE := $(TARGET_LIB).$(VERSION).so)
endif

debug: CFLAGS += -g
debug: version_check $(OUTPUT_FILE)

$(OUTPUT_FILE): $(SOURCES)
	echo $(SOURCES) $(OUTPUT_FILE)
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

OUTPUT_OBJECTS = $(wildcard *.so)

clean:
	rm -f $(OUTPUT_OBJECTS) $(VERSION_FILE)
	@echo 1 > $(VERSION_FILE)