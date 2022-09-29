Inject
---

Inject is a raw socket packet crafter and injector.

### Supported Protocols
- IP
- ICMP
- TCP
- UDP

### Installation
* For installation, go to the project directory and compile with : 
    `make`
* For uninstallation : 
    `make clean`

### Example Usages
* IP packet :
    `./inject ip -S 192.168.1.50 -D 192.168.1.1`

* ICMP echo request :
    `./inject icmp -S 192.168.1.50 -D 192.168.1.1 -t 8`

* TCP syn packet :
     `./inject tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f syn`

> you can use multiple flags together.
* TCP syn, ack packet
     `./inject tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f syn -f ack`

> send tcp packet with a payload using files
* TCP psh packet with data
     `./inject tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f psh -a (file)`

* UDP packet
     `./inject udp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 4000`

> send udp packet with a payload using files
* UDP packet with data
     `./inject udp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 4000 -a (file)`
