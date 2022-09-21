#ifndef ICMP
#define ICMP

void set_icmp(char *buffer, unsigned char type,
		unsigned char code, unsigned short seq);

#endif
