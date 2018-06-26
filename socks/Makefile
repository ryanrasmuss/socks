sources = socks.c

objects = $(sources:.c=.o)

shared_lib = libsocks.so

CC = gcc
CFLAGS = -shared -Wall

all: $(shared_lib)

$(shared_lib): $(objects)
	$(CC) $(CFLAGS) -o $@ $<

$(objects): %.o : %.c
	$(CC) -c $< -o $@

clean:
	rm -v $(shared_lib)
	rm -v $(objects)
