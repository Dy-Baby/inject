CC = gcc
CFLAGS = -Wall

SOURCES = src/main.c src/sockf.c src/send.c src/error_func.c\
	 src/random.c src/get_addr.c src/checksum.c src/ip.c\
	 src/icmp.c src/tcp.c src/udp.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = inject

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY = clean
clean:
	rm -f $(TARGET) $(OBJECTS)
