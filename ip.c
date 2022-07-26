#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "type.h"
#include "ip.h"
#include "checksum.h"

void set_ip(struct ip_hdr *iph, struct ip_args *ipa)
{
	switch (ipa->protocol) {
	case IPPROTO_ICMP:
		iph->length = sizeof(struct ip_hdr) + sizeof(struct icmp_hdr);
		break;
	case IPPROTO_TCP:
		iph->length = sizeof(struct ip_hdr) + sizeof(struct tcp_hdr);
		break;
	case IPPROTO_UDP:
		iph->length = sizeof(struct ip_hdr) + sizeof(struct tcp_hdr);
		break;
	default:
		iph->length = sizeof(struct ip_hdr);
		break;
	}
	iph->ver_ihl = 0x45;
	iph->service = 0x00;
	iph->ident = 0x00;
	iph->frag = 0x00;
	if (!ipa->ttl)
		ipa->ttl = DEFAULT_TTL;
	iph->ttl = ipa->ttl;
	iph->protocol = ipa->protocol;
	iph->check = 0;
	iph->src = ipa->src;
	iph->dst = ipa->dst;
	iph->check = checksum((unsigned short *)iph, iph->length);
}
