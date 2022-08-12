#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "type.h"
#include "udp.h"
#include "random.h"
#include "checksum.h"

unsigned short udp_check(struct udp_hdr *udph, struct ip_hdr *iph)
{
	struct psd_hdr psh;
	char *psd;
	unsigned short check;

	psh.src = iph->src;
	psh.dst = iph->dst;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.length = htons(sizeof(struct udp_hdr));

	psd = malloc(sizeof(struct udp_hdr) + sizeof(struct psd_hdr));
	memcpy(psd, (char *)&psh, sizeof(struct psd_hdr));
	memcpy(psd + sizeof(struct psd_hdr), udph, sizeof(struct udp_hdr));

	check = checksum((unsigned short *)psd,
			 sizeof(struct udp_hdr) + sizeof(struct psd_hdr));
	free(psd);

	return check;
}

void set_udp(struct udp_hdr *udph, struct ip_hdr *iph, unsigned short src,
	     unsigned short dst)
{
	if (!src)
		src = rand_port();
	udph->src = htons(src);
	if (!dst)
		dst = rand_port();
	udph->dst = htons(dst);
	udph->length = htons(sizeof(struct udp_hdr));
	udph->check = 0;
	udph->check = udp_check(udph, iph);
}
