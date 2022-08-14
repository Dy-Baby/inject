#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "type.h"
#include "ip.h"
#include "random.h"
#include "checksum.h"

void set_ip(struct ip_hdr *iph, unsigned int src, unsigned int dst,
		unsigned char ttl, unsigned char protocol)
{
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

	iph->ver_ihl = 0x45;
	iph->service = 0x00;
	iph->ident = 0x00;
	iph->frag = 0x00;
	iph->ttl = (!ttl) ? ttl : DEFAULT_TTL;
	iph->protocol = protocol;
	iph->check = 0;
	iph->src = (!src) ? rand_addr() : src;
	iph->dst = dst;
	iph->check = checksum((unsigned short *)iph, iph->length);
}
