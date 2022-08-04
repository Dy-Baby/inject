CC = gcc
CFLAGS = -Wall

SOURCES = main.c sockf.c send.c checksum.c error_func.c ip.c icmp.c tcp.c udp.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = inject

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY = clean
clean:
	rm -f $(TARGET) $(OBJECTS)
