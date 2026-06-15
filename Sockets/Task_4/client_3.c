#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <net/if.h>

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
    struct sockaddr_ll address;
    socklen_t addrlen = sizeof(address);
    int port = 1234;

    client_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (client_fd == -1) {
        printf("error()");
        exit(1);
    }
    
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    
    struct ethhdr *eth = (struct ethhdr*)buffer;
 
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
     
    struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));

    char *data = buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
    
    const char *message = "Hello, UDP!";
    strcpy(data, message);

    udp->source = htons(7777);
    udp->dest = htons(port);
    udp->len = htons(sizeof(struct udphdr) + strlen(message));
    udp->check = 0;

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message));
    iph->id = htons(7777);
    iph->frag_off = htons(0x4000);;
    iph->ttl = 64;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.31.125");
    iph->daddr = inet_addr("192.168.31.189");
    
    iph->check = calculate_checksum((uint16_t*)iph, iph->ihl * 4);
    
    unsigned char dest[6] = {0x10, 0x68, 0x38, 0x00, 0xfe, 0x6b};
    memcpy(eth->h_dest, dest, 6);
    unsigned char source[6] = {0x08, 0x00, 0x27, 0x1f, 0x69, 0x96};
    memcpy(eth->h_source, source, 6);
    eth->h_proto = htons(ETH_P_IP);

    
    address.sll_family = AF_PACKET;
    address.sll_ifindex = 2;  // Индекс интерфейса (eth0, lo, etc)
    address.sll_halen = 6;
    memcpy(address.sll_addr, dest, 6);
    ssize_t sent = sendto(client_fd, buffer, sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message), 0, 
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

        struct ethhdr *eth = (struct ethhdr*)recv;
        
        if(ntohs(eth->h_proto) != ETH_P_IP){
            printf("eth->h_proto != htons(ETH_P_IP)\n");
            continue;
        }

        if(memcmp(eth->h_dest, source, 6) != 0){
            printf("eth->h_dest != source\n");
            continue;
        }
        
        struct iphdr *ip = (struct iphdr *)(recv + sizeof(struct ethhdr));

        if(ip->version != 4){
            printf("Не та версия ip->version = %d\n", ip->version);
            continue;
        }

        if(ip->protocol != IPPROTO_UDP){
            printf("Не UDP протокол: %d\n", ip->protocol);
            continue;
        }

        int ip_header_len = ip->ihl * 4;
        struct udphdr* server_udp = (struct udphdr*)(recv + ip_header_len + sizeof(struct ethhdr));

        char *resp_data = recv + ip_header_len + sizeof(struct udphdr) + sizeof(struct ethhdr);
        int size_data = size - ip_header_len - sizeof(struct udphdr) - sizeof(struct ethhdr);

        int src_port = ntohs(server_udp->source);
        int dst_port = ntohs(server_udp->dest);

        printf("%d -> %d\n", src_port, dst_port);

        if(ntohs(server_udp->dest) == 7777){
            if(size_data > 0){
                resp_data[size_data] = '\0';
                printf("%s\n", resp_data);
            }
            break;
        }
        
    }

    close(client_fd);
    return 0;
}