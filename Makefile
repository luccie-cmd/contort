CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wswitch-enum -Wmissing-prototypes -pedantic -std=c11 -Iinclude -ggdb
SRC_DIR=src
BUILD_DIR=build

$(BUILD_DIR)/main: $(SRC_DIR)/*.c
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^