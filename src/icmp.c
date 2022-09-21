#include <unistd.h>
#include <arpa/inet.h>
#include "type.h"
#include "icmp.h"
#include "checksum.h"

void set_icmp(char *buffer, unsigned char type,
		unsigned char code, unsigned short seq)
{
	struct icmp_hdr *icmph = (struct icmp_hdr *)
		(buffer + sizeof(struct ip_hdr));

	icmph->type = type;
	icmph->code = code;
	icmph->seq = seq;
	icmph->id = htons(getpid());
	icmph->check = 0;
	icmph->check = checksum((unsigned short *)icmph, sizeof(struct icmp_hdr));
}
