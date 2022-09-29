#ifndef SOCKF
#define SOCKF

int init_socket();
int bind_iface(int sockfd, char *iface);
void close_sock(int sockfd);

#endif
