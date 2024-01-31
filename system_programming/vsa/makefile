CCFLAGS = -ansi -pedantic-errors -Wall -Wextra
CC = gcc
TARGET = a.out
SOURCES = vsa.c vsa_test.c test_help.c
OBJs = vsa_test.o vsa.o test_help.o
HEADER = vsa.h test_help.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(OBJs)
	$(CC) $(SOURCES) -o $(TARGET) $(CCFLAGS)

debug: CCFLAGS += -g
debug: $(TARGET)
	cgdb ./$(TARGET)
	
vlg: $(TARGET)
	valgrind ./$(TARGET)

release: CCFLAGS += -DNDEBUG -O3
release: $(TARGET)

$(OBJs): $(HEADER)

run: $(TARGET)
	@./$(TARGET)

clean:
	rm $(TARGET) $(OBJs)

.PHONY: all clean run
