#ifndef TYPE
#define TYPE

#define BUFF_SIZE 65535
#define DEFAULT_TTL 248

struct eth_hdr {
	unsigned char dst[6];
	unsigned char src[6];
	unsigned short protocol;
};

struct arp_hdr {
	unsigned short htype;
	unsigned short ptype;
	unsigned char hlen;
	unsigned char plen;
	unsigned short oper;
	unsigned char src_mac[6];
	unsigned char src_ip[4];
	unsigned char dst_mac[6];
	unsigned char dst_ip[4];
};

struct ip_hdr {
	unsigned char ver_ihl;
	unsigned char service;
	unsigned short length;
	unsigned short ident;
	unsigned short frag;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short check;
	unsigned int src;
	unsigned int dst;
};

struct icmp_hdr {
	unsigned char type;
	unsigned char code;
	unsigned short check;
	unsigned short id;
	unsigned short seq;
};

struct tcp_hdr {
	unsigned short src;
	unsigned short dst;
	unsigned int seq;
	unsigned int ack;
	unsigned char len_res;
	unsigned char flag;
	unsigned short win;
	unsigned short check;
	unsigned short urgp;
};

struct udp_hdr {
	unsigned short src;
	unsigned short dst;
	unsigned short length;
	unsigned short check;
};

struct psd_hdr {
	unsigned int src;
	unsigned int dst;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short length;
};

#endif
