#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "output.h"
#include "type.h"

void print_ip(char *buffer)
{
	struct sockaddr_in src, dst;
	struct ip_hdr *iph = (struct ip_hdr *)buffer;

	src.sin_addr.s_addr = iph->src;
	dst.sin_addr.s_addr = iph->dst;

	printf("IP source       : %s\n", inet_ntoa(src.sin_addr));
	printf("IP destination  : %s\n", inet_ntoa(dst.sin_addr));
	printf("IP tos          : %d\n", iph->service);
	printf("IP ttl          : %d\n", iph->ttl);
	printf("IP id           : %d\n", iph->ident);
	printf("IP check        : %x\n", iph->check);
}

void print_icmp(char *buffer)
{
	struct icmp_hdr *icmph = (struct icmp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("ICMP type       : ");
	if (icmph->type == 0) printf("echo reply\n");
	if (icmph->type == 3) printf("destination unreachable\n");
	if (icmph->type == 4) printf("source quench\n");
	if (icmph->type == 5) printf("redirect");
	if (icmph->type == 8) printf("echo request\n");
	if (icmph->type == 9) printf("router advertisement\n");
	if (icmph->type == 10) printf("router selection\n");
	if (icmph->type == 11) printf("time exceeded\n");
	if (icmph->type == 12) printf("parameter problem\n");
	if (icmph->type == 13) printf("timestamp\n");
	if (icmph->type == 14) printf("timestamp reply\n");
	if (icmph->type == 15) printf("information request\n");
	if (icmph->type == 16) printf("information reply\n");
	if (icmph->type == 17) printf("address mask request\n");
	if (icmph->type == 18) printf("address mask reply\n");
	if (icmph->type == 30) printf("traceroute\n");	
	printf("ICMP code       : %d\n", icmph->code);
	printf("ICMP id         : %d\n", icmph->id);
	printf("ICMP seq        : %d\n", icmph->seq);
	printf("ICMP check      : %x\n", icmph->check);
}

void print_tcp(char *buffer)
{
	struct tcp_hdr *tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("TCP source      : %d\n", htons(tcph->src));
	printf("TCP destination : %d\n", htons(tcph->dst));
	printf("TCP seq         : %d\n", tcph->seq);
	printf("TCP ack         : %d\n", tcph->ack);
	printf("TCP flag        : ");
	if ((tcph->flag | 1) == tcph->flag) printf("fin,");
	if ((tcph->flag | 2) == tcph->flag) printf("syn,");
	if ((tcph->flag | 4) == tcph->flag) printf("rst,");
	if ((tcph->flag | 8) == tcph->flag) printf("psh,");
	if ((tcph->flag | 16) == tcph->flag) printf("ack,");
	if ((tcph->flag | 32) == tcph->flag) printf("urg,");
	printf("\n");
	printf("TCP check       : %x\n", tcph->check);
}

void print_udp(char *buffer)
{
	struct udp_hdr *udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("UDP source      : %d\n", htons(udph->src));
	printf("UDP destination : %d\n", htons(udph->dst));
	printf("UDP checksum    : %x\n", udph->check);
}
