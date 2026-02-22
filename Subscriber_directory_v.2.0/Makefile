CC = gcc
CFLAGS = -Wall -Wextra -Werror
BUILD_DIR = build

all: lib
	$(CC) $(CFLAGS) $(BUILD_DIR)/lib.o src/main.c -o program

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

lib: $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $@/$@.c -o $(BUILD_DIR)/$@.o

cppcheck:
	cppcheck --enable=all --inconclusive --error-exitcode=1 --suppress=missingIncludeSystem src/main.c lib/lib.c

clean:
	rm -rf $(BUILD_DIR) program

.PHONY: all clean