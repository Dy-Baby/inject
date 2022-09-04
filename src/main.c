#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "type.h"
#include "error_func.h"
#include "sockf.h"
#include "send.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "udp.h"

#define BUFF_SIZE 4096

int sockfd;
unsigned int src_addr, dst_addr;
unsigned char ttl, protocol, type, tcp_flag;
unsigned short src_port = 0, dst_port = 0;
int counter = 1;

void inject()
{
	char buffer[BUFF_SIZE];
	struct sockaddr_in sock_dst;
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	struct icmp_hdr *icmph;
	struct tcp_hdr *tcph;
	struct udp_hdr *udph;

	set_ip(iph, src_addr, dst_addr, ttl, protocol);

	switch (protocol) {
	case IPPROTO_ICMP:
		icmph = (struct icmp_hdr *)(buffer + sizeof(struct ip_hdr));
		set_icmp(icmph, type, 0, 1);
		break;
	case IPPROTO_TCP:
		tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));
		set_tcp(tcph, iph, src_port, dst_port, tcp_flag, 1, 1);
		break;
	case IPPROTO_UDP:
		udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));
		set_udp(udph, iph, src_port, dst_port);
		break;
	}

	sock_dst.sin_family = AF_INET;
	sock_dst.sin_addr.s_addr = dst_addr;
	if (protocol == IPPROTO_TCP || protocol == IPPROTO_UDP)
		sock_dst.sin_port = (protocol == IPPROTO_TCP) ? tcph->dst :
								      udph->dst;

	send_data(sockfd, buffer, iph->length, &sock_dst);
}

void print_usage()
{
	printf("\n usage : ./inject [protocol] [options]\n\n\
 protocol :\n\
\tip : ip packets\n\
\ticmp : icmp packets\n\
\ttcp : tcp packets\n\
\tudp : udp packets\n\n\
 options :\n\
\t-s : source ip address\n\
\t-d : destination ip address\n\
\t-l : ttl\n\n\
\t-t : icmp type\n\n\
\t-o : source port\n\
\t-p : destination port\n\
\t-f : tcp flag (syn, ack, psh, fin, rst, urg)\n\n\
\t-c : number of packets to send\n\n\
\t-h : this help message\n\n");
	exit(EXIT_SUCCESS);
}

void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 2)
		print_usage();

	if (!strcmp(argv[1], "ip"))
		protocol = IPPROTO_RAW;
	if (!strcmp(argv[1], "icmp"))
		protocol = IPPROTO_ICMP;
	if (!strcmp(argv[1], "tcp"))
		protocol = IPPROTO_TCP;
	if (!strcmp(argv[1], "udp"))
		protocol = IPPROTO_UDP;

	while ((opt = getopt(argc, argv, "s:d:l:t:o:p:f:c:h")) != -1) {
		switch (opt) {
		case 's':
			src_addr = inet_addr(optarg);
			break;
		case 'd':
			dst_addr = inet_addr(optarg);
			break;
		case 'l':
			ttl = atoi(optarg);
			break;
		case 't':
			type = atoi(optarg);
			break;
		case 'o':
			src_port = (unsigned short)atoi(optarg);
			break;
		case 'p':
			dst_port = (unsigned short)atoi(optarg);
			break;
		case 'f':
			if (!strcmp(optarg, "fin"))
				tcp_flag |= 1;
			if (!strcmp(optarg, "syn"))
				tcp_flag |= 2;
			if (!strcmp(optarg, "rst"))
				tcp_flag |= 4;
			if (!strcmp(optarg, "psh"))
				tcp_flag |= 8;
			if (!strcmp(optarg, "ack"))
				tcp_flag |= 16;
			if (!strcmp(optarg, "urg"))
				tcp_flag |= 32;
			break;
		case 'c':
			counter = atoi(optarg);
			break;
		case 'h':
			print_usage();
		case '?':
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ind;

	parser(argc, argv);

	if (getuid() != 0) {
		printf("permission denied\n");
		exit(EXIT_FAILURE);
	}
	if (!dst_addr) {
		printf("[ERROR] [main.c/main/%d] :\
 destination address not specified.\n",
		       __LINE__);
		exit(EXIT_FAILURE);
	}

	sockfd = init_socket();
	srand(time(NULL));
	for (ind = 0; ind < counter; ind += 1)
		inject();

	return 0;
}
