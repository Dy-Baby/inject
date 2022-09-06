#include <unistd.h>
#include <arpa/inet.h>
#include "type.h"
#include "icmp.h"
#include "checksum.h"

void set_icmp(struct icmp_hdr *icmph, unsigned char type, unsigned char code,
	      unsigned short seq)
{
	icmph->type = type;
	icmph->code = code;
	icmph->seq = seq;
	icmph->id = htons(getpid());
	icmph->check = 0;
	icmph->check = checksum((unsigned short *)icmph, sizeof(struct icmp_hdr));
}
