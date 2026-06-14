#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

uint16_t calculate_checksum(uint16_t *buffer, int len) {
    uint32_t sum = 0;
    uint16_t result;

    while (len > 1) {
        sum += *buffer++;
        len -= 2;
    }
    
    if (len == 1) {
        sum += *(uint8_t*)buffer;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    
    result = ~sum;
    return result;
}

int main() {
    int client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int port = 1234;
    int opt = 1;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);

    client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (client_fd == -1) {
        perror("socket()");
        exit(1);
    }
    
    if(setsockopt(client_fd, IPPROTO_IP, IP_HDRINCL, (char *)&opt, sizeof(opt)) == -1){
        perror("setsockopt() failed");
        exit(-1);
    }
    
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    

    //IP header
    struct iphdr *iph = (struct iphdr *)buffer;
     
    //UDP header
    struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct iphdr));

    char *data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);
    
    const char *message = "Hello, UDP!";
    strcpy(data, message);

    udp->source = htons(7777);
    udp->dest = htons(port);
    udp->len = htons(sizeof(struct udphdr) + strlen(message));
    udp->check = 0;

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message);
    iph->id = htons(7777);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = htonl(INADDR_LOOPBACK);
    iph->daddr = address.sin_addr.s_addr;

    iph->check = calculate_checksum((uint16_t*)iph, iph->ihl *4);

    ssize_t sent = sendto(client_fd, buffer, sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message), 0, 
                          (struct sockaddr*)&address, addrlen);
    if(sent < 0){
        perror("sendto()");
        exit(1);
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