TARGET = $(shell basename $(CURDIR))
TARGET_LIB = lib$(TARGET).so
LIB_NAME = $(shell dirname $(PWD))

define newline


endef

DEP_LIST = reactor thread_pool logger dir_monitor async_injection
RUN_MAKEFILES := $(foreach DEP,$(DEP_LIST),$(MAKE) $(MAKECMDGOALS) -C ../$(DEP) $(newline))
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

SOURCES = $(TARGET).cpp
OBJS = $(SOURCES:.cpp=.o)
HEADER = $(SOURCES:.cpp=.hpp)

.PHONY: all clean debug release

all: 
	$(MAKE) debug -s
	make test -s
#	$(MAKE) release -s

debug: CFLAGS += -g
debug: $(TARGET_LIB) $(TARGET)
			
release: CFLAGS += -DNDEBUG -O3
release: $(TARGET_LIB)

$(TARGET): $(TARGET)_test.o 
	$(CC) $< $(LDFLAGS) -l$(TARGET) $(DEP_FLAGS) -o $@
	
$(TARGET).o: $(TARGET).cpp
	$(CC) -fPIC -c $(CFLAGS) $< -o $(OBJ_DIR)/$@ 

$(TARGET)_test.o: $(TARGET)_test.cpp
	$(CC) $(CFLAGS) -c $(TARGET)_test.cpp

$(TARGET_LIB): $(OBJS)
	$(RUN_MAKEFILES)
	$(CC) -shared $(OBJ_DIR)/$< $(LDFLAGS) $(DEP_FLAGS) -o $(LIB_DIR)/$@

test: LIB_DIR = $(LIB_DEBUG)
test: $(TARGET)
	./$(TARGET)

vlg: LIB_DIR = $(LIB_DEBUG)
vlg: $(TARGET)
	valgrind ./$(TARGET)

gdb: 
	make debug
	gdb ./$(TARGET)
	
$(SOURCES): $(INCLUDE_DIR)$(HEADER)

$(SOURCES:.cpp=.d):%.d:%.cpp
	$(CC) $(CFLAGS) -MM $< >$@

-include $(SOURCES:.cpp=.d)

clean:
	rm -f $(LIB_DEBUG)$(TARGET_LIB) $(LIB_RELEASE)$(TARGET_LIB) \
	$(OBJ_DEBUG)$(OBJS) $(OBJ_RELEASE)/$(OBJS) $(SOURCES:.cpp=.d) $(TARGET) \
	$(TARGET)_test.o
		


