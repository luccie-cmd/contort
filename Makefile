CC=gcc
CFLAGS=-Wall -Wextra  -Wswitch-enum -Wmissing-prototypes -pedantic -std=c11 -Iinclude -ggdb
SRC_DIR=src
BUILD_DIR=build
LIBS=$(SRC_DIR)/token.c $(SRC_DIR)/lexer.c $(SRC_DIR)/ast.c $(SRC_DIR)/mystring.c

all: always $(BUILD_DIR)/compiler $(BUILD_DIR)/emulator

$(BUILD_DIR)/compiler: $(SRC_DIR)/compiler.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/emulator: $(SRC_DIR)/emulator.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

always:
	mkdir -p build