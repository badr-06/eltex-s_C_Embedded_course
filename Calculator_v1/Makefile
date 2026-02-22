CC = gcc
CFLAGS = -Wall -Wextra -Werror
DIR = objects

all: program

$(DIR)/%.o: library/%.c
	$(CC) -fPIC $(CFLAGS) -c $^ -o $@

objects_files: $(DIR)/add.o $(DIR)/sub.o $(DIR)/mul.o $(DIR)/div.o

libmy_library.a: $(DIR)/add.o $(DIR)/sub.o $(DIR)/mul.o $(DIR)/div.o
	ar rcs libmy_library.a $(DIR)/*.o

program: libmy_library.a
	$(CC) $(CFLAGS) main.c libmy_library.a -o program

clean:
	rm -rf libmy_library.a program $(DIR)/*

.PHONY: objects_files clean