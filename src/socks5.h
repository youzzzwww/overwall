#ifndef _SOCKS5_H_
#define _SOCKS5_H_
#include <sys/types.h>
#include <sys/socket.h>

int handle_init_frame(int soc);
int socks5_init_size(const char* buffer, int size);
int write_init_reply(int soc);

int handle_socks5_request(int soc);
int socks5_remote_sock(const char* buffer, int size);
void write_socks5_reply(int soc, int remote_soc);
int get_sock_with_addr(struct sockaddr_in* pserv_addr);
int get_sock_with_name(const char* hostname, const char* port);
#endif
