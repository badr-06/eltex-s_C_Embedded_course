all: client_1 client_2 client_3 server server_eth

client_1:
	gcc -Wall -Wextra -Werror client_1.c -o $@

client_2:
	gcc -Wall -Wextra -Werror client_2.c -o $@

client_3:
	gcc -Wall -Wextra -Werror client_3.c -o $@

server:
	gcc -Wall -Wextra -Werror server.c -o $@

server_eth:
	gcc -Wall -Wextra -Werror server_eth.c -o $@

rm:
	rm -rf client_1 client_2 client_3 server server_eth