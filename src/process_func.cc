#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <algorithm>

#include "utils.h"
#include "socks5.h"
#include "process_func.h"

std::mutex accept_lock;
typedef enum {Init, Connecting, Stream} Socks5Status;

void set_nonblocking(int soc) {
    int val = fcntl(soc, F_GETFL, 0);
    fcntl(soc, F_SETFL, val | O_NONBLOCK);
}

void on_listen(const int listen_soc)
{
    struct sockaddr_in cli_addr;
	char client_ip[30];
    socklen_t len = sizeof(cli_addr);
    int connfd;
    while (true) {
        accept_lock.lock();
        connfd = accept(listen_soc, (struct sockaddr*)&cli_addr, &len);
        accept_lock.unlock();

		inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, 30);
		syslog(LOG_INFO, "accept new connection from %s", client_ip);
        on_connect(connfd);
        close(connfd);
    }
}

void on_connect(int connfd) {
  int maxfd = connfd + 1;
  int remotefd = -1;
  fd_set rset, wset;
  int select_no;
  Socks5Status status = Init;
  CircularQueue local_queue, remote_queue;

  set_nonblocking(connfd);
  while (true) {
      FD_ZERO(&rset);
      FD_ZERO(&wset);
      if (status != Stream) {
          FD_SET(connfd, &rset);
      } else if (status == Stream) {
          if (!queue_full(&local_queue)) {
              FD_SET(connfd, &rset);
          }
          if (!queue_empty(&local_queue) && remotefd>0) {
              FD_SET(remotefd, &wset);
          }
          if (!queue_full(&remote_queue) && remotefd>0) {
              FD_SET(remotefd, &rset);
          }
          if (!queue_empty(&remote_queue)) {
              FD_SET(connfd, &wset);
          }
      }
      select_no = select(maxfd, &rset, &wset, NULL, NULL);
      if (select_no < 0) {
          return;
      }

      if (FD_ISSET(connfd, &rset)) {
		  if (status == Init) {
			  if (handle_init_frame(connfd)) {
				  status = Connecting;
			  } else {
				  return;
			  }
		  } else if (status == Connecting) {
			  remotefd = handle_socks5_request(connfd);
			  if (remotefd > 0) {
				  set_nonblocking(remotefd);
				  maxfd = std::max(connfd, remotefd) + 1;
				  status = Stream;
			  } else {
				  return;
			  }
		  } else if (status == Stream) {
			  if (!queue_full(&local_queue)) {
				  int nbytes = read_from_soc(&local_queue, connfd);
				  if (nbytes > 0) {
					  FD_SET(remotefd, &wset);
				  } else {
					  close(remotefd);
					  return;
				  }
			  }
		  }
	  }
	  if (FD_ISSET(remotefd, &rset)) {
		  if (!queue_full(&remote_queue)) {
			  int nbytes = read_from_soc(&remote_queue, remotefd);
			  if (nbytes > 0) {
				  FD_SET(connfd, &wset);
			  } else {
				  close(remotefd);
				  return;
			  }
		  }
	  }
	  if (FD_ISSET(connfd, &wset)) {
		  if (!queue_empty(&remote_queue)) {
			  write_to_soc(&remote_queue, connfd);
		  }
	  }
	  if (FD_ISSET(remotefd, &wset)) {
		  if (!queue_empty(&local_queue)) {
			  write_to_soc(&local_queue, remotefd);
		  }
	  }
  }
}
