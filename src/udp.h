#ifndef UDP
#define UDP

#include "type.h"

unsigned short udp_check(struct udp_hdr *udph, struct ip_hdr *iph);

void set_udp(struct udp_hdr *udph, struct ip_hdr *iph, unsigned short src,
	     unsigned short dst);

#endif
