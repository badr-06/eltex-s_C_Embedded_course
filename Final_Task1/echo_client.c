#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUF 4096

int sock;
int my_port = 8888;
int port_server = 7777;

int func_send(struct sockaddr_in *server) {
  char msg[1024];

  fgets(msg, sizeof(msg), stdin);

  msg[strcspn(msg, "\n")] = '\0';

  char packet[MAX_BUF];

  struct udphdr *udp = (struct udphdr *)packet;

  char *data_send = packet + sizeof(struct udphdr);

  strcpy(data_send, msg);

  udp->source = htons(my_port);
  udp->dest = htons(port_server);
  udp->len = htons(sizeof(struct udphdr) + strlen(msg));
  udp->check = 0;

  sendto(sock, packet, sizeof(struct udphdr) + strlen(msg), 0,
         (struct sockaddr *)server, sizeof(*server));

  if (strcmp(msg, "CLOSE") == 0) return 1;

  return 0;
}

int func_read() {
  char buffer[MAX_BUF];
  memset(buffer, 0, sizeof(buffer));
  ssize_t len;

  len = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);

  if (len <= 0) return 1;

  struct iphdr *ip = (struct iphdr *)buffer;

  struct udphdr *udp_recv = (struct udphdr *)(buffer + ip->ihl * 4);

  if (udp_recv->dest != htons(my_port)) {
    return 1;
  }

  char *data = buffer + ip->ihl * 4 + sizeof(struct udphdr);
  int data_len = ntohs(udp_recv->len) - sizeof(struct udphdr);

  printf("server: %s\n", data);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 2) {
    my_port = atoi(argv[1]);
  }

  sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (sock < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in server;

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(port_server);

  while (1) {
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    FD_SET(STDIN_FILENO, &readfds);

    int max_fd = sock > STDIN_FILENO ? sock : STDIN_FILENO;

    int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      if (func_send(&server) == 1) break;
    }

    if (FD_ISSET(sock, &readfds)) {
      func_read();
    }
  }

  close(sock);

  return 0;
}