#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "type.h"
#include "error_func.h"
#include "output.h"
#include "sockf.h"
#include "send.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "udp.h"

//#define BUFF_SIZE 2048

int sockfd;
unsigned int src_addr = 0, dst_addr = 0;
unsigned char ttl, protocol = 0, icmp_type = 0, tcp_flag = 0;
unsigned short src_port = 0, dst_port = 0;
int verbose = 0, count = 1, ind = 0;

void sig_close()
{
	printf("%d packets injected.\n", ind);
	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}

void inject()
{
	char buffer[BUFF_SIZE];
	int status;
	struct sockaddr_in sock_dst;

	memset(buffer, 0, BUFF_SIZE);
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	set_ip(buffer, src_addr, dst_addr, ttl, protocol);

	if (!protocol) err_exit("protocol is not valid.");
	switch (protocol) {
	case IPPROTO_ICMP:
		set_icmp(buffer, icmp_type, 0, 1);
		break;
	case IPPROTO_TCP:
		set_tcp(buffer, src_port, dst_port, tcp_flag, 1, 1);
		break;
	case IPPROTO_UDP:
		set_udp(buffer, src_port, dst_port);
		break;
	}

	sock_dst.sin_family = AF_INET;
	sock_dst.sin_addr.s_addr = dst_addr;
	if (protocol == IPPROTO_TCP || protocol == IPPROTO_UDP)
		sock_dst.sin_port = dst_port;

	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	status = send_data(sockfd, buffer, iph->length, &sock_dst);

	if (verbose)
		output(buffer, protocol, status, ind, count);
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
\t-s [address] : source ip address\n\
\t-d [address] : destination ip address\n\
\t-l [ttl] : ttl\n\n\
\t-t [type] : icmp type\n\n\
\t-o [port] : source port\n\
\t-p [port] : destination port\n\
\t-f [flag] : tcp flag (syn, ack, psh, fin, rst, urg)\n\n\
\t-c [count] : number of packets to send\n\n\
\t-v : verbose\n\n\
\t-h : this help message\n\n");
	exit(EXIT_FAILURE);
}

void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 2)
		print_usage();

	if (!strcmp(argv[1], "ip")) inject_ip(argc, argv);
	if (!strcmp(argv[1], "icmp")) inject_icmp(argc, argv);
	if (!strcmp(argv[1], "tcp")) inject_tcp(argc, argv);
	if (!strcmp(argv[1], "udp")) protocol = IPPROTO_UDP;

	while ((opt = getopt(argc, argv, "s:d:l:t:o:p:f:c:vh")) != -1) {
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
			icmp_type = atoi(optarg);
			break;
		case 'o':
			src_port = (unsigned short)atoi(optarg);
			break;
		case 'p':
			dst_port = (unsigned short)atoi(optarg);
			break;
		case 'f':
			if (!strcmp(optarg, "fin")) tcp_flag |= 1;
			if (!strcmp(optarg, "syn")) tcp_flag |= 2;
			if (!strcmp(optarg, "rst")) tcp_flag |= 4;
			if (!strcmp(optarg, "psh")) tcp_flag |= 8;
			if (!strcmp(optarg, "ack")) tcp_flag |= 16;
			if (!strcmp(optarg, "urg")) tcp_flag |= 32;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'c':
			count = atoi(optarg);
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
	signal(SIGINT, sig_close);

	parser(argc, argv);

	if (getuid()) err_exit("permission denied.");
	if (!dst_addr) err_exit("destination address not specified.");

	sockfd = init_socket();

	srand(time(NULL));
	for (ind = 0; ind < count; ind += 1)
		inject();

	sig_close();

	return 0;
}
