Inject
---

Inject is a raw socket packet crafter and injector.

### Supported Protocols
- IP (Source address, Destination address, TTL)
- ICMP (ICMP Type)
- TCP (Source port, Destination port, TCP Flag)
- UDP (Source port, Destination port)

### Installation
* For installation, go to the project directory and compile with : 
    `make`
* For uninstallation : 
    `make clean`

### Example Usages
* IP packet :
    `./inject ip -s 192.168.1.50 -d 192.168.1.1`

* ICMP echo request :
    `./inject icmp -s 192.168.1.50 -d 192.168.1.1 -T 8`

* TCP syn packet :
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -S 5000 -D 80 -f syn`

> you can use multiple flags together.
* TCP syn, ack packet
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -S 5000 - D 80 -f syn -f ack`

> send tcp packet with a payload using files
* TCP psh packet with data
     `./inject tcp -s 192.168.1.50 -d 192.168.1.1 -S 5000 -D 80 -f psh -a (file)`

* UDP packet
     `./inject udp -s 192.168.1.50 -d 192.168.1.1 -S 5000 -D 4000`

> send udp packet with a payload using files
* UDP packet with data
     `./inject udp -s 192.168.1.50 -d 192.168.1.1 -S 5000 -D 4000 -a (file)`
