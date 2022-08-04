#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "send.h"

int send_data(int sockfd, char *buffer, size_t len, struct sockaddr_in *dst,
	      int c)
{
	if (sendto(sockfd, buffer, len, 0, (struct sockaddr *)dst, sizeof(*dst)) < 0) {
		err_msg("send.c/send_data", errno);
		return errno;
	}

	return 0;
}
