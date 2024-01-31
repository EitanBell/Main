TARGET = $(shell basename $(CURDIR))
TARGET_LIB = lib$(TARGET).so
LIB_NAME = $(shell dirname $(PWD))

define newline


endef

DEP_LIST = scheduler
INCLUDE_DIR = $(LIB_NAME)/include/
LIBS_DIRECTORY = $(LIB_NAME)/libs/
DEBUG_LIB = $(LIBS_DIRECTORY)debug/
RELEASE_LIB = $(LIBS_DIRECTORY)/release/
OBJ_DIRECTORY = $(LIB_NAME)/obj/
DEBUG_OBJ = $(OBJ_DIRECTORY)debug/
RELEASE_OBJ = $(OBJ_DIRECTORY)release/
RUN_MAKEFILES := $(foreach DEP,$(DEP_LIST),$(MAKE) $(MAKECMDGOALS) -C ../$(DEP) $(newline))
OBJ_DIR = $(OBJ_DIRECTORY)$(MAKECMDGOALS)
LIB_DIR = $(LIBS_DIRECTORY)$(MAKECMDGOALS)

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)
DEP_FLAGS = $(addprefix -l,$(DEP_LIST))

SOURCES = $(TARGET).c 
OBJS = $(SOURCES:.c=.o)
HEADER = $(SOURCES:.c=.h)

all: 
	$(MAKE) debug -s 
	$(MAKE) release -s

debug: CFLAGS += -g
debug: $(TARGET_LIB)
			
release: CFLAGS += -DNDEBUG -O3
release: $(TARGET_LIB)

$(TARGET).out: $(TARGET)_test.o 
	$(CC) $(CFLAGS) $< $(LDFLAGS) -l$(TARGET) $(DEP_FLAGS) -o $@
	
$(TARGET).o: $(TARGET).c 
	$(CC) -fPIC -c $(CFLAGS)  $(TARGET).c -o $(OBJ_DIR)/$(TARGET).o 

$(TARGET)_test.o: $(TARGET)_test.c
	$(CC) $(CFLAGS) -c $(TARGET)_test.c 

$(TARGET_LIB): $(OBJS)
	$(RUN_MAKEFILES)
	$(CC) -shared $(OBJ_DIR)/$< $(LDFLAGS) $(DEP_FLAGS) -o $(LIB_DIR)/$@

test: LIB_DIR = $(DEBUG_LIB)
test: $(TARGET).out
	./$(TARGET).out

gdb: LIB_DIR = $(DEBUG_LIB)
gdb: $(TARGET).out
	gdb ./$(TARGET).out

vlg: LIB_DIR = $(DEBUG_LIB)
vlg: $(TARGET).out
	valgrind ./$(TARGET).out


$(SOURCES): $(INCLUDE_DIR)$(HEADER)

$(SOURCES:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

-include $(SOURCES:.c=.d)

clean:
	rm -f $(DEBUG_LIB)$(TARGET_LIB) $(RELEASE_LIB)$(TARGET_LIB) \
	$(DEBUG_OBJ)$(OBJS) $(RELEASE_OBJ)/$(OBJS) $(SOURCES:.c=.d) $(TARGET).out \
	$(TARGET)_test.o
		
.PHONY: all clean debug release vlg gdb

