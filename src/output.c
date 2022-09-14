#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "output.h"
#include "type.h"

void output(char *buffer, int protocol)
{
	struct sockaddr_in src, dst;
	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	struct icmp_hdr *icmph;
	struct tcp_hdr *tcph;
	struct udp_hdr *udph;

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
		printf("%d --> %d / ", htons(tcph->src), htons(tcph->dst));
                break;
        case IPPROTO_UDP:
                udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));
		printf("%d --> %d / ", htons(udph->src), htons(udph->dst));
                break;
	}
	printf("\n");
}
