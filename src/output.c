#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "output.h"
#include "type.h"

static void get_icmp_type(unsigned char type, char *type_str)
{
	if (type == 0) strcpy(type_str, "echo reply");
	if (type == 3) strcpy(type_str, "destination unreachable");
	if (type == 4) strcpy(type_str, "source quench");
	if (type == 5) strcpy(type_str, "redirect");
	if (type == 8) strcpy(type_str, "echo request");
	if (type == 11) strcpy(type_str, "time exceeded");
	if (type == 12) strcpy(type_str, "parameter unintelligible");
	if (type == 13) strcpy(type_str, "time-stamp request");
	if (type == 14) strcpy(type_str, "time-stamp reply");
	if (type == 15) strcpy(type_str, "information request");
	if (type == 16) strcpy(type_str, "information reply");
	if (type == 17) strcpy(type_str, "address mask request");
	if (type == 18) strcpy(type_str, "address mask reply");
}

static void get_tcp_flag(unsigned char flag, char *flag_str)
{
	if ((flag | 1) == flag) strcat(flag_str, "fin,");
        if ((flag | 2) == flag) strcat(flag_str, "syn,");
        if ((flag | 4) == flag) strcat(flag_str, "rst,");
        if ((flag | 8) == flag) strcat(flag_str, "psh,");
        if ((flag | 16) == flag) strcat(flag_str, "ack,");
        if ((flag | 32) == flag) strcat(flag_str, "urg,");
}

void print_ip(char *buffer)
{
	struct sockaddr_in src, dst;
	struct ip_hdr *iph = (struct ip_hdr *)buffer;

	src.sin_addr.s_addr = iph->src;
	dst.sin_addr.s_addr = iph->dst;

	printf("IP source : %s\n", inet_ntoa(src.sin_addr));
	printf("IP destination : %s\n", inet_ntoa(dst.sin_addr));
	printf("IP ttl : %d\n", iph->ttl);
	printf("IP id : %d\n", iph->ident);
	printf("IP check : %x\n", iph->check);
}

void print_icmp(char *buffer)
{
	struct icmp_hdr *icmph = (struct icmp_hdr *)(buffer + sizeof(struct ip_hdr));
	char type_str[50];

	memset(type_str, 0, 50);
	get_icmp_type(icmph->type, type_str);

	printf("ICMP type : %s\n", type_str);
	printf("ICMP code : %d\n", icmph->code);
	printf("ICMP id : %d\n", icmph->id);
	printf("ICMP seq : %d\n", icmph->seq);
	printf("ICMP check : %x\n", icmph->check);
}

void print_tcp(char *buffer)
{
	struct tcp_hdr *tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));
	char flag_str[32];

	memset(flag_str, 0, 32);
	get_tcp_flag(tcph->flag, flag_str);

	printf("TCP source : %d\n", htons(tcph->src));
	printf("TCP destination : %d\n", htons(tcph->dst));
	printf("TCP seq : %d\n", tcph->seq);
	printf("TCP ack : %d\n", tcph->ack);
	printf("TCP flag : %s\n", flag_str);
	printf("TCP check : %x\n", tcph->check);
}

void print_udp(char *buffer)
{
	struct udp_hdr *udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("UDP source : %d\n", htons(udph->src));
	printf("UDP destination : %d\n", htons(udph->dst));
	printf("UDP checksum : %x\n", udph->check);
}
