#define _GNU_SOURCE
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <errno.h>
#include "error_func.h"
#include "get_addr.h"

unsigned int get_address()
{
	struct ifaddrs *addr, *temp;
	struct sockaddr_in *ip_addr;
	unsigned int ip;

	if (getifaddrs(&addr) == -1) {
		err_msg("get_addr.c", "get_address", __LINE__, errno);
		return -1;
	}

	for (temp = addr; temp != NULL; temp = temp->ifa_next) {
		if (temp->ifa_addr == NULL) continue;

		if (temp->ifa_addr->sa_family == AF_INET &&
		    strcmp(temp->ifa_name, "lo")) {
			ip_addr = (struct sockaddr_in *)temp->ifa_addr;
			ip = ip_addr->sin_addr.s_addr;

			break;
		}
	}

	freeifaddrs(addr);

	return ip;
}
