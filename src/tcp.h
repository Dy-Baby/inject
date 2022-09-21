#ifndef TCP
#define TCP

void set_tcp(char *buffer, unsigned short src, unsigned short dst,
		unsigned char flag, unsigned int seq, unsigned int ack);

#endif
