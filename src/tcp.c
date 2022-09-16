#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "type.h"
#include "tcp.h"
#include "random.h"
#include "checksum.h"

static unsigned short tcp_check(struct tcp_hdr *tcph, struct ip_hdr *iph)
{
	struct psd_hdr psh;
	char *psd;
	unsigned short check;

	psh.src = iph->src;
	psh.dst = iph->dst;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.length = htons(sizeof(struct tcp_hdr));

	psd = malloc(sizeof(struct tcp_hdr) + sizeof(struct psd_hdr));
	memcpy(psd, (char *)&psh, sizeof(struct psd_hdr));
	memcpy(psd + sizeof(struct psd_hdr), tcph, sizeof(struct tcp_hdr));

	check = checksum((unsigned short *)psd,
			 sizeof(struct tcp_hdr) + sizeof(struct psd_hdr));
	free(psd);

	return check;
}

void set_tcp(struct tcp_hdr *tcph, struct ip_hdr *iph, unsigned short src,
	     unsigned short dst, unsigned char flag, unsigned int seq,
	     unsigned int ack)
{
	tcph->src = (src) ? htons(src) : htons(rand_port());
	tcph->dst = (dst) ? htons(dst) : htons(rand_port());
	tcph->seq = seq;
	tcph->ack = ack;
	tcph->len_res = 0x50;
	tcph->flag = flag;
	tcph->win = 0x0fff;
	tcph->check = 0;
	tcph->urgp = 0x0;
	tcph->check = tcp_check(tcph, iph);
}
