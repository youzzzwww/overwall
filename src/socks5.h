#ifndef _SOCKS5_H_
#define _SOCKS5_H_
#include <sys/types.h>
#include <sys/socket.h>

int socks5_init_size(const char* buffer, int size);

int write_init_reply(int soc);

int socks5_remote_sock(const char* buffer, int size, int* frame_size);
int get_sock_with_addr(struct sockaddr_in* pserv_addr);
int get_sock_with_name(const char* hostname, const char* port);
#endif
