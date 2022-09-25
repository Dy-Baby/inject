#ifndef TCP
#define TCP

void set_tcp(char *buffer, unsigned short src, unsigned short dst,
		unsigned char flag, unsigned int seq, unsigned int ack);
void inject_tcp(int arc, char *argv[]);

#endif
