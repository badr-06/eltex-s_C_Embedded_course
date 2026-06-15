#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUF 4096
#define MAX_CLIENTS 100

int sock;

struct client_info {
  uint32_t ip;
  uint16_t port;
  int counter;
};

struct client_info clients[MAX_CLIENTS];
int client_count = 0;

int find_client(uint32_t ip, uint16_t port) {
  int i;

  for (i = 0; i < client_count; i++) {
    if (clients[i].ip == ip && clients[i].port == port) {
      return i;
    }
  }

  return -1;
}

int add_client(uint32_t ip, uint16_t port) {
  clients[client_count].ip = ip;
  clients[client_count].port = port;
  clients[client_count].counter = 0;

  client_count++;

  return client_count - 1;
}

void signalHandler(int sig) {
  close(sock);
  exit(sig);
}

int main() {
  signal(SIGINT, signalHandler);

  char buffer[MAX_BUF];
  int port = 7777;
  int opt = 1;

  sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (sock < 0) {
    perror("socket");
    return 1;
  }

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("setsockopt()");
  }

  while (1) {
    ssize_t len;
    len = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);

    if (len <= 0) continue;

    struct iphdr *ip = (struct iphdr *)buffer;

    struct udphdr *udp = (struct udphdr *)(buffer + ip->ihl * 4);

    if (udp->dest != htons(port)) {
      continue;
    }

    char *data = buffer + ip->ihl * 4 + sizeof(struct udphdr);

    int data_len = ntohs(udp->len) - sizeof(struct udphdr);

    char msg[1024];

    memcpy(msg, data, data_len);
    msg[data_len] = '\0';

    uint32_t client_ip = ip->saddr;
    uint16_t client_port = ntohs(udp->source);

    if (strcmp(msg, "CLOSE") == 0) {
      int idx = find_client(client_ip, client_port);

      if (idx != -1) {
        int i;

        for (i = idx; i < client_count - 1; i++) {
          clients[i] = clients[i + 1];
        }

        client_count--;
      }

      printf("client removed\n");
      continue;
    }

    printf("сообщение от клиента с портом %d: %s\n", ntohs(udp->source), msg);

    int idx = find_client(client_ip, client_port);

    if (idx == -1) {
      idx = add_client(client_ip, client_port);
    }

    clients[idx].counter++;

    char packet[MAX_BUF];
    memset(packet, 0, sizeof(packet));
    struct udphdr *udp_cl = (struct udphdr *)packet;

    char *data_send = packet + sizeof(struct udphdr);

    sprintf(data_send, "%s %d", msg, clients[idx].counter);

    udp_cl->source = htons(port);
    udp_cl->dest = udp->source;
    udp_cl->check = 0;
    udp_cl->len = sizeof(struct udphdr) + strlen(data_send);

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = client_ip;
    addr.sin_port = udp->source;
    printf("%d\n", ntohs(udp_cl->dest));
    sendto(sock, packet, sizeof(struct udphdr) + strlen(data_send), 0,
           (struct sockaddr *)&addr, sizeof(addr));
  }

  close(sock);

  return 0;
}