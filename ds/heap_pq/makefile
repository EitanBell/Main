

#------ Variable definitions -------#

TARGET = $(shell basename $(PWD))
PARENT_DIR = $(shell dirname $(PWD))

#------ Paths definitions -------#

RELEASE_DIR = release
DEBUG_DIR = debug

DEBUG_FLAGS := -g
RELEASE_FLAGS := -DNDEBUG -O3

#------ Directories definitions -------#

INCLUDE_DIR := $(PARENT_DIR)/include

RELEASE_LIB_DIR := $(PARENT_DIR)/libs/$(RELEASE_DIR)
RELEASE_OBJ_DIR := $(PARENT_DIR)/obj/$(RELEASE_DIR)

DEBUG_LIB_DIR := $(PARENT_DIR)/libs/$(DEBUG_DIR)
DEBUG_OBJ_DIR := $(PARENT_DIR)/obj/$(DEBUG_DIR)

#------ Dependencies -------#

DEPS = heap
DEP_HEADERS = $(addsuffix .h,$(addprefix $(INCLUDE_DIR)/,$(DEPS)))
DEP_DEBUG_LIBS = $(addsuffix .so,$(addprefix $(DEBUG_LIB_DIR)/lib,$(DEPS)))
DEP_RELEASE_LIBS = $(addsuffix .so,$(addprefix $(RELEASE_LIB_DIR)/lib,$(DEPS)))

DEP_CREATE_DEBUG = $(addsuffix .debug_dep,$(DEPS))
DEP_CREATE_RELEASE = $(addsuffix .release_dep,$(DEPS))

#------ Compiler definitions -------#

CC = gcc
BASE_CFLAGS := -fPIC -ansi -pedantic-errors -Wall -Wextra -I$(INCLUDE_DIR)

#------ Rules -------#

all: release debug

debug: $(DEP_CREATE_DEBUG) $(DEBUG_LIB_DIR)/lib$(TARGET).so

release: $(DEP_CREATE_RELEASE) $(RELEASE_LIB_DIR)/lib$(TARGET).so

test: debug $(TARGET)_test.c $(DEBUG_LIB_DIR)/lib$(TARGET).so
	$(CC) $(BASE_CFLAGS) $(DEBUG_FLAGS) -o $@ -L$(DEBUG_LIB_DIR) $(filter-out $<,$^) $(DEP_DEBUG_LIBS)
	
%.debug_dep:
	@ cd $(PARENT_DIR)/$* && $(MAKE) debug

%.release_dep:
	@ cd $(PARENT_DIR)/$* && $(MAKE) release

$(DEBUG_LIB_DIR)/lib%.so: $(DEBUG_OBJ_DIR)/%.o
	$(CC) -shared $< -o $@ -L$(DEBUG_LIB_DIR) $(filter-out $<,$^) $(DEP_DEBUG_LIBS)

$(RELEASE_LIB_DIR)/lib%.so: $(RELEASE_OBJ_DIR)/%.o
	$(CC) -shared $< -o $@ -L$(RELEASE_LIB_DIR) $(filter-out $<,$^) $(DEP_RELEASE_LIBS)

$(DEBUG_OBJ_DIR)/%.o: %.c $(INCLUDE_DIR)/%.h $(DEP_HEADERS)
	$(CC) $(BASE_CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: %.c $(INCLUDE_DIR)/%.h $(DEP_HEADERS)
	$(CC) $(BASE_CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

clean: 
	find $(PARENT_DIR) \( -name "*$(TARGET).so" -o -name "*$(TARGET).o" \) -type f -delete
	rm -f test

.PHONY = clean release debug all test

.SECONDARY:

