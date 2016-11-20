#ifndef _PROCESS_FUNC_H_
#define _PROCESS_FUNC_H_

void set_nonblocking(int soc);

void on_listen(const int listen_soc);
void on_connect(int connfd);
#endif
