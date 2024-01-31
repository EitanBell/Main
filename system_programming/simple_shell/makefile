CCFLAGS = -ansi -pedantic-errors -Wall -Wextra
CC = gcc
TARGET = a.out
SOURCES = simple_shell.c simple_shell_test.c 
OBJs = simple_shell_test.o simple_shell.o
HEADER = simple_shell.h

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
