#ifndef UDP
#define UDP

#include "type.h"

void set_udp(struct udp_hdr *udph, unsigned short src,
		unsigned short dst);

#endif
