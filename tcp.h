#ifndef TCP
#define TCP

#include "type.h"

unsigned short tcp_check(struct tcp_hdr *tcph, struct ip_hdr *iph);

void set_tcp(struct tcp_hdr *tcph, struct ip_hdr *iph,
                unsigned short src, unsigned short dst,
                unsigned char flag, unsigned int seq,
                unsigned int ack)

#endif
