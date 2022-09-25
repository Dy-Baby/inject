#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "error_func.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "udp.h"

void print_usage()
{
	printf("\n usage : ./inject [protocol] [options]\n\n\
 protocol :\n\
\tip : ip packets\n\
\ticmp : icmp packets\n\
\ttcp : tcp packets\n\
\tudp : udp packets\n\n\
 options :\n\
\t-h : this help message\n\n");
	exit(EXIT_FAILURE);
}

void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 2) print_usage();

	if (!strcmp(argv[1], "ip")) inject_ip(argc, argv);
	if (!strcmp(argv[1], "icmp")) inject_icmp(argc, argv);
	if (!strcmp(argv[1], "tcp")) inject_tcp(argc, argv);
	if (!strcmp(argv[1], "udp")) inject_udp(argc, argv);

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
			print_usage();
		}
	}
}

int main(int argc, char *argv[])
{
	if (getuid()) err_exit("permission denied.");
	parser(argc, argv);

	return 0;
}
