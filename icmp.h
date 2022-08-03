#ifndef ICMP
#define ICMP

#include "type.h"

void set_icmp(struct icmp_hdr *icmph, unsigned char type,
		unsigned char code, unsigned short seq);

#endif
