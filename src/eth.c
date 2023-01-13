#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "sockf.h"
#include "get_addr.h"
#include "send.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "eth.h"

void set_eth(unsigned char *dst, unsigned char *src, unsigned short protocol);
static void eth_usage();
static void parser(int argc, char *argv[]);
void inject_eth(int argc, char *argv[]);
