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
#include "get_addr.h"
#include "checksum.h"

static unsigned int src_addr, dst_addr;
static unsigned char ttl;
static int count = 1, verbose = 0;

void set_ip(char *buffer, size_t payload_size,
		unsigned int src, unsigned int dst,
		unsigned char ttl, unsigned char protocol)
{
	struct ip_hdr *iph = (struct ip_hdr *)buffer;

	iph->length = sizeof(struct ip_hdr);
	switch (protocol) {
	case IPPROTO_ICMP:
		iph->length += sizeof(struct icmp_hdr);
		break;
	case IPPROTO_TCP:
		iph->length += sizeof(struct tcp_hdr);
		break;
	case IPPROTO_UDP:
		iph->length += sizeof(struct tcp_hdr);
		break;
	}
	iph->length += payload_size;

	iph->ver_ihl = 0x45;
	iph->service = 0x00;
	iph->ident = htons(getpid());
	iph->frag = 0x00;
	iph->ttl = (ttl) ? ttl : DEFAULT_TTL;
	iph->protocol = protocol;
	iph->check = 0;
	iph->src = (src) ? src : get_address();
	iph->dst = dst;
	iph->check = checksum((unsigned short *)iph, iph->length);
}

static void ip_usage()
{
	printf("\n general options :\n\n\
\t-c [count] : number of packets to send\n\
\t-v : verbose\n\
\t-h : this help message\n");

	printf("\n IP options :\n\n\
\t-S [address] : source address\n\
\t-D [address] : destination address\n\
\t-T [ttl] : ttl\n\n");
	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3) ip_usage();

	while ((opt = getopt(argc, argv, "c:vhS:D:T:")) != -1) {
		switch (opt) {
		case 'c':
			count = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
			ip_usage();
		case 'S':
			src_addr = inet_addr(optarg);
			break;
		case 'D':
			dst_addr = inet_addr(optarg);
			break;
		case 'T':
			ttl = atoi(optarg);
			break;
		case '?':
			break;
		}
	}
}

void inject_ip(int argc, char *argv[])
{
	char buffer[BUFF_SIZE];
	struct sockaddr_in sock_dst;
	int sockfd, ind;

	parser(argc, argv);
	if (!dst_addr) err_exit("destination address not specified.");

	memset(buffer, 0, BUFF_SIZE);
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	sockfd = init_socket();
	sock_dst.sin_family = AF_INET;
	sock_dst.sin_addr.s_addr = dst_addr;

	set_ip(buffer, 0, src_addr, dst_addr, ttl, 0);

	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	for (ind = 0; ind < count; ind += 1)
		send_data(sockfd, buffer, iph->length, &sock_dst);

	if (verbose)
		print_ip(buffer);

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
