#ifndef IP
#define IP

#include "type.h"

void set_ip(struct ip_hdr *iph, unsigned int src, unsigned int dst,
	    unsigned char ttl, unsigned char protocol);

#endif
