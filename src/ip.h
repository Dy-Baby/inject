#ifndef IP
#define IP

void set_ip(char *buffer, unsigned int src, unsigned int dst,
	    unsigned char ttl, unsigned char protocol);
void inject_ip(int argc, char *argv[]);

#endif
