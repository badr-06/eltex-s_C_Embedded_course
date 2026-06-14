#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int port = 1234;
    
    client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (client_fd == -1) {
        perror("socket()");
        exit(1);
    }
    
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    struct udphdr *udp = (struct udphdr*)buffer;
    char *data = buffer + sizeof(struct udphdr);
    
    const char *message = "Hello, UDP!";
    strcpy(data, message);

    udp->source = htons(7777);
    udp->dest = htons(port);
    udp->len = htons(sizeof(struct udphdr) + strlen(message));
    udp->check = 0;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);
    
    ssize_t sent = sendto(client_fd, buffer, sizeof(struct udphdr) + strlen(message), 0, 
                          (struct sockaddr*)&address, addrlen);
    
    if (sent == -1) {
        perror("sendto");
    }

    while(1){
        char recv[4096];
        ssize_t size = recvfrom(client_fd, recv, sizeof(recv) - 1, 0, NULL, NULL);
        if(size == -1){
            perror("recvfrom()");
            exit(1);
        }

        struct iphdr *ip = (struct iphdr *)recv;

        if(ip->version != 4){
            continue;
        }

        if(ip->protocol != IPPROTO_UDP){
            printf("Не UDP протокол: %d\n", ip->protocol);
            continue;
        }
        
        int ip_header_len = ip->ihl * 4;
        struct udphdr* server_udp = (struct udphdr*)(recv + ip_header_len);

        char *resp_data = recv + ip_header_len + sizeof(struct udphdr);
        int size_data = size - ip_header_len - sizeof(struct udphdr);

        int src_port = ntohs(server_udp->source);
        int dst_port = ntohs(server_udp->dest);

        printf("%d -> %d\n", src_port, dst_port);

        if(ntohs(server_udp->dest) == 7777){
            resp_data[size_data] = '\0';
            printf("%s\n", resp_data);
            break;
        }
        
    }

    close(client_fd);
    return 0;
}