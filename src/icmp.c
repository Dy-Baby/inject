#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sockf.h"
#include "send.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "ip.h"
#include "icmp.h"
#include "checksum.h"

static unsigned int src_addr, dst_addr;
static unsigned char ttl, icmp_type, icmp_code;
static int count = 1, verbose = 0;

void set_icmp(char *buffer, unsigned char type,
		unsigned char code, unsigned short seq)
{
	struct icmp_hdr *icmph = (struct icmp_hdr *)
		(buffer + sizeof(struct ip_hdr));

	icmph->type = type;
	icmph->code = code;
	icmph->seq = seq;
	icmph->id = htons(getpid());
	icmph->check = 0;
	icmph->check = checksum((unsigned short *)icmph, sizeof(struct icmp_hdr));
}

static void icmp_usage()
{
	printf("\n general options :\n\n\
\t-c [count] : number of packets to send\n\
\t-v : verbose\n\
\t-h : this help message\n\n");

        printf("\n IP options :\n\n\
\t-s [address] : source address\n\
\t-d [address] : destination address\n\
\t-t [ttl] : ttl\n\n");

	printf("\n ICMP options :\n\n\
\t-T [type] : icmp type\n\
\t-C [code] : icmp code\n\n");
	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3) icmp_usage();

	while ((opt = getopt(argc, argv, "c:vhs:d:t:T:C:")) != -1) {
		switch (opt) {
		case 'c':
			count = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
			icmp_usage();
		case 's':
			src_addr = inet_addr(optarg);
			break;
		case 'd':
			dst_addr = inet_addr(optarg);
			break;
		case 't':
			ttl = atoi(optarg);
			break;
		case 'T':
			icmp_type = atoi(optarg);
			break;
		case 'C':
			icmp_code = atoi(optarg);
			break;
		case '?':
			break;
		}
	}
}

void inject_icmp(int argc, char *argv[])
{
	char buffer[BUFF_SIZE];
	struct sockaddr_in sock_dst;
	int sockfd, ind, status;

	memset(buffer, 0, BUFF_SIZE);
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	parser(argc, argv);

	sockfd = init_socket();
	sock_dst.sin_family = AF_INET;
	sock_dst.sin_addr.s_addr = dst_addr;

	if (!dst_addr) err_exit("destination address not specified.");
	set_ip(buffer, src_addr, dst_addr, ttl, IPPROTO_ICMP);

	set_icmp(buffer, icmp_type, icmp_code, 0);
	struct ip_hdr *iph = (struct ip_hdr *)buffer;

	for (ind = 0; ind < count; ind += 1) {
		status = send_data(sockfd, buffer, iph->length, &sock_dst);
		if (verbose)
			output(buffer, IPPROTO_ICMP, status, ind, count);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
