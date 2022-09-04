#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "sockf.h"
#include "error_func.h"

int init_socket()
{
	int sockfd, enable = 1;

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		err_msg("sockf.c", "init_socket", __LINE__, errno);
		return errno;
	}
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(int)) <
	    0) {
		err_msg("sockf.c", "init_socket", __LINE__, errno);
		return errno;
	}

	return sockfd;
}

void close_sock(int sockfd)
{
	if (close(sockfd) == -1)
		err_msg("sockf.c", "close_sock", __LINE__, errno);
}
