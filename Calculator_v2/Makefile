CC = gcc
CFLAGS = -Wall -Wextra -Werror
DIR = objects

all: program

$(DIR)/%.o: library/%.c
	$(CC) -fPIC $(CFLAGS) -c $^ -o $@

objects_files: $(DIR)/add.o $(DIR)/sub.o $(DIR)/mul.o $(DIR)/div.o

libmy_library.so: $(DIR)/add.o $(DIR)/sub.o $(DIR)/mul.o $(DIR)/div.o
	gcc -shared $(DIR)/*.o -o libmy_library.so

program: libmy_library.so
	$(CC) $(CFLAGS) main.c -L. -lmy_library -Wl,-rpath,. -o program

clean:
	rm -rf libmy_library.so program $(DIR)/*

.PHONY: objects_files clean