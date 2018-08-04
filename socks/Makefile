sources = socks.c

objects = $(sources:.c=.o)

shared_lib = libsocks.so

CC = gcc
CFLAGS = -Wall -fPIC

all: $(shared_lib)

$(shared_lib): $(objects)
	$(CC) -shared -o $(shared_lib) $<

$(objects): %.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -v $(shared_lib)
	rm -v $(objects)
