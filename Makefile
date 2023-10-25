# Output executable name
OUTPUT = preprocessing

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Main source file
MAIN = process.c

# Additional source files
ADD_SRC = queue.c

all:
	$(CC) $(CFLAGS) $(MAIN) $(ADD_SRC) -o $(OUTPUT)

test:
	$(CC) $(CFLAGS) test.c $(ADD_SRC) -o test

clean:
	rm -f $(OUTPUT) test
