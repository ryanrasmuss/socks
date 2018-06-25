sources := test_client.c server.c

socks := socks.a

socks_src = socks.c

socks_objs = socks.o

binaries = $(sources:.c=)

CC = gcc

AR = ar

ARFLAGS = rcs

CFLAGS = -Wall

all: $(socks) $(binaries)

$(socks): $(socks_objs)
	$(AR) $(ARFLAGS) $@ $(socks_objs)

$(socks_objs): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(binaries): % : %.c
	$(CC) $(CFLAGS) $(socks) $< -o $@

clean:
	rm -v $(binaries)
