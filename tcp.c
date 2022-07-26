#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "type.h"
#include "tcp.h"
#include "checksum.h"

unsigned short tcp_check(struct tcp_hdr *tcph, struct ip_hdr *iph)
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

void set_tcp(struct tcp_hdr *tcph, struct tcp_args *tcpa, struct ip_hdr *iph)
{
	tcph->src = tcpa->src;
	tcph->dst = tcpa->dst;
	tcph->seq = tcpa->seq;
	tcph->ack = tcpa->ack;
	tcph->len_res = 0x50;
	tcph->flag = tcpa->flag;
	tcph->win = 0x0fff;
	tcph->check = 0;
	tcph->urgp = 0x0;
	tcph->check = tcp_check(tcph, iph);
}
