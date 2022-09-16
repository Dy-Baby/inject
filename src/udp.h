#ifndef UDP
#define UDP

#include "type.h"

void set_udp(struct udp_hdr *udph, struct ip_hdr *iph, unsigned short src,
	     unsigned short dst);

#endif
