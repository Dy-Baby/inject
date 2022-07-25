#ifndef TCP
#define TCP

#include "type.h"

unsigned short tcp_check(struct tcp_hdr *tcph,struct ip_hdr *iph);
void set_tcp(struct tcp_hdr *tcph,struct tcp_args *tcpa,\
        struct ip_hdr *iph);

#endif
