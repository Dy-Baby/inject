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

unsigned int src_addr, dst_addr;
unsigned char ttl,  protocol, type, tcp_flag;
unsigned short src_port = 0, dst_port = 0;

void inject()
{
	srand(time(NULL));

	int sockfd;
	char buffer[BUFF_SIZE];
	struct sockaddr_in sock_dst;
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	sockfd = init_socket();
	sock_dst.sin_family = AF_INET;
	sock_dst.sin_addr.s_addr = dst_addr;
	if (protocol == IPPROTO_TCP || protocol == IPPROTO_UDP)
		sock_dst.sin_port = dst_port;

	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	set_ip(iph, src_addr, dst_addr, ttl, protocol);

	if (protocol == IPPROTO_ICMP) {
		struct icmp_hdr *icmph = (struct icmp_hdr *)
			(buffer + sizeof(struct ip_hdr));
		set_icmp(icmph, type, 0, 1);
	}
	if (protocol ==  IPPROTO_TCP) {
		struct tcp_hdr *tcph = (struct tcp_hdr *)
			(buffer + sizeof(struct ip_hdr));
		set_tcp(tcph, iph, src_port, dst_port, tcp_flag, 1, 1);
	}
	if (protocol == IPPROTO_UDP) {
               struct udp_hdr *udph = (struct udp_hdr *)
                       (buffer + sizeof(struct ip_hdr));
               set_udp(udph, iph, src_port, dst_port);
	}

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
\t-h : this help message\n\n");
}

void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 2) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	if (!strcmp(argv[1], "ip"))
		protocol = IPPROTO_RAW;
	if (!strcmp(argv[1], "icmp"))
		protocol = IPPROTO_ICMP;
	if (!strcmp(argv[1], "tcp"))
		protocol = IPPROTO_TCP;
	if (!strcmp(argv[1], "udp"))
		protocol = IPPROTO_UDP;

	while ((opt = getopt(argc, argv, "s:d:l:t:o:p:f:h")) != -1) {
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
			src_port = atoi(optarg);
			break;
		case 'p':
			dst_port = atoi(optarg);
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
		case 'h':
			print_usage();
			exit(EXIT_SUCCESS);
		case '?':
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	parser(argc, argv);
	inject();

	return 0;
}