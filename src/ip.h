#ifndef IP
#define IP

#include <sys/types.h>

void set_ip(char *buffer, size_t payload_size,
		unsigned int src, unsigned int dst,
		unsigned char ttl, unsigned char protocol);
void inject_ip(int argc, char *argv[]);

#endif
