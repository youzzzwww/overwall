#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "socks5.h"

int socks5_init_size(const char* buffer, int size) {
    char version = 0;
    char nmethod = 0;
    if (size > 2) {
        memcpy(&version, buffer, 1);
        if (version == 0x05) {
            memcpy(&nmethod, buffer+1, 1);
            return 2 + nmethod;
        }
    }
    return -1;
}

int write_init_reply(int soc) {
    InitReply reply;
    return write(soc, &reply, sizeof(InitReply));
}

int socks5_remote_sock(const char* buffer, int size, int* frame_size) {
    char version = 0;
    char cmd = 0;
    char atype = 0;

    if (size > 6) {
        memcpy(&version, buffer, 1);
        if (version != 0x05) {
            return -1;
        }
        memcpy(&cmd, buffer+1, 1);
        if (cmd == 0x01) {
            //connecting
        }
        memcpy(&atype, buffer+3, 1);
        if (atype == 0x01) {
            //ipv4
            struct sockaddr_in serv_addr;
            bzero((char*)&serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            memcpy((char*)&serv_addr.sin_addr.s_addr,
                    buffer+4, 4);
            memcpy((char*)&serv_addr.sin_port,
                    buffer+8, 2);
            *frame_size = 10;
            return get_sock_with_addr(&serv_addr);
        } else if (atype == 0x03) {
            uint8_t host_len = 0;
            memcpy(&host_len, buffer+4, 1);
            if (host_len + 7 <= size) {
                char* host_str = (char*)malloc(host_len + 1);
                memset(host_str, 0, host_len+1);
                memcpy(host_str, buffer+5, host_len);

                char port_str[6] = {0};
                uint16_t port = 0;
                memcpy(&port, buffer+5+host_len, 2);
                port = ntohs(port);
                snprintf(port_str, 6, "%d", port);
                int soc = get_sock_with_name(host_str, port_str);
                free(host_str);
                *frame_size = 7 + host_len;
                return soc;
            } else {
                return -1;
            }
        }
    }
    return -1;
}

int get_sock_with_addr(struct sockaddr_in* pserv_addr) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;    
    }
    if (connect(sockfd, (struct sockaddr*)pserv_addr, sizeof(*pserv_addr)) < 0) {
        return -1;
    }
    return sockfd;
}

int get_sock_with_name(const char* hostname, const char* port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */
    s = getaddrinfo(hostname, port, &hints, &result);
    if (s != 0)
        return -1;

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;
                                    
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            return sfd;                  /* Success */
        close(sfd);
    }
    return -1;
}

