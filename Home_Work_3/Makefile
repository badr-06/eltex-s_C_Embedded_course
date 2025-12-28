CC = gcc
CFLAGS = -Wall -Wextra -Werror
TEST_FLAGS = -lcheck -lsubunit -lrt -lpthread -lm
BUILD_DIR = build

all: main1 main2 main3 main4

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

main1: src/main1.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

main2: src/main2.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

main3: src/main3.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

main4: src/main4.c lib/lib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test: lib/lib.c tests/test.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_FLAGS)

clean:
	rm -rf $(BUILD_DIR) main1 main2 main3 main4 test

.PHONY: all clean