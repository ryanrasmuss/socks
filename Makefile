sources = test_client.c server.c

srcs_dir = test-src

binaries = $(sources:.c=)

LIBR = libsocks.so
SOCKS_DIR = socks
SOCKS_LIBR = $(SOCKS_DIR)/$(LIBR)

all: $(SOCKS_LIBR) $(binaries)

CC = gcc
CFLAGS = -Wall

INCLUDE = -I$(SOCKS_DIR) -L$(SOCKS_DIR)

$(SOCKS_LIBR):
	$(MAKE) -C $(SOCKS_DIR)

$(binaries): %: $(srcs_dir)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< -lsocks.o

clean:
	$(MAKE) -C $(SOCKS_DIR) clean
	rm -v $(binaries)
