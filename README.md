Inject
---

Inject is a Raw Socket packet crafter.

### Supported Protocols
- IP (Source address, Destination address, TTL)
- ICMP (ICMP Type)
- TCP (Source port, Destination port, TCP Flag)
- UDP (Source port, Destination port)
> Currently working on data support.

### Installation
* For installation, go to the project directory and compile with
    `make`
* For uninstallation
    `make clean`

### Example Usages
* ICMP echo request :
    `./inject icmp -s 192.168.1.50 -d 192.168.1.1 -t 8`

* TCP syn packet :
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -o 5000 - p 80 -f syn`
> you can use multiple flags together.

* TCP syn, ack packet
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -o 5000 - p 80 -f syn -f ack`

* UDP packet
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -o 5000 - p 4000`

