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
#include "tcp.h"
#include "random.h"
#include "checksum.h"

static unsigned char *src_addr = NULL, *dst_addr = NULL;
static unsigned char ttl, service = 0, tcp_flag;
static unsigned short src_port, dst_port;
static int count = 1, verbose = 0;
static char *file_name = NULL, *iface = NULL;

static unsigned short tcp_check(struct ip_hdr *iph, struct tcp_hdr *tcph,
				char *payload, size_t payload_size)
{
	struct psd_hdr psh;
	char *psd;
	size_t psd_size;
	unsigned short check;

	inet_pton(AF_INET, (const char *)iph->src, &psh.src);
	inet_pton(AF_INET, (const char *)iph->dst, &psh.dst);
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.length = htons(sizeof(struct tcp_hdr) + payload_size);

	psd_size = sizeof(struct psd_hdr) + sizeof(struct tcp_hdr) + payload_size;

	psd = malloc(psd_size);
	memset(psd, 0, psd_size);

	memcpy(psd, (char *)&psh, sizeof(struct psd_hdr));
	memcpy(psd + sizeof(struct psd_hdr), tcph, sizeof(struct tcp_hdr));
	memcpy(psd + sizeof(struct psd_hdr) + sizeof(struct tcp_hdr),
			payload, payload_size);

	check = checksum((unsigned short *)psd, psd_size);
	free(psd);

	return check;
}

void set_tcp(char *buffer, char *payload, size_t payload_size,
		unsigned short src, unsigned short dst,
		unsigned char flag, unsigned int seq, unsigned int ack)
{
	struct ip_hdr *iph = (struct ip_hdr *)buffer;
	struct tcp_hdr *tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));
	char *ptr = (buffer + sizeof(struct ip_hdr) + sizeof(struct tcp_hdr));

	strncat(ptr, payload, payload_size);
	tcph->src = (src) ? htons(src) : htons(rand_port());
	tcph->dst = (dst) ? htons(dst) : htons(rand_port());
	tcph->seq = seq;
	tcph->ack = ack;
	tcph->len_res = 0x50;
	tcph->flag = flag;
	tcph->win = 0x0fff;
	tcph->check = 0;
	tcph->urgp = 0x0;
	tcph->check = tcp_check(iph, tcph, payload, payload_size);
}

static void tcp_usage()
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

	printf("\n TCP options :\n\n\
\t-s [port] : source port\n\
\t-d [port] : destination port\n\
\t-f [flag] : tcp flag (syn, ack, psh, fin, rst, urg)\n\
\t-a [file] : payload file\n\n");
	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
        int opt;

        if (argc < 3) tcp_usage();

        while ((opt = getopt(argc, argv, "i:c:vhS:D:T:o:s:d:f:a:")) != -1) {
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
                        tcp_usage();
                case 'S':
                        src_addr = (unsigned char *)optarg;
                        break;
                case 'D':
                        dst_addr = (unsigned char *)optarg;
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
		case 'f':
			if (!strcmp(optarg, "fin")) tcp_flag |= 1;
			if (!strcmp(optarg, "syn")) tcp_flag |= 2;
			if (!strcmp(optarg, "rst")) tcp_flag |= 4;
			if (!strcmp(optarg, "psh")) tcp_flag |= 8;
			if (!strcmp(optarg, "ack")) tcp_flag |= 16;
			if (!strcmp(optarg, "urg")) tcp_flag |= 32;
			break;
		case 'a':
			file_name = optarg;
			break;
                case '?':
                        break;
                }
        }
}

void inject_tcp(int argc, char *argv[])
{
       char buffer[BUFF_SIZE], *payload = NULL;
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
       inet_pton(AF_INET, (const char *)dst_addr, &sock_dst.sin_addr.s_addr);
       sock_dst.sin_port = dst_port;

       if (file_name) {
	       if ((payload = read_file(file_name)) == NULL)
		       exit(EXIT_FAILURE);
	       payload_size = strlen(payload);
       }

       set_ip(buffer, payload_size, src_addr, dst_addr, ttl, service, IPPROTO_TCP);
       set_tcp(buffer, payload, payload_size, src_port, dst_port, tcp_flag, 1, 1);

       struct ip_hdr *iph = (struct ip_hdr *)buffer;
       for (ind = 0; ind < count; ind += 1)
               send_data(sockfd, buffer, iph->length, &sock_dst);

       if (verbose) {
	       print_ip(buffer);
	       print_tcp(buffer);
       }

       if (file_name) free(payload);
       close_sock(sockfd);

       exit(EXIT_SUCCESS);
}
