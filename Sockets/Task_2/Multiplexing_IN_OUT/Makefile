all: server client

server:
	gcc -Wall -Wextra -Werror server.c -o $@

client:
	gcc -Wall -Wextra -Werror client.c -o $@

client_udp:
	gcc -Wall -Wextra -Werror client_udp.c -o $@

rm:
	rm -rf client server client_udp