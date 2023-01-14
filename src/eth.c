#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "sockf.h"
#include "get_addr.h"
#include "send.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "eth.h"

static unsigned char src_mac[6], dst_mac[6];
static unsigned short protocol;
static int count = 1, verbose = 0, src_mac_control = 0, dst_mac_control = 0;
static char *iface = NULL;

void set_eth(char *buffer, unsigned char *dst, unsigned char *src, unsigned short proto)
{
	struct eth_hdr *ethh = (struct eth_hdr *)buffer;

	memcpy(ethh->dst, dst, 6);
	memcpy(ethh->src, src, 6);
	ethh->protocol = htons(proto);
}

static void eth_usage()
{
	printf("\n general options :\n\n\
\t-i [interface] : network interface\n\
\t-c [count] : number of packets to send\n\
\t-v : verbose\n\
\t-h : this help message\n");

	printf("\n ETH options :\n\n\
\t-M [mac address] : source mac address (in XX:XX:XX:XX:XX:XX format)\n\
\t-K [mac address] : destination mac address (in XX:XX:XX:XX:XX:XX format)\n\
\t-p [protocol] : protocol\n\n");
	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3) eth_usage();

	while ((opt = getopt(argc, argv, "i:c:vhM:K:p:")) != -1) {
		switch (opt) {
		case 'i':
			iface = optarg;
			break;
		case 'c':
			count = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
			eth_usage();
			break;
		case 'M':
			sscanf(optarg, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				&src_mac[0], &src_mac[1], &src_mac[2],
				&src_mac[3], &src_mac[4], &src_mac[5]);
			src_mac_control = 1;
			break;
		case 'K':
			sscanf(optarg, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				&dst_mac[0], &dst_mac[1], &dst_mac[2],
				&dst_mac[3], &dst_mac[4], &dst_mac[5]);
			dst_mac_control = 1;
			break;
		case 'p':
			protocol = atoi(optarg);
			break;
		case '?':
			break;
		}
	}
}

void inject_eth(int argc, char *argv[])
{
	char buffer[BUFF_SIZE];
	struct ifreq ifr;
	struct sockaddr_ll device;
	int sockfd, ind;

	parser(argc, argv);

	memset(buffer, 0, BUFF_SIZE);
	memset(&ifr, 0, sizeof(struct ifreq));
	memset(&device, 0, sizeof(struct sockaddr_ll));

	sockfd = init_packet_socket();

	if (!iface) err_exit("network interface not specified.");

	if (!src_mac_control) {
		memcpy(ifr.ifr_name, iface, strlen(iface));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
			err_msg("eth.c", "inject_eth", __LINE__, errno);

		memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);
	}

	if (!dst_mac_control) memset(dst_mac, 0xff, 6);

	if ((device.sll_ifindex = if_nametoindex(iface)) == 0)
		err_msg("eth.c", "inject_eth", __LINE__, errno);
	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr, src_mac, 6);
	device.sll_halen = 6;

	set_eth(buffer, dst_mac, src_mac, protocol);

	for (ind = 0; ind < count; ind += 1)
		send_packet_data(sockfd, buffer, sizeof(struct eth_hdr), &device);

	if (verbose)
		print_eth(buffer);

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
