TARGET = $(shell basename $(PWD))

INCLUDE_PATH = ../include/
DBG_LIB_PATH = ../libs/debug
DBG_OBJ_PATH = ../obj/debug
REL_LIB_PATH = ../libs/release
REL_OBJ_PATH = ../obj/release

DEPS = semaphore 
DEPS_HEADERS = $(addprefix $(INCLUDE_PATH),$(addsuffix .hpp, $(DEPS)))
DEPS_DBG_LIBS = $(addprefix $(DBG_LIB_PATH)/lib_,$(addsuffix .so, $(DEPS)))
DEPS_REL_LIBS = $(addprefix $(REL_LIB_PATH)/lib_,$(addsuffix .so, $(DEPS)))

DEPS_DBG = $(addsuffix .deps_dbg,$(DEPS))
DEPS_REL = $(addsuffix .deps_rel,$(DEPS))

CFLAGS = -fPIC -ansi -pedantic-errors -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = -shared
DBGFLAGS = -std=c++11 -g
RELFLAGS = -DNDEBUG -O3

CC = g++
SOURCE = $(TARGET).cpp
TEST = $(TARGET)_test.cpp

all: release debug

release: CFLAGS += $(RELFLAGS)
release: $(DEPS_REL) $(REL_LIB_PATH)/lib_$(TARGET).so

debug: CFLAGS += $(DBGFLAGS)
debug: $(DEPS_DBG) $(DBG_LIB_PATH)/lib_$(TARGET).so

test: debug $(TEST) $(DBG_LIB_PATH)/lib_$(TARGET).so
	$(CC) $(CFLAGS) $(DBGFLAGS) -L$(DBG_LIB_PATH) -Wl,-rpath,. $(filter-out $<,$^) $(DEPS_DBG_LIBS) -o $(TARGET)

%.deps_dbg:
	@ cd ../$* && make debug

%.deps_rel:
	@ cd ../$* && make release

$(REL_LIB_PATH)/lib_%.so: $(REL_OBJ_PATH)/%.o
	$(CC) $(LDFLAGS) $< -L$(REL_LIB_PATH) $(filter-out $<,$^) $(DEPS_REL_LIBS) -o $@

$(DBG_LIB_PATH)/lib_%.so: $(DBG_OBJ_PATH)/%.o
	$(CC) $(LDFLAGS) $< -L$(DBG_LIB_PATH) $(filter-out $<,$^) $(DEPS_DBG_LIBS) -o $@

$(REL_OBJ_PATH)/%.o: %.cpp $(INCLUDE_PATH)/%.hpp $(DEPS_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(DBG_OBJ_PATH)/%.o: %.cpp $(INCLUDE_PATH)/%.hpp $(DEPS_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(REL_OBJ_PATH)/$(TARGET).o $(DBG_OBJ_PATH)/$(TARGET).o 
	rm -f $(REL_LIB_PATH)/lib_$(TARGET).so $(DBG_LIB_PATH)/lib_$(TARGET).so
	rm -f $(TARGET)

.PHONY: all release debug test clean

.SECONDARY:

