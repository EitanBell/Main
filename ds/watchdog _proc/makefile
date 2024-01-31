TARGET = $(shell basename $(CURDIR))
TARGET_LIB = lib$(TARGET).so
LIB_NAME = $(shell dirname $(PWD))

define newline


endef

DEP_LIST = scheduler uid
RUN_MAKEFILES := $(foreach DEP,$(DEP_LIST),$(MAKE) $(MAKECMDGOALS) -C ../$(DEP) $(newline))
OBJ_DIR = $(LIB_NAME)/obj/$(MAKECMDGOALS)
LIB_DIR = $(LIB_NAME)/libs/$(MAKECMDGOALS)
INCLUDE_DIR = $(LIB_NAME)/include/
LIB_DEBUG = $(LIB_NAME)/libs/debug/
OBJ_DEBUG = $(LIB_NAME)/obj/debug/

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)
DEP_FLAGS = $(addprefix -l,$(DEP_LIST))

SOURCES = $(TARGET).c 
OBJS = $(SOURCES:.c=.o)

.PHONY: all clean debug

all: 
	$(MAKE) debug -s 

debug: CFLAGS += -g
debug: $(TARGET_LIB) $(TARGET).out
			
$(TARGET).out: $(OBJ_DIR)/$(TARGET).o 
	$(CC) $< $(LDFLAGS) $(DEP_FLAGS) -o $@
	
$(TARGET).o: $(TARGET).c 
	$(CC) -fPIC -c $(CFLAGS) $< -o $(OBJ_DIR)/$@ 

$(TARGET_LIB): $(OBJS)
	$(RUN_MAKEFILES)
	$(CC) -shared $(OBJ_DIR)/$< $(LDFLAGS) $(DEP_FLAGS) -o $(LIB_DIR)/$@

vlg: LIB_DIR = $(LIB_DEBUG)
vlg: $(TARGET).out
	valgrind ./$(TARGET).out

gdb: 
	make debug
	cgdb ./$(TARGET).out
	
$(SOURCES:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

-include $(SOURCES:.c=.d)

clean:
	rm -f $(LIB_DEBUG)$(TARGET_LIB) \
	$(OBJ_DEBUG)$(OBJS) $(SOURCES:.c=.d) $(TARGET).out 
		

