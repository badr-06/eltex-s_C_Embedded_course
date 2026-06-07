#!/bin/bash

all: program kill

program:
	gcc -Wall -Wextra -Werror main.c -o $@

kill:
	gcc -Wall -Wextra -Werror kill.c -o $@

clean:
	rm -rf program kill