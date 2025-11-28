CC = gcc
CFLAGS = -Wall -Wextra -Werror
TEST_FLAGS = -lcheck -lsubunit -lrt -lpthread -lm
BUILD_DIR = build

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

main1: $(BUILD_DIR)/main1.o
	$(CC) $< -o main1

main2: $(BUILD_DIR)/main2.o
	$(CC) $< -o main2

main3: $(BUILD_DIR)/main3.o
	$(CC) $< -o main3

main4: $(BUILD_DIR)/main4.o
	$(CC) $< -o main4

$(BUILD_DIR)/%.o: src/*.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: tests/test.c
	$(CC) $(CFLAGS) -Ilib tests/test.c -o test $(TEST_FLAGS)

all: main1 main2 main3 main4

clean:
	rm -rf $(BUILD_DIR) main1 main2 main3 main4 test

.PHONY: all clean