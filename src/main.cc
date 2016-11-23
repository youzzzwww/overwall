#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include <thread>
#include <vector>

#include "process_func.h"

int parse_commandline(int argc, char** argv, int* port, int* thread_num)
{
    if (argc < 3) {
        perror("usage -P/--port <listening_port> -T/--thread <thread_num>");
        return -1;
    }

    for (int i=1; i < argc-1; ++i) {
        if (strcmp(argv[i], "-P")==0 || strcmp(argv[i], "--port")==0) {
            *port = atoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-T")==0 || strcmp(argv[i], "--thread")==0) {
            *thread_num = atoi(argv[i+1]);
        }
    }
    return 0;
}

int tcp_listen(const char* ip, const int port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }
    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (ip == NULL)
        serv_addr.sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    int enable = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(int));
    setsockopt(listenfd, SOL_TCP, TCP_NODELAY, &enable, sizeof(int));
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        return -1;
    }
    if (listen(listenfd, SOMAXCONN) < 0) {
        perror("error on listening");
        return -1;
    }
    return listenfd;
}

int main(int argc, char** argv)
{
	openlog(NULL, LOG_PID, LOG_DAEMON);

    int port = 0, thread_num = 100;
    int listen_fd = 0;
    if (parse_commandline(argc, argv, &port, &thread_num) != 0)
        return -1;
    listen_fd = tcp_listen(NULL, port);
    if (listen_fd < 0) {
		syslog(LOG_ERR, "%s", "listenfd not available, abort.");
        return -1;
	}
	syslog(LOG_INFO, "port is %d, thread_num is %d, listening soc is %d.\n",
			port, thread_num, listen_fd);

	std::vector<std::thread> p_array;
	for (int i=0; i<thread_num; ++i) {
		p_array.push_back(std::thread(&on_listen, listen_fd));
	}
	for (auto& x : p_array) {
		x.join();
	}
    return 0;
}
