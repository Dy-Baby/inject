#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "output.h"
#include "type.h"

static void get_tcp_flag(unsigned char flag, char *flag_str)
{
	if ((flag | 1) == flag) strcat(flag_str, "fin,");
        if ((flag | 2) == flag) strcat(flag_str, "syn,");
        if ((flag | 4) == flag) strcat(flag_str, "rst,");
        if ((flag | 8) == flag) strcat(flag_str, "psh,");
        if ((flag | 16) == flag) strcat(flag_str, "ack,");
        if ((flag | 32) == flag) strcat(flag_str, "urg,");
}

void output(char *buffer, int protocol)
{
	struct sockaddr_in src, dst;
	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	struct icmp_hdr *icmph;
	struct tcp_hdr *tcph;
	struct udp_hdr *udph;

	char flag_str[24];

	memset(flag_str, 0, 24);
	src.sin_addr.s_addr = iph->src;
	dst.sin_addr.s_addr = iph->dst;

	printf("%s", inet_ntoa(src.sin_addr));
	printf(" --> ");
	printf("%s", inet_ntoa(dst.sin_addr));
	printf(" / ");

	switch (protocol) {
	case IPPROTO_ICMP:
                icmph = (struct icmp_hdr *)(buffer + sizeof(struct ip_hdr));
                printf("type:%d code:%d / ", icmph->type, icmph->code);
		break;
        case IPPROTO_TCP:
                tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));
		get_tcp_flag(tcph->flag, flag_str);
		printf("%d --> %d %s / ", htons(tcph->src), htons(tcph->dst),
					flag_str);
                break;
        case IPPROTO_UDP:
                udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));
		printf("%d --> %d / ", htons(udph->src), htons(udph->dst));
                break;
	}
	printf("\n");
}
