Depocillin
---

Depocillin is a raw socket packet crafter and injector.

### Supported Protocols and Features
- ETHERNET (MAC Addresses, Protocol, Payload)
- ARP (MAC Addresses, IP Addresses, ARP operation)
- IP (IP Adresses, TTL, Service)
- ICMP (Type, Code)
- TCP (Ports, Flag, Payload)
- UDP (Ports, Payload)

### Installation
* For installation, go to the project directory and compile with : 
    `make`
* For uninstallation : 
    `make clean`

### Example Usages
* ETHERNET packet :
    `./depocillin eth -i wlan0 -M aa:bb:cc:dd:ee:ff -K ff:ee:dd:cc:bb:aa`

> send ethernet packet with a payload using file
* ETHERNET packet with data :
    `./depocillin eth -i wlan0 -M aa:bb:cc:dd:ee:ff -K ff:ee:dd:cc:bb:aa -a (file)`

* ARP packet :
    `./depocillin arp -i wlan0 -S 192.168.1.50 -D 192.168.1.1 -r 1`

* IP packet :
    `./depocillin ip -S 192.168.1.50 -D 192.168.1.1`

* ICMP echo request :
    `./depocillin icmp -S 192.168.1.50 -D 192.168.1.1 -t 8`

* TCP syn packet :
     `./depocillin tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f syn`

> you can use multiple flags together
* TCP syn, ack packet :
     `./depocillin tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f syn -f ack`

> send tcp packet with a payload using file
* TCP psh packet with data :
     `./depocillin tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f psh -a (file)`

* UDP packet :
     `./depocillin udp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 4000`

> send udp packet with a payload using file
* UDP packet with data :
     `./depocillin udp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 4000 -a (file)`
