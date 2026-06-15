all: echo_client echo_server

echo_server:
	gcc -Wall -Wextra -Werror echo_server.c -o $@

echo_client:
	gcc -Wall -Wextra -Werror echo_client.c -o $@

rm:
	rm -rf echo_client echo_server