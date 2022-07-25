#ifndef SEND
#define SEND

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int send_data(int sockfd,char *buffer,size_t len,\
        struct sockaddr_in *dst,int c);

#endif
