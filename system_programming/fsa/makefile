CCFLAGS = -ansi -pedantic-errors -Wall -Wextra
CC = gcc
TARGET = a.out
SOURCES = fsa.c fsa_test.c
OBJs = fsa_test.o fsa.o 
HEADER = fsa.h

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
