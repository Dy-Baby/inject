#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sockf.h"
#include "send.h"
#include "data.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "ip.h"
#include "udp.h"
#include "random.h"
#include "checksum.h"

static unsigned int src_addr, dst_addr;
static unsigned char ttl, service = 0;
static unsigned short src_port, dst_port;
static int count = 1, verbose = 0;
static char *file_name = NULL, *iface = NULL;

static unsigned short udp_check(struct ip_hdr *iph, struct udp_hdr *udph,
				char *payload, size_t payload_size)
{
	struct psd_hdr psh;
	char *psd;
	size_t psd_size;
	unsigned short check;

	psh.src = iph->src;
	psh.dst = iph->dst;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.length = htons(sizeof(struct udp_hdr) + payload_size);

	psd_size = sizeof(struct psd_hdr) + sizeof(struct udp_hdr) + payload_size;

	psd = malloc(psd_size);
	memset(psd, 0, psd_size);

	memcpy(psd, (char *)&psh, sizeof(struct psd_hdr));
	memcpy(psd + sizeof(struct psd_hdr), udph, sizeof(struct udp_hdr));
	memcpy(psd + sizeof(struct psd_hdr) + sizeof(struct udp_hdr),
			payload, payload_size);

	check = checksum((unsigned short *)psd, psd_size);
	free(psd);

	return check;
}

void set_udp(char *buffer, char *payload, size_t payload_size,
		unsigned short src, unsigned short dst)
{
	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	struct udp_hdr *udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));
	char *ptr = (buffer + sizeof(struct ip_hdr) + sizeof(struct udp_hdr));

	strncat(ptr, payload, payload_size);
	udph->src = (src) ? htons(src) : htons(rand_port());
	udph->dst = (dst) ? htons(dst) : htons(rand_port());
	udph->length = htons(sizeof(struct udp_hdr) + payload_size);
	udph->check = 0;
	udph->check = udp_check(iph, udph, payload, payload_size);
}

static void udp_usage()
{
        printf("\n general options :\n\n\
\t-i [interface] : network interface\n\
\t-c [count] : number of packets to send\n\
\t-v : verbose\n\
\t-h : this help message\n");

        printf("\n IP options :\n\n\
\t-S [address] : source address\n\
\t-D [address] : destination address\n\
\t-T [ttl] : ttl\n\
\t-o [service] : type of service\n");

        printf("\n UDP options :\n\n\
\t-s [port] : source port\n\
\t-d [port] : destination port\n\
\t-a [file] : payload file\n\n");
        exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
        int opt;

        if (argc < 3) udp_usage();

        while ((opt = getopt(argc, argv, "i:c:vhS:D:T:o:s:d:a:")) != -1) {
                switch (opt) {
		case 'i':
			iface = optarg;
			break;
                case 'c':
                        count = atoi(optarg);
                        break;
                case 'v':
                        verbose = 1;
                        break;
                case 'h':
                        udp_usage();
                case 'S':
                        src_addr = inet_addr(optarg);
                        break;
                case 'D':
                        dst_addr = inet_addr(optarg);
                        break;
                case 'T':
                        ttl = atoi(optarg);
                        break;
		case 'o':
			service = atoi(optarg);
			break;
                case 's':
                        src_port = atoi(optarg);
                        break;
                case 'd':
                        dst_port = atoi(optarg);
                        break;
		case 'a':
			file_name = optarg;
			break;
                case '?':
                        break;
                }
        }
}

void inject_udp(int argc, char *argv[])
{
       char buffer[BUFF_SIZE], *payload;
       struct sockaddr_in sock_dst;
       int sockfd, ind;
       size_t payload_size = 0;

       parser(argc, argv);
       if (!dst_addr) err_exit("destination address not specified.");

       srand(time(NULL));
       memset(buffer, 0, BUFF_SIZE);
       memset(&sock_dst, 0, sizeof(struct sockaddr_in));

       sockfd = init_socket();
       if (iface) bind_iface(sockfd, iface);

       sock_dst.sin_family = AF_INET;
       sock_dst.sin_addr.s_addr = dst_addr;
       sock_dst.sin_port = dst_port;

       if (file_name) {
	       if ((payload = read_file(file_name)) == NULL)
		       exit(EXIT_FAILURE);
	       payload_size = strlen(payload);
       }

       set_ip(buffer, payload_size, src_addr, dst_addr, ttl, service, IPPROTO_UDP);
       set_udp(buffer, payload, payload_size, src_port, dst_port);

       struct ip_hdr *iph = (struct ip_hdr *)buffer;
       for (ind = 0; ind < count; ind += 1)
               send_data(sockfd, buffer, iph->length, &sock_dst);

       if (verbose) {
	       print_ip(buffer);
	       print_udp(buffer);
       }

       if (file_name) free(payload);
       close_sock(sockfd);

       exit(EXIT_SUCCESS);
}
