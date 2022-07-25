#include <unistd.h>
#include <arpa/inet.h>
#include "type.h"
#include "icmp.h"
#include "checksum.h"

void set_icmp(struct icmp_hdr *icmph,struct icmp_args *icmpa){
    icmph->type=icmpa->type;
    icmph->code=icmpa->code;
    icmph->seq=icmpa->seq;
    icmph->id=htons(getpid());
    icmph->check=0;
    icmph->check=checksum((unsigned short *)icmph,sizeof(struct icmp_hdr));
}
