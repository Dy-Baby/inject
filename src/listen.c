#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>

#include "sockf.h"
#include "type.h"
#include "listen.h"
#include "output.h"
#include "error_func.h"
#include "usage.h"

static int flag = 0;

static void output_packet(char *buffer, int fl)
{
	struct eth_hdr *ethh = (struct eth_hdr *)buffer;
	unsigned short protocol = ethh->protocol;

	if ((fl & ETH_FLAG) == ETH_FLAG)
		print_eth(buffer);

	if ((protocol == htons(ETH_P_ARP)) && ((fl & ARP_FLAG) == ARP_FLAG))
		print_arp(buffer);

	if (protocol == htons(ETH_P_IP)) {
		struct ip_hdr *iph = (struct ip_hdr *)(buffer + sizeof(struct eth_hdr));

		if ((fl & IP_FLAG) == IP_FLAG)
			print_ip(buffer);

		switch (iph->protocol) {
		case 1:
			if ((fl & ICMP_FLAG) == ICMP_FLAG)
				print_icmp(buffer);
			break;
		case 6:
			if ((fl & TCP_FLAG) == TCP_FLAG)
				print_tcp(buffer);
			break;
		case 17:
			if ((fl & UDP_FLAG) == UDP_FLAG)
				print_udp(buffer);
			break;
		}
	}
}

static void usage()
{
	printf("\n general options :\n\n\
\t-h : this help message\n");
	sniff_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "eaictuh")) != -1) {
		switch (opt) {
		case 'e':
			flag |= ETH_FLAG;
			break;
		case 'a':
			flag |= ARP_FLAG;
			break;
		case 'i':
			flag |= IP_FLAG;
			break;
		case 'c':
			flag |= ICMP_FLAG;
			break;
		case 't':
			flag |= TCP_FLAG;
			break;
		case 'u':
			flag |= UDP_FLAG;
			break;
		case 'h':
			usage();
		case '?':
			break;
		}
	}

	if (flag == 0) flag |= ALL;
}

void sniff_network(int argc, char *argv[])
{
	int sockfd, total;
	char buffer[BUFF_SIZE];

	parser(argc, argv);
	sockfd = init_packet_socket();

	while (1) {
		memset(buffer, 0, BUFF_SIZE);
		total = recvfrom(sockfd, buffer, BUFF_SIZE, 0, NULL, NULL);
		if (total < 0 || total == 0 ) {
			err_msg("listen.c", "sniff_network", __LINE__, errno);
			break;
		}
		output_packet(buffer, flag);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
