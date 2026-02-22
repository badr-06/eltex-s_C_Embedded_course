CC = gcc
CFLAGS = -Wall -Wextra -Werror
DIR = plagins

all: plagins build

$(DIR)/add.so: library/add.c
	$(CC) -shared -fPIC $(CFLAGS) library/add.c -o $@

$(DIR)/sub.so: library/sub.c
	$(CC) -shared -fPIC $(CFLAGS) library/sub.c -o $@

$(DIR)/mul.so: library/mul.c
	$(CC) -shared -fPIC $(CFLAGS) library/mul.c -o $@

$(DIR)/div.so: library/div.c
	$(CC) -shared -fPIC $(CFLAGS) library/div.c -o $@

all_libraries: $(DIR)/add.so $(DIR)/sub.so $(DIR)/mul.so $(DIR)/div.so

build: main.c all_libraries
	$(CC) $(CFLAGS) main.c -Wl,-rpath,./$(DIR) -o program

clean:
	rm -rf $(DIR)/* program

.PHONY: all_libraries clean