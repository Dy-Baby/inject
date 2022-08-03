#ifndef TYPE
#define TYPE

#define DEFAULT_TTL 248

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
struct icmp_args {
	unsigned char type;
	unsigned char code;
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
struct tcp_args {
	unsigned short src;
	unsigned short dst;
	unsigned char flag;
	unsigned int seq;
	unsigned int ack;
};

struct udp_hdr {
	unsigned short src;
	unsigned short dst;
	unsigned short length;
	unsigned short check;
};
struct udp_args {
	unsigned short src;
	unsigned short dst;
};

struct psd_hdr {
	unsigned int src;
	unsigned int dst;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short length;
};

#endif
